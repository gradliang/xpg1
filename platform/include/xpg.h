
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

void* loadXpg(const char * filepath);
int closeXpg(void * handle);


#ifdef __cplusplus
} // extern "C" 
#endif

#endif // XPG_H_
