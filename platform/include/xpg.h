
#ifndef XPG_H_
#define XPG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define PASS    (0)
#define FAIL    (-1)

typedef unsigned char   u8_t;
typedef unsigned short  u16_t;
typedef unsigned int    u32_t;

typedef struct {
    u32_t       width;
    u32_t       height;
}RawRoleInfo;

typedef struct {
    u32_t index;
    u32_t x;
    u32_t y;
    u32_t roleId;
    u32_t typeFlag;
    u32_t flag;
    u32_t touchEnable;
    u32_t touchFlag;
}RawSpriteInfo;

typedef struct {
    u32_t eventKey;
    u32_t pageKey;
    u32_t actionKey;
}RawCommandInfo;

typedef struct {
    u32_t pageIndex;
	u32_t numSprites;
	u32_t numCmd;
	u32_t hashKey;
	u32_t spriteBegin;
	u32_t commandBegin;
}RawPageInfo;

int loadXpg(const char * filepath);
int closeXpg();
u32_t getXpgImageSize(u32_t roleIndex);
u32_t getXpgReadImage(u32_t roleIndex, u8_t * buffer);

extern RawRoleInfo   * g_rawRoles;
extern RawSpriteInfo * g_rawSprites;
extern RawCommandInfo* g_rawCommands;
extern RawPageInfo   * g_rawPages;
extern u32_t           g_rawRoleNum;
extern u32_t           g_rawSpriteNum;
extern u32_t           g_rawCommandNum;
extern u32_t           g_rawPageNum;

#ifdef __cplusplus
} // extern "C" 
#endif

#endif // XPG_H_
