#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xpg.h"

typedef struct {
	FILE *		fileHandle;
}XpgFileInfo;

typedef struct {
	char		fileName[100];
	u32_t		filesize;
}CommonFileInfo;

static int getHeadDataInfo(u8_t * data, CommonFileInfo * pFileInfo);
static u32_t o2u(const u8_t * str);

void* loadXpg(const char * filepath)
{
	int errcode = 0;
	XpgFileInfo * xpg;
	FILE * xpghandle;
	char flag[8];
	CommonFileInfo fileinfo;
	u8_t headdata[512];
	size_t  rdsize;
	u32_t skipsize;
	
	if (filepath == NULL || filepath[0] == 0) {
		printf("-E- %s() param is NULL. \n", __FUNCTION__);
		return NULL;
	}
	xpg = (XpgFileInfo*) malloc(sizeof(XpgFileInfo));
	if (xpg == NULL)
	{
		printf("-E- memory alloc fail in %s (%d) \n", __FILE__, __LINE__);
		return NULL;
	}
	memset(xpg, 0, sizeof(xpg));

	xpghandle = fopen(filepath, "rb");
	if (xpghandle == NULL) {
		printf("-E- xpg file open error \n");
		free(xpg);
		return NULL;
	}

	xpg->fileHandle = xpghandle;
	fread(flag, 1, 8, xpghandle);
	if (0 != strncmp(flag, "index.xpg", 8)) {
		printf("-E- xpg file format error, flag error \n");
		fclose(xpghandle);
		free(xpg);
		return NULL;
	}

	fseek(xpghandle, 0, SEEK_SET);

	while (1)
	{
		rdsize = fread(headdata, 1, 512, xpghandle);
		if (feof(xpghandle))
			break;
		if (rdsize != 512) {
			printf("-E- xpg file format error, file break \n");
			errcode = -1;
			break;
		}
		
		errcode = getHeadDataInfo(headdata, &fileinfo);
		if (errcode != 0) {
			printf("-E- xpg file format error, file break 2 \n");
			break;
		}
		skipsize = fileinfo.filesize / 512;
		skipsize *= 512;
		if (fileinfo.filesize % 512)
			skipsize += 512;
		fseek(xpghandle, skipsize, SEEK_CUR);

		//printf("file=%s, size=%d \n", fileinfo.fileName, fileinfo.filesize);
	}
	

	return xpg;
}

int closeXpg(void * handle)
{
	XpgFileInfo * xpg;

	if (handle == NULL)
		return FAIL;
	xpg = (XpgFileInfo*) handle;

	if (xpg->fileHandle != NULL) {
		fclose(xpg->fileHandle);
		xpg->fileHandle = NULL;
	}

	return PASS;
}

static int getHeadDataInfo(u8_t * data, CommonFileInfo * pFileInfo) 
{
	u8_t chkdata[512];
	u32_t myChecksum = 0;
	u32_t readChecksum;
	size_t i;

	memcpy(chkdata, data, 512);
	chkdata[100 + 8 + 8 + 8 + 12 + 12 + 0] = 0x20;
	chkdata[100 + 8 + 8 + 8 + 12 + 12 + 1] = 0x20;
	chkdata[100 + 8 + 8 + 8 + 12 + 12 + 2] = 0x20;
	chkdata[100 + 8 + 8 + 8 + 12 + 12 + 3] = 0x20;
	chkdata[100 + 8 + 8 + 8 + 12 + 12 + 4] = 0x20;
	chkdata[100 + 8 + 8 + 8 + 12 + 12 + 5] = 0x20;
	chkdata[100 + 8 + 8 + 8 + 12 + 12 + 6] = 0x20;
	chkdata[100 + 8 + 8 + 8 + 12 + 12 + 7] = 0x20;

	for (i = 0; i < 512; i++)
		myChecksum += chkdata[i];
	readChecksum = o2u(&data[100 + 8 + 8 + 8 + 12 + 12]);

	//printf("myChecksum = %o, checksum = %o \n", myChecksum, readChecksum);

	if (myChecksum != readChecksum) {
		printf("-E- xpg file format error, checksum error \n");
		return -1;
	}

	strncpy(pFileInfo->fileName, data, 100);
	pFileInfo->filesize = o2u(&data[100 + 8 + 8 + 8]);

	return 0;
}

static u32_t o2u(const u8_t * str)
{
	u32_t  r0 = 0;
	u32_t  i = 0;
	while (str[i] >= '0' && str[i] <= '7') {
		r0 <<= 3;
		r0 += (str[i] - '0');
		i++;
	}
	return r0;
}




