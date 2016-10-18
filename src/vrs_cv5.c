#include "vrs_cv5.h"


void adc_init(void)
{
	 GPIO_InitTypeDef GPIO_InitStruct;
	 ADC_InitTypeDef ADC_InitStruct;

	 /* Enable GPIO clock */
	 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);//Opraviù a upraviù

	 /* Configure ADCx Channel 2 as analog input */
	 GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	 GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	 GPIO_Init(GPIOA, &GPIO_InitStruct);

	 /* Enable the HSI oscillator */
	 RCC_HSICmd(ENABLE);

	 /* Check that HSI oscillator is ready */
	 while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

	 /* Enable ADC clock */
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	 /* Initialize ADC structure */
	 ADC_StructInit(&ADC_InitStruct);

	 /* ADC1 configuration */
	 ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
	 ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
	 ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	 ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	 ADC_InitStruct.ADC_NbrOfConversion = 1;
	 ADC_Init(ADC1, &ADC_InitStruct);

	/* ADCx regular channel8 configuration */
	 ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_16Cycles);

	 /* Enable the ADC */
	 ADC_Cmd(ADC1, ENABLE);

	 /* Wait until the ADC1 is ready */
	 while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
	 {
	 }
	 /* Start ADC Software Conversion */
	 ADC_SoftwareStartConv(ADC1);
}

void nvic_init(void) {

	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitStructure.NVIC_IRQChannel = ADC1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);

	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
	//ADC_ITConfig(ADC1, ADC_IT_OVR, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; //zoznam preruöenÌ n·jdete v s˙bore stm32l1xx.h
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
}

void uart_init (void){

	GPIO_InitTypeDef gpioInitStruct;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	gpioInitStruct.GPIO_Mode = GPIO_Mode_AF;
	gpioInitStruct.GPIO_OType = GPIO_OType_PP;
	gpioInitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	gpioInitStruct.GPIO_Speed = GPIO_Speed_40MHz;
	gpioInitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOA, &gpioInitStruct);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 19200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	USART_Cmd(USART1, ENABLE);
}

void ADC1_IRQHandler (void){

}

void USART1_IRQHandler (void){

}

