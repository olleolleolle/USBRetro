// sony_jog.h
#ifndef SONY_JOG_HEADER_H
#define SONY_JOG_HEADER_H

#include "device_interface.h"
#include "device_utils.h"
#include "tusb.h"

extern DeviceInterface sony_jog_interface;

// Sony Jog Controller (PCVA-JC1)
typedef struct TU_ATTR_PACKED
{
  // btn byte
  struct {
    uint8_t a:       1;
    uint8_t b:       1;
    uint8_t c:       1;
    uint8_t d:       1;
    uint8_t e:       1;
    uint8_t f:       1;
    uint8_t none:    1;
    uint8_t button_pressed: 1;
  };
  
  // dir byte
  struct {
    uint8_t center: 1;
    uint8_t up:     1;
    uint8_t down:   1;
    uint8_t left:   1;
    uint8_t right:  1;
    uint8_t none2:  2;
    uint8_t arrow_pressed: 1;
  };
  
  // jog byte
  struct {
    uint8_t turned2: 1;
    uint8_t none3: 5;
    uint8_t turnedLeft: 1;
    uint8_t turned: 1;
  };

} sony_jog_report_t;

uint32_t buttons;

#endif
