#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "LPC54608.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "fsl_usart.h"
#include "fsl_adc.h"
#include "fsl_clock.h"
#include "fsl_power.h"
#include "math.h"
#include "fsl_pint.h"
#include "fsl_inputmux.h"
/* TODO: insert other definitions and declarations here. */
#define LED3_PORT 2U
#define LED2_PORT 3U
#define LED1_PORT 3U
#define LED3_PIN 2U
#define LED2_PIN 3U
#define LED1_PIN 14U

#define SW_PORT 0U
#define SW1_PIN 4U
#define SW2_PIN 5U
#define SW3_PIN 6U
#define SW4_PIN 1U

#define ADC_BASE            ADC0
#define ADC_SENSOR			0U
#define ADC_CANAL1          4U
#define ADC_CANAL2          5U
#define ADC_CANAL3          6U
#define ADC_MAX             4095U
#define ADC_Vref            3.3
#define ValMin .6656
#define VSens .00204
#define TempMin -40

//Variables
uint8_t dato[11];
float vpaso = 3.3/4095;
uint8_t mensaje[100];
char leepuerto;
char bandera1 = 0x00;
char bandera = 0x00;
char puerto =0x00;
float ADC_SENS;
uint16_t ADC_SENSINT;
float ADC_POT1;
uint16_t ADC_POT1INT;
float ADC_POT2;
uint16_t ADC_POT2INT;
float ADC_POT3;
uint16_t ADC_POT3INT;
float_t ADC_CH1_Voltage;				//Voltage Conversion Result
adc_result_info_t adcInfoLectura;	//Structure for ADC Conversion
//IRQ Handler
void FLEXCOMM0_IRQHandler(void);
void LED_config(void);
void ADC_PWR_Config(void);
void ADC_Calib(void);
float ADCtoVoltageConversion(uint32_t ADC_Count);
signed char ADCtoTemperatureConversion(uint32_t ADC_Count);
void ADC_Request(uint32_t CANAL);
void botonpresionado(void);
void riteport(void);
/*
 * @brief   Application entry point.
 */
int main(void) {

	usart_config_t config;
	adc_config_t adcConfigStruct;
	adc_conv_seq_config_t adcConvSeqConfigStruct;
  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitPins();
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
    BOARD_BootClockPLL180M();
    BOARD_InitDebugConsole();
    LED_config();
    //UART Configuration
    USART_GetDefaultConfig(&config);
    config.baudRate_Bps = 9600U;
    config.parityMode = kUSART_ParityDisabled;
    config.stopBitCount = kUSART_OneStopBit;
    config.loopback = false;
    config.enableRx = true;
    config.enableTx = true;

    //UART init
    USART_Init(USART0,&config,CLOCK_GetFlexCommClkFreq(0));
    //Enviar mensaje de bienvenida
    //Habilitar interrupcion
    USART_EnableInterrupts(USART0,kUSART_RxLevelInterruptEnable|kUSART_RxErrorInterruptEnable);
    EnableIRQ(FLEXCOMM0_IRQn);

	ADC_PWR_Config();
	ADC_Calib();

	//ADC Configuration
	adcConfigStruct.clockMode = kADC_ClockSynchronousMode;
	adcConfigStruct.clockDividerNumber = 2U;
	adcConfigStruct.resolution = kADC_Resolution12bit;
	adcConfigStruct.enableBypassCalibration = false;
	adcConfigStruct.sampleTimeNumber = 0U;

	ADC_Init(ADC_BASE,&adcConfigStruct);

	//Enables Temperature sensor
	ADC_EnableTemperatureSensor(ADC_BASE,true);

	adcConvSeqConfigStruct.channelMask = (1U<<ADC_CANAL1)+(1U<<ADC_SENSOR)+(1U<<ADC_CANAL2)+(1U<<ADC_CANAL3);
	adcConvSeqConfigStruct.triggerMask = 0;
	adcConvSeqConfigStruct.triggerPolarity = kADC_TriggerPolarityPositiveEdge;
	adcConvSeqConfigStruct.enableSingleStep = false;
	adcConvSeqConfigStruct.enableSyncBypass = false;
	adcConvSeqConfigStruct.interruptMode = kADC_InterruptForEachSequence;

	ADC_SetConvSeqAConfig(ADC_BASE,&adcConvSeqConfigStruct);
	ADC_EnableConvSeqA(ADC_BASE,true);
	ADC_Request(ADC_CANAL1);

    while(1) {
    	if(bandera1&0x01)
    	{
    		bandera1&=~0x01;
    		Writeport();
        	switch (dato[0])
        	{
        	case 'N':
        		break;
        	case 'A':
        		break;
        	case 'S':

        		break;
            	default:
            		break;
        	}
        }
    }
    return 0 ;
}

