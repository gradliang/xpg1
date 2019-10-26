
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

RawRoleInfo * getRawRoleList();
RawSpriteInfo * getRawSpriteList();
RawCommandInfo * getRawCmdList();
RawPageInfo * getRawPageList();
u32_t  getRawRoleTotal();
u32_t  getRawSpriteTotal();
u32_t  getRawCmdTotal();
u32_t  getRawPageTotal();

#ifdef __cplusplus
} // extern "C" 
#endif

#endif // XPG_H_
