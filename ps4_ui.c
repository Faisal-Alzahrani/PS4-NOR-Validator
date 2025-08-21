#include <stdio.h>
#include <stdarg.h>

#include "ps4_def.h"
#include "ps4_validator.h"
#include "ps4_patcher.h"
#include "ps4_wifi.h"
#include "ps4_ui.h"
#include "cprintf.h"

//define something for Windows (32-bit and 64-bit, this part is common)
static const char* logo =

"==========================================================\n"
"=           PS4 Serial Flash NOR Dump Validator          =\n"
"==========================================================\n"
"                    version: 1.4 BETA                     \n"
"Written in C By Faisal Alzahrani                July, 2023\n"
"==========================================================\n"
;

char names[10][FILENAME_MAX];
char* filename;
int count = 0;
int choice = -1;
PS4Nor target;
PS4Nor delta;
void ps4_print_logo(void);
void ps4_print_file_menu(void);
void ps4_print_main_menu(void);
void ps4_print_flags_menu(void);
void ps4_print_repair_menu(void);
void ps4_save(void);
void ps4_print_note(Color color, const char* note);
void ps4_print_menu_list(const char* title, const char* list);
int ps4_get_user_input(int count);


int ps4_ui_init(int argc, char* argv[])
{
    //ps4_init(&delta, 0);
	ps4_print_file_menu();
	/*
	if (argc > 1)
	{
		char* filename = argv[1];
		ps4_init(&target, filename);
		ps4_validate(&target, &delta, 0);
		system("pause");
	}
	else
	{
		ps4_print_file_menu();
		return 0;
	}
	*/
	return -1;
}

void  ps4_print_logo()
{
	cc_clear();
	cc_set_color(LIME);
	printf(logo);
	cc_set_color(WHITE);
}

void ps4_print_note(Color color, const char* note)
{
	cc_set_color(color);
	printf("----------------------------------------------------------\n");
	printf(note);
	printf("\n----------------------------------------------------------\n\n");
	cc_set_color(WHITE);
	
}

void ps4_print_file_menu()
{
    for (;;)
    {
        ps4_print_logo();
        ps4_print_note(
            YELLOW,
            "Place the NOR dump files in directory.\n"
            "The files extension must be \"*.bin\" or \"*.hex\"."
            );

        cc_set_color(LIME);
        printf(

            "FILE SELECTION MENU:\n"
            "====================\n"
            );
        cc_set_color(SILVER);

        DIR* d;
        struct dirent* dir;
        d = opendir(".");

        if(d)
        {
            count = 0;
            while (count < 10 && (dir = readdir(d)) != NULL)
            {
                memset(names[count], '\0', FILENAME_MAX);
                char* s = dir->d_name;
                unsigned int len = strlen(s);
                int r = 1;

                if (len > 3)
                {
                    r = strncmp(s + len - 3, "bin", 3);
                    if (r) r = strncmp(s + len - 3, "BIN", 3);

                    if (r)
                    {
                        r = strncmp(s + len - 3, "hex", 3);
                        if (r) r = strncmp(s + len - 3, "HEX", 3);
                    }
                }

                if (!r)
                {
                    strcpy(names[count], s);
                    printf("%2u.%s\n", count + 1, s);
                    ++count;
                }
            }

            closedir(d);

            if (count > 0)
            {
                cc_set_color(GREY);
                printf(" 0: EXIT\n\n" );
                choice = -1;


                while (!(choice >= 0 && choice <= count))
                {
                    cc_set_color(RED);
                    printf("Make a choice:");
                    cc_set_color(WHITE);
                    scanf("%u", &choice);
                }

                if (choice == 0)
                {
                    exit(0);
                }

                filename = names[choice - 1];

                if (ps4_init(&target, filename))
                {
                    ps4_print_main_menu();
                }
                else
                {
                    cc_set_color(YELLOW);
                    printf("invalid file...\n");
                    system("pause");
                }
            }
            else
            {
                ps4_print_note(YELLOW,"Copy files to this folder then hit enter..");
                system("pause");
            }
        }
        else
        {
            ps4_print_note(YELLOW,"Could not open current directory..");
            system("pause");
            system("exit");
        }
    }
}


void ps4_print_menu_list(const char* title, const char* list)
{
	cc_set_color(YELLOW);
	printf(title);
	cc_set_color(SILVER);
	printf(list);
	cc_set_color(GREY);
	printf(" 0. EXIT.\n");
}

int ps4_get_user_input(int itemsCount)
{
	int x = -1;
	do
	{
		cc_set_color(RED);
		printf("\nMake a choice:");
		cc_set_color(WHITE);
		scanf("%u", &x);
	} while (!(x > -1 && x < itemsCount +1));

	

	return x;
}

