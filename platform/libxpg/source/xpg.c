#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xpg.h"

typedef struct {
	u32_t		offset;
	u32_t		size;
}RoleOffSizeInfo;

typedef struct {
	FILE *				fileHandle;
	RoleOffSizeInfo*	rolesInfo;
	u32_t				roleNum;
}XpgFileInfo;

typedef struct {
	char		fileName[100];
	u32_t		filesize;
}CommonFileInfo;

RawRoleInfo   * g_rawRoles = NULL;
RawSpriteInfo * g_rawSprites = NULL;
RawCommandInfo* g_rawCommands = NULL;
RawPageInfo   * g_rawPages = NULL;
u32_t           g_rawRoleNum = 0;
u32_t           g_rawSpriteNum = 0;
u32_t           g_rawCommandNum = 0;
u32_t           g_rawPageNum = 0;

static XpgFileInfo  xpgFileInfo;

static int getHeadDataInfo(u8_t * data, CommonFileInfo * pFileInfo);
static u32_t o2u(const u8_t * str);
static u32_t getU32(const u8_t * buffer, u32_t index);
static u16_t getU16(const u8_t * buffer, u32_t index);

RawRoleInfo * getRawRoleList()
{
	return g_rawRoles;
}

RawSpriteInfo * getRawSpriteList()
{
	return g_rawSprites;
}

RawCommandInfo * getRawCmdList()
{
	return g_rawCommands;
}

RawPageInfo * getRawPageList()
{
	return g_rawPages;
}

u32_t  getRawRoleTotal()
{
	return g_rawRoleNum;
}
u32_t  getRawSpriteTotal()
{
	return g_rawSpriteNum;
}

u32_t  getRawCmdTotal()
{
	return g_rawCommandNum;
}

u32_t  getRawPageTotal()
{
	return g_rawPageNum;
}

