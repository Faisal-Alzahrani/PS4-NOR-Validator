#include "ps4_validator.h"
#include "ps4_md5.h"
#include "ps4_def.h"
#include "ps4_bintools.h"
#include <stdio.h>

const char* C0020001_md5[] =
{
"42ABB3B655F6085F029A408FE7E94831",
"92D4149F8C165ABAF88A3EC93C491980",
"86AEDE5276E8948B2CA6EED320E72266",
"571A67C2D0F64AB8CF1A65C201F0C60D",
"C5DCA09C92A2F0362D00BDE4EDB7548B",
"13C18569CA45E3732FB17D9C14160081",
"D51C9935B5409313041177FB0393550B",
"D7914CEC3A7246A7496990AFECE89E6C",
"98C81530AFE8733105F7E44007141CA7",
"7E9425D70C3211B7AECB49A3016EA5A5",
"A601D993986C83D9DB38C52A212D1C8B",
"E77157F9C23DC6CBD8DEBFC24E32DD6D",
"7AA816B366FCE4ADBEC2B07B53E1482F",
"9EFC56DAF6C27AB00922BAA38D49F8AB",
"5F33857A42763B4529184B277D39CF4B",
"602EC0863C9685657F1130B13F7BEE83",
"B658224645F34392019D21F1DEE74889",
"869188408528D852D7564BC17A1BB41E",
"9976779772ADFEDDBF74B5A4F3047854",
"904B5CEDEC777856BAC9814EAA812409",
"D6464623C989D9C253A42633E6EE5570",
"C6D2A1E37B00294F046766967E494546",
"D32D83437BE52724079253AA47F7B597",
"3CBA75B7C1C2160EF4CCA725DC852D2B",
"3BEC446D0DF351CB389A88D29B304B4F",
"AB64A42582EC8DF5856281ACF2D5E4AF",
"AE56092BB5BD407E7382DE881B62DBF4",
"726933C6AAA436DFCE8E4CC87DDD39FC",
"471B4C1550972E04C108A259D53F8F8C",
"561E20F02710609AEBD3BF145C38951C",
"AE2C159938934CC195F058276C1C0C39",
"4AF2BC22C357607ADA80C2F79BCF3CC5",
"B1F075FAAB40C62F483AE779C56E5228",
"B42867B30217CB255FF855DCEB92FE92"
};

const unsigned int C0020001_size[] =
{
	0x6E9A4,
0x6E89C,
0x6E878,
0x6E2F0,
0x6E17C,
0x6E0EC,
0x6DDA8,
0x6EB38,
0x6F55C,
0x6F5C4,
0x6981E,
0x697A1,
0x69645,
0x6A2B7,
0x69B85,
0x27C00,
0x695FE,
0x6A01B,
0x6A1FD,
0x6A064,
0x69796,
0x6A5FC,
0x6979D,
0x6FCDC,
0x279CE,
0x279BE,
0x27ABE,
0x25EFE,
0x27F0E,
0x214BE,
0x261AE,
0x6A627,
0x6FD00,
0x27B6E
};

const float C0020001_fw[][12] =
{
	{4.70,4.72,4.73,4.74,5.01,5.05,5.50,5.53,5.55,7.02,8.03,0},
	{4.00,4.01,4.05,4.07,0},
	{1.61,1.62,0},
	{1.62,7.51,0},
	{1.61,1.62,0},
	{1.61,1.62,0},
	{1.06,1.50,1.51,0},
	{6.00,6.02,6.20,0},
	{1.61,4.50,5.01,5.56,6.71,6.72,7.00,7.02,7.50,7.51,7.55,0},
	{4.71,7.55,8.00,8.01,8.03,8.50,8.52,0},
	{5.00,5.03,5.53,5.55,0},
	{4.06,4.70,4.73,0},
	{4.01,4.07,0},
	{0},
	{6.00,6.02,6.20,7.00,0},
	{0},
	{6.70,8.03,8.50,0},
	{6.50,6.51,6.70,6.71,6.72,7.00,7.02,7.50,7.51,7.55,0},
	{0},
	{6.72,8.00,8.01,8.03,8.50,8.52,0},
	{4.5,0},
	{8.00,9.00,0},
	{4.55,0},
	{9.03,0},
	{7.00,7.02,7.50,7.51,7.55,0},
	{6.50,6.70,6.71,6.72,0},
	{8.52,9.00,0},
	{6.00,6.20,7.21,0},
	{8.03,8.50,8.52,0},
	{5.03,0},
	{5.55,0},
	{9.5,0},
	{9.5,0},
	{9.5,0}
};

