#include "xpgHash.h"


//SDBMHash
unsigned int xpgHash(const char* str, unsigned int len)
{

   unsigned int hash = 0;
   unsigned int i    = 0;

   for(i = 0; i < len; str++, i++)
   {

      hash = (*str) + (hash << 6) + (hash << 16) - hash;

   }

   return (hash & 0x7FFFFFFF);   
}
/* End Of SDBM Hash Function */



