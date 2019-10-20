#ifndef _AK_VIDEO_INPUT_H_
#define _AK_VIDEO_INPUT_H_

typedef int VI_DEV;
typedef int VI_CHN;

#define VIDEO_DEV0	0
#define VIDEO_DEV1	1

#define VIDEO_CHN0	0
#define VIDEO_CHN1	1
#define VIDEO_CHN2	2
#define VIDEO_CHN3	3

/* vi module errno list */
enum ak_vi_errno {                                                            //错误代码
	ERROR_VI_INVALID_DEVID					= ( MODULE_ID_VI << 24 ) + 0,
	ERROR_VI_CHANNEL_NOT_DISABLE			= ( MODULE_ID_VI << 24 ) + 1,
	ERROR_VI_DEVID_NOT_OPEN					= ( MODULE_ID_VI << 24 ) + 2,
	ERROR_VI_DEVID_NOT_MATCH				= ( MODULE_ID_VI << 24 ) + 3,
	ERROR_VI_ATTR_PARM_WRONG				= ( MODULE_ID_VI << 24 ) + 4,
	ERROR_VI_INVALID_CHNID					= ( MODULE_ID_VI << 24 ) + 5,
	ERROR_VI_REGISTER_CHANNEL_FAIL			= ( MODULE_ID_VI << 24 ) + 6,
	ERROR_VI_CHANNEL_DEVICE_NOT_OPEN		= ( MODULE_ID_VI << 24 ) + 7,
	ERROR_VI_DEV_NOT_ENABLE					= ( MODULE_ID_VI << 24 ) + 8,
};

typedef enum AK_VI_DATA_PATH {
	VI_PATH_BYPASS = 0,								//ISP is bypass
	VI_PATH_ISP = 1,								//ISP enable
	VI_PATH_RAW = 2,								//capture raw data, for debug
	VI_PATH_TOTAL
}VI_DATA_PATH_E;

typedef enum AK_VI_DATA_TYPE {
	VI_DATA_TYPE_YUV420SP = 0,						//yuv 数据类型
	VI_DATA_TYPE_RGB = 1,							//rgb数据类型
	VI_DATA_TYPR_RAW = 2,
	VI_DATA_TYPE_TOTAL
}VI_DATA_TYPE_E;

typedef enum AK_VI_INTF_MODE {
	VI_INTF_DVP = 0,								//DVP 并行接口
	VI_INTF_MIPI_1 = 1,								//MIPI串行1线接口
	VI_INTF_MIPI_2 = 2,								//MIPI串行2线接口
	VI_INTF_TOTAL
}VI_INTF_MODE_E;

struct crop_info {
	int		left;	// x position of crop
	int		top;	// y position of crop
	int		width;	// width of crop
	int		height;	// height of crop
};

typedef struct AK_VI_DEV_ATTR{
	VI_DEV 				dev_id;						//vi设备号
	VI_INTF_MODE_E 		interf_mode;				//interface mode, dvp/mipi1/mipi2
	int					fd;							//vi设备对应的设备文件描述符
	VI_DATA_PATH_E		data_path;					//数据路径，1为启用ISP
	VI_DATA_TYPE_E		data_type;					//数据类型，RGB/ YUV
	struct crop_info	crop;						//crop info of device	
	int 				max_width;					//max value of width of resolution
	int					max_height;					//max value of height of resolution
	int					frame_rate;					//source frame_rate of the ISP 
}VI_DEV_ATTR;

typedef struct AK_VI_RECTANGLE{
	int		width;									//width lenght of resolution
	int		height;									//height lenght of resolution
}RECTANGLE_S;

typedef struct AK_VI_CHN_ATTR {
	VI_CHN			chn_id;							//VI 通道号。
	int				frame_rate;						//当前通道的目标帧率
	RECTANGLE_S		res;							//目标分辨率
	int				frame_depth;					//frame buffer number of the channel
}VI_CHN_ATTR;

struct video_resolution {
	int		width;
	int		height;
};

