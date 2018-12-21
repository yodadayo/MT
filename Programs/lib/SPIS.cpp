/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@bug.st>
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "SPIS.h"
#include <extern.h>

#define SCBR  8
#define DLYBCT 84


SPIClass::SPIClass(Spi *_spi, uint32_t _id, void(*_initCb)(void)) :
	spi(_spi), id(_id), initCb(_initCb), initialized(false)
{
	// Empty
}

void SPIClass::begin() {
	init();

	// NPCS control is left to the user

	// Default speed set to 4Mhz
	setClockDivider(BOARD_SPI_DEFAULT_SS, SCBR);
	setDataMode(BOARD_SPI_DEFAULT_SS, SPI_MODE0);
	setBitOrder(BOARD_SPI_DEFAULT_SS, MSBFIRST);
}
///////////////////////////////////////////////
void SPIClass::begin(uint8_t _pin) {
	init();

	uint32_t spiPin = BOARD_PIN_TO_SPI_PIN(_pin);
	PIO_Configure(
		g_APinDescription[spiPin].pPort,
		g_APinDescription[spiPin].ulPinType,
		g_APinDescription[spiPin].ulPin,
		g_APinDescription[spiPin].ulPinConfiguration);
	

	// Default speed set to 4Mhz
	setClockDivider(_pin,SCBR);
	setDataMode(_pin, SPI_MODE0);
	setBitOrder(_pin, MSBFIRST);
//pmc_enable_periph_clk( id );
}



//////追加　スレーブモード
void SPIClass::beginSlave(uint8_t _pin) {
	initSlave();

	uint32_t spiPin = BOARD_PIN_TO_SPI_PIN(_pin);
	PIO_Configure(
		g_APinDescription[spiPin].pPort,
		g_APinDescription[spiPin].ulPinType,
		g_APinDescription[spiPin].ulPin,
		g_APinDescription[spiPin].ulPinConfiguration);
	

	// Default speed set to 4Mhz
	setClockDivider(_pin, SCBR);
	setDataMode(_pin, SPI_MODE0);
	setBitOrder(_pin, MSBFIRST);
//pmc_enable_periph_clk( id );
}

void SPIClass::init() {
	if (initialized)
		return;
	initCb();
	SPI_Configure(spi, id, SPI_MR_MSTR | SPI_MR_PS | SPI_MR_MODFDIS| SPI_MR_WDRBT);
	SPI_Enable(spi);
	spi->SPI_IER =  (SPI_IER_NSSR | SPI_IER_TDRE | SPI_IER_RDRF);
	initialized = true;
}


//////追加　スレーブモード
void SPIClass::initSlave() {
	if (initialized)
		return;
	initCb();
	SPI_Configure(spi, id, 0);
	SPI_Enable(spi);
	////////////////////////////////////////////////////////////////////
	//Write protection
	spi->SPI_WPMR = 0;
	//Control Register
	spi->SPI_CR = (SPI_CR_LASTXFER | SPI_CR_SPIEN);

	//Mode Register
	int val SPI_PCS(0);//0(10) 1(4) 2(52)
	spi->SPI_MR = (SPI_MR_PCS(val) | SPI_MR_MODFDIS | SPI_MR_PS | SPI_MR_WDRBT);

	//Trasmit Data Register
	spi->SPI_TDR = (SPI_TDR_PCS(val) | SPI_CR_LASTXFER);

	//Interrupt Enable Register
	spi->SPI_IER = ( SPI_IER_RDRF);

	//Chip Select Register
	spi->SPI_CSR[0]= (SPI_CSR_DLYBCT(DLYBCT) | SPI_CSR_SCBR(SCBR)| 
						  SPI_CSR_CSAAT | SPI_CSR_NCPHA|SPI_CSR_BITS_16_BIT);
	///////////////////////////////////////////////////////////////////
	spi->SPI_WPMR = 0;
	spi->SPI_CR = (SPI_CR_LASTXFER | SPI_CR_SPIEN);
	spi->SPI_CSR[0]= (SPI_CSR_DLYBCT(DLYBCT) | SPI_CSR_SCBR(SCBR)| SPI_CSR_CSAAT | SPI_CSR_NCPHA|SPI_CSR_BITS_16_BIT);
	initialized = true;
}

void SPIClass::end(uint8_t _pin) {
	uint32_t spiPin = BOARD_PIN_TO_SPI_PIN(_pin);
	// Setting the pin as INPUT will disconnect it from SPI peripheral
	pinMode(spiPin, INPUT);
}

void SPIClass::end() {
	SPI_Disable(spi);
	initialized = false;
}

void SPIClass::setBitOrder(uint8_t _pin, BitOrder _bitOrder) {
	uint32_t ch = BOARD_PIN_TO_SPI_CHANNEL(_pin);
	bitOrder[ch] = _bitOrder;
}

void SPIClass::setDataMode(uint8_t _pin, uint8_t _mode) {
	uint32_t ch = BOARD_PIN_TO_SPI_CHANNEL(_pin);
	mode[ch] = _mode | SPI_CSR_CSAAT;
	// SPI_CSR_DLYBCT(1) keeps CS enabled for 32 MCLK after a completed
	// transfer. Some device needs that for working properly.
	SPI_ConfigureNPCS(spi, ch, mode[ch] | SPI_CSR_SCBR(divider[ch]) | SPI_CSR_DLYBCT(DLYBCT)|SPI_CSR_BITS_16_BIT);
}

