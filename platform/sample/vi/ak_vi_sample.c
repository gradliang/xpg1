/**
* Copyright (C) 2018 Anyka(Guangzhou) Microelectronics Technology CO.,LTD.
* File Name: ak_vi_demo.c
* Description: This is a simple example to show how the VI module working.
* Notes:
* History: V1.0.0
*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <ctype.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

#include "ak_common.h"
#include "ak_common_graphics.h"
#include "ak_vo.h"
#include "ak_vi.h"
#include "ak_mem.h"
#include "ak_log.h"
#include "ak_tde.h"
#include "png.h"        //解码库文件
#include "pngstruct.h"  //解码库文件
#include "pnginfo.h"    //解码库文件

#define MODULE_ID_VI 7
#define PNG_BYTES_TO_CHECK 	 8
int save_num = 1000;							//save frame numbers
const char *yuv_path = "/mnt/yuv_data/";	//yuv save path

 struct pic_info
 {
    char *pathname;
    unsigned char *pData;
    unsigned short width;
    unsigned short height;
    png_byte bpp;
    int color_type;
 };

#if 1
/* 
 * check_dir: check whether the 'path' was exist.
 * path[IN]: pointer to the path which will be checking.
 * return: 1 on exist, 0 is not.
 */
static int check_dir(const char *path)
{
	struct stat buf = {0};

	if (NULL == path)
		return 0;

	stat(path, &buf);
	if (S_ISDIR(buf.st_mode)) {
		return 1;
	} else {
		return 0;
	}
}

/* 
 * save_yuv_data - use to save yuv data to file
 * path[IN]: pointer to saving directory
 * index[IN]: frame number index
 * frame[IN]: pointer to yuv data, include main and sub channel data
 * attr[IN]:  vi channel attribute.
 */
static void save_yuv_data(const char *path, int index, 
		struct video_input_frame *frame, VI_CHN_ATTR *attr)
{
	FILE *fd = NULL;
	unsigned int len = 0;
	unsigned char *buf = NULL;
	struct ak_date date;
	char time_str[32] = {0};
	char file_path[255] = {0};

	ak_print_normal(MODULE_ID_VI, "saving frame, index=%d\n", index);

	/* construct file name */
	ak_get_localdate(&date);
	ak_date_to_string(&date, time_str);
	sprintf(file_path, "%smain_%s_%d_%dx%d.yuv", path, time_str, index,
			attr->res.width, attr->res.height);

	/* 
	 * open appointed file to save YUV data
	 * save main channel yuv here
	 */
	fd = fopen(file_path, "w+b");
	if (fd) {
		buf = frame->vi_frame.data;
		len = frame->vi_frame.len;
		do {
			len -= fwrite(buf, 1, len, fd);
		} while (len != 0);
		
		fclose(fd);
	} else {
		ak_print_normal(MODULE_ID_VI, "open YUV file failed!!\n");
	}
		
#if 0
	/* generate YUV file name */
	sprintf(file_path, "%ssub_%s_%d_%dx%d.yuv", path, time_str, index,
			attr->res[VIDEO_CHN_SUB].width, attr->res[VIDEO_CHN_SUB].height);

	/* 
	 * save sub channel yuv here
	 */
	fd = fopen(file_path, "w+b");
	if (fd) {
		buf = frame->vi_frame[VIDEO_CHN_SUB].data;
		len = frame->vi_frame[VIDEO_CHN_SUB].len;
		do {
			len -= fwrite(buf, 1, len, fd);
		} while (len != 0);

		fclose(fd);
	} else {
		ak_print_normal(MODULE_ID_VI, "open YUV file failed!!\n");
	}
#endif
}

/*
 * vi_capture_loop: loop to get and release yuv, between get and release,
 *                  here we just save the frame to file, on your platform,
 *                  you can rewrite the save_function with your code.
 * vi_handle[IN]: pointer to vi handle, return by ak_vi_open()
 * number[IN]: save numbers
 * path[IN]:   save directory path, if NULL, will not save anymore.
 * attr[IN]:   vi channel attribute.
 */