#define WIFI_OFFSET 0x144200
#define WIFI_LEN_OFFSET 0x144024
#define WIFI_LEN_BUFFER_SIZE 0x6B000
char buffer[WIFI_LEN_BUFFER_SIZE];

void ps4_check_wifi(PS4Nor* obj)
{
	unsigned char* C0020001 = obj->buffer + WIFI_OFFSET;
	uint32_t size = *(uint32_t*)(obj->buffer + WIFI_LEN_OFFSET);
	int idx = -1;
	
	cprintf(stdout, CF_YELLOW "checking WIFI/Bluetooth module...\n" CC_RESET);

	if (ps4_md5_get_wifi_fw_list(obj))
	{
		cprintf(stdout,CF_GREEN "WIFI/Bluetooth module is ok.\n" CC_RESET);
		system("pause");
		return;
	}
	else
	{
		for (int i = 0; i < ARRAY_SIZE(C0020001_size); ++i)
		{
			if (size == C0020001_size[i])
			{
				idx = i;
				unsigned char md5[33];
				ps4_md5(md5, C0020001, size);

				if (0 == strcmp(md5, C0020001_md5[i]))
				{
					cprintf(stdout, CF_GREEN "WIFI/Bluetooth module is ok.\n" CC_RESET);
					system("pause");
					return;
				}
				else
				{
					break;
				}
			}
		}
	}

	cprintf(stdout, CF_YELLOW "Unable to confirm WIFI/Bluetooth state. The module is likely corrupted.\n" CC_RESET);

	if (idx == -1)
	{
		cprintf(stdout, CF_RED "Unable to find suitable patch. Aborting repair.\n" CC_RESET);
		system("pause");
		return;
	}

	
	float v1 = obj->version;
	float * ar = C0020001_fw[idx];
	float v2 = 0;

	for (int i = 0;  ar[i] > 0; ++i)
	{
		float diff;

		do
		{
			v2 = ar[i];
			diff =  fabs(v1-v2);
		} while(diff < 0.1);
	}


	cprintf(stdout,
		CF_GREEN "current WIFI firmware     ="CF_BLUE " %5.2f\n"
		CF_GREEN "replacement WIFI firmware ="CF_BLUE " %5.2f\n" CC_RESET, v1, v2);

	char c;

	do 
	{
		cprintf(stdout, CF_RED "\nDo you wish to apply this patch? (y/n):" CC_RESET);

		do
		{
			scanf("%c", &c);
		} while (c == '\n');
		
	} while (c != 'y' && c != 'Y' && c != 'n' && c != 'N');

	if (c == 'y' || c=='Y')
	{
		char filename[FILENAME_MAX];
		sprintf(filename, "C0020001\\C0020001_%02u.bin", idx + 1);

		FILE* f = fopen(filename, "rb+");

		if (!f)
		{
			cprintf(stdout, CF_YELLOW "Unable to open patch file.\n" CC_RESET);
			system("pause");
			return;
		}

		fread(buffer, 1, size, f);
		fclose(f);
		memcpy(C0020001, buffer, size);
		cprintf(stdout, CF_GREEN "........DONE!\n" CC_RESET);
		system("pause");
	}
	else
	{
		cprintf(stdout, CF_GREEN "........ABORTING!\n" CC_RESET);
		system("pause");
	}
}