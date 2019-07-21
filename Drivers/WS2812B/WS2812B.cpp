/*
 * WS2812B.cpp
 *
 *  Created on: 2019/06/01
 *      Author: yuhei
 */

#include "WS2812B.h"

WS2812B::WS2812B() {
    // TODO Auto-generated constructor stub
    this->hTim = NULL;
    this->tChannel = TIM_CHANNEL_1;
    this->xBuffer = NULL;
}

WS2812B::~WS2812B() {
    // TODO Auto-generated destructor stub
}

void WS2812B::init(TIM_HandleTypeDef * hTim, uint32_t tChannel) {
    this->hTim = hTim;
    this->tChannel = tChannel;

    HAL_TIM_Base_Start(this->hTim);
    __HAL_TIM_SET_COMPARE(this->hTim, tChannel, 0);
    HAL_TIM_PWM_Start(this->hTim, tChannel);
}

void WS2812B::encode(const uint8_t * pucGRBValues, TimerCounter_t * buffer) {
    for (uint32_t color_i = 0; color_i < 3; color_i++) {
        for (uint32_t bit_i = 0; bit_i < 8; bit_i++) {
            uint8_t bit_mask = 0x80 >> bit_i;
            if ((pucGRBValues[color_i] & bit_mask) > 0) {
                buffer[8 * color_i + bit_i] = 7;
            } else {
                buffer[8 * color_i + bit_i] = 3;
            }
        }
    }
}

void WS2812B::clear() {
    for (uint32_t i = 0; i < (WS2812B_BUFFER_LENGTH - 1); i++) {
        this->xBuffer[i] = WS2812B_PWM_ZERO;
    }
    this->xBuffer[WS2812B_BUFFER_LENGTH] = 0;
}

void WS2812B::output() {
    HAL_TIM_PWM_Start_DMA(this->hTim, this->tChannel, (uint32_t *) this->xBuffer, WS2812B_BUFFER_LENGTH);
}

void WS2812B::shift(bool cw) {
    TimerCounter_t temp[24];
    if (cw) {
        WS2812B::__copy(temp, this->xBuffer, 24);
        for (uint32_t i = 0; i < (WS2812B_LED_LENGTH - 1); i++) {
            WS2812B::__copy(&this->xBuffer[24 * i], &this->xBuffer[24 * (i + 1)], 24);
        }
        WS2812B::__copy(&this->xBuffer[24 * (WS2812B_LED_LENGTH - 1)], temp, 24);
    } else {
        WS2812B::__copy(temp, &this->xBuffer[24 * (WS2812B_LED_LENGTH - 1)], 24);
        for (uint32_t i = (WS2812B_LED_LENGTH - 1); i > 0; i--) {
            WS2812B::__copy(&this->xBuffer[24 * i], &this->xBuffer[24 * (i - 1)], 24);
        }
        WS2812B::__copy(this->xBuffer, temp, 24);
    }
}

void WS2812B::__copy(TimerCounter_t * to, TimerCounter_t * from, uint32_t xLength) {
    for (uint32_t i = 0; i < xLength; i++) {
        to[i] = from[i];
    }
}