void SPIClass::setClockDivider(uint8_t _pin, uint8_t _divider) {
	uint32_t ch = BOARD_PIN_TO_SPI_CHANNEL(_pin);
	divider[ch] = _divider;
	// SPI_CSR_DLYBCT(1) keeps CS enabled for 32 MCLK after a completed
	// transfer. Some device needs that for working properly.
	SPI_ConfigureNPCS(spi, ch, mode[ch] | SPI_CSR_SCBR(divider[ch]) | SPI_CSR_DLYBCT(DLYBCT)|SPI_CSR_BITS_16_BIT);
}

uint16_t SPIClass::transfer(byte _pin, uint16_t _data, SPITransferMode _mode) {
	uint32_t ch = BOARD_PIN_TO_SPI_CHANNEL(_pin);
	// Reverse bit order
	if (bitOrder[ch] == LSBFIRST)
		_data = __REV(__RBIT(_data));
	uint32_t d = _data | SPI_PCS(ch);
	if (_mode == SPI_LAST)
		d |= SPI_TDR_LASTXFER;
  	
	// SPI_Write(spi, _channel, _data);
    while ((spi->SPI_SR & SPI_SR_TDRE) == 0)
    	;
 				
    spi->SPI_TDR = d;
  					
    // return SPI_Read(spi);

    while ((spi->SPI_SR & SPI_SR_RDRF) == 0)
    	;
  					 
    d = spi->SPI_RDR;
  					 
	// Reverse bit order
	if (bitOrder[ch] == LSBFIRST)
		d = __REV(__RBIT(d));
  					
    return d & 0xFFFF;
}
//追加分///////////////////////////
byte SPIClass::receiver(void) {
	uint32_t d;
	
	////////////////////////////////////////////////////////////////////
	spi->SPI_WPMR = 0;
	spi->SPI_CR = (SPI_CR_LASTXFER | SPI_CR_SPIEN);
	spi->SPI_CSR[0]= (SPI_CSR_DLYBCT(DLYBCT) | SPI_CSR_SCBR(SCBR)| SPI_CSR_CSAAT | SPI_CSR_NCPHA|SPI_CSR_BITS_16_BIT);
	///////////////////////////////////////////////////////////////////
    int rdrfcnt=0;
	// return SPI_Read(spi);
	while ((spi->SPI_SR & SPI_SR_RDRF) == 0){
    }
	spi->SPI_CR = SPI_CR_SPIDIS;
	d = spi->SPI_RDR;
  	Serial.println(d,BIN);
	spi->SPI_CR =   SPI_CR_SWRST; //SWRST  //SPIDIS
	return d & 0xFF;

}



void SPIClass::get_data(void){
		uint32_t source=0;
		uint8_t vnum;
		uint16_t vol;

		uint16_t analog ;

		source = spi->SPI_RDR;
		vnum = ((source&0xF000)>>12)&0xF;
		vol  = ((source&0x0FFF)>>0)&0x0FFF;
		volume[vnum]=vol;
		spi->SPI_TDR = slave_TDR;
		

}

void SPIClass::get_data2(void){
	uint16_t source=0;
		uint8_t  vnum_0;
		uint8_t vol_0;
		uint8_t  vnum_1;
		uint8_t vol_1;

		source = spi->SPI_RDR;
		vnum_0=((source&0xF000)>>12)&0xF;
		vol_0= ((source&0x0F00)>>8)&0xF;
		volume[vnum_0]= MIN[vnum_0] + ((double)vol_0/(double)15)*((double)MAX[vnum_0]-(double)MIN[vnum_0]); //2700*((double)vol_0/(double)15);
		vnum_1=((source&0x00F0)>>4)&0xF;
		vol_1= ((source&0x000F)>>0)&0xF;
		volume[vnum_1]=MIN[vnum_1] + ((double)vol_1/(double)15)*((double)MAX[vnum_1]-(double)MIN[vnum_1]);//2700*((double)vol_1/(double)15);
		

}

void SPIClass::attachInterrupt(void) {
	// Should be enableInterrupt()
}

void SPIClass::detachInterrupt(void) {
	// Should be disableInterrupt()
}

#if SPI_INTERFACES_COUNT > 0
static void SPI_0_Init(void) {
	PIO_Configure(
			g_APinDescription[PIN_SPI_MOSI].pPort,
			g_APinDescription[PIN_SPI_MOSI].ulPinType,
			g_APinDescription[PIN_SPI_MOSI].ulPin,
			g_APinDescription[PIN_SPI_MOSI].ulPinConfiguration);
	PIO_Configure(
			g_APinDescription[PIN_SPI_MISO].pPort,
			g_APinDescription[PIN_SPI_MISO].ulPinType,
			g_APinDescription[PIN_SPI_MISO].ulPin,
			g_APinDescription[PIN_SPI_MISO].ulPinConfiguration);
	PIO_Configure(
			g_APinDescription[PIN_SPI_SCK].pPort,
			g_APinDescription[PIN_SPI_SCK].ulPinType,
			g_APinDescription[PIN_SPI_SCK].ulPin,
			g_APinDescription[PIN_SPI_SCK].ulPinConfiguration);
}

SPIClass SPI(SPI_INTERFACE, SPI_INTERFACE_ID, SPI_0_Init);
#endif
