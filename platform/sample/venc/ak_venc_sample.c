#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <getopt.h>
#include <unistd.h>

#include "ak_common.h"
#include "ak_log.h"
#include "ak_common_video.h"
#include "ak_venc.h"
#include "ak_thread.h"
#include "ak_mem.h"
#include "ak_vi.h"

#ifdef AK_RTOS
#include "rtthread.h"
#define THREAD_PRIO 90
#else
#define THREAD_PRIO -1
#endif

#define RECORD_READ_LEN      4096*10 /* read video file buffer length */
#define DE_VIDEO_SIZE_MAX    5
/* this is length for parse */
#define LEN_HINT                512
#define LEN_OPTION_SHORT        512

/* mipi screen res */
#define MIPI_SC_WIDTH 1280
#define MIPI_SC_HEIGHT 800

/* RGB screen res */
#define RGB_SC_WIDTH  1024
#define RGB_SC_HEIGHT  600

#define MAX_ENC_NUM   4

struct venc_pair
{
    int venc_handle;
    char *file;      //rtsp client handle
    char save_file[128];
    int en_type;
};

/* resolation define */
struct resolution_t {
	unsigned int width;
	unsigned int height;
	unsigned char str[20];
};

static char *pc_prog_name = NULL;                      //demo名称
static char *file  = NULL;                             //file name
static char *type         = NULL;                      //get the type input
static char *save_path    = "/mnt/video_encode";
static int res            = 3;
static int encoder_num = 1;
static int source      = 0;
static int frame_num   = 1000;
ak_mutex_t refresh_flag_lock;

/* decoder resolution */
static struct resolution_t resolutions[DE_VIDEO_SIZE_MAX] = {
    {640,   360,   "DE_VIDEO_SIZE_360P"},
    {640,   480,   "DE_VIDEO_SIZE_VGA"},
    {1280,  720,   "DE_VIDEO_SIZE_720P"},
    {1920,	1080,  "DE_VIDEO_SIZE_1080P"},
    {2560,  1920,  "DE_VIDEO_SIZE_1920P"}
};

/* this is the message to print */
char ac_option_hint[  ][ LEN_HINT ] = {                                         //操作提示数组
    "打印帮助信息" ,
    "编码数据源 0 - 文件, 1 - 从vi获取",
    "从vi获取的帧的数量",
    "编码的文件源路径 :",
    "编码输出数据格式 h264 h265 jpeg ",
    "编码的数量 ：[1-4]",
    "码流分辨率，max 2560*1920",
    "编码数据保存成文件的目录 ",
    "",
};

/* opt for print the message */
struct option option_long[ ] = {
    { "help"              , no_argument       , NULL , 'h' } ,      //"打印帮助信息" ,
    { "source-in"         , required_argument , NULL , 's' } ,      //"屏幕类型" ,
    { "frame-num"         , required_argument , NULL , 'm' } ,      //"屏幕类型" ,
    { "file-dir"          , required_argument , NULL , 'f' } ,      //"文件路径" ,
    { "data-output"       , required_argument , NULL , 'o' } ,      //"数据格式" ,
    { "encode-num"        , required_argument , NULL , 'n' } ,      //"解码数量" ,
    { "resolution"        , required_argument , NULL , 'r' } ,      //"分辨率" ,
    { "save-path"         , required_argument , NULL , 'p' } ,      //"解码数量" ,
    { 0                   , 0                 , 0    , 0   } ,
};

/*
* get_option_short: fill the stort option string.
* return: option short string addr.
*/
char *get_option_short( struct option *p_option, int i_num_option, char *pc_option_short, int i_len_option )
{
    int i;
    int i_offset = 0;
    char c_option;

    /* get the option */
    for( i = 0 ; i < i_num_option ; i ++ )
    {
        if( ( c_option = p_option[ i ].val ) == 0 ) 
        {
            continue;
        }

        switch( p_option[ i ].has_arg )
        {
        case no_argument:
            /* if no argument, set the offset for default */
            i_offset += snprintf( pc_option_short + i_offset , i_len_option - i_offset , "%c" , c_option );
            break;
        case required_argument:
            /* required argument offset calculate */
            i_offset += snprintf( pc_option_short + i_offset , i_len_option - i_offset , "%c:" , c_option );
            break;
        case optional_argument:
            /* calculate the option offset */
            i_offset += snprintf( pc_option_short + i_offset , i_len_option - i_offset , "%c::" , c_option );
            break;
        }
    }
    return pc_option_short;
}

