#include <time.h>

#include "ps4_def.h"
#include "ps4_patcher.h"
#include "ps4_validator.h"
#include "ps4_ui.h"


void ps4_toggle(PS4Nor* obj, unsigned int offset, unsigned char on, unsigned char off)
{
	unsigned char *buffer = obj->buffer;

	if (buffer[offset] == on)
	{
		buffer[offset] = off;
	}
	else if (buffer[offset] == off)
	{
		buffer[offset] = on;
	}
	else
	{
		buffer[offset] = off;
	}
}

void ps4_toggle_uart(PS4Nor* obj)
{
	unsigned char* buffer = obj->buffer;

	if (buffer[UART_ON_BOOT] != 0xFF)
	{
		buffer[UART_ON_BOOT] = 0xFF;
		buffer[EXTRA_UART_ON_BOOT] = 0xFF;
	}
	else
	{
		buffer[UART_ON_BOOT] = 0x01;
		buffer[EXTRA_UART_ON_BOOT] = 0x01;
	}
}

void ps4_set_memory_clock(PS4Nor* obj)
{
	unsigned char* buffer = obj->buffer;
	unsigned int x = buffer[GDDR5_CLOCK];

	if (x == 0xFF)
	{
		printf("current GDDR5 memory clock speed = (default)");
	}
	else
	{
		cprintf(stdout,"current GDDR5 memory clock speed =" CF_YELLOW" %4u MHz"CC_RESET, x * 0x19);
	}


	do
	{
		cprintf(stdout,"\nSelect a new value between" CF_YELLOW " [600~2250] "CC_RESET"Mhz(0 = default) :");
		scanf("%u", &x);
	} while (x != 0 && ( x > 2250 || x < 600));

	if (x == 0)
	{
		buffer[GDDR5_CLOCK] = 0xFF;
	}
	else
	{
		buffer[GDDR5_CLOCK] = x / 0x19;
	}
}
	

void ps4_set_boot_mode(PS4Nor* obj)
{
	unsigned char* buffer = obj->buffer;
	unsigned int x = buffer[BOOT_MODE];
	char* s;

	if (x == 0xFF)
	{
		s = "Release Mode (default)";
	}
	else if (x == 0xFB)
	{
		s = "Assist Mode";
	}else if (x == 0xFE)
	{
		s = "Development Mode";
	}
	else
	{
		s = "uknown";
	}

	cprintf(stdout, "Current boot mode ="CF_YELLOW" %s\n\n"CC_RESET, s);

	ps4_print_menu_list(
		"OPTIONS:\n"
		"========\n",
		" 1. Release Mode (default)\n"
		" 2. Assist Mode\n"
		" 3. Development Mode\n"
	);

	int c = ps4_get_user_input(3);

	if (c == 1)
	{
		x = 0xFF;
	}
	else if (c == 2)
	{
		x = 0xFB;
	}
	else if (c == 3)
	{
		x = 0xFE;
	}
	else
	{
		return;
	}

	buffer[BOOT_MODE] = x ;
}

void ps4_set_gddr5_debug_flag(PS4Nor* obj)
{
	unsigned char* buffer = obj->buffer;
	unsigned char flag = buffer[GDDR5_DEBUG_FLAG];

	for (;;)
	{
		if (flag == 0xFF)
		{
			flag = 0;
		}

		cc_clear();
		ps4_print_note( PINK, "Current activated GDDR5 flags" );
		cc_set_color(SILVER);

		#define ON CF_GREEN "on" CC_RESET
		#define OFF CF_RED "off" CC_RESET

		cprintf(stdout, "%s = %s\n", " 1.Read DBI disabled           ", (flag & 0x01) ? ON : OFF);
		cprintf(stdout, "%s = %s\n", " 2.Write DBI disabled          ", (flag & 0x02) ? ON : OFF);
		cprintf(stdout, "%s = %s\n", " 3.ABI disabled                ", (flag & 0x04) ? ON : OFF);
		cprintf(stdout, "%s = %s\n", " 4.Force auto-precharge enabled", (flag & 0x08) ? ON : OFF);
		cprintf(stdout, "%s = %s\n", " 5.Bank swap disabled          ", (flag & 0x10) ? ON : OFF);
		cprintf(stdout, "%s = %s\n", " 6.Bank swizzle mode disabled  ", (flag & 0x20) ? ON : OFF);
		cprintf(stdout, "%s = %s\n", " 7.all activated               ", (flag ==0x3F) ? ON : OFF);
		cprintf(stdout, "%s = %s\n", " 8.all deactivated             ", (flag ==0x00) ? ON : OFF);

		cc_set_color(GREY);
		printf(
			" 0. EXIT.\n"
			);

		int x = ps4_get_user_input(8);

        if ((x < 8) && (flag > 0x3F))
		{
			flag = 0;
		}

		switch (x)
		{
		case 1: flag ^= 0x01; break;
		case 2: flag ^= 0x01 << 1; break;
		case 3: flag ^= 0x01 << 2; break;
		case 4: flag ^= 0x01 << 3; break;
		case 5: flag ^= 0x01 << 4; break;
		case 6: flag ^= 0x01 << 5; break;
		case 7: flag = 0x3F; break;
		case 8: flag = 0xFF; break;
		case 0: return;
		default: break;
		}


		buffer[GDDR5_DEBUG_FLAG] = flag;
	}
}

