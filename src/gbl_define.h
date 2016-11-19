#ifndef GBL_DEFINE_H
#define GBL_DEFINE_H

#include <stdio.h>

#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))



#ifdef DEBUG
#define JPRINTF(f,l,x...) do{printf("[%s:%d]:",f, l); printf(x);}while(0)
#define DPRINTF(x...) do{printf(x);}while(0)
#else
#define JPRINTF(f,l,x...)
#define DPRINTF(x...)
#endif










#endif //GBL_DEFINE_H
