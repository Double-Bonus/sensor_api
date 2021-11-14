#include <stdint.h>
#include <stdbool.h>

#pragma once

// for now enought!
typedef enum eLTC2943Result {
  LTC2943_RESULT_OK = 0,
  LTC2943_FAILED,
  LTC2943_MEM_ERROR
} eLTC2943Result_t;

typedef enum eLTC2943AdcMode {
  adcm_undefined = 0,
  adcm_Automatic,     // continuously performing voltage, current and temperature conversions
  adcm_Scan,          // performing voltage, current and temperature conversion every 10s
  adcm_Manual,        // performing single conversions of voltage, current and temperature then sleep
  adcm_Sleep,
  adcm_last
} eLTC2943AdcMode_t;

#ifdef __cplusplus
extern "C" {
#endif

eLTC2943Result_t check_temp_alert(bool *alert_state);

eLTC2943Result_t check_volt_alert(bool *alert_state);

eLTC2943Result_t get_adc_mode(eLTC2943AdcMode_t *adc_mode);

eLTC2943Result_t set_adc_mode(const eLTC2943AdcMode_t adc_mode);

void say_hi_from_api(void);


#ifdef __cplusplus
}
#endif