static void vi_capture_loop(VI_DEV dev_id, int number, const char *path,
		VI_CHN_ATTR *attr)
{
	int skip = 0;
	int count = 0;
	struct video_input_frame frame;

	ak_print_normal(MODULE_ID_VI, "capture start\n");

	/*
	 * To get frame by loop
	 */
    while (count < number) {
		memset(&frame, 0x00, sizeof(frame));

		/* to get frame */
		int ret = ak_vi_get_frame(VIDEO_CHN0, &frame);
		if (!ret) {
			/* 
			 * Here, you can implement your code to use this frame.
			 * Notice, do not occupy it too long.
			 */
			// ak_print_normal_ex(MODULE_ID_VI, "[%d] main chn yuv len: %u\n", count,
			// 		frame.vi_frame.len);
			// ak_print_normal_ex(MODULE_ID_VI, "[%d] sub  chn yuv len: %u\n\n", count,
			// 		frame.vi_frame.len);
			// ak_print_normal_ex(MODULE_ID_VI, "[%d] main chn phyaddr: %lu\n", count,
			// 		frame.phyaddr);

			/* 
			 * TODO: Write your code here to replace save_yuv_data() 
			 * to implement your things.
			 */

            struct ak_vo_obj    obj;                    /* init the obj struct */
            memset(&obj, 0, sizeof(struct ak_vo_obj));

            /* if the data is tileyuv */
#if 0
    attr.res[VIDEO_CHN_MAIN].width = width;//1920;
    attr.res[VIDEO_CHN_MAIN].height = height;//1080;
    attr.res[VIDEO_CHN_SUB].width = subwidth;
    attr.res[VIDEO_CHN_SUB].height = subheight;
#endif

            /* if the data type is YUV420SP */
            obj.format = GP_FORMAT_YUV420SP;
            obj.vo_layer.width = attr->res.width;   /* real width of vdec output data */
            obj.vo_layer.height = attr->res.height; /* real height of vdec output data */
            obj.cmd = GP_OPT_SCALE;                             /* SCALE opt */

            /* set the pos and range to show */
            obj.vo_layer.clip_pos.top = 0;
            obj.vo_layer.clip_pos.left = 0;
            obj.vo_layer.clip_pos.width = attr->res.width;
            obj.vo_layer.clip_pos.height = attr->res.height;
            //obj.vo_layer.dma_addr = ak_mem_dma_vaddr2paddr(frame->yuv_data.data);
            //unsigned char *address = (unsigned char *)ak_mem_dma_alloc(MODULE_ID_VI, frame.vi_frame[VIDEO_CHN_MAIN].len);
            //memcpy(address, frame.vi_frame[VIDEO_CHN_MAIN].data, frame.vi_frame[VIDEO_CHN_MAIN].len);
            //ak_mem_dma_vaddr2paddr(address, &(obj.vo_layer.dma_addr));
            obj.vo_layer.dma_addr = frame.phyaddr;

            /* set the param */
            obj.dst_layer.top = 0;
            obj.dst_layer.left = 0;
            obj.dst_layer.width = 1024-100;
            obj.dst_layer.height = 600-100;

            ak_vo_add_obj(&obj, AK_VO_LAYER_VIDEO_1);

            int cmd = AK_VO_REFRESH_VIDEO_GROUP & 0x100;
            cmd |= AK_VO_REFRESH_GUI_GROUP & 0x30000;
            ak_vo_refresh_screen(cmd);
            //ak_mem_dma_free(address);

			if (skip > 0)
				skip--;
			else if (count < 3) {
				if (check_dir(path))
					save_yuv_data(path, count, &frame, attr);
				else if (path)
					ak_print_warning(MODULE_ID_VI, "Please create directory: %s to save yuvs\n", path);

				/* add counter */
				count++;
			}
			else
				;
				//count++;

			/* 
			 * in this context, this frame was useless,
			 * release frame data
			 */
			ak_vi_release_frame(VIDEO_CHN0, &frame);
		} else {

			/* 
			 *	If getting too fast, it will have no data,
			 *	just take breath.
			 */
			ak_print_normal_ex(MODULE_ID_VI, "get frmae failed!\n");
			ak_sleep_ms(10);
		}
	}

	ak_print_normal(MODULE_ID_VI,"capture finish\n\n");
}
#endif

