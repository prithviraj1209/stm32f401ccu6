#include "stm32f4xx.h"

void i2c_config(void){
/************* STEPS FOLLOWED*************************
 * 
 * 1. Enable I2C clock and GPIO clock.
 * 2. Configure I2C pins for Alternate Function.
 *      a. Select Alternate Function in MODER register.
 *      b. Select Open-Drain output.
 *      c. Select HIGH SPEED for pins.
 *      d. Select PULL UP for pins.
 *      e. Configure Alternate Function in AFR register.
 * 3. Reset the I2C.
 * 4. Program the input peripheral clock in I2C_CR2 register.
 * 5. Configure the clock control register.
 * 6. Configure the rise time register.
 * 7. Program the I2C_CR1 register to enable the peripheral.
 * 
 ****************************************************/
    // 1. Enable I2C clock and GPIO clock
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;  // Enable I2C1 clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // Enable GPIOB clock

    // 2. Configure I2C pins for Alternate Function
    // a. Select Alternate Function in MODER register
    GPIOB->MODER &= ~((0x3 << (8 * 2)) | (0x3 << (9 * 2))); // Clear MODER8 and MODER9
    GPIOB->MODER |= (0x2 << (8 * 2)) | (0x2 << (9 * 2));    // Set Alternate Function (AF) for PB8 (I2C1_SCL) and PB9 (I2C1_SDA)

    // b. Select Open-Drain output
    GPIOB->OTYPER |= (0x1 << 8) | (0x1 << 9); // Set Open-Drain for PB8 and PB9

    // c. Select HIGH SPEED for pins
    GPIOB->OSPEEDR |= (0x3 << (8 * 2)) | (0x3 << (9 * 2)); // High speed for PB8 and PB9

    // d. Select PULL UP for pins
    GPIOB->PUPDR &= ~((0x3 << (8 * 2)) | (0x3 << (9 * 2))); // Clear PUPDR8 and PUPDR9
    GPIOB->PUPDR |= (0x1 << (8 * 2)) | (0x1 << (9 * 2));    // Set Pull-Up for PB8 and PB9

    // e. Configure Alternate Function in AFR register
    GPIOB->AFR[1] &= ~((0xF << ((8 - 8) * 4)) | (0xF << ((9 - 8) * 4))); // Clear AFR for PB8 and PB9
    GPIOB->AFR[1] |= (0x4 << ((8 - 8) * 4)) | (0x4 << ((9 - 8) * 4));    // Set AF4 (I2C1) for PB8 and PB9

    // 3. Reset the I2C
    I2C1->CR1 |= I2C_CR1_SWRST;
    I2C1->CR1 &= ~I2C_CR1_SWRST;

    // 4. Program the input peripheral clock in I2C_CR2 register
    I2C1->CR2 = 42; // Assuming 42 MHz APB1 clock

    // 5. Configure the clock control register
    I2C1->CCR = 210; // 100kHz I2C clock assuming 42 MHz APB1 clock

    // 6. Configure the rise time register
    I2C1->TRISE = 43; // Max rise time = 1000ns / (1/42MHz) + 1

    // 7. Program the I2C_CR1 register to enable the peripheral
    I2C1->CR1 |= I2C_CR1_PE;
	
}

void i2c_startSeq(void){
	  /*************** STEPS FOLLOWED******************
     * 1. Send the start sequnce.
     * 2. Wait for the SB bit to be set.
    ************************************************/
    // 1. Send the start sequence
    I2C1->CR1 |= I2C_CR1_START;

    // 2. Wait for the SB bit to be set
    while (!(I2C1->SR1 & I2C_SR1_SB));
}

void i2c_stopSeq(void){	
		//1. Send the STOP sequence
		I2C1->CR1 |= I2C_CR1_STOP;
}

void i2c_write(uint8_t data) {
    // 1. Wait for TXE bit to be set
    while (!(I2C1->SR1 & I2C_SR1_TXE));

    // 2. Send data into DR register
    I2C1->DR = data;

    // 3. Wait for BTF bit to be set
    while (!(I2C1->SR1 & I2C_SR1_BTF));
}

void i2c_writeMul(uint8_t *data, uint8_t size){
		// 1. Wait for TXE bit to be set
    while (!(I2C1->SR1 & I2C_SR1_TXE));

    // 2. Send data into DR register
    for (uint8_t i = 0; i < size; i++) {
        while (!(I2C1->SR1 & I2C_SR1_TXE)); // Wait for TXE bit to be set
        I2C1->DR = data[i]; // Send data
    }

    // 3. Wait for BTF bit to be set
    while (!(I2C1->SR1 & I2C_SR1_BTF));
}

void i2c_address(uint8_t address){
		// 1. Send address into DR res
		I2C1->DR = address & 0xFE;
	
		// 2. Wait for ack
		while (!(I2C1->SR1 & I2C_SR1_ADDR));
	
		// 3. Clear the ADDR flag by reading SR1 and SR2
		(void)I2C1->SR2;
}

