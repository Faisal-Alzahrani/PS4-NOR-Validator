#ifndef PS4_UI_H
#define PS4_UI_H

#include "cprintf.h"

int ps4_ui_init(int argc, char* argv[]);
void ps4_print_note(Color color, const char* note);
void ps4_print_menu_list(const char* title, const char* list);
int ps4_get_user_input(int count);


#endif //PS4_UI_H