int loadXpg(const char * filepath)
{
	int errcode = 0;
	XpgFileInfo * xpg;
	FILE * xpghandle;
	char flag[8];
	CommonFileInfo fileinfo;
	u8_t headdata[512];
	size_t  rdsize, filecount;
	u32_t skipsize;
	u32_t i, j, k;
	u8_t *	xpgdata = NULL;				// xpg head file data
	u32_t	xpgsize;					// xpg head file size
	
	if (filepath == NULL || filepath[0] == 0) {
		printf("-E- %s() param is NULL. \n", __FUNCTION__);
		return -1;
	}
	xpg = &xpgFileInfo;
	memset(xpg, 0, sizeof(xpg));

	xpghandle = fopen(filepath, "rb");
	if (xpghandle == NULL) {
		printf("-E- xpg file open error \n");
		return -1;
	}

	fread(flag, 1, 8, xpghandle);
	if (0 != strncmp(flag, "index.xpg", 8)) {
		printf("-E- xpg file format error, flag error \n");
		fclose(xpghandle);
		return -1;
	}

	fseek(xpghandle, 0x020c, SEEK_SET);
	fread(&xpg->roleNum, 1, 4, xpghandle);
	xpg->rolesInfo = (RoleOffSizeInfo*) malloc((xpg->roleNum + 1) * sizeof(RoleOffSizeInfo));
	memset(xpg->rolesInfo, 0, (xpg->roleNum + 1) * sizeof(RoleOffSizeInfo));
	//printf("roleNum = %d\n", xpg->roleNum);

	fseek(xpghandle, 0, SEEK_SET);

	filecount = 0;
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

		if (filecount != 0) {
			xpg->rolesInfo[filecount - 1].offset = ftell(xpghandle);
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
		if (filecount == 0) {
			xpgdata = (u8_t*) malloc(skipsize);
			xpgsize = fileinfo.filesize;
			fread(xpgdata, 1, skipsize, xpghandle);
		}
		else {
			fseek(xpghandle, skipsize, SEEK_CUR);
		}

		if (filecount != 0)
			xpg->rolesInfo[filecount - 1].size = fileinfo.filesize;

		filecount ++;
		//printf("file=%s, size=%d \n", fileinfo.fileName, fileinfo.filesize);
	}

	//for (i = 0; i < xpg->roleNum; i++) {
	//	printf("%03d. offset = 0x%08x, size = %d\n", i, xpg->rolesInfo[i].offset, xpg->rolesInfo[i].size);
	//}
	
	fclose(xpghandle);

	if (errcode) {
		goto XpgHeadDataFormatFail;
	}
	
	// parse xpg head
	char XPG6[4];
	u32_t  version;
	u32_t  roleOffset;
	u32_t  pageHeadOffset;
	memcpy(XPG6, &xpgdata[0], 4);
	if (0 != strncmp(XPG6, "XPG6", 4)) {
		printf("-E- xpg flag error, file break \n");
		goto XpgHeadDataFormatFail;
	}
	version = getU32(xpgdata, 4);
	if (version != 1) {
		printf("-E- xpg format version error, version != 1 \n");
		goto XpgHeadDataFormatFail;
	}
	roleOffset = getU32(xpgdata, 8);
	g_rawRoleNum = getU32(xpgdata, 12);
	pageHeadOffset = getU32(xpgdata, 16);
	g_rawPageNum = getU32(xpgdata, 20);

	g_rawRoles = (RawRoleInfo*) malloc((g_rawRoleNum + 1) * sizeof(RawRoleInfo));
	memset(g_rawRoles, 0, (g_rawRoleNum + 1) * sizeof(RawRoleInfo));

	// role info
	for (i = 0; i < g_rawRoleNum; i ++ ) 
	{
		g_rawRoles[i].width = getU32(xpgdata, roleOffset + 32 * i + 0);
		g_rawRoles[i].height = getU32(xpgdata, roleOffset + 32 * i + 4);
	}

	// page info
	g_rawSpriteNum = 0;
	g_rawCommandNum = 0;
	for (i = 0; i < g_rawPageNum; i ++ )
	{
		u32_t spriteNum = getU32(xpgdata, pageHeadOffset + 32 * i + 8);
		u32_t cmdNum = getU32(xpgdata, pageHeadOffset + 32 * i + 16);
		g_rawSpriteNum += spriteNum;
		g_rawCommandNum += cmdNum;
	}

	g_rawSprites = (RawSpriteInfo*) malloc((g_rawSpriteNum + 1) * sizeof(RawSpriteInfo));
	memset(g_rawSprites, 0, (g_rawSpriteNum + 1) * sizeof(RawSpriteInfo));

	g_rawCommands = (RawCommandInfo*) malloc((g_rawCommandNum + 1) * sizeof(RawCommandInfo));
	memset(g_rawCommands, 0, (g_rawCommandNum + 1) * sizeof(RawCommandInfo));

	g_rawPages = (RawPageInfo*) malloc((g_rawPageNum + 1) * sizeof(RawPageInfo));
	memset(g_rawPages, 0, (g_rawPageNum + 1) * sizeof(RawPageInfo));

	//printf("pageTotal = %d, role total = %d, sprite total = %d, command total = %d\n", g_rawPageNum, g_rawRoleNum, g_rawSpriteNum, g_rawCommandNum);

	// read page
	u32_t spriteCounter = 0;
	u32_t commandCounter = 0;
	for (i = 0; i < g_rawPageNum; i ++ )
	{
		RawPageInfo * pstPage = & g_rawPages[i];
		pstPage->pageIndex = getU32(xpgdata, pageHeadOffset + 32 * i + 0);
		u32_t offsetSprites = getU32(xpgdata, pageHeadOffset + 32 * i + 4);
		pstPage->numSprites = getU32(xpgdata, pageHeadOffset + 32 * i + 8);
		u32_t offsetCmd = getU32(xpgdata, pageHeadOffset + 32 * i + 12);
		pstPage->numCmd = getU32(xpgdata, pageHeadOffset + 32 * i + 16);
		pstPage->hashKey = getU32(xpgdata, pageHeadOffset + 32 * i + 20);
		pstPage->spriteBegin = spriteCounter;
		pstPage->commandBegin = commandCounter;

		for (j = 0; j < pstPage->numSprites; j++)
		{
			RawSpriteInfo * pstSprite = & g_rawSprites[spriteCounter];
			pstSprite->index = getU32(xpgdata, offsetSprites + 52 * j + 0);
			pstSprite->x = getU32(xpgdata, offsetSprites + 52 * j + 4);
			pstSprite->y = getU32(xpgdata, offsetSprites + 52 * j + 8);
			pstSprite->roleId = getU32(xpgdata, offsetSprites + 52 * j + 12);
			pstSprite->typeFlag = getU32(xpgdata, offsetSprites + 52 * j + 16);
			pstSprite->touchEnable = getU32(xpgdata, offsetSprites + 52 * j + 20);
			pstSprite->touchFlag = getU32(xpgdata, offsetSprites + 52 * j + 24);
			pstSprite->flag = getU32(xpgdata, offsetSprites + 52 * j + 28);
			spriteCounter++;
		}

		for (k = 0; k < pstPage->numCmd; k++)
		{
			RawCommandInfo * pstCommand = & g_rawCommands[commandCounter];
    		pstCommand->eventKey = getU16(xpgdata, offsetCmd + 4 + 8 * k + 0);
    		pstCommand->pageKey = getU16(xpgdata, offsetCmd + 4 + 8 * k + 2);
    		pstCommand->actionKey = getU16(xpgdata, offsetCmd + 4 + 8 * k + 4);
			commandCounter++;
			//printf("event=%d, page=%d, action=%d\n", pstCommand->eventKey, pstCommand->pageKey, pstCommand->actionKey);
		}
	}

	xpg->fileHandle = fopen(filepath, "rb");
	printf("xpg load OK. \n");
	errcode = 0;
	goto End;

XpgHeadDataFormatFail:
	errcode = -1;
	printf("-E- xpg load fail. \n");
End:
	if (xpgdata) {
		free(xpgdata);
		xpgdata = NULL;
	}
	return errcode;
}

