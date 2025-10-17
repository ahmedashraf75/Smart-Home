/*
 * ADC_driver.h
 *
 * Created: 4/10/2023 12:22:00 PM
 *  Author: Mariam Ashraf
 */ 


#ifndef ADC_DRIVER_H_
#define ADC_DRIVER_H_
void ADC_vinit(void);
unsigned short ADC_u16Read(uint8_t channel_num);

#endif /* ADC_DRIVER_H_ */