/*
    *函数功能 ：判断一个图片数是不是png图片
    *函数参数 ：path是图片文件的路径名加文件名
    *返回值   ：如果是png则返回0，不是或出错返回-1
*/
static int is_png(char *path) {
	FILE *fp = NULL;
	char buf[PNG_BYTES_TO_CHECK] = {0}; 

	/* Open the prospective PNG file. */   
	if ((fp = fopen(path, "rb")) == NULL)		
		return -1;	

	/* Read in some of the signature bytes */	
	if (fread(buf, 1, PNG_BYTES_TO_CHECK, fp) != PNG_BYTES_TO_CHECK)	   
		return -1; 
	//比较头文件标识，是png格式则返回0，不是返回非0
	//png 文件头标识 (8 bytes)   89 50 4E 47 0D 0A 1A 0A	 
	return(png_sig_cmp(buf, (png_size_t)0, PNG_BYTES_TO_CHECK));
}

static int png_analyze (struct pic_info *pPic)
{
	int i, j, pos=0;
	png_structp png_ptr; //定义一个png指针
	png_infop info_ptr;
	png_bytep* row_ptr;  //实际存储rgb数据的buf，二维指针
	FILE *fp = NULL; 

	if ((fp = fopen(pPic->pathname, "rb")) == NULL) {
		// fprintf(stderr, "fopen %s error.\n", pPic->pathname);
		return -1;
	}
			
	//第1步：初始化相关结构体
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL)
	{
		fprintf(stderr, "init %s error.\n", pPic->pathname);
		fclose(fp);
		return -1;
	}
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		fprintf(stderr, "create %s error.\n", pPic->pathname);
		fclose(fp);
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return -1;
	}

	//第2步：设置错误返回点	
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);
		return -1;
	}

	//第3步：将解码的结构体和图片文件绑定
	png_init_io(png_ptr, fp);
	
	//第4步：读文件
	/*当内存足够大可以一次性读入所有的png数据，可以使用高层函数
	该函数将会把所有的图片数据解码到info_ptr数据结构中*/	 
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND | PNG_TRANSFORM_STRIP_ALPHA, NULL); 

	//第5步：提取打印文件信息
	pPic->width = info_ptr->width;
	pPic->height = info_ptr->height;
	pPic->bpp = info_ptr->pixel_depth;
	pPic->color_type = info_ptr->color_type; 	
	printf("picture resolution: %d*%d\n", pPic->width, pPic->height);
	printf("picture bpp: %d\n", pPic->bpp);
	printf("picture colortype: %d\n", pPic->color_type);
	
	// 第6步: 读取真正的图像信息
	row_ptr = png_get_rows(png_ptr, info_ptr);
	switch (info_ptr->color_type) {
		case PNG_COLOR_TYPE_RGB_ALPHA:
            pPic->pData = (unsigned char *)ak_mem_dma_alloc(1, pPic->width * pPic->height * 4);
			for (i = 0; i < pPic->height; i++) {
				for (j = 0; j < pPic->width*4; j+=4) {
					pPic->pData[pos++] = row_ptr[i][j+0]; // red
					pPic->pData[pos++] = row_ptr[i][j+1]; // green
					pPic->pData[pos++] = row_ptr[i][j+2]; // blue
				}
			}
			break;
		case PNG_COLOR_TYPE_RGB:
            pPic->pData = (unsigned char *)ak_mem_dma_alloc(1, pPic->width * pPic->height * 3);
            //  printf("rgbData :\n");
			for (i = 0; i < pPic->height; i++) {
				for (j = 0; j < pPic->width*3; j+=3) {
					pPic->pData[pos++] = row_ptr[i][j+0]; // red
					pPic->pData[pos++] = row_ptr[i][j+1]; // green
					pPic->pData[pos++] = row_ptr[i][j+2]; // blue	
                    // printf(" %02x%02x%02x ", row_ptr[i][j+0],row_ptr[i][j+1],row_ptr[i][j+2]);
				}
                // printf("\n");
			}
			break;
		default:
			break;	
	}
	
	//第7步：扫尾
	png_destroy_read_struct(&png_ptr, &info_ptr, 0);
	fclose(fp);
	return 0;	
}

