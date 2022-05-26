/**
 *
 */

#include <F2837xD_device.h>

unsigned spi_transmitChar(unsigned int u16Data){


    u16Data = u16Data & 0xFF; // to be sure, that only 8 bits are transmitted
    while (SpibRegs.SPISTS.bit.BUFFULL_FLAG == 1); // while buffer is full, wait
    if (SpibRegs.SPISTS.bit.BUFFULL_FLAG == 0){
        SpibRegs.SPITXBUF = u16Data << 8; // must be left-justified

    }
    while(SpibRegs.SPISTS.bit.INT_FLAG==0);

    return(SpibRegs.SPIRXBUF);

}

void spi_transmitData(unsigned int *pData, unsigned int u16Length){

    unsigned int u16Counter;

    for (u16Counter = 0; u16Counter < u16Length; u16Counter++){
        spi_transmitChar(*(pData+u16Counter));
       // spi_transmitChar(pData[u16Counter]);
    }


}

