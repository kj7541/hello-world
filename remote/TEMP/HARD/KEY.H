#ifndef _key_h
#define _key_h
#include "delay.h"
// PU ZHONG KAI FA BAN

#include "sys.h"
#define k0 PAin(0)
#define k1 PAin(1)
#define k2 PAin(2)
#define sw1 PCin(3)
#define sw2 PBin(1)
void key_init(void);
u8 key_scan(void);


#endif 