/* add the  obj to GUI layer */
int add_logo_to_gui(void)
{
    /* get the logo file */
    struct pic_info pic_logo;
    pic_logo.pathname = "/usr/share/1024X600.png";
    png_analyze(&pic_logo);

    struct ak_vo_obj gui_obj;

    /* set obj src info*/	
    gui_obj.format = GP_FORMAT_BGR888;
    gui_obj.cmd = GP_OPT_SCALE;
    gui_obj.vo_layer.width = pic_logo.width;
    gui_obj.vo_layer.height = pic_logo.height;
    gui_obj.vo_layer.clip_pos.top = 0;
    gui_obj.vo_layer.clip_pos.left = 0;
    gui_obj.vo_layer.clip_pos.width = pic_logo.width;
    gui_obj.vo_layer.clip_pos.height = pic_logo.height;

    ak_mem_dma_vaddr2paddr(pic_logo.pData, &(gui_obj.vo_layer.dma_addr));

    /* set dst_layer 1 info*/
    gui_obj.dst_layer.top = 0;
    gui_obj.dst_layer.left = 0;
    gui_obj.dst_layer.width = 1024;
    gui_obj.dst_layer.height = 100;

    /* display obj 1*/
    ak_vo_add_obj(&gui_obj, AK_VO_LAYER_GUI_1);
	

    struct pic_info pic_logo2;
    pic_logo2.pathname = "/usr/share/100X500.png";
    png_analyze(&pic_logo2);

    struct ak_vo_obj gui_obj2;
    /* set obj src info*/	
    gui_obj2.format = GP_FORMAT_BGR888;
    gui_obj2.cmd = GP_OPT_SCALE;
    gui_obj2.vo_layer.width = pic_logo2.width;
    gui_obj2.vo_layer.height = pic_logo2.height;
    gui_obj2.vo_layer.clip_pos.top = 0;
    gui_obj2.vo_layer.clip_pos.left = 0;
    gui_obj2.vo_layer.clip_pos.width = pic_logo2.width;
    gui_obj2.vo_layer.clip_pos.height = pic_logo2.height;

    ak_mem_dma_vaddr2paddr(pic_logo2.pData, &(gui_obj2.vo_layer.dma_addr));

    /* set dst_layer 1 info*/
    gui_obj2.dst_layer.top = 0;
    gui_obj2.dst_layer.left = 0;
    gui_obj2.dst_layer.width = 100;
    gui_obj2.dst_layer.height = 500;

    /* display obj 1*/
    ak_vo_add_obj(&gui_obj2, AK_VO_LAYER_GUI_2);

    /* free source */
    ak_mem_dma_free(pic_logo.pData);
    ak_mem_dma_free(pic_logo2.pData);

    return AK_SUCCESS;
}


/**
 * Preconditions:
 * 1??TF card is already mounted
 * 2??yuv_data is already created in /mnt
 * 3??ircut is already opened at day mode
 * 4??your main video progress must stop
 */