void usage(const char * name)
{
    ak_print_normal(MODULE_ID_VDEC, "eg.: %s -s 0 -f /mnt/yuv/  -n 1 -o h264 -r 3 -p /mnt/video\n", name);
    ak_print_normal(MODULE_ID_VDEC, "source -s:     0 - data from file or 1 - data from vi \n");
    ak_print_normal(MODULE_ID_VDEC, "encode-num -n:     1-4\n");
    ak_print_normal(MODULE_ID_VDEC, "encode data type -o:          h264,  h265,  or  jpeg\n");
    ak_print_normal(MODULE_ID_VDEC, "resolution -r:  value 0 ~ 4\n");
    ak_print_normal(MODULE_ID_VDEC, "                      0 - 640*360\n");
    ak_print_normal(MODULE_ID_VDEC, "                      1 - 640*480\n");
    ak_print_normal(MODULE_ID_VDEC, "                      2 - 1280*720\n");
    ak_print_normal(MODULE_ID_VDEC, "                      3 - 1920*1080\n");
    ak_print_normal(MODULE_ID_VDEC, "                      4 - 2560*1920\n");
    ak_print_normal(MODULE_ID_VDEC, "NOTE: if you want to encode the data from file, you need parameter -f. Otherwise "
                                        "you should set the parameter -m to inform the frame num vi need to gather\n");
}

/* if opt is not supported, print the help message */
static int help_hint(void)
{
    int i;
 
    ak_print_normal(MODULE_ID_VDEC, "%s\n" , pc_prog_name);
    /* parse the all supported option */
    for(i = 0; i < sizeof(option_long) / sizeof(struct option); i ++)
    {
        if( option_long[ i ].val != 0 ) 
            ak_print_normal(MODULE_ID_VDEC, "\t--%-16s -%c %s\n" ,
                                                option_long[ i ].name , option_long[ i ].val , ac_option_hint[ i ]);
    }
    usage(pc_prog_name);
    ak_print_normal(MODULE_ID_VDEC, "\n\n");
    return 0;
}

/* parse the option from cmd line */
int parse_option( int argc, char **argv )
{
    int i_option;
 
    char ac_option_short[ LEN_OPTION_SHORT ];
    int i_array_num = sizeof( option_long ) / sizeof( struct option ) ; /* get the option num*/
    char c_flag = AK_TRUE;
    pc_prog_name = argv[ 0 ];   /* get the option */

    get_option_short( option_long, i_array_num , ac_option_short , LEN_OPTION_SHORT );  /* parse the cmd line input */
    while((i_option = getopt_long(argc , argv , ac_option_short , option_long , NULL)) > 0)
    {
        switch(i_option)
        {
        case 'h' :                                                          //help
            help_hint();
            c_flag = AK_FALSE;
            goto parse_option_end;
        case 's' :                                                          //data file path
            source = atoi( optarg );
            break;
        case 'f' :                                                          //data file path
            file = optarg;
            break;
        case 'o' :                                                          //data_file format 
            type = optarg;
            break;
        case 'n' :                                                          //decode num 
            encoder_num = atoi( optarg );
            break;
        case 'r' :                                                          //decode num 
            res = atoi( optarg );
            break;
        case 'p' :                                                          //decode num 
            save_path = optarg;
            break;
        case 'm' :                                                          //decode num 
            frame_num = atoi( optarg );
            break;
        default :
            help_hint();
            c_flag = AK_FALSE;
            goto parse_option_end;
        }
    }
parse_option_end:
    return c_flag;
}