void FLEXCOMM0_IRQHandler(void)
{

	if((kUSART_RxFifoNotEmptyFlag | kUSART_RxError) & USART_GetStatusFlags(USART0))
	{
		//Leer dato entrante
		USART_ReadBlocking(USART0,dato,11);
		bandera1|=0x01;
	}
}

void LED_config()
{
	gpio_pin_config_t led_config = {
				kGPIO_DigitalOutput,
				1
	};
	GPIO_PortInit(GPIO,LED3_PORT);
	GPIO_PinInit(GPIO,LED3_PORT,LED3_PIN,&led_config);
	GPIO_PortInit(GPIO,LED2_PORT);
	GPIO_PinInit(GPIO,LED2_PORT,LED2_PIN,&led_config);
	GPIO_PinInit(GPIO,LED1_PORT,LED1_PIN,&led_config);
	GPIO_PortMaskedSet(GPIO,LED2_PORT,0x0000FFFF);
	GPIO_PortMaskedWrite(GPIO,LED2_PORT,0xFFFFFFFF);
	GPIO_PortMaskedSet(GPIO,LED3_PORT,0x0000FFFF);
	GPIO_PortMaskedWrite(GPIO,LED3_PORT,0xFFFFFFFF);
	GPIO_PortInit(GPIO,0U);
	GPIO_PortInit(GPIO,1U);
}

void ADC_PWR_Config(void)
{
    POWER_DisablePD(kPDRUNCFG_PD_VDDA);
    POWER_DisablePD(kPDRUNCFG_PD_ADC0);
    POWER_DisablePD(kPDRUNCFG_PD_VD2_ANA);
    POWER_DisablePD(kPDRUNCFG_PD_VREFP);
    POWER_DisablePD(kPDRUNCFG_PD_TS);

    CLOCK_EnableClock(kCLOCK_Adc0);
}

void ADC_Calib(void)
{
    ADC_BASE->CTRL |= ADC_CTRL_BYPASSCAL_MASK;
    if(ADC_DoOffsetCalibration(ADC_BASE,CLOCK_GetFreq(kCLOCK_BusClk)))
    {
    	PRINTF("Calibración OK");
    }
    else
    {
    	PRINTF("Error en Calibración");
    }
}

void ADC_Request(uint32_t CANAL)
{
    ADC_DoSoftwareTriggerConvSeqA(ADC_BASE);
    while(!ADC_GetChannelConversionResult(ADC_BASE,CANAL,&adcInfoLectura))
    {

    }
    //ADC_GetConvSeqAGlobalConversionResult(ADC_BASE,&adcInfoLectura);
}
float ADCtoVoltageConversion(uint32_t ADC_Count)
{
	return (ADC_Count * ADC_Vref)/ADC_MAX;
}