void ps4_patch_corrupted_CoreOS(PS4Nor* obj)
{
	unsigned char* buffer = obj->buffer;

#define COREOS_START 0x003C0000
#define COREOS_END   0x01D40000 

	srand((unsigned int)time(NULL));

	cprintf(stdout, CF_YELLOW "Search for corrupted sections..\n" CC_RESET);
	int found = 0;
	int repaired = 0;

	for (int i = COREOS_START; i < COREOS_END; ++i)
	{
		int count = 0;

		while (i+ count < COREOS_END && buffer[i+ count] == 0x00)
		{
			++count;
		}

		if (count <4)
		{
			count = 0;

			while (i+ count < COREOS_END && buffer[i+count] == 0xFF)
			{
				++count;
			}
		}

		if (count > 3)
		{
			++found;
			char c = 0;
			cprintf(stdout, CF_AQUA "%08X->%08X length= %08X is blank.\n" CC_RESET , i, i+count,count);

			do
			{
				cprintf(stdout, CF_RED "\nDo you wish to patch this section? (y/n):"CC_RESET);
				
				do
				{
					scanf("%c", &c);
				} while (c == '\n');
				
			} while (c != 'y' && c != 'Y' && c != 'n' && c != 'N');
			

			if (c == 'y'|| c == 'Y')
			{
				++repaired;

				for (int j = 0; j < count; ++j)
				{
					unsigned char patch;

					do
					{
						patch = (unsigned int)rand() % 0xFF;
					} while (patch == 0x00 || patch == 0xFF);

					buffer[i + j] = patch;
				}
			}
		}

		i += count;
	}

	cprintf( stdout, CF_GREEN "found = %2u repaired = %2u ...DONE!\n" CC_RESET, found, repaired);
	system("pause");
}

void ps4_patch_eap_key(PS4Nor* obj)
{
	unsigned char* buffer = obj->buffer;

	cprintf(stdout, CF_AQUA "\n0x1C9200 EAP KEY 1:\n\n" CC_RESET);

	for (int i = 0; i < EAP_LEN; ++i)
	{
		if (i > 0 && (i % 30) == 0)
		{
			printf("\n");
		}

		printf("%02X ", buffer[EAP_KEY_1 + i]);
	}

	cprintf(stdout, CF_AQUA "\n\n0x1CC200 EAP KEY 2:\n\n" CC_RESET);

	for (int i = 0; i < EAP_LEN; ++i)
	{
		if (i > 0 && (i % 30) == 0)
		{
			printf("\n");
		}

		printf("%02X ", buffer[EAP_KEY_2 + i]);

	}

	ps4_print_menu_list(
		"\n\n"
		"OPTIONS:\n"
		"========\n",
		" 1. Copy key 1 over key 2.\n"
		" 2. Copy key 2 over key 1.\n"
		" 3. Swap keys.\n"
	);

	int c = ps4_get_user_input(3);

	if (c == 0)
	{
		return;
	}
	else if (c == 1)
	{
		memcpy(buffer + EAP_KEY_2, buffer + EAP_KEY_1, EAP_LEN);
	}
	else if (c == 2)
	{
		memcpy(buffer + EAP_KEY_1, buffer + EAP_KEY_2, EAP_LEN);
	}
	else if (c == 3)
	{
		for (int i = 0; i < EAP_LEN; ++i)
		{
			unsigned char c = buffer[EAP_KEY_1 + i];
			buffer[EAP_KEY_1 + i] = buffer[EAP_KEY_2 + i];
			buffer[EAP_KEY_2 + i] = c;
		}
	}

	cprintf(stdout, CF_GREEN "\n............DONE!\n" CC_RESET);
	system("pause");
}
