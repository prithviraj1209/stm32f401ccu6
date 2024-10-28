#include "i2c_stm32f401ccu6.h"
#include "LCD_i2c.h"

static void writeExpander(uint8_t value, uint8_t mode);
static void writeNibble(uint8_t value_n);

void lcd_init(void){
		
}

void lcd_sendCommand(uint8_t cmd){
		writeExpander(cmd, RS0);

}

void lcd_sendData(uint8_t data){
		writeExpander(data, RS1);
}

static void writeExpander(uint8_t value, uint8_t mode){
		uint8_t hNibble = (value & 0xF0);
		uint8_t lNibble = ((value << 4) & 0xF0);
		writeNibble(hNibble | mode | ENABLE);
		writeNibble(lNibble | mode | ENABLE);
}

static void writeNibble(uint8_t value_n){
		i2c_transmit(DEVICE_ADDR, value_n);
		i2c_transmit(DEVICE_ADDR, (value_n & (~ENABLE)));
}

