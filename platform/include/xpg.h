
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
typedef void *          XPGHandle;

typedef struct {
    u32_t       width;
    u32_t       height;
}RawRoleInfo;

typedef struct {
    int a;
}RawSpriteInfo;

typedef struct {
    int a;
}RawScriptInfo;

typedef struct {
    int a;
}RawPageInfo;

XPGHandle loadXpg(const char * filepath);
int closeXpg(void * handle);

extern RawRoleInfo   * g_rawRoles;
extern RawSpriteInfo * g_rawSprites;
extern RawScriptInfo * g_rawScripts;
extern RawPageInfo   * g_rawPages;
extern u32_t           g_rawRoleNum;
extern u32_t           g_rawSpriteNum;
extern u32_t           g_rawScriptNum;
extern u32_t           g_rawPageNum;

#ifdef __cplusplus
} // extern "C" 
#endif

#endif // XPG_H_
