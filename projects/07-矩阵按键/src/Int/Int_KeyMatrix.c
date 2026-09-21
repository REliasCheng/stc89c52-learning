#include "Int_KeyMatrix.h"
#include <STC89C5xRC.H>

// u8 Int_KeyMatrix_CheckSW()
// {
//     // 检测第1行
//     P2 = 0xFE;
//     if (P24 == 0) {
//         Com_Util_Delay1ms(10);
//         if (P24 == 0) {
//             while (P24 == 0);
//             return 5;
//         }
//     }

//     if (P25 == 0) {
//         Com_Util_Delay1ms(10);
//         if (P25 == 0) {
//             while (P25 == 0);
//             return 6;
//         }
//     }

//     if (P26 == 0) {
//         Com_Util_Delay1ms(10);
//         if (P26 == 0) {
//             while (P26 == 0);
//             return 7;
//         }
//     }

//     if (P27 == 0) {
//         Com_Util_Delay1ms(10);
//         if (P27 == 0) {
//             while (P27 == 0);
//             return 8;
//         }
//     }

//     // 检测第2行
//     P2 = 0xFD;
//     if (P24 == 0) {
//         Com_Util_Delay1ms(10);
//         if (P24 == 0) {
//             while (P24 == 0);
//             return 9;
//         }
//     }

//     if (P25 == 0) {
//         Com_Util_Delay1ms(10);
//         if (P25 == 0) {
//             while (P25 == 0);
//             return 10;
//         }
//     }

//     if (P26 == 0) {
//         Com_Util_Delay1ms(10);
//         if (P26 == 0) {
//             while (P26 == 0);
//             return 11;
//         }
//     }

//     if (P27 == 0) {
//         Com_Util_Delay1ms(10);
//         if (P27 == 0) {
//             while (P27 == 0);
//             return 12;
//         }
//     }

//     // 检测第3行
//     P2 = 0xFB;
//     if (P24 == 0) {
//         Com_Util_Delay1ms(10);
//         if (P24 == 0) {
//             while (P24 == 0);
//             return 13;
//         }
//     }

//     if (P25 == 0) {
//         Com_Util_Delay1ms(10);
//         if (P25 == 0) {
//             while (P25 == 0);
//             return 14;
//         }
//     }

//     if (P26 == 0) {
//         Com_Util_Delay1ms(10);
//         if (P26 == 0) {
//             while (P26 == 0);
//             return 15;
//         }
//     }

//     if (P27 == 0) {
//         Com_Util_Delay1ms(10);
//         if (P27 == 0) {
//             while (P27 == 0);
//             return 16;
//         }
//     }

//     // 检测第4行
//     P2 = 0xF7;
//     if (P24 == 0) {
//         Com_Util_Delay1ms(10);
//         if (P24 == 0) {
//             while (P24 == 0);
//             return 17;
//         }
//     }

//     if (P25 == 0) {
//         Com_Util_Delay1ms(10);
//         if (P25 == 0) {
//             while (P25 == 0);
//             return 18;
//         }
//     }

//     if (P26 == 0) {
//         Com_Util_Delay1ms(10);
//         if (P26 == 0) {
//             while (P26 == 0);
//             return 19;
//         }
//     }

//     if (P27 == 0) {
//         Com_Util_Delay1ms(10);
//         if (P27 == 0) {
//             while (P27 == 0);
//             return 20;
//         }
//     }
//     return 0;
// }

// u8 Int_KeyMatrix_CheckSW()
// {
//     u8 i;
//     u8 lines[4] = {0xFE, 0xFD, 0xFB, 0xF7};
//     for (i = 0; i < 4; i++) {
//         P2 = lines[i];
//         if (P24 == 0) {
//             Com_Util_Delay1ms(10);
//             if (P24 == 0) {
//                 while (P24 == 0);
//                 return 5 + 4 * i;
//             }
//         }

//         if (P25 == 0) {
//             Com_Util_Delay1ms(10);
//             if (P25 == 0) {
//                 while (P25 == 0);
//                 return 6 + 4 * i;
//             }
//         }

//         if (P26 == 0) {
//             Com_Util_Delay1ms(10);
//             if (P26 == 0) {
//                 while (P26 == 0);
//                 return 7 + 4 * i;
//             }
//         }

//         if (P27 == 0) {
//             Com_Util_Delay1ms(10);
//             if (P27 == 0) {
//                 while (P27 == 0);
//                 return 8 + 4 * i;
//             }
//         }
//     }
//     return 0;
// }

u8 Int_KeyMatrix_CheckSW()
{
    u8 i, j;
    u8 lines[4]   = {0xFE, 0xFD, 0xFB, 0xF7};
    u8 columns[4] = {0x10, 0x20, 0x40, 0x80};
    for (i = 0; i < 4; i++) {
        P2 = lines[i];
        for (j = 0; j < 4; j++) {
            if ((P2 & columns[j]) == 0x00) {
                Com_Util_Delay1ms(10);
                if ((P2 & columns[j]) == 0x00) {
                    while ((P2 & columns[j]) == 0x00);
                    return 5 + j + 4 * i;
                }
            }
        }
    }
    return 0;
}