enum video_channel {
	VIDEO_CHN_MAIN,
	VIDEO_CHN_SUB,
	VIDEO_CHN_NUM
};

struct video_input_frame {
	struct frame vi_frame;
	unsigned long phyaddr;
	int	type;
	void *mdinfo;
};

/* ak_vi_get_version
 * return the vi lib version
 */
const char* ak_vi_get_version(void);

/**
 * ak_vi_open: open video input device
 * @dev_id[IN]: video input device ID
 * return: AK_SUCCESS if successful, error code if failed
 * notes:
 */
int  ak_vi_open(VI_DEV dev_id);

/**
 * ak_vi_close: close video input device
 * @dev_id[IN]: video input device ID
 * return: AK_SUCCESS if successful, error code if failed
 * notes:
 */
int  ak_vi_close(VI_DEV dev_id);

/* ak_vi_load_sensor_cfg 	-- load isp config file 
 * dev_id[IN]		: 	video input device ID
 * config_file[IN]	:	config file path 
 * return AK_SUCCESS if success, Error Code if failed
 */
int ak_vi_load_sensor_cfg(VI_DEV dev_id, const char *config_file);

/* ak_vi_get_sensor_resolution	---	 get sensor resolution by device id
 * dev_id[IN]		: 	video input device ID
 * res[OUT]			:	pointer to record the resolution of vi device 	
 * return AK_SUCCESS if success, AK_FAILED if failed
 */
int ak_vi_get_sensor_resolution(VI_DEV dev_id, struct video_resolution *res);

/* ak_vi_set_dev_attr -- set dev basic attr
 * dev_id[IN] : 	video input device ID
 * dev_attr   :		video ipnout device attribution
 * return AK_SUCCESS if successful, error code if failed
 */
int ak_vi_set_dev_attr(VI_DEV dev_id,  VI_DEV_ATTR *dev_attr);

/* ak_vi_get_dev_attr -- set dev basic attr
 * dev_id[IN] : 	video input device ID
 * dev_attr[OUT]   :		video ipnout device attribution
 * return AK_SUCCESS if successful, error code if failed
 */
int ak_vi_get_dev_attr(VI_DEV dev_id, VI_DEV_ATTR *dev_attr);

/* ak_vi_set_chn_attr	--	set channel attribute
 * chn_id[IN]			: 		channal id
 * chn_attr[IN]			:		channel attribute
 */
int ak_vi_set_chn_attr(VI_CHN chn_id, VI_CHN_ATTR *chn_attr);

/* ak_vi_enable_dev  --		enable vi device to start capture
 * dev_id[IN]			: 		vi device id
 * return AK_SUCCESS if success, error code if failed
 * */
int ak_vi_enable_dev(VI_DEV dev_id);

/* ak_vi_disable_dev  --		disable vi device to start capture
 * dev_id[IN]			: 		vi device id
 * return AK_SUCCESS if success, error code if failed
 * */
int ak_vi_disable_dev(VI_DEV dev_id);

/**
 * ak_vi_get_frame: get frame
 * chn_id	:	channel id 
 * @frame[OUT]: store frames
 * return: 0 success, otherwise failed
 */
int ak_vi_get_frame(VI_CHN chn_id, struct video_input_frame *frame);

/**
 * ak_vi_release_frame:  release  frame
 * chn_id	:	channel id 
 * @frame[IN]:  pointer to vi frames
 * return: AK_SUCCESS if  success, error code if failed
 */
int ak_vi_release_frame(VI_CHN chn_id, struct video_input_frame *frame);
#if 0
#include "ak_common.h"

enum video_dev_type {
	VIDEO_DEV0 = 0x00,
	VIDEO_DEV_NUM
};

enum encode_use_type {
	VIDEO_ENCODE = 0x00,
	MOVE_DETECT,
	RAW_DATA
};

enum video_channel {
	VIDEO_CHN_MAIN,
	VIDEO_CHN_SUB,
	VIDEO_CHN_NUM
};

