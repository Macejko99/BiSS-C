/**
 *
 */

#ifndef SPI_COMMUNICATION_H_
#define SPI_COMMUNICATION_H_

unsigned spi_transmitChar(unsigned int u16Data);
void spi_transmitData(unsigned int *pData, unsigned int u16Length);

#endif /* SPI_COMMUNICATION_H_ */
