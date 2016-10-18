/*
 * vrs_cv5.h
 *
 *  Created on: 18. 10. 2016
 *      Author: Peter
 */

#ifndef VRS_CV5_H_
#define VRS_CV5_H_

#include "stm32l1xx.h"

void adc_init(void);
void LED_init(void);
void nvic_init(void);
void usart_init(void);
void ADC1_IRQHandler (void);
void USART2_IRQHandler (void);

#endif /* VRS_CV5_H_ */