enum video_daynight_mode {
	VI_MODE_DAY_OUTDOOR,
	VI_MODE_NIGHTTIME,
	VI_MODE_DAY_INDOOR,
	VI_MODE_NUM
};

enum video_work_scene {
	VIDEO_SCENE_UNKNOWN = -1,
	VIDEO_SCENE_INDOOR = 0x00,
	VIDEO_SCENE_OUTDOOR
};


/* video input module config */
struct video_channel_attr {
	struct crop_info crop;
	struct video_resolution res[VIDEO_CHN_NUM];
};

struct video_input_frame {
	struct frame vi_frame[VIDEO_CHN_NUM];
	void *mdinfo;
};

/**
 * ak_vi_get_version - get video in version
 * return: version string
 */
const char* ak_vi_get_version(void);

/**  
 * ak_vi_match_sensor: match sensor according to appointed config file.
 * @config_file[IN]: it can be config file absolutely path, or a directory name.
 * If it is a directory name, it will search config file which format is 
 * correct to match.
 * return: 0 success, -1 failed
 * notes:
 */
int ak_vi_match_sensor(const char *config_file);

/**
 * ak_vi_open: open video input device
 * @dev[IN]: video input device ID, for more detail, see defination of 
 *          'enum video_dev_type'
 * return: 0 success, -1 failed
 * notes: must call this after 'ak_vi_match_sensor()' success.
 */
void* ak_vi_open(enum video_dev_type dev);

/**
 * ak_vi_get_channel_attr: get channel attribution
 * @handle[IN]: return by 'ak_vi_open()'
 * @attr[OUT]: channel attribution
 * return: 0 success, -1 failed
 * notes:
 */
int ak_vi_get_channel_attr(void *handle, struct video_channel_attr *attr);

/**
 * ak_vi_set_channel_attr: set channel attribution
 * @handle[IN]: return by 'ak_vi_open()'
 * @attr[IN]: channel attribution
 * return: 0 success, -1 failed
 * notes:
 */
int ak_vi_set_channel_attr(void *handle,
		const struct video_channel_attr *attr);

/**
 * ak_vi_change_channel_attr - change channel attribution
 * @handle[IN]: return by 'ak_vi_open()'
 * @attr[IN]: new channel attribution
 * return: 0 success, -1 failed
 * notes: IMPORTANT-you can change channel attribution in real time.
 */
int ak_vi_change_channel_attr(const void *handle,
		const struct video_channel_attr *attr);

/**
 * ak_vi_get_flip_mirror - get flip and mirror
 * @handle[IN]: return by 'ak_vi_open()'
 * @flip_enable[OUT]: flip flag
 * @mirror_enable[OUT]: mirror flag
 * return: 0 success, -1 failed
 * notes:
 */
int ak_vi_get_flip_mirror(void *handle, int *flip_enable, int *mirror_enable);

/**
 * ak_vi_set_flip_mirror - set flip and mirror
 * @handle[IN]: return by 'ak_vi_open()'
 * @flip_enable[IN]:flip flag
 * @mirror_enable[IN]:mirror flag
 * return: 0 success, -1 failed
 * notes:
 */
int ak_vi_set_flip_mirror(void *handle, int flip_enable, int mirror_enable);

/**
 * ak_vi_capture_on - open isp capture
 * @handle[IN], return by 'ak_vi_open()'
 * return: 0 success, -1 failed
 * notes:
 */
int ak_vi_capture_on(void *handle);

/*
 * ak_vi_capture_off - close isp capture
 * @handle[IN], return by 'ak_vi_open()'
 * return: 0 success, -1 failed
 * notes:
 */
int ak_vi_capture_off(void *handle);

/**
 * ak_vi_reset - reset vi device
 * @handle[IN]: return by ak vi open
 * return: 0 success, -1 failed
 */
int ak_vi_reset(void *handle);

