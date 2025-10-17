/*
 * spi_driver.h
 *
 * 2/9/2023 7:32:27 PM
 *  Author: Mariam
 */ 


#ifndef SPI_DRIVER_H_
#define SPI_DRIVER_H_
void SPI_MasterInit(void);
void SPI_SlaveInit(void);
unsigned char SPI_MasterTransmitchar(unsigned char Data);
unsigned char SPI_SlaveReceivecharWaiting(unsigned char Data);
unsigned char SPI_SlaveReceivechar(unsigned char Data);
void SPI_MasterTransmitstring(unsigned char *ptr);
#endif /* SPI_DRIVER_H_ */