/* get the encode data from file path */
void *video_encode_to_file_th(void *arg)
{
    ak_thread_set_name("venc_th");
    struct venc_pair *venc_th = (struct venc_pair *)arg; //venc handle

    /* a dir opt */
    char file_name[128];            //record the file name
    unsigned char *data = NULL;     // data memory
    int read_len = 0;               //read len from file
    char *file = venc_th->file;     //file path
    int ret = -1;
    FILE *save_fp = NULL;

    DIR *dir = NULL;
    struct dirent *entry = NULL;

    /* this dir only contain the jpeg file */
    dir = opendir(file);//
    if (dir == NULL)
    {
        ak_print_error_ex(MODULE_ID_VENC, "can't open file dir[%s]\n", file);
        return NULL;
    }
    printf("------------%s %d %s------------\n", __func__, __LINE__, file);

    /* init the save file name and open it */
    memset(venc_th->save_file, 0x00, sizeof(venc_th->save_file));
    if (H264_ENC_TYPE == venc_th->en_type)
        sprintf(venc_th->save_file, "%s/h264_%d_%dx%d.str", save_path, venc_th->venc_handle, resolutions[res].width, resolutions[res].height);
    else
        sprintf(venc_th->save_file, "%s/h265_%d_%dx%d.str", save_path, venc_th->venc_handle, resolutions[res].width, resolutions[res].height);

    /* save file open */
    if (MJPEG_ENC_TYPE != venc_th->en_type)
        save_fp = fopen(venc_th->save_file, "w+");

    /* frame num cal */
    int i = 0;

    /* a loop for read all the file */
    while ((entry = readdir(dir)))
    {
        /* if save as jpeg data */
        if (MJPEG_ENC_TYPE == venc_th->en_type)
        {
            memset(venc_th->save_file, 0x00, sizeof(venc_th->save_file));
            sprintf(venc_th->save_file, "%s/%d_num_%d_%dx%d.jpeg", save_path, venc_th->venc_handle, i, resolutions[res].width, resolutions[res].height);
            save_fp = fopen(venc_th->save_file, "w+");
        }

        /* clean the buf */
        memset(file_name, 0, sizeof(file_name));
        /* get the regular file */
        if (entry->d_type == DT_REG)
        {
            /* open video file */
            sprintf(file_name, "%s/%s",  file, entry->d_name);
        }
        else
            /* if not the regular file */
            continue;

        /* get the file data */
        FILE *fp = fopen(file_name, "r");
        if(NULL == fp)
        {
            ak_print_error_ex(MODULE_ID_VENC, "open [%s] failed\n", file_name);
            continue;
        }

        /* file size */
        fseek(fp, 0, SEEK_END);
        int file_size = ftell(fp);

        /* dest size of the source to be encoded */
        int src_len = resolutions[res].height * resolutions[res].width * 3 / 2;

        /* check the size */
        if(src_len != file_size)
        {
            ak_print_error_ex(MODULE_ID_VENC, "file size %d is not match resolution index [%d]\n", file_size, res);
            fclose(fp);
            continue;
        }

        /* alloc the mem */
        data = (unsigned char *)ak_mem_dma_alloc(MODULE_ID_VENC, file_size);
        if(data == NULL)
        {
            ak_print_error_ex(MODULE_ID_VENC,"Can't alloc [%d] bytes dma memory for file [%s]\n",file_size, file_name);
            fclose(fp);
            continue;
        }
        memset(data, 0x00, file_size);

        /* set file from the beginning */
        fseek(fp, 0, SEEK_SET);

        /* read the record file stream */
        read_len = fread(data, sizeof(char), file_size, fp);

        /* check the file */
        if(read_len != file_size)
            ak_print_error_ex(MODULE_ID_VENC, "file is not read to the end!\n");

        /* if get the data */
        if(read_len > 0)
        {
            /* send it to encode */
            struct video_stream *stream = ak_mem_alloc(MODULE_ID_VENC, sizeof(struct video_stream));
            ret = ak_venc_encode_frame(venc_th->venc_handle, data, file_size, NULL, stream);
            if (ret)
            {
                /* send to encode failed */
                ak_print_error_ex(MODULE_ID_VENC, "send to encode failed\n");
            }
            else
            {
                /* if success, save the data to file */
                fwrite(stream->data, stream->len, 1, save_fp);
                ak_venc_release_stream(venc_th->venc_handle, stream);
                i++;
            }

            ak_mem_free(stream);
        }
        else if(0 == read_len)
        {
            /* read to the end of file */
            ak_print_normal_ex(MODULE_ID_VENC, "\n\tread to the end of file\n");
        }
        else 
        {
            /* error is not support */
            ak_print_error_ex(MODULE_ID_VENC, "read file error\n");
        }

        /* release the buff */
        if (NULL != data)
        {
            ak_mem_dma_free(data);
            data = NULL;
        }

        /* save as a jpeg picture */
        if (MJPEG_ENC_TYPE == venc_th->en_type)
        {
            if (NULL != save_fp)
            {
                fclose(save_fp);
                save_fp = NULL;
            }
        }

        /* inform that sending data is over */
        posix_fadvise(fileno(fp), 0, file_size, POSIX_FADV_DONTNEED);
        fclose(fp);
    }

    /* finished */
    if (MJPEG_ENC_TYPE != venc_th->en_type)
        fclose(save_fp);

    /* close dir and success */
    closedir(dir);

    return NULL;
}

