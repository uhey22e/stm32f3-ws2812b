/*
 * app_main.cpp
 *
 *  Created on: 2019/06/01
 *      Author: yuhei
 */

#include "main.h"
#include "tim.h"
#include "stm32f3xx_hal.h"
#include "WS2812B.h"

#ifdef __cplusplus
extern "C" {
void app_main(void);
}
#endif

static const uint8_t colors1[] = { 0x0A, 0x00, 0x0A };
static const uint8_t colors2[] = { 0x0A, 0x0A, 0x00 };
static const uint8_t colors3[] = { 0x00, 0x0A, 0x0A };
static const uint8_t colors4[] = { 0x07, 0x07, 0x07 };

void app_main(void) {
    WS2812B ws2812b;
    TimerCounter_t led_buffer[WS2812B_BUFFER_LENGTH];
    ws2812b.init(&htim3, TIM_CHANNEL_1);
    ws2812b.xBuffer = led_buffer;

    ws2812b.clear();
    ws2812b.output();
    HAL_Delay(1);

    ws2812b.clear();
    WS2812B::encode(colors1, &ws2812b.xBuffer[0]);
    WS2812B::encode(colors2, &ws2812b.xBuffer[24 * 4]);
    WS2812B::encode(colors3, &ws2812b.xBuffer[24 * 8]);
    WS2812B::encode(colors4, &ws2812b.xBuffer[24 * 12]);
    ws2812b.output();

    while (1) {
        for (uint32_t i = 0; i < 32; i++) {
            HAL_Delay(50);
            ws2812b.shift(false);
            ws2812b.output();
        }
        for (uint32_t i = 0; i < 32; i++) {
            HAL_Delay(50);
            ws2812b.shift(true);
            ws2812b.output();
        }
    }
}

