#ifndef __INT_DIGITALTUBE_H__
#define __INT_DIGITALTUBE_H__
#define SMG_EN P36
#define LED_EN P34

#include "Com_Util.h"

void Int_DigitalTube_Init();
void Int_DigitalTube_DisplayNum(u32 num);
void Int_DigitalTube_Refresh();

#endif /* __INT_DIGITALTUBE_H__ */