int closeXpg()
{
	XpgFileInfo * xpg = &xpgFileInfo;

	if (xpg->fileHandle != NULL) {
		fclose(xpg->fileHandle);
		xpg->fileHandle = NULL;
	}
	if (xpg->rolesInfo != NULL) {
		free(xpg->rolesInfo);
		xpg->rolesInfo = NULL;
		xpg->roleNum = 0;
	}
	return PASS;
}

u32_t getXpgImageSize(u32_t roleIndex)
{
	XpgFileInfo * xpg = &xpgFileInfo;
	if (roleIndex >= xpg->roleNum)
		return 0;
	return xpg->rolesInfo[roleIndex].size;
}

u32_t getXpgReadImage(u32_t roleIndex, u8_t * buffer)
{
	u32_t n;
	XpgFileInfo * xpg = &xpgFileInfo;
	if (roleIndex >= xpg->roleNum)
		return 0;
	if (xpg->fileHandle == NULL)
		return 0;
	if (buffer == NULL)
		return 0;
	fseek(xpg->fileHandle, xpg->rolesInfo[roleIndex].offset, SEEK_SET);
	n = fread(buffer, 1, xpg->rolesInfo[roleIndex].size, xpg->fileHandle);
	return n;
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

static u32_t getU32(const u8_t * b, u32_t index)
{
	return (b[index + 3] << 24) | (b[index + 2] << 16) | (b[index + 1] << 8) | b[index];
}

static u16_t getU16(const u8_t * b, u32_t index)
{
	return (b[index + 1] << 8) | b[index];
}