void ps4_print_main_menu()
{
	for (;;)
	{
		cc_clear();
		ps4_info(&target);
		ps4_print_menu_list(
			"\nMAIN MENU:\n"
			"==========\n",
			" 1. Validate.\n"
			" 2. Set System flags.\n"
			" 3. Repair.\n"
			" 4. Save.\n"
		);

		int x = ps4_get_user_input(4);

		cc_clear();

		switch (x)
		{
		case 0:
			return;
		case 1:
			ps4_validate(&target, &delta, 0);
			system("pause");
			break;
		case 2:
			ps4_print_flags_menu();
			break;
		case 3:
			ps4_print_repair_menu();
			break;
		case 4:
			ps4_save();
			system("pause");
			break;
		default:
			break;
		}
	}
	
}

char* str_flags_menu =
" 1. Set Memory Clock (GDDR5).   8. Toggle Registry Recovery Mode.\n"
" 2. Set Boot Mode.              9. Toggle Arcade Mode.\n"
" 3. Toggle Memory Budget Mode. 10. Toggle Memory Test.\n"
" 4. Toggle Slow HDD Mode.      11. Range/Keystorage Test.\n"
" 5. Toggle Show Mode.          12. Set GDDR5 Debug flags.\n"
" 6. Toggle IDU Mode.           13. Swap O with X Button.\n"
" 7. Toggle Update Mode.        14. Toggle UART.\n"
;

void ps4_print_flags_menu()
{
	for (;;)
	{
		cc_clear();
		ps4_check_system_flags(&target);
		ps4_print_menu_list(
			"\nSYSTEM FLAGS MENU:\n"
			"==================\n",
			str_flags_menu
		);

		int x = ps4_get_user_input(14);
		cc_clear();

		switch (x)
		{
		case 0:
			return;
		case 1:
			ps4_print_note(PINK,
				"Setting the value at 1000 Mhz or lower,\n"
				"could resolve some Loadbios & DCT[X] errors.\n"
				"PS4 effetive memory clock is 1375 Mhz."
			);
			ps4_set_memory_clock(&target);
			break;
		case 2:ps4_set_boot_mode(&target); break;
		case 3:ps4_toggle(&target, MEMORY_BUDGET, 0xFE, 0xFF); break;
		case 4:ps4_toggle(&target, SLOW_HDD_MODE, 0x01, 0xFF); break;
		case 5:ps4_toggle(&target, SHOW_MODE, 0xFE, 0xFF); break;
		case 6:ps4_toggle(&target, IDU_MODE, 0x01, 0xFF); break;
		case 7:ps4_toggle(&target, UPDATE_MODE, 0x10, 0x00); break;
		case 8:ps4_toggle(&target, REGISTRY_RECOVERY_MODE, 0x01, 0x00); break;
		case 9:ps4_toggle(&target, ARCADE_MODE, 0x01, 0xFF); break;
		case 10:ps4_toggle(&target,MEMTEST, 0x01, 0xFF); break;
		case 11:ps4_toggle(&target,RNGTEST, 0x01, 0xFF); break;
		case 12:ps4_set_gddr5_debug_flag(&target); break;
		case 13:ps4_toggle(&target,SWAP_O_WITH_X, 0x00, 0x01); break;
		case 14:ps4_toggle_uart(&target); break;
		default:
			break;
		}
	}
}


void ps4_save()
{
	int len = strlen(filename) - 4;
	//reuse memory.. bad coding..
	memcpy(names[0], filename, len);
	memcpy(names[0] + len, "_patched.bin", strlen("_patched.bin"));

	FILE* file = fopen(names[0], "wb+");

	if (file)
	{
		fwrite(&target, sizeof(char), NOR_SIZE, file);
		fclose(file);
		ps4_print_note(PINK, "File saved successfully.");
	}
	else
	{
		ps4_print_note(RED, "Could not save file.");
	}
}

char* str_repair_menu =
" 1. Patch Corrupt CoreOS (SU-30631-3 Error).\n"
" 2. Patch Corrupted EAP Key ( Panic EAP Key Not Available Error).\n"
" 3. Patch WIFI-Bluetooth (repair controller sync problem).\n"
;

void ps4_print_repair_menu()
{
	for (;;)
	{
		cc_clear();

		ps4_print_note(
			PINK,
			"All repairs here are experimental.\n"
			"Back up your data before attempting any of them."
		);

		ps4_print_menu_list(
			"REPAIR MENU:\n"
			"============\n",
			str_repair_menu
		);

		int x = ps4_get_user_input(3);
		cc_clear();

		switch (x)
		{
		case 0:
			return;
		case 1:
			ps4_print_note(PINK,
				"[Experimental] Find blank sections in the CoreOS\n"
				"section and patch them with random data.\n"
				"Attempt updating afterward."
			);
			ps4_patch_corrupted_CoreOS(&target);
			break;
		case 2:
			ps4_print_note(PINK,
				"[Experimental] Copy the non-empty key over the other\n"
				" or swap them if both are present."
			);
			ps4_patch_eap_key(&target);
			break;
		case 3:
			ps4_print_note(PINK,
				"[Experimental] Validate and replace the WIFI-bluetooth\n"
				"module if validation failed."
			);
			ps4_check_wifi(&target);
			break;
		default:
			break;
		}
	}
}
