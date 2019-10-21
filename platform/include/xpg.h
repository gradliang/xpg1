
#ifndef XPG_H_
#define XPG_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char   u8_t;
typedef unsigned short  u16_t;
typedef unsigned int    u32_t;

void* LoadXpg(const char * filepath);
int CloseXpg(void * handle);


#ifdef __cplusplus
} // extern "C" 
#endif

#endif // XPG_H_