/**
 * ak_vi_get_frame: get frame
 * @handle[IN]: return by 'ak_vi_open()'
 * @frame[OUT]: store frames
 * return: 0 success, otherwise failed
 */
int ak_vi_get_frame(void *handle, struct video_input_frame *frame);

/**
 * ak_vi_release_frame: release frame buffer after used
 * @handle[IN]: return by 'ak_vi_open()'
 * @frame[IN]: store frames
 * return: 0 success, otherwise failed
 * notes:
 */
int ak_vi_release_frame(void *handle, struct video_input_frame *frame);

/**
 * ak_vi_get_fps - get current capture fps
 * @handle[IN]: return by 'ak_vi_open()'
 * return: fps value
 * notes:this fps is sensor fps, not video encode fps
 */
int ak_vi_get_fps(void *handle);

/**
 * ak_vi_set_fps - set capture fps
 * @handle[IN]: return by 'ak_vi_open()'
 * @fps[IN]: the fps value to be set
 * return: 0 success, -1 failed
 * notes: this fps is sensor fps , not video encode fps;
 * 		 the value of 'fps' do not out of range [1, sensor capture max fps]
 */
int ak_vi_set_fps(void *handle, int fps);

/**
 * ak_vi_switch_mode - switch day/night mode
 * @handle[IN]: return by 'ak_vi_open()'
 * @mode[IN]: for more detail see defination of enum video_daynight_mode 
 * return: 0 success, -1 failed
 * notes:
 */
int ak_vi_switch_mode(void *handle, enum video_daynight_mode mode);

/**
 * ak_vi_get_sensor_resolution - get sensor max resolution supported
 * @handle[IN]: return by 'ak_vi_open()'
 * @res[OUT]: store sensor resolution information
 * return: 0 success, -1 failed
 * notes:
 */
int ak_vi_get_sensor_resolution(void *handle, struct video_resolution *res);

/**
 * ak_vi_set_switch_fps_enable - enable or disable auto-switch fps function 
 * @handle[IN]: return by 'ak_vi_open()' 
 * @enable[IN]: enable = 1, disable = 0
 * return: 0 success, -1 failed
 * notes:
 */
int ak_vi_set_switch_fps_enable(void *handle, int enable);

/**
 * ak_vi_get_handle - get vi handle
 * @dev[IN]:device in enum video_dev_type
 * return: handle pointer, NULL failed
 * notes:
 */
void *ak_vi_get_handle(enum video_dev_type dev);

/**
 * ak_vi_get_work_scene - get appointed video device's current work scene
 * @dev[IN]: device in enum video_dev_type
 * return: current work scene
 * notes:
 */
enum video_work_scene ak_vi_get_work_scene(enum video_dev_type dev);

/**
 * ak_vi_start_scene_switch: start scene switch
 * return: void
 */
void ak_vi_start_scene_switch(void);

/**
 * ak_vi_stop_scene_switch: stop scene switch 
 * return: void
 */
void ak_vi_stop_scene_switch(void);

/**
 * ak_vi_get_scene_mode: get current scene mode 
 * return: enum video_daynight_mode
 */
enum video_daynight_mode ak_vi_get_scene_mode(void);

/**
 * ak_vi_clear_buffer - clear vi capture buffer
 * @handle[IN]: return by ak vi open
 * return: 0 success, -1 failed
 * notes: if stopped encoding at all
 *		you can call this function before encoding again
 */
int ak_vi_clear_buffer(void *handle);

/**
 * ak_vi_close - close vi and release resource
 * @handle[IN]: return by 'ak_vi_open()'
 * return: 0 success, -1 failed
 * notes:
 */
int ak_vi_close(void *handle);

/**
 * ak_vi_set_box_rect - set box rect info
 * @x[IN]: left of rect
 * @y[IN]: top of rect
 * @width[IN]: width of rect
 * @height[IN]: height of rect
 * return: 0 success, -1 failed
 * notes:
 */
int ak_vi_set_box_rect(int x, int y, int width, int height);
#endif
#endif