int main(int argc, char **argv)
{
    /* start the application */
    sdk_run_config config;
    config.mem_trace_flag = SDK_RUN_DEBUG;
    ak_sdk_init( &config );

    ak_print_normal(MODULE_ID_VI, "*****************************************\n");
	ak_print_normal(MODULE_ID_VI, "** vi demo version: %s **\n", ak_vi_get_version());
    ak_print_normal(MODULE_ID_VI, "*****************************************\n");
	
	if (argc != 3) {
		ak_print_error(MODULE_ID_VI, "%s [isp config path] [yuv store path]!\n", argv[0]);	
		return -1;
	}

	/* 
	 * step 0: global value initialize
	 */
	int ret = -1;								//return value
	char *cfg = argv[1];			//isp config file storage path
	yuv_path = argv[2];
#if 0
	int width = 720;
	int height = 288;
	int subwidth = 360;
	int subheight = 144;
#elif 0
	int width = 180;
	int height = 96;
	int subwidth = 96;
	int subheight = 96;
#elif 0
	int width = 320;
	int height = 96;
	int subwidth = 320;
	int subheight = 96;
#elif 0
	int width = 640;
	int height = 160;
	int subwidth = 320;
	int subheight = 96;
#elif 0
	int width = 640;
	int height = 320;
	int subwidth = 320;
	int subheight = 96;
#elif 0
	int width = 720;
	int height = 288;
	int subwidth = 320;
	int subheight = 96;
#elif 0
	int width = 992;
	int height = 320;
	int subwidth = 320;
	int subheight = 96;
#elif 0
	int width = 1184;
	int height = 320;
	int subwidth = 320;
	int subheight = 96;
#elif 0
	int width = 1184;
	int height = 704;
	//int subwidth = 320;
	//int subheight = 96;
#elif 1
	int width = 1280;
	int height = 720;
	//int subwidth = 320;
	//int subheight = 96;
#else
	int width = 1984;
	int height = 1472;
	int subwidth = 320;
	int subheight = 96;
#endif

	/* open vo device */
	struct ak_vo_param vo_param = {0};
	vo_param.width = 1024;
	vo_param.height = 600;
	vo_param.format = GP_FORMAT_RGB888;
	vo_param.rotate = AK_GP_ROTATE_NONE;
    printf("------------%s %d------------\n",__func__,__LINE__);
    /* start vo open */
	ret = ak_vo_open(&vo_param, DEV_NUM);
	if(ret != AK_SUCCESS)
	{
		ak_print_error_ex(MODULE_ID_VI, "ak_vo_open failed![%d]\n",ret);
		return ret;
	}
    printf("------------%s %d------------\n",__func__,__LINE__);

	/* create the gui layer */
    struct ak_vo_layer_in gui_layer;
    /* only support the given log file */
    gui_layer.create_layer.width  = 1024;    /* logo res */
    gui_layer.create_layer.height = 100;    /* logo res */
    gui_layer.create_layer.left  = 0;       /* logo pos */
    gui_layer.create_layer.top   = 0;       /* logo pos */
    gui_layer.format             = GP_FORMAT_RGB888;  /* gui layer format */

    gui_layer.layer_opt          = GP_OPT_COLORKEY; /* support the colorkey opt */

    gui_layer.colorkey.coloract = COLOR_DELETE;       /* delete the color */
    gui_layer.colorkey.color_min = 0xFFFF00;        /* min value */
    gui_layer.colorkey.color_max = 0xFFFFFF;        /* max value */

    struct ak_vo_layer_out gui_info;                /* output the gui layer info */
    ret = ak_vo_create_gui_layer(&gui_layer, AK_VO_LAYER_GUI_1, &gui_info);
    if(ret != 0)
    {
        ak_print_error_ex(MODULE_ID_VO, "ak_vo_create_gui_layer failed![%d]\n",ret);
        ak_vo_destroy_layer(AK_VO_LAYER_VIDEO_1);
        ak_vo_close(DEV_NUM);
        return AK_FAILED;	
    }

	/* create the gui layer */
    struct ak_vo_layer_in gui_layer2;
    /* only support the given log file */
    gui_layer2.create_layer.width  = 100;    /* logo res */
    gui_layer2.create_layer.height = 500;    /* logo res */
    gui_layer2.create_layer.left  = 0;       /* logo pos */
    gui_layer2.create_layer.top   = 100;       /* logo pos */
    gui_layer2.format             = GP_FORMAT_RGB888;  /* gui layer format */

    gui_layer2.layer_opt          = GP_OPT_COLORKEY; /* support the colorkey opt */

    gui_layer2.colorkey.coloract = COLOR_DELETE;       /* delete the color */
    gui_layer2.colorkey.color_min = 0xFFFF00;        /* min value */
    gui_layer2.colorkey.color_max = 0xFFFFFF;        /* max value */

    struct ak_vo_layer_out gui_info2;                /* output the gui layer info */
    ret = ak_vo_create_gui_layer(&gui_layer2, AK_VO_LAYER_GUI_2, &gui_info2);
    if(ret != 0)
    {
        ak_print_error_ex(MODULE_ID_VO, "ak_vo_create_gui_layer failed![%d]\n",ret);
        ak_vo_destroy_layer(AK_VO_LAYER_VIDEO_1);
        ak_vo_close(DEV_NUM);
        return AK_FAILED;	
    }
	
	/* create the video layer */
	struct ak_vo_layer_in video_layer;
	video_layer.create_layer.width  = vo_param.width-100;
	video_layer.create_layer.height = vo_param.height-100;
	video_layer.create_layer.left  = 100;
	video_layer.create_layer.top   = 100;
	video_layer.layer_opt          = 0;
    //video_layer.format             = GP_FORMAT_RGB888;
    /* create the video_1 */
	ak_vo_create_video_layer(&video_layer, AK_VO_LAYER_VIDEO_1);
    printf("------------%s %d------------\n",__func__,__LINE__);
    
    /* use the double buff mode */
    ak_vo_set_fbuffer_mode(AK_VO_BUFF_DOUBLE);
	/* add logo to gui layer */
    ret = add_logo_to_gui();
    if(ret != 0)
    {
        ak_print_error_ex(MODULE_ID_VO, "add_logo_to_gui failed![%d]\n",ret);
        goto exit;	
    }

	/* open vi flow */

	/* 
	 * step 1: open video input device
	 */
	ret = ak_vi_open(VIDEO_DEV0);
	if (AK_SUCCESS != ret) {
		ak_print_error_ex(MODULE_ID_VI, "vi device open failed\n");	
		goto exit;
	}

	/*
	 * step 2: load isp config
	 */
	ret = ak_vi_load_sensor_cfg(VIDEO_DEV0, cfg);
	if (AK_SUCCESS != ret) {
		ak_print_error_ex(MODULE_ID_VI, "vi device load cfg [%s] failed!\n", cfg);	
		goto exit;
	}

	/* 
	 * step 3: get sensor support max resolution
	 */
	struct video_resolution res;				//max sensor resolution
	VI_DEV_ATTR	dev_attr;
	dev_attr.dev_id = VIDEO_DEV0;
	dev_attr.crop.left = 0;
	dev_attr.crop.top = 0;
	dev_attr.crop.width = width;
	dev_attr.crop.height = height;
	dev_attr.max_width = width;
	dev_attr.max_height = height;

	ret = ak_vi_get_sensor_resolution(VIDEO_DEV0, &res);
	if (ret) {
		ak_vi_close(VIDEO_DEV0);
		goto exit;
	} else {
		ak_print_normal_ex(MODULE_ID_VI, "get dev res w:[%d]h:[%d]\n",res.width, res.height);
		dev_attr.crop.width = res.width;
		dev_attr.crop.height = res.height;
	}

	/* 
	 * step 4: set vi working parameters 
	 * default parameters: 25fps, day mode, auto frame-control
	 */
	ret = ak_vi_set_dev_attr(VIDEO_DEV0, &dev_attr);
	if (ret) {
		ak_print_error_ex(MODULE_ID_VI, "vi device set device attribute failed!\n");
		ak_vi_close(VIDEO_DEV0);
		goto exit;
	}

	/*
	 * step 5: set channel attribute
	 */
	VI_CHN_ATTR chn_attr;
	chn_attr.chn_id = VIDEO_CHN0;
	chn_attr.res.width = width;
	chn_attr.res.height = height;
	ret = ak_vi_set_chn_attr(VIDEO_CHN0, &chn_attr);
	if (ret) {
		ak_print_error_ex(MODULE_ID_VI, "vi device set channel attribute failed!\n");
		ak_vi_close(VIDEO_DEV0);
		goto exit;
	}

	ak_print_normal_ex(MODULE_ID_VI, "vi device set sub channel\n");
#if 0
	int subwidth = 320;
	int subheight = 96;
	chn_attr.chn_id = VIDEO_CHN1;
	chn_attr.res.width = subwidth;
	chn_attr.res.height = subheight;
	ret = ak_vi_set_chn_attr(VIDEO_CHN1, &chn_attr);
	if (ret) {
		ak_print_error_ex(MODULE_ID_VI, "vi device set channel attribute failed!\n");
		ak_vi_close(VIDEO_DEV0);
		goto exit;
	}
#endif

	/* 
	 * step 6: start capture frames
	 */
	ret = ak_vi_enable_dev(VIDEO_DEV0);
	if (ret) {
		ak_print_error_ex(MODULE_ID_VI, "vi device enable device  failed!\n");
		ak_vi_close(VIDEO_DEV0);
		goto exit;
	}

	/* 
	 * step 6: start to capture and save yuv frames 
	 */
	vi_capture_loop(VIDEO_DEV0, save_num, yuv_path, &chn_attr);
	ak_sleep_ms(5000);

	/*
	 * step 7: release resource
	 */
	ak_vi_disable_dev(VIDEO_DEV0);

	ret = ak_vi_close(VIDEO_DEV0);



exit:
    /* destroy the layer, release the src */
    ak_vo_destroy_layer(AK_VO_LAYER_VIDEO_1);
    ak_vo_destroy_layer(AK_VO_LAYER_GUI_1);

    /* close the vo */
    ak_vo_close(DEV_NUM);

	/* exit */
	ak_print_normal(MODULE_ID_VI, "exit vi demo\n");
	return ret;
}
