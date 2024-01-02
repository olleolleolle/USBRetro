// sony_psvr2.h
#ifndef SONY_PSVR2_HEADER_H
#define SONY_PSVR2_HEADER_H

#include "device_interface.h"
#include "device_utils.h"
#include "tusb.h"

extern DeviceInterface sony_psvr2_interface;

// Sony PSVR2 Sense controllers (Left/Right)
typedef struct TU_ATTR_PACKED
{
  uint8_t x1, y1, // analog x,y axis
          x2,     // R2 trigger axis
          y2,     // touch R2 axis
          rx,     // touch R1 axis
          ry, rz;

  struct {
    uint8_t square   : 1; //
    uint8_t cross    : 1; //
    uint8_t circle   : 1; //
    uint8_t triangle : 1; //
    uint8_t l1       : 1; // back grip button L1
    uint8_t r1       : 1; //
    uint8_t l2       : 1; //
    uint8_t r2       : 1; //
  };

  struct {
    uint8_t share  : 1; //
    uint8_t option : 1; //
    uint8_t l3     : 1; //
    uint8_t r3     : 1; //
    uint8_t ps     : 1; // playstation button
    uint8_t unkn_a : 2;
    uint8_t t_l2   : 1; //
  };

  struct {
    uint8_t t_triangle : 1; //
    uint8_t t_square   : 1; // track pad click
    uint8_t t_l3       : 1; // touching analog stick (L3)
    uint8_t t_l1       : 1; //
    uint8_t unkn_b     : 4;
  };

  int16_t  gyro[3];  // x, y, z;
  int16_t  accel[3]; // x, y, z

  int8_t counter;

} sony_psvr2_report_t;

uint32_t buttons;

#endif
