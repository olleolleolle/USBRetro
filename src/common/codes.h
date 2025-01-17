// input_codes.h

#ifndef INPUT_CODES_H
#define INPUT_CODES_H

#include <stdint.h>
#include "tusb.h"
#include "globals.h"

// Define constants
#define CODE_LENGTH 10
#ifndef KONAMI_CODE
#define KONAMI_CODE {0x01, 0x01, 0x04, 0x04, 0x08, 0x02, 0x08, 0x02, 0x20, 0x10}
#endif

// Declaration of global variables
uint32_t code_buffer[CODE_LENGTH];
uint32_t konami_code[CODE_LENGTH];
bool is_fun;
unsigned char fun_inc;
unsigned char fun_player;

// Function declarations
void codes_task(void);
void __not_in_flash_func(shift_buffer_and_insert)(uint32_t new_value);
void __not_in_flash_func(check_for_konami_code)(void);

#endif // INPUT_CODES_H
