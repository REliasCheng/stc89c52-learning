#ifndef __INT_LEDMATRIX_H__
#define __INT_LEDMATRIX_H__
#include "Com_Util.h"
void Int_LEDMatrix_Init();

void Int_LEDMatrix_SetPic(u8 pic[]);

void Int_LEDMatrix_Shift(u8 pic);

#endif /* __INT_LEDMATRIX_H__ */