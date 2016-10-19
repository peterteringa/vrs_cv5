#include "vrs_cv5.h"

//extern volatile uint16_t AD_value = 0;

void LED_init (void){

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.GPIO_Pin 	= GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode 	= GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed 	= GPIO_Speed_400KHz;

	GPIO_Init(GPIOA, &GPIO_InitStruct);

}

void adc_init(void)
{
	 GPIO_InitTypeDef	GPIO_InitStruct;
	 ADC_InitTypeDef	ADC_InitStruct;

	 /* Enable GPIO clock */
	 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);//Opraviù a upraviù

	 /* Configure ADCx Channel 2 as analog input */
	 GPIO_InitStruct.GPIO_Pin 	= GPIO_Pin_0;
	 GPIO_InitStruct.GPIO_Mode 	= GPIO_Mode_AN;
	 GPIO_InitStruct.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
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
	 ADC_InitStruct.ADC_Resolution 				= ADC_Resolution_12b;
	 ADC_InitStruct.ADC_ContinuousConvMode 		= DISABLE;
	 ADC_InitStruct.ADC_ExternalTrigConvEdge 	= ADC_ExternalTrigConvEdge_None;
	 ADC_InitStruct.ADC_DataAlign 				= ADC_DataAlign_Right;
	 ADC_InitStruct.ADC_NbrOfConversion 		= 1;

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
	NVIC_InitStructure.NVIC_IRQChannel 						= ADC1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;

	NVIC_Init(&NVIC_InitStructure);

	//ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
	//ADC_ITConfig(ADC1, ADC_IT_OVR, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel 						= USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;

	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
}

void usart_init (void){

	GPIO_InitTypeDef gpioInitStruct;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	gpioInitStruct.GPIO_Mode 	= GPIO_Mode_AF;
	gpioInitStruct.GPIO_OType 	= GPIO_OType_PP;
	gpioInitStruct.GPIO_Pin 	= GPIO_Pin_2 | GPIO_Pin_3;
	gpioInitStruct.GPIO_Speed 	= GPIO_Speed_40MHz;
	gpioInitStruct.GPIO_PuPd 	= GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOA, &gpioInitStruct);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate 				= 19200;
	USART_InitStructure.USART_WordLength 			= USART_WordLength_8b;
	USART_InitStructure.USART_StopBits 				= USART_StopBits_1;
	USART_InitStructure.USART_Parity 				= USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode 					= USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);

	USART_Cmd(USART2, ENABLE);
}

/*void ADC1_IRQHandler (void){
	if (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)){
		AD_value=ADC_GetConversionValue(ADC1)*1000/4095*33;
	}
}

void USART2_IRQHandler (void){

	  uint16_t k = AD_value/10000;
	  AD_value -= k*10000;
	  k += '0';
	  USART_ClearFlag(USART2, USART_FLAG_TC);
	  USART_SendData(USART2, k);
	  while (!USART_GetFlagStatus(USART2, USART_FLAG_TC)) {}

	  USART_ClearFlag(USART2, USART_FLAG_TC);
	  USART_SendData(USART2, ',');
	  while (!USART_GetFlagStatus(USART2, USART_FLAG_TC)) {}

	  k = AD_value/1000;
	  AD_value -= k*1000;
	  k += '0';
	  USART_ClearFlag(USART2, USART_FLAG_TC);
	  USART_SendData(USART2, k);
	  while (!USART_GetFlagStatus(USART2, USART_FLAG_TC)) {}

	  k = AD_value/100;
	  AD_value -= k*100;
	  k += '0';
	  USART_ClearFlag(USART2, USART_FLAG_TC);
	  USART_SendData(USART2, k);
	  while (!USART_GetFlagStatus(USART2, USART_FLAG_TC)) {}

	  USART_ClearFlag(USART2, USART_FLAG_TC);
	  USART_SendData(USART2, 'V');
	  while (!USART_GetFlagStatus(USART2, USART_FLAG_TC)) {}

	  USART_ClearFlag(USART2, USART_FLAG_TC);
	  USART_SendData(USART2, ' ');
	  while (!USART_GetFlagStatus(USART2, USART_FLAG_TC)) {}
}
*/
