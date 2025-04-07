
#include "i2c_init.h"

/*!
* @brief:	Function for initialization I2C 
* @note:	PB11 - SDA2
*					PB10 - SCL2
*/
void 
init_I2C2(void){
	
	// Enable GPIOB and alt. func. 
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;   			
	
	// PB10 - SCL
	GPIOB->CRH &= (~GPIO_CRH_MODE10); 		// reset MODE10
	GPIOB->CRH |= GPIO_CRH_MODE10; 				// 01: Output mode, max speed 10 MHz.
	GPIOB->CRH &= (~GPIO_CRH_CNF10); 			// reset CNF10
	GPIOB->CRH |= GPIO_CRH_CNF10; 				// 11: Alternate function output Open-drain
	
	// PB11 - SDA
	GPIOB->CRH &= (~GPIO_CRH_MODE11); 		// reset MODE11
	GPIOB->CRH |= GPIO_CRH_MODE11; 				// 01: Output mode, max speed 10 MHz.
	GPIOB->CRH &= (~GPIO_CRH_CNF11); 			// reset CNF11
	GPIOB->CRH |= GPIO_CRH_CNF11; 				// 11: Alternate function output Open-drain

	// Reset i2c
	I2C2->CR1 |= I2C_CR1_SWRST;
	// Enable I2C2 Clock
	RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;   				
	 
	I2C_InitTypeDef Setup_I2C2 = { 
		.I2C_Mode = I2C_Mode_I2C, 
		.I2C_OwnAddress1 = 0x55, 
		.I2C_DutyCycle = I2C_DutyCycle_2,
		.I2C_ClockSpeed = 400000,
		.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit, 
		.I2C_Ack = I2C_Ack_Enable
	}; 
	
	I2C_Init(I2C2, &Setup_I2C2); 
	
	// Enable I2C  
	I2C2->CR1 |= I2C_CR1_PE; 
}

/*!
* @brief: 	START-bit generation functione
* @param: 	i2c - pointer to descriptor i2C;  
* @retval:	0 - is OK; (-1) - ERROR. 
*/
int8_t 
I2C_SendStartBit(I2C_TypeDef *i2c){
	
	// bus busy?
	if( (i2c->SR2 & I2C_SR2_BUSY) ){
		// the bus is busy...			
		return -1; 
	} 
	
	// check Start bit
	if( (i2c->SR1 & I2C_SR1_SB) ){
		// if here, then SB (Start Bit) is set, then there is some error on the line			
		return -1; 
	} 
	
	// generating start
	i2c->CR1 |= I2C_CR1_START;
	// waiting for the bit to signal that START is on the line
	while( !(i2c->SR1 & I2C_SR1_SB) )
		continue; 
	(void) i2c->SR1;
	
	return 0; 
}

/*!
* @brief: 	STOP-bit generation functione
* @param: 	i2c - pointer to descriptor i2C;  
* @retval:	0 - is OK; (-1) - ERROR. 
*/
int8_t 
I2C_SendStopBit(I2C_TypeDef *i2c){

	// generating stop
	i2c->CR1 |= I2C_CR1_STOP;

	// waiting, when I switch to slave mode
	while( i2c->SR2 & I2C_SR2_MSL )
		continue; 
	
	return 0; 
}

/*!
* @brief: 	Function for sending an address byte with a command (read or write)
* @param: 	send_addres - address (no need to move!!);
*						direction - command, takes values: READ_DATA или WRITE_DATA
* @retval:	0 - is OK; (-1) - ERROR. 
*/
int8_t 
I2C_SendAddr(I2C_TypeDef *i2c, uint8_t send_addres, uint8_t direction){
	
	// waiting for it to switch to master mode
	while(!(i2c->SR2 & I2C_SR2_MSL))
		continue; 
	
	if( direction > 0 )	
		send_addres = (send_addres << 0) | 1U; 
	else							
		send_addres = (send_addres << 0) & ~1U;
	
	// transmit the device address
	i2c->DR = send_addres;
	
	// waiting for the address to be transferred
	while( !(I2C2->SR1 & I2C_SR1_ADDR) )
		continue; 
	(void) I2C2->SR1;
	(void) I2C2->SR2;
	
	// waiting here for the SB bit to reset
	while( i2c->SR1 & I2C_SR1_SB )
		continue; 
	
	return 0; 
} 

/*!
* @brief: 	Function for reading
* @param: 	i2c - descriptor; 
*						addr_device - address (no need to move!!);
*						addr_reg - address of registr; 
*						data_out - pointer to the array of returned data; 
*						quant_bytes - number of bytes to read.
* @retval:	0 - is OK; (-1) - ERROR. 
*/
int8_t 
I2C_ReadData(I2C_TypeDef *i2c, uint8_t addr_device, uint8_t addr_reg,  uint8_t* data_out, uint8_t quant_bytes){
	
	I2C_SendStartBit(i2c); 
	I2C_SendAddr(i2c, addr_device, WRITE_DATA); 
	
	/* передаю адрес регистра */ 
	I2C2->DR = addr_reg;	
	while( !(I2C2->SR1 & I2C_SR1_BTF) )
		continue;	
	
	// отправляю СТОП-бит
	I2C_SendStopBit(i2c);

	// повторный СТАРТ-бит и отправка адреса устройства 	
	I2C_SendStartBit(i2c); 
	I2C_SendAddr(i2c, addr_device, READ_DATA);
	
	
	i2c->CR1 |= I2C_CR1_ACK;
	
	uint8_t count_bytes = 0;
	while(count_bytes < quant_bytes){
		
		if( count_bytes == (quant_bytes-1) )
			i2c->CR1 &= ~I2C_CR1_ACK;
		
		while( !(i2c->SR1 & I2C_SR1_RXNE) )
			continue;
		
		data_out[count_bytes] = i2c->DR;
			
		++count_bytes; 
	}
	
	I2C_SendStopBit(i2c); 	
	return 0;
} 	