void Writeport(void)
{
	if(dato[0]=='N')
	{
		if(dato[1]=='R' && dato[2]=='C') //Set Port Status
		{
			if(dato[3]=='0')//Port
			{
				if(dato[4]=='0' && dato[5]=='0')//byte 1 y byte 0
					{
						puerto=0x00;
						sprintf(mensaje,"NRR0000000;");
						USART_WriteBlocking(USART0,mensaje,strlen(mensaje));
					}else if(dato[4]=='0' && dato[5]=='1')
					{
						puerto =0x01;
						sprintf(mensaje,"NRR0010000;");
						USART_WriteBlocking(USART0,mensaje,strlen(mensaje));
					}else if(dato[4]=='0' && dato[5]=='2')
					{
						puerto =0x02;
						sprintf(mensaje,"NRR0020000;");
						USART_WriteBlocking(USART0,mensaje,strlen(mensaje));
					}else if(dato[4]=='0' && dato[5]=='3')
					{
						puerto =0x03;
						sprintf(mensaje,"NRR0030000;");
						USART_WriteBlocking(USART0,mensaje,strlen(mensaje));
					}else if(dato[4]=='0' && dato[5]=='4')
					{
						puerto = 0x04;
						sprintf(mensaje,"NRR0040000;");
						USART_WriteBlocking(USART0,mensaje,strlen(mensaje));
					}else if(dato[4]=='0' && dato[5]=='5')
					{
						puerto = 0x05;
						sprintf(mensaje,"NRR0050000;");
						USART_WriteBlocking(USART0,mensaje,strlen(mensaje));
					}else if(dato[4]=='0' && dato[5]=='6')
					{
						puerto = 0x06;
						sprintf(mensaje,"NRR0060000;");
						USART_WriteBlocking(USART0,mensaje,strlen(mensaje));
					}else if(dato[4]=='0' && dato[5]=='7')
					{
						puerto = 0x07;
						sprintf(mensaje,"NRR0070000;");
						USART_WriteBlocking(USART0,mensaje,strlen(mensaje));
					}else
					{
						sprintf(mensaje,"CMNTVL0000;");
						USART_WriteBlocking(USART0,mensaje,strlen(mensaje));
					}
			}else
			{
				sprintf(mensaje,"CMNTVL0000;");
				USART_WriteBlocking(USART0,mensaje,strlen(mensaje));
			}
		}else if(dato[1]=='W' && dato[2]=='C')//get port status
		{
			if(dato[3]=='0')//Port
			{
				sprintf(mensaje,"NWR00%X0000;",puerto);
				USART_WriteBlocking(USART0,mensaje,strlen(mensaje));
			}else{
				sprintf(mensaje,"CMNTVL0000;");
				USART_WriteBlocking(USART0,mensaje,strlen(mensaje));
			}
		}else if(dato[1]=='A' && dato[2]=='C')//ADC
		{
			if(dato[3]=='0')//PORT
			{
				ADC_Request(ADC_CANAL1); 	//Request conversion for J12.2
				ADC_CH1_Voltage = ADCtoVoltageConversion(adcInfoLectura.result); //Converts to Volts
				sprintf(mensaje,"NAR%f",ADC_CH1_Voltage);//Displays Voltage
				USART_WriteBlocking(USART0,mensaje,strlen(mensaje));
			}else{
				sprintf(mensaje,"CMNTVL0000;");
				USART_WriteBlocking(USART0,mensaje,strlen(mensaje));
			}
		}else if(dato[1]=='E' && dato[2]=='C')//SET PIN
		{
			if(dato[3]=='0')//PORT
			{
				if(dato[4]=='0' && dato[5]=='0')//PIN Y  VALOR
					{
						puerto &= ~0x01;
						sprintf(mensaje,"NER0000000;");
						USART_WriteBlocking(USART0,mensaje,strlen(mensaje));
					}else if(dato[4]=='0' && dato[5]=='1')
					{
						puerto |= 0x01;
						sprintf(mensaje,"NER0010000;");
						USART_WriteBlocking(USART0,mensaje,strlen(mensaje));
					}else if(dato[4]=='1' && dato[5]=='0')
					{
						puerto &= ~0x02;
						sprintf(mensaje,"NER0100000;");
						USART_WriteBlocking(USART0,mensaje,strlen(mensaje));
					}else if(dato[4]=='1' && dato[5]=='1')
					{
						puerto |= 0x02;
						sprintf(mensaje,"NER0110000;");
						USART_WriteBlocking(USART0,mensaje,strlen(mensaje));
					}else if(dato[4]=='2' && dato[5]=='0')
					{
						puerto &= ~0x04;
						sprintf(mensaje,"NER0200000;");
						USART_WriteBlocking(USART0,mensaje,strlen(mensaje));
					}else if(dato[4]=='2' && dato[5]=='1')
					{
						puerto |= 0x04;
						sprintf(mensaje,"NER0210000;");
						USART_WriteBlocking(USART0,mensaje,strlen(mensaje));
					}else
					{
						sprintf(mensaje,"CMNTVL0000;");
						USART_WriteBlocking(USART0,mensaje,strlen(mensaje));
					}
			}else
			{
				sprintf(mensaje,"CMNTVL0000;");
				USART_WriteBlocking(USART0,mensaje,strlen(mensaje));
			}

		}else if(dato[1]=='L' && dato[2]=='C')//GET PIN STATUS
		{
			leepuerto = GPIO_PortRead(GPIO,SW_PORT);
			if(dato[3]=='0')//PORT
			{
				if(dato[4]=='3')//PIN
				{
					if(!(GPIO_PortRead(GPIO,SW4_PIN)&(1U<<SW4_PIN)))
					{
						sprintf(mensaje,"NLR0310000;");
						USART_WriteBlocking(USART0,mensaje,strlen(mensaje));
					}
					else
					{
						sprintf(mensaje,"NLR0300000;");
						USART_WriteBlocking(USART0,mensaje,strlen(mensaje));
					}
				}else if(dato[4]=='2')
				{
					if(!(leepuerto&(1U<<SW1_PIN)))
					{
						sprintf(mensaje,"NLR0210000;");
						USART_WriteBlocking(USART0,mensaje,strlen(mensaje));
					}
					else
					{
						sprintf(mensaje,"NLR0200000;");
						USART_WriteBlocking(USART0,mensaje,strlen(mensaje));
					}
				}else if(dato[4]=='1')
				{
					if(!(leepuerto&(1U<<SW2_PIN)))
					{
						sprintf(mensaje,"NLR0110000;");
						USART_WriteBlocking(USART0,mensaje,strlen(mensaje));
					}
					else
					{
						sprintf(mensaje,"NLR0100000;");
						USART_WriteBlocking(USART0,mensaje,strlen(mensaje));
					}
				}else if(dato[4]=='0')
				{
					if(!(leepuerto&(1U<<SW3_PIN)))
					{
						sprintf(mensaje,"NLR0010000;");
						USART_WriteBlocking(USART0,mensaje,strlen(mensaje));
					}
					else
					{
						sprintf(mensaje,"NLR0000000;");
						USART_WriteBlocking(USART0,mensaje,strlen(mensaje));
					}
				}else
				{
					sprintf(mensaje,"CMNTVL0000;");
					USART_WriteBlocking(USART0,mensaje,strlen(mensaje));
				}
			}else
			{
				sprintf(mensaje,"CMNTVL0000;");
				USART_WriteBlocking(USART0,mensaje,strlen(mensaje));
			}
		}else
		{
			sprintf(mensaje,"CMNTVL0000;");
			USART_WriteBlocking(USART0,mensaje,strlen(mensaje));
		}
		if(puerto&0x01)
		{
			GPIO_PinWrite(GPIO,LED1_PORT,LED1_PIN,0);
		}else
		{
			GPIO_PinWrite(GPIO,LED1_PORT,LED1_PIN,1);
		}
		if(puerto&0x02)
		{
			GPIO_PinWrite(GPIO,LED2_PORT,LED2_PIN,0);
		}else
		{
			GPIO_PinWrite(GPIO,LED2_PORT,LED2_PIN,1);
		}
		if(puerto&0x04)
		{
			GPIO_PinWrite(GPIO,LED3_PORT,LED3_PIN,0);
		}else
		{
			GPIO_PinWrite(GPIO,LED3_PORT,LED3_PIN,1);
		}
	}else
	{
		sprintf(mensaje,"CMNTVL0000;");
		USART_WriteBlocking(USART0,mensaje,strlen(mensaje));
	}
}
