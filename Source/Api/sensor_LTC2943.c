/**
 * @file sensor_LTC2943.c
 * @brief API function implementation for LTC2943 IC
 * @date 2021-11-13
 * 
 */

// TODO
/*
    1. Change chip's ADC mode into one of these modes: automatic mode, scan mode, manual mode, sleep.
    2. Get chip's ADC mode.
*/


/* Includes -----------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h> // just to show working basic build
#include "sensor_LTC2943.h"
#include "i2c.h"

/* Private typedef ----------------------------*/

// status register bitmap (Rename!)
typedef enum eStatusRegister {
  strg_UndervoltageLockoutAlert = 0,
  strg_VoltageAlert,
  strg_ChargeAlertLow,
  strg_ChargeAlertHigh,
  strg_TemperatureAlert,
  strg_AccumulatedChargeOverUnderFlow,
  strg_CurrentAlert,
  strg_Reserved = 7
} eStatusRegister_t;

/* Private define -----------------------------*/
#define LTC2943_SLAVE_I2C_ADDR 0x64 // 0b0110'0100

/* Internal IC Registers map (from Table 1. Register Map) */
#define REG_A_STATUS 0x00
#define REG_A_CONTROL 0x01
#define REG_C_ACC_CHARGE_MSB 0x02
#define REG_C_ACC_CHARGE_LSB 0x03
// ...... (as oher aren't used in this example)
#define REG_X_TEMP_THR_L 0x17

/* Private macro ------------------------------*/
/* Private variables --------------------------*/
/* Private function prototypes ----------------*/
/* Private functions --------------------------*/

//rename prbl
static bool is_ltc2943_setup()
{
	return i2cIsInitialized();
}

static eLTC2943Result_t read_register(const uint8_t *regs, uint8_t *data_out, const uint16_t size)
{
	eLTC2943Result_t res = LTC2943_FAILED;

	if (NULL != regs) {
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


/* Functions ----------------------------------*/
eLTC2943Result_t check_temp_alert(bool *alert_state)
{
  return read_ltc2943_alert(alert_state, strg_TemperatureAlert);
}

eLTC2943Result_t check_volt_alert(bool *alert_state)
{
  return read_ltc2943_alert(alert_state, strg_VoltageAlert);
}

// function to show working app
void say_hi_from_api(void)
{
   printf("Hello from api! :) \n");
}
