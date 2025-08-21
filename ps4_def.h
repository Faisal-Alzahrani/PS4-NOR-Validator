#ifndef PS4_DEF_H
#define PS4_DEF_H

#if defined(__GNUC__) || defined(__clang__)
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#include <dirent.h>
#endif

#ifdef _MSC_VER
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#include "dirent.h"
#endif

#if defined(_WIN64)|| defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#define ENABLE_COLOR_HACK
#include <Windows.h>

#endif

#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdarg.h>

#include "cprintf.h"
#include "md5.h"

//most data here are taken from https://www.psdevwiki.com/ps4/Flash-Main

//#define DEBUG

// 32 MB is the size of a PS4 flash
#define NOR_SIZE 33554432
#define HEADER 0x00000000
#define MAC_ADDRESS 0x1C4021
#define MAC_LEN 6
#define MOTHERBOARD_SERIAL 0x1C8000
#define MOTHERBOARD_SERIAL_LEN 14
#define CONSOL_SERIAL 0x1C8030
#define CONSOL_SERIAL_LEN 0xA
#define SKU_VERSION 0x1C8040
#define SKU_VERSION_LEN 0xE

//useless patch..? with overlapping offsets??
//patche1: 1CA000-1CAFFF <-> 1CD000-1CDFFF
// token & flags backup
//patche2: 1C9000-1CA610 <-> 1CC000-1CD610
#define PATCH_1CA000_1CAFFF_12 0x1CAFFF
#define PATCH_1CA000_1CAFFF_11 0x1CA000
#define PATCH_1CA000_1CAFFF_12 0x1CAFFF
//patche: 2

#define EAP_KEY_1 0x1C9200
#define EAP_KEY_2 0x1CC200
#define EAP_LEN 0x60
//Development Mode= 0xFE Assist Mode= 0xFB Release Mode= 0xFF (default)
#define BOOT_MODE     0x1C9000
//Large Mode=FE Normal Mode= 0xFF (default)
#define MEMORY_BUDGET 0x1C9003
////0x01 to enable 0xFF to disable (default)
#define SLOW_HDD_MODE 0x1C9005
//vsh_4K Mode (0xFE ON) (0xFF OFF)
#define SHOW_MODE   0x1C9010  
//0x01 to enable 0xFF to disable (default)
#define IDU_MODE      0x1CA600
//enable= 0x10 disable =0x00 (default)
#define UPDATE_MODE   0x1CA601
//enable= 0x01 disable =0x00 (default)
#define REGISTRY_RECOVERY_MODE 0x1CA603
//0x01 to enable 0xFF to disable (default)
#define ARCADE_MODE   0x1CA609
//memory test 0x01 enable, 0xFF to disable (default)
#define MEMTEST 0x1C9310
//memory range test 0x01 enable, 0xFF to disable (default)
#define RNGTEST 0x1C9312
//GDDR5 whatever value is here is multiplied by 0x19 to get final value
//max value = 0xED *0x19= 5925Mhz
//500Mhz will semi-brick the console with DCT errors
#define GDDR5_CLOCK 0x1C9320
//likely gddr5DebugFlag:
//0x1 = Read DBI disabled 
//0x2 = Write DBI disabled 
//0x4 = ABI disabled
//0x8 = Force auto precharge enabled 
//0x10= Bank swap disabled
//0x20= Bank swizzle mode disabled
//0x3F= Everything set
//0xFF= Everything disabled
#define GDDR5_DEBUG_FLAG 0x1C9329
//UART boot param  (0x01 ON) (0xFF OFF)
#define EXTRA_UART_ON_BOOT  0x1C931F  
#define UART_ON_BOOT 0x1CC31F
//manufacturing mode (all zeroes for enabled, all FFs for disabled)
#define MANUFACTURING_MODE 0x1CBC00
#define MANUFACTURING_MODE_LEN 0x20 
//Circle Button Behaviour: 0x00 = Circle Accept, 0x01 = Circle Go Back (default)
#define SWAP_O_WITH_X 0x1CA040
//??
#define SOCUID 0x1C8010
// P/N and S/N
#define HDD 0x1C9C00
#define HDD_LEN 64
#define HDD_TYPE 0x1C9C3C
#define HDD_TYPE_LEN 4
#define FW_VERSION  0x1CA604 //then this for firmware 1.6X and older..
#define FW_VERSION2 0x1C9068 //check this first..
#define FW_VERSION_LEN 4
#define FW_COUNTER 0x1CA5D8
#define FW_PATCH_COUNTER 0x1CA5D9
#define FW_COUNTER_LEN 1

//len=1
#define SAMUBOOT 0x1C9323

//len=2
#define FW_SLOT1 0x1C906A
#define FW_SLOT2 0x1CC06A