/*!
* @brief: 	Function for writing bytes
* @param: 	i2c - descriptor; 
*						addr_device - address (no need to move!!);
*						data_in - pointer to an array of bytes to write to the device; 
*						quant_bytes - number of bytes to write.
* @retval:	0 - is OK; (-1) - ERROR. 
*/
int8_t 
I2C_WriteBytes(I2C_TypeDef *i2c, uint8_t addr_device, uint8_t* data_in, uint8_t quant_bytes){

	I2C_SendStartBit(i2c); 
	I2C_SendAddr(i2c, addr_device, WRITE_DATA); 
	
	i2c->DR = *data_in;	
	while( !(i2c->SR1 & I2C_SR1_TXE) )
		continue;
	
	++data_in; 
	--quant_bytes; 
	while( quant_bytes-- != 0 ){
		
		i2c->DR = *data_in++;	
		while( !(i2c->SR1 & I2C_SR1_BTF) )
			continue; 
	}
	
	I2C_SendStopBit(i2c); 	
	return 0;
}

/*!
* @brief: 	Function to write one byte
* @param: 	i2c - descriptor; 
*						addr_device - address (no need to move!!);
*						data_in - byte to write.  
* @retval:	0 - is OK; (-1) - ERROR. 
*/
int8_t 
I2C_WriteByte(I2C_TypeDef *i2c, uint8_t addr_device, uint8_t data_in){
	
	I2C_SendStartBit(i2c); 
	I2C_SendAddr(i2c, addr_device, WRITE_DATA);
	
	I2C2->DR = data_in;	
	while( !(I2C2->SR1 & I2C_SR1_BTF) )
		continue;	
 
	I2C_SendStopBit(i2c); 	
	return 0;
	
}


/*!
* @brief: 	Function for searching devices on the line
* @param: 	i2c - pointer to descriptor i2C; 
*						addr - address for check. 
* @retval:	0 - no device for this address; 
						1 - yes device for this address. 
*/
int8_t 
scanAddrDevice(I2C_TypeDef *i2c, uint8_t addr){
	
	int8_t result = 0; 
		
	if((i2c->SR2 & I2C_SR2_BUSY)){
		// если тут, значит шина занята, на шине идет обмен и линии SCL, SDA  к 0 	
		return -1; 
	} 
	
	if((i2c->SR1 & I2C_SR1_SB)){
		// если тут, значит SB (Start Bit) выставлен, значит какая-то ошибка на линии 	
		return -1; 
	} 
	
	// addr <<= 1; 																			// сдвигаю адрес на 1 бит влево 
	addr &= 0b11111110;																// в последний бит ставлю 0, чтобы задать команду на чтение байта 							 
	
	/* генерация СТАРТ-бита */ 
	i2c->CR1 |= I2C_CR1_START;
	
	/* тут жду, когда СТАРТ появится на линии */ 
	while(!(i2c->SR1 & I2C_SR1_SB)){
		continue;
	}
	
	
	/* --------------------- для очистки I2C_SR1_SB -------------------------- */ 
	/* чтение SR1 для сброса бита SB */ 
	(void)i2c->SR1; 
	/* запись в регистр DR адреса ведомого у стройства на линии */ 
	i2c->DR = addr;
	/* --------------------------------------------------------------------- */ 
	
	/* тут жду, когда сбросится бит SB */ 
	while( i2c->SR1 & I2C_SR1_SB ){
		continue; 
	}
	
	
	/* задержка перед чтением бита подтверждения от ведомого */ 	
	for(int i=0; i<100000; ++i)
		continue; 
	
	
	/* проверяю наличие бита подтверждения */ 
	if( i2c->SR1 & I2C_SR1_ADDR )
		result = 1; 
	else
		result = 0; 
	
	(void) i2c->SR1;
	(void) i2c->SR2;
	
	
	/* генерация СТОП-бита */ 
	i2c->CR1 |= I2C_CR1_STOP;
				
	// жду, когда и2с выйдет из ведущего режима
	// жду когда задетектится СТОП-бит на шине
	while((i2c->SR2 & I2C_SR2_MSL))
		continue; 
			
	return result; 
	
}

void 
sendCommand(I2C_TypeDef *i2c, uint8_t addr_device, uint8_t* data, uint8_t quant_bytes){
	
	I2C_SendStartBit(i2c); 
	I2C_SendAddr(i2c, addr_device, WRITE_DATA); 
	
	// отправляю байт, что будет отправляться команда
	i2c->DR = 0x00;	
	while( !(i2c->SR1 & I2C_SR1_TXE) )
		continue;
	
	
	while( quant_bytes-- != 0 ){
		
		i2c->DR = *data++;	
		while( !(i2c->SR1 & I2C_SR1_BTF) )
			continue; 
	}
	
	I2C_SendStopBit(i2c); 	
	
}

void 
sendData(I2C_TypeDef *i2c, uint8_t addr_device, uint8_t* data, uint32_t quant_bytes){
	
	I2C_SendStartBit(i2c); 
	I2C_SendAddr(i2c, addr_device, WRITE_DATA); 
	
	// отправляю байт, что будет отправляться команда
	i2c->DR = 0x40;	
	while( !(i2c->SR1 & I2C_SR1_TXE) )
		continue;
	
	while( quant_bytes-- != 0 ){
		
		i2c->DR = *data++;	
		while( !(i2c->SR1 & I2C_SR1_BTF) )
			continue; 
	}
	
	I2C_SendStopBit(i2c); 	
	
}






