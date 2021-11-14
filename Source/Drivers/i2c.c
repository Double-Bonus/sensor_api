/**
 * @file i2c.c
 * @brief I2C driver functions
 * @date 2021-11-13
 * 
 */

#include "i2c.h"


bool i2cInit(void)
{
  return true;
}

bool i2cDeinit(void)
{
  return true;
}

bool i2cIsInitialized(void)
{
  return true;
}

// Read data from I2C device to pDst buffer
bool i2cRead(uint8_t address, uint8_t *pDst, uint16_t dataSize)
{
  return true;
}

// Write data from pSrc buffer to I2C device
bool i2cWrite(uint8_t address, const uint8_t *pSrc, uint16_t dataSize)
{
  return true;
}