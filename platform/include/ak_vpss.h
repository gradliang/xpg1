#ifndef __AK_VPSS_H__
#define __AK_VPSS_H__

/********************** effect *******************************/
enum vpss_effect_type {
	/* HUE to SHARP, value: [-50, 50], 0 means use the value in ISP config file */
	VPSS_EFFECT_HUE = 0x00,
	VPSS_EFFECT_BRIGHTNESS,
	VPSS_EFFECT_SATURATION,
	VPSS_EFFECT_CONTRAST,
	VPSS_EFFECT_SHARP,
	
	VPSS_STYLE_ID,		//[0, 2]
	VPSS_POWER_HZ		//50 or 60
};

/********************** public *******************************/

/**
 * ak_vpss_get_version - get vpss version
 * return: version string
 */
const char *ak_vpss_get_version(void);


/**  
 * ak_vpss_its_start - start anyka ISP tool server
 * notes: If you wan't use ISP tool to debug, do not start this server.
 */
void ak_vpss_its_start(void);

/**  
 * ak_vpss_its_stop - stop anyka ISP tool server
 * notes:
 */
void ak_vpss_its_stop(void);


#endif
