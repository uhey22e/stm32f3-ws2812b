/*
 * WS2812B.h
 *
 *  Created on: 2019/06/01
 *      Author: yuhei
 */

#ifndef WS2812B_WS2812B_H_
#define WS2812B_WS2812B_H_

#include "stdint.h"

#include "stm32f3xx_hal.h"

#define WS2812B_PWM_ZERO 3
#define WS2812B_PWM_ONE  7

#define WS2812B_LED_LENGTH 16
#define WS2812B_BUFFER_LENGTH (1 + 3 * 8 * WS2812B_LED_LENGTH)

typedef uint16_t TimerCounter_t;

class WS2812B {
public:
    WS2812B();
    ~WS2812B();

    TimerCounter_t * xBuffer;

    void init(TIM_HandleTypeDef * hTim, uint32_t tChannel);
    void clear();
    void shift(bool cw = true);

    void output();

    static void encode(const uint8_t * pucGRBValues, TimerCounter_t * buffer);


private:
    TIM_HandleTypeDef* hTim;
    uint32_t tChannel;

    void __copy(TimerCounter_t * from, TimerCounter_t * to, uint32_t xLength);
};

#endif /* WS2812B_WS2812B_H_ */
