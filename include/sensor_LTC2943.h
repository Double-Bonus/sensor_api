#include <stdint.h>
#include <stdbool.h>

#pragma once

// for now enought!
typedef enum eLTC2943Result {
  LTC2943_RESULT_OK = 0,
  LTC2943_FAILED,
  LTC2943_MEM_ERROR
} eLTC2943Result_t;

#ifdef __cplusplus
extern "C" {
#endif

eLTC2943Result_t check_temp_alert(bool *alert_state);

eLTC2943Result_t check_volt_alert(bool *alert_state);

void hello_from_api(void);


#ifdef __cplusplus
}
#endif
