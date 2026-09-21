#ifndef __DRI_TIMER0_H__
#define __DRI_TIMER0_H__
typedef void (*Timer0_Callback)(void);

void Dri_Timer0_Init();

/**
 * @brief 注册回调函数
 *
 * @param callback 函数指针
 * @return bit 1:成功 0:失败
 */
bit Dri_Timer0_RegisterCallback(Timer0_Callback callback);

/**
 * @brief 取消注册回调函数
 *
 * @param callback 函数指针
 * @return bit 1:成功 0:失败
 */
bit Dri_Timer0_DeregisterCallback(Timer0_Callback callback);

#endif /* __DRI_TIMER0_H__ */