//len=16
#define CORE_SWCH 0x201000
#define SAMU_SL1  0x204000
#define SAMU_SL2  0x242000

//len=64
#define SYS_FLAGS 0x1C9310


#define WIFIBT_VERSION 0x014421E
#define WIFIBT_LEN 0x11

#ifdef UNUSED
static char* CODENAMES[] =
{
	"Orbis",
	"Liverpool",
	"Colwick",
	"Newcastle",
	"Aeolia",
	"Belize",
	"Baikal",
	"Cavern",
	"Torus",
	"Trooper",
	"Luke",
	"Jedi",
	"Morpheus",
	"StarSha",
	"Jaguar",
	"Kratos",
	"Syrup",
	"Onion",
	"Onion+",
	"Garlic" 
};

static char* CODENAMES_TRANS[] =
{
	"PlayStation 4",
	"PS4 GPU Part of APU",
	"PS4 System Controller",
	"PS4 Southbridge (most likely)",
	"PS4 Southbridge (first revision)",
	"PS4 Southbridge (second and third revision)",
	"PS4 Southbridge (fourth revision)",
	"PS4 DevKit motherboard",
	"PS4 Wireless communication module first revision",
	"PS4 Wireless communication module second revision",
	"PS4 Camera",
	"PS4 DualShock 4 controller",
	"PlayStation Virtual Reality",
	"uknown",
	"PS4 CPU Part of APU",
	"PS4 Arcade",
	"PS4 Prototype motherboard",
	"PS4 bus connecting the GDDR5 DRAM controller to the northbridge and the northbridge to the GPU L2 cache and GPU blocks",
	"PS4 bus connecting the GDDR5 DRAM controller to the northbridge and the northbridge to the GPU L2 cache and GPU blocks",
	"PS4 bus connecting the GDDR5 DRAM controller to the GPU L2, Color Buffer (CB) and Depth Buffer (DB) caches" };

static int SEG_OFFSETS[] =
{
	0x00000000,
	0x00001000,
	0x00002000,
	0x00003000,
	0x00004000,
	0x00064000,
	0x000C4000,
	0x00144000,
	0x001C4000,
	0x001D0000,
	0x00200000,
	0x00201000,
	0x00202000,
	0x00203000,
	0x00204000,
	0x00242000,
	0x00280000,
	0x00300000,
	0x00380000,
	0x003C0000,
	0x01080000,
	0x01D40000
};

static int SEG_LEN[] =
{
	0x1000,
	0x1000,
	0x1000,
	0x1000,
	0x60000,
	0x60000,
	0x80000,
	0x80000,
	0xC000,
	0x30000,
	0x1000,
	0x1000,
	0x1000,
	0x1000,
	0x3E000,
	0x3E000,
	0x80000,
	0x80000,
	0x40000,
	0xCC0000,
	0xCC0000,
	0x2C0000
};

static char* SEG_DES[] =
{
	"Segment 0: Header",                          //0x00000000
	"Segment 0: Active Slot",                     //0x00001000
	"Segment 0: MBR1 (for sflash0s1.cryptx32)",   //0x00002000
	"Segment 0: MBR2 (for sflash0s1.cryptx32b)",  //0x00003000
	"sflash0s0x32: emc_ipl",                      //0x00004000
	"sflash0s0x32b: emc_ipl",                     //0x00064000
	"sflash0s0x33: eap_kbl",                      //0x000C4000
	"sflash0s0x38: torus2_fw",                    //0x00144000
	"sflash0s0x34: nvs",//Non Volatile Storage    //0x001C4000
	"sflash0s0x0: blank",                         //0x001D0000
	"Segment 1: Header (XTS encrypted)",          //0x00200000
	"Segment 1: Active Slot (XTS encrypted)",     //0x00201000
	"Segment 1: MBR1 (for sflash0s1.cryptx2) (XTS encrypted)", //0x00202000
	"Segment 1: MBR2 (for sflash0s1.cryptx2b) (XTS encrypted)",//0x00203000
	"sflash0s1.cryptx2: sam_ipl",                       //0x00204000
	"sflash0s1.cryptx2b: sam_ipl",                      //0x00242000
	"sflash0s1.cryptx1: idata",                         //0x00280000
	"sflash0s1.cryptx39: bd_hrl",                       //0x00300000
	"sflash0s1.cryptx6: Virtual TRM",                   //0x00380000
	"sflash0s1.cryptx3: secure kernel&secure modules",  //0x003C0000
	"sflash0s1.cryptx3b: secure kernel&secure modules", //0x01080000
	"sflash0s1.cryptx40: blank"                         //0x01D40000
};
#endif
#endif //PS4_DEF_H