/* get the encode data from file path */
void *video_encode_from_vi_th(void *arg)
{
    ak_thread_set_name("venc_th");
    struct venc_pair *venc_th = (struct venc_pair *)arg; //venc handle

    /* a dir opt */
    int count = 0;
    FILE *save_fp = NULL;
    struct video_input_frame frame;

    ak_print_normal(MODULE_ID_VI, "capture start\n");

    /* init the save file name and open it */
    memset(venc_th->save_file, 0x00, sizeof(venc_th->save_file));
    if (H264_ENC_TYPE == venc_th->en_type)
        sprintf(venc_th->save_file, "%s/h264_%d_1280x720.str", save_path, venc_th->venc_handle);
    else
        sprintf(venc_th->save_file, "%s/h265_%d_1280x720.str", save_path, venc_th->venc_handle);

    /* save file open */
    if (MJPEG_ENC_TYPE != venc_th->en_type)
        save_fp = fopen(venc_th->save_file, "w+");

    /* frame num cal */
    while(count < frame_num)
    {
        memset(&frame, 0x00, sizeof(frame));

        /* if save as jpeg data */
        if (MJPEG_ENC_TYPE == venc_th->en_type)
        {
            memset(venc_th->save_file, 0x00, sizeof(venc_th->save_file));
            sprintf(venc_th->save_file, "%s/%d_num_%d_1280x720.jpeg", save_path, venc_th->venc_handle, count);
            save_fp = fopen(venc_th->save_file, "w+");
        }

        /* to get frame */
        int ret = ak_vi_get_frame(VIDEO_CHN0, &frame);
        if (!ret) 
        {
            /* send it to encode */
            struct video_stream *stream = ak_mem_alloc(MODULE_ID_VENC, sizeof(struct video_stream));
            ret = ak_venc_encode_frame(venc_th->venc_handle, frame.vi_frame.data, frame.vi_frame.len, frame.mdinfo, stream);
            ak_sleep_ms(100);
            if (ret)
            {
                /* send to encode failed */
                ak_print_error_ex(MODULE_ID_VENC, "send to encode failed\n");
            }
            else
            {
                fwrite(stream->data, stream->len, 1, save_fp);
                ak_venc_release_stream(venc_th->venc_handle, stream);
                count++;
            }

            ak_mem_free(stream);
            ak_vi_release_frame(VIDEO_CHN0, &frame);
        }
        else
        {
            /* 
             *	If getting too fast, it will have no data,
             *	just take breath.
             */
            ak_print_normal_ex(MODULE_ID_VI, "get frmae failed!\n");
            ak_sleep_ms(10);
        }

        /* save as a jpeg picture */
        if (MJPEG_ENC_TYPE == venc_th->en_type)
        {
            if (NULL != save_fp)
            {
                fclose(save_fp);
                save_fp = NULL;
            }
        }
    }

    /* finished */
    if (MJPEG_ENC_TYPE != venc_th->en_type)
        fclose(save_fp);

    return NULL;
}

/**
 * note: read the appointed video file, decode, and then output to vo  to playing
 */
