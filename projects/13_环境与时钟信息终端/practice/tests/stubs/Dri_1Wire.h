#ifndef TEST_DRI_1WIRE_H
#define TEST_DRI_1WIRE_H
void Dri_1Wire_Init(void);
void Dri_1Wire_WriteByte(unsigned char value);
unsigned char Dri_1Wire_ReadByte(void);
#endif
