/**
 * @file sensor_LTC2943.c
 * @brief API functions implementation for LTC2943 IC
 * @date 2021-11-13
 * 
 */

/* Includes -----------------------------------*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h> // just to show working basic build
#include "sensor_LTC2943.h"
#include "i2c.h"

/* Private typedef ----------------------------*/
// status register bitmap
typedef enum eStatusRegister {
  strg_UndervoltageLockoutAlert = 0,
  strg_VoltageAlert,
  strg_ChargeAlertLow,
  strg_ChargeAlertHigh,
  strg_TemperatureAlert,
  strg_AccumChargeOverUnderFlow,
  strg_CurrentAlert,
  strg_Reserved = 7
} eStatusRegister_t;

/* Private define -----------------------------*/
#define LTC2943_SLAVE_I2C_ADDR 0x64 // 0b0110'0100

/* Internal IC Registers map (from Table 1. Register Map) */
#define REG_A_STATUS 0x00
#define REG_B_CONTROL 0x01
#define REG_C_ACC_CHARGE_MSB 0x02
#define REG_D_ACC_CHARGE_LSB 0x03
// ...... (as others aren't used in this example)
#define REG_X_TEMP_THR_L 0x17

/* Private macro ------------------------------*/
/* Private variables --------------------------*/
/* Private function prototypes ----------------*/
/* Private functions --------------------------*/
static bool is_ltc2943_setup()
{
	return i2cIsInitialized();
}

static eLTC2943Result_t read_register(const uint8_t *regs, uint8_t *data_out, const uint16_t size)
{
	eLTC2943Result_t res = LTC2943_FAILED;

	if ((NULL != regs) || (0 == size)) {
	  if (i2cWrite(LTC2943_SLAVE_I2C_ADDR, regs, 1)) {
	    if (i2cRead(LTC2943_SLAVE_I2C_ADDR, data_out, size)) {
			res = LTC2943_RESULT_OK;
		}
	  }
	} else {
		res = LTC2943_MEM_ERROR;
	}
	return res;
}

static eLTC2943Result_t write_register(const uint8_t *regs, const uint8_t *data_in, const uint16_t size)
{
	eLTC2943Result_t res = LTC2943_FAILED;
	uint8_t *temp_buff = NULL;

	if((NULL == data_in) || (0 == size) || (NULL == regs)) {
		res = LTC2943_MEM_ERROR;
	} else {
		temp_buff = calloc(1, size + 1);
	}
	if(NULL != temp_buff) {
		memcpy(temp_buff, regs, 1);
		memcpy(temp_buff + 1, data_in, size);
		if (i2cWrite(LTC2943_SLAVE_I2C_ADDR, temp_buff, size + 1)) {
		  res = LTC2943_RESULT_OK;
		}
		free(temp_buff);
	}
	return res;
}

static eLTC2943Result_t read_ltc2943_alert(bool *alert_state, const eStatusRegister_t bit_nr)
{
	eLTC2943Result_t res = LTC2943_FAILED;
    const uint8_t stat_rg = REG_A_STATUS;
	uint8_t reg_data = 0;

	if ((is_ltc2943_setup()) && (NULL != alert_state)) {
      res = read_register(&stat_rg, &reg_data, sizeof(reg_data));
	  if(LTC2943_RESULT_OK == res) {
        *alert_state = (reg_data & (1 << bit_nr));
	  }
	}
	return res;
}

/**
 * @brief Helper function to get ADC mode from register data
 * 
 * @param data - register data
 * @return ADC mode enum 
 */
static eLTC2943AdcMode_t get_adc_mode_from_data(const uint8_t data)
{
   eLTC2943AdcMode_t mode = adcm_undefined;

   switch(data & 0x03) { // Last two bits (B[7:6]) shows ADC Mode
	case 0: 
		mode = adcm_Sleep;
		break;
	case 1:
		mode = adcm_Manual;
		break;
	case 2:
		mode = adcm_Scan;
		break;
	case 3:
		mode = adcm_Automatic;
		break;
   }
   return mode;
}

/* Functions ----------------------------------*/
/**
 * @brief Function checks if one of the temperature limits was exceeded
 * @param alert_state - pointer to response bool
 * @return LTC2943_RESULT_OK if operation was performed successfully. 
 */
eLTC2943Result_t check_temp_alert(bool *alert_state)
{
  return read_ltc2943_alert(alert_state, strg_TemperatureAlert);
}

/**
 * @brief Function checks if one of the current limits was exceeded
 * 
 * @param alert_state - pointer to response bool
 * @return LTC2943_RESULT_OK if operation was performed successfully. 
 */
eLTC2943Result_t check_volt_alert(bool *alert_state)
{
  return read_ltc2943_alert(alert_state, strg_VoltageAlert);
}

/**
 * @brief Gets currently active LTC2943 ADC mode to
 * 
 * @param adc_mode - Pointer to ADC mode
 * @return LTC2943_RESULT_OK if operation was performed successfully.
 */
eLTC2943Result_t get_adc_mode(eLTC2943AdcMode_t *adc_mode)
{
	eLTC2943Result_t res = LTC2943_FAILED;
    const uint8_t cntr_rg_ptr = REG_B_CONTROL;
	uint8_t reg_data = 0;
	
	if (!is_ltc2943_setup()) {
		 res = LTC2943_FAILED;
	} else if(NULL == adc_mode){
         res = LTC2943_MEM_ERROR;
	} else {
		*adc_mode = adcm_undefined;
		res = read_register(&cntr_rg_ptr, &reg_data, sizeof(reg_data));
		if(LTC2943_RESULT_OK == res) {
		*adc_mode = get_adc_mode_from_data(reg_data);
		res = LTC2943_RESULT_OK;
		}
	}
	return res;
}

/**
 * @brief Sets LTC2943 ADC mode to given parameter
 * 
 * @param adc_mode - Enum for ADC mode
 * @return LTC2943_RESULT_OK if operation was performed successfully.
 */
eLTC2943Result_t set_adc_mode(const eLTC2943AdcMode_t adc_mode)
{
   eLTC2943Result_t res = LTC2943_FAILED;
	const uint8_t cntr_rg_ptr = REG_B_CONTROL;
	uint8_t reg_data = 0;

	if (!is_ltc2943_setup() || ((adcm_undefined == adc_mode) || (adcm_last == adc_mode))) {
		res = LTC2943_FAILED;
	} else {
		// get all register data bc we change only last 2 bits
		res = read_register(&cntr_rg_ptr, &reg_data, sizeof(reg_data));
		if(LTC2943_RESULT_OK == res) {
		reg_data = (reg_data & (~0x3)) | adc_mode; // clear last 2 bits and set to new adc mode
		res = write_register(&cntr_rg_ptr, &reg_data, sizeof(reg_data));
		}
	}
	return res;
}

// function just to show working app
void say_hi_from_api(void)
{
   printf("Hello from api! :) \n");
}