#ifdef AK_RTOS
int venc_sample_main(int argc, char **argv)
#else
int main(int argc, char **argv)
#endif
{
    /* init sdk running */
    sdk_run_config config;
    config.mem_trace_flag = SDK_RUN_DEBUG;
    ak_sdk_init( &config );

    ak_print_normal_ex(MODULE_ID_VDEC, "\t--------- %s ---------\n", argv[0]);
    ak_print_normal_ex(MODULE_ID_VDEC, "\tvenc lib version: %s\n\n", ak_venc_get_version());

    /* start to parse the opt */
    if( parse_option( argc, argv ) == AK_FALSE )
    {                                                                           //解释和配置选项
        return AK_FAILED;
    }

    /* SUPPORT the 4 decoder open */
    if (encoder_num > MAX_ENC_NUM || encoder_num < 1)
    {
        ak_print_error_ex(MODULE_ID_VDEC, "Only support the max [%d] decoder\n", MAX_ENC_NUM);
        return AK_FAILED;
    }

    /* SUPPORT the 1 decoder open 2560*1920 */
    if (encoder_num > 1 && (res == DE_VIDEO_SIZE_MAX-1))
    {
        ak_print_error_ex(MODULE_ID_VDEC, "Only support 1 decoder for 2560*1920\n");
        return AK_FAILED;
    }

    if (source < 0 || source > 1)
    {
        ak_print_error_ex(MODULE_ID_VDEC, "encode source in error : 0 - data from file, 1 - data from vi camera \n");
        return AK_FAILED;
    }

    /* get the type for output */
    if (type == NULL)
    {
        ak_print_error_ex(MODULE_ID_VDEC, "please input the type\n");
        /* print the hint and notice */
        help_hint();
        usage(argv[0]);
        return AK_FAILED;
    }

    /* get the type for input */
    int encoder_type = 0;
    if(!strcmp(type,"h264"))
    {
        /* h264 */
        ak_print_normal(MODULE_ID_VDEC, "h264 success\n");
        encoder_type = H264_ENC_TYPE;
    }
    else if(!strcmp(type,"h265"))
    {
        /* h265 */
        ak_print_normal(MODULE_ID_VDEC, "h265 success\n");
        encoder_type = HEVC_ENC_TYPE;
    }
    else if(!strcmp(type,"jpeg"))
    {
        /* jpeg */
        ak_print_normal(MODULE_ID_VDEC, "jpeg success\n");
        encoder_type = MJPEG_ENC_TYPE;
    }
    else
        ak_print_normal_ex(MODULE_ID_VDEC, "unsupport video enccode input type [%s] \n", type);

    int ready_open = encoder_num;
    int ret = -1;
    int i = 0;
    int width = resolutions[res].width;
    int height = resolutions[res].height;
    int handle_id = -1;
    struct venc_pair enc_pair[MAX_ENC_NUM];
    ak_pthread_t venc_stream_th;

    if (source)
    {
        width  = 1280;
        height = 720;
        /* step 1: open video input device */
        ret = ak_vi_open(VIDEO_DEV0);
        if (AK_SUCCESS != ret) 
        {
            ak_print_error_ex(MODULE_ID_VI, "vi device open failed\n");	
            return AK_FAILED;
        }

        /*
         * step 2: load isp config
         */
        char *cfg = "/etc/jffs2/isp_pr2000_dvp_pal_25fps_27M.conf";
        ret = ak_vi_load_sensor_cfg(VIDEO_DEV0, cfg);
        if (AK_SUCCESS != ret)
        {
            ak_print_error_ex(MODULE_ID_VI, "vi device load cfg [%s] failed!\n", cfg);	
            return AK_FAILED;
        }

        /* 
         * step 3: get sensor support max resolution
         */
        struct video_resolution venc_res;				//max sensor resolution
        VI_DEV_ATTR	dev_attr;
        dev_attr.dev_id = VIDEO_DEV0;
        dev_attr.crop.left = 0;
        dev_attr.crop.top = 0;
        dev_attr.crop.width = width;
        dev_attr.crop.height = height;
        dev_attr.max_width = width;
        dev_attr.max_height = height;

        ret = ak_vi_get_sensor_resolution(VIDEO_DEV0, &venc_res);
        if (ret)
        {
            ak_vi_close(VIDEO_DEV0);
            return -1;
        } 
        else 
        {
            ak_print_normal_ex(MODULE_ID_VI, "get dev res w:[%d]h:[%d]\n",venc_res.width, venc_res.height);
            dev_attr.crop.width = venc_res.width;
            dev_attr.crop.height = venc_res.height;
        }

        /* 
         * step 4: set vi working parameters 
         * default parameters: 25fps, day mode, auto frame-control
         */
        ret = ak_vi_set_dev_attr(VIDEO_DEV0, &dev_attr);
        if (ret) 
        {
            ak_print_error_ex(MODULE_ID_VI, "vi device set device attribute failed!\n");
            ak_vi_close(VIDEO_DEV0);
            return -1;
        }

        /*
         * step 5: set channel attribute
         */
        VI_CHN_ATTR chn_attr;
        chn_attr.chn_id = VIDEO_CHN0;
        chn_attr.res.width = width;
        chn_attr.res.height = height;
        ret = ak_vi_set_chn_attr(VIDEO_CHN0, &chn_attr);
        if (ret) 
        {
            ak_print_error_ex(MODULE_ID_VI, "vi device set channel attribute failed!\n");
            ak_vi_close(VIDEO_DEV0);
            return -1;
        }

        /* 
         * step 6: start capture frames
         */
        ret = ak_vi_enable_dev(VIDEO_DEV0);
        if (ret)
        {
            ak_print_error_ex(MODULE_ID_VI, "vi device enable device  failed!\n");
            ak_vi_close(VIDEO_DEV0);
            return -1;
        }
        
    }

    while ( i < ready_open )
    {
        if (access(save_path, W_OK) != 0)
        {
            if (mkdir(save_path, 0755)) 
            {
                ak_print_error_ex(MODULE_ID_VENC, "mkdir: %s failed\n", save_path);
                return AK_FAILED;
            }
        }

        /* open venc */
        struct venc_param ve_param;
        ve_param.width  = width;            //resolution width
        ve_param.height = height;           //resolution height
        ve_param.fps    = 20;               //fps set
        ve_param.goplen = 50;               //gop set
        ve_param.target_kbps = 800;         //k bps
        ve_param.max_kbps    = 1024;        //max kbps
        ve_param.br_mode     = BR_MODE_CBR; //br mode
        ve_param.minqp       = 25;          //qp set
        ve_param.maxqp       = 50;          //qp max value
        ve_param.initqp       = (ve_param.minqp + ve_param.maxqp)/2;    //qp value
        ve_param.jpeg_qlevel = JPEG_QLEVEL_DEFAULT;     //jpeg qlevel
        ve_param.chroma_mode = CHROMA_4_2_0;            //chroma mode
        ve_param.max_picture_size = 0;                  //0 means default
        ve_param.enc_level        = 30;                 //enc level
        ve_param.smart_mode       = 0;                  //smart mode set
        ve_param.smart_goplen     = 100;                //smart mode value
        ve_param.smart_quality    = 50;                 //quality
        ve_param.smart_static_value = 0;                //value
        ve_param.enc_out_type = encoder_type;           //enc type

        /* decode type is h264*/
        if(encoder_type == H264_ENC_TYPE)
        {
            /* profile type */
            ve_param.profile     = PROFILE_MAIN;
            
        }
        /*decode type is jpeg*/
        else if(encoder_type == MJPEG_ENC_TYPE)
        {
            /* jpeg enc profile */
            ve_param.initqp = 80;
            ve_param.profile = PROFILE_JPEG;
        }
        /*decode type is h265*/
        else
        {
            /* hevc profile */
            ve_param.profile = PROFILE_HEVC_MAIN;
        }

        ret = ak_venc_open(&ve_param, &handle_id);

        if (ret || (-1 == handle_id) )
        {
            ak_print_error_ex(MODULE_ID_VENC, "open venc failed\n");
            goto erro;
        }

        /* recode the file */
        enc_pair[i].file = file;
        enc_pair[i].venc_handle = handle_id;
        enc_pair[i].en_type     = encoder_type;
        printf("------------%s %d %s------------\n", __func__, __LINE__, file);

        /* create the venc thread */
        if (source)
            ak_thread_create(&venc_stream_th, video_encode_from_vi_th, &enc_pair[i], ANYKA_THREAD_MIN_STACK_SIZE, THREAD_PRIO);
        else
            ak_thread_create(&venc_stream_th, video_encode_to_file_th, &enc_pair[i], ANYKA_THREAD_MIN_STACK_SIZE, THREAD_PRIO);

        i++;
    }

    /* WAITER for the last thread exit */
    ak_thread_join(venc_stream_th);

erro:
    /* close the video encoder and video decoder */
    for(i = 0; i<ready_open; i++)
    {
        int handle_id = enc_pair[i].venc_handle;
        if(handle_id != -1)
        {
            /* close the venc*/
            ak_venc_close(handle_id);
        }
    }

    return ret;
}

#ifdef AK_RTOS
MSH_CMD_EXPORT(vdec_sample_main, Video Decode Sample Program);
#endif

/* end of file */

