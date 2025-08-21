#ifndef PS4_PATCHER_H
#define PS4_PATCHER_H

#include "ps4_validator.h"

void ps4_toggle(PS4Nor* obj, unsigned int offset, unsigned char on, unsigned char off);
void ps4_toggle_uart(PS4Nor* obj);
void ps4_set_memory_clock(PS4Nor* obj);
void ps4_set_boot_mode(PS4Nor* obj);
void ps4_set_gddr5_debug_flag(PS4Nor* obj);
void ps4_patch_corrupted_CoreOS(PS4Nor* obj);
void ps4_patch_eap_key(PS4Nor* obj);

#endif //PS4_PATCHER_H
