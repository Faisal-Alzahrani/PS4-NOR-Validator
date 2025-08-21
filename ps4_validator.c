
#include "ps4_headers.h"
#include "ps4_bintools.h"
#include "ps4_md5.h"
#include "ps4_validator.h"



const char  WIFIBT_TORUS_2[]= { 0x74, 0x6F, 0x72, 0x75, 0x73, 0x32, 0x5F, 0x66, 0x77, 0x2E, 0x62, 0x69, 0x6E };
const char  WIFIBT_TORUS_1[]={ 0x9F, 0xE5, 0x1C, 0xF0, 0x9F, 0xE5, 0x1C, 0xF0, 0x9F, 0xE5, 0x1C, 0xF0, 0x9F };

//TABLE
//{offset, len, type, n, firmware version,note}
//type:
//0= unencrypted
//1= static zeros(0x00)
//2= static blank (0xFF)
//3= static value, value = n
//4= static binary,  idx = n
//5= static per-console
//6= static per-firmware
//7= encrypted
//8= encrypted per-console
//9= encrypted per-firmware

#define TB_RECORD_SIZE 6
#define TB_OFFSET 0
#define TB_LEN 1
#define TB_TYPE 2
#define TB_N 3
#define TB_VERSION 4
#define TB_NOTE 5

#define TB_DYNAMIC 0
#define TB_STATIC_00 1
#define TB_STATIC_FF 2
#define TB_STATIC_BYTE 3
#define TB_STATIC_STR 4
#define TB_PER_CONSOLE 5
#define TB_PER_FIRMWARE 6
#define TB_ENCRYPED 7
#define TB_PER_CONSOLE_ENCRYPED 8
#define TB_PER_FIRMWARE_ENCRYPED 9

const unsigned int tb_sections[][TB_RECORD_SIZE] =
{
	//0x1C4000->0x1CA000 table
	{0x001C4000,0x6   ,0,0,0,0},
	{0x001C4006,0x2   ,0,4,0,1},//hard drive flag?
	{0x001C4008,0x18  ,2,0,0,0},//0xFF
	{0x001C4020,0x1   ,3,1,0,0},//01
	{0x001C4021,0x6   ,0,0,0,2},//MAC address
	{0x001C4027,0x27  ,2,0,0,0},//0xFF
	{0x001C404E,0x3   ,0,0,0,0},
	{0x001C4051,0xA   ,0,0,1,0},//ident
	{0x001C405B,0x5   ,2,0,0,0},//0xFF
	{0x001C4060,0x5   ,0,0,1,0},//ident
	{0x001C4065,0xB   ,2,0,0,0},//0xFF
	{0x001C4070,0x06  ,0,0,1,0},//ident
	{0x001C4076,0x1   ,3,1,0,0},//01
	{0x001C4077,0x3   ,2,0,0,0},//0xFF
	{0x001C407A,0x5   ,1,0,0,0},//0x00
	{0x001C407F,0x2   ,0,0,1,0},//ident
	{0x001C4081,0x1   ,3,0xFF,0,0},//0xFF
	{0x001C4082,0x3   ,0,0,1,0},//ident
	{0x001C4085,0xA   ,2,0,0,0},//0xFF
	{0x001C408F,0x4   ,0,0,0,0},//FAT
	{0x001C4093,0x6   ,0,0,1,0},//ident
	{0x001C4099,0x1   ,3,0xFF,0,0},//0xFF
	{0x001C409A,0x2   ,0,0,1,0},//ident
	{0x001C409C,0x4   ,2,0,0,0},//0xFF
	{0x001C40A0,0xC   ,0,0,0,0},//0xFF
	{0x001C40AC,0x4   ,0,0,1,0},//ident
	{0x001C40B0,0x74E ,2,0,0,0},//0xFF?? 
	{0x001C47FE,0X2   ,0,0,10,3},//hard drive flag 2 BECC 
	{0x001C4800,0x1   ,3,0xFF,0,0},
	{0x001C4801,0x1   ,3,0,0,0},
	{0x001C4802,0xE   ,2,0,0,0},//0xFF??
	{0x001C4810,0x12  ,0,0,0,0},
	{0x001C4822,0xC   ,0,0,1,0},
	{0x001C482E,0x1E  ,2,0,0,0},//0xFF??
	{0x001C484C,0x2   ,0,0,0,0},
	{0x001C484E,0x6   ,2,0,0,0},//0xFF??
	{0x001C4854,0x2   ,0,0,0,0},
	{0x001C4856,0x1A  ,2,0,0,0},//0xFF??
	{0x001C4870,0xC   ,0,0,0,0},
	{0x001C487C,0x24  ,2,0,0,0},//0xFF??
	{0x001C48A0,0xA   ,4,0,0,0},
	{0x001C48AA,0x8   ,0,0,1,0},
	{0x001C48B2,0xA   ,4,1,0,0},
	{0x001C48BC,0x742 ,2,0,0,0},//0xFF??
	{0x001C4FFE,0x14  ,0,0,1,0},
	{0x001C5012,0x6   ,2,0,0,0},//0xFF??
	{0x001C5018,0xA   ,1,0,0,0},//0x00??
	{0x001C5022,0xE   ,2,0,0,0},//0x00??
	{0x001C5030,0x34  ,5,0,0,0},
	{0x001C5064,0x19C ,2,0,0,0},
	{0x001C5200,0x18  ,0,0,0,0},
	{0x001C5218,0x8   ,2,0,0,0},
	{0x001C5220,0x1C  ,0,0,0,0},
	{0x001C523C,0x4   ,2,0,0,0},
	{0x001C5240,0x1C  ,0,0,0,0},
	{0x001C525C,0x4   ,2,0,0,0},
	{0x001C5260,0x1C  ,0,0,0,0},
	{0x001C527C,0x4   ,2,0,0,0},
	{0x001C5280,0x18  ,0,0,0,0},
	{0x001C5298,0x8   ,2,0,0,0},
	{0x001C52A0,0x18  ,0,0,0,0},
	{0x001C52B8,0x8   ,2,0,0,0},
	{0x001C52C0,0x18  ,0,0,0,0},
	{0x001C52D8,0x8   ,2,0,0,0},
	{0x001C52E0,0x18  ,0,0,0,0},
	{0x001C52F8,0x8   ,2,0,0,0},
	{0x001C5300,0x18  ,0,0,0,0},
	{0x001C5318,0x8   ,2,0,0,0},
	{0x001C5320,0x18  ,0,0,0,0},
	{0x001C5338,0x8   ,2,0,0,0},
	{0x001C5340,0x18  ,0,0,0,0},
	{0x001C5358,0x8   ,2,0,0,0},
	{0x001C5360,0x18  ,0,0,0,0},
	{0x001C5378,0x8   ,2,0,0,0},
	{0x001C5380,0x18  ,0,0,0,0},
	{0x001C5398,0x8   ,2,0,0,0},
	{0x001C53A0,0x18  ,0,0,0,0},
	{0x001C53B8,0x8   ,2,0,0,0},
	{0x001C53C0,0x18  ,0,0,0,0},
	{0x001C53D8,0x8   ,2,0,0,0},
	{0x001C53E0,0x18  ,0,0,0,0},
	{0x001C53F8,0x8   ,2,0,0,0},
	{0x001C5400,0x18  ,0,0,0,0},
	{0x001C5418,0x8   ,2,0,0,0},
	{0x001C5420,0x18  ,0,0,0,0},
	{0x001C5438,0x8   ,2,0,0,0},
	{0x001C5440,0x18  ,0,0,0,0},
	{0x001C5458,0x8   ,2,0,0,0},
	{0x001C5460,0x18  ,0,0,0,0},
	{0x001C5478,0x8   ,2,0,0,0},
	{0x001C5480,0x18  ,0,0,0,0},
	{0x001C5498,0x8   ,2,0,0,0},
	{0x001C54A0,0x18  ,0,0,0,0},
	{0x001C54B8,0x8   ,2,0,0,0},
	{0x001C54C0,0x18  ,0,0,0,0},
	{0x001C54D8,0x8   ,2,0,0,0},
	{0x001C54E0,0x18  ,0,0,0,0},
	{0x001C54F8,0x8   ,2,0,0,0},
	{0x001C5500,0x18  ,0,0,0,0},
	{0x001C5518,0x8   ,2,0,0,0},
	{0x001C5520,0x18  ,0,0,0,0},
	{0x001C5538,0x8   ,2,0,0,0},
	{0x001C5540,0x18  ,0,0,0,0},
	{0x001C5558,0x8   ,2,0,0,0},
	{0x001C5560,0x18  ,0,0,0,0},
	{0x001C5578,0x8   ,2,0,0,0},
	{0x001C5580,0x18  ,0,0,0,0},
	{0x001C5598,0x8   ,2,0,0,0},
	{0x001C55A0,0x18  ,0,0,0,0},
	{0x001C55B8,0x8   ,2,0,0,0},
	{0x001C55C0,0x18  ,0,0,0,0},
	{0x001C55D8,0x8   ,2,0,0,0},
	{0x001C55E0,0x18  ,0,0,0,0},
	{0x001C55F8,0x3   ,2,0,0,0},//0xFF??
	{0x001C55FB,0xA05 ,2,0,0,0},//0xFF??
	{0x001C6000,0x8   ,0,0,0,0},
	{0x001C6008,0x8   ,0,0,0,0},
	{0x001C6010,0xFF0 ,2,0,0,0},
	{0x001C7000,0x40  ,0,0,0,0},
	{0x001C7040,0x10  ,0,0,0,0},//trsw_attach
	{0x001C7050,0x50  ,2,0,0,0},
	{0x001C70A0,0x2   ,0,0,0,0},//get_icc_max
	{0x001C70A2,0xF5E ,2,0,0,0},
	{0x001C8000,0xE   ,0,0,0,4},//Motherboard S/N
	{0x001C800E,0x2   ,2,0,0,0},//0xFF
	{0x001C8010,0x10  ,5,0,0,0},//SOCUID
	{0x001C8020,0xE   ,5,0,0,0},//SOCUID 2??
	{0x001C802E,0x2   ,0,0,1,0},
	{0x001C8030,0xA   ,0,0,0,5},//Console S/N
	{0x001C803A,0x2   ,2,0,0,0},
	{0x001C803C,0x5   ,2,0,0,0},//0xFF
	{0x001C8041,0xD   ,0,0,0,9},//SKU Model
	{0x001C804E,0x12  ,2,0,0,0},
	{0x001C8060,0x38  ,0,0,0,0},
	{0x001C8098,0x20  ,5,0,0,0},
	{0x001C80B8,0x8   ,2,0,0,0},
	{0x001C80C0,0xD   ,0,0,0,0},
	{0x001C80CD,0x33  ,2,0,0,0},
	{0x001C8100,0x20  ,1,0,0,0},
	{0x001C8120,0x6B0 ,2,0,0,0},
	{0x001C87D0,0x10  ,0,0,0,0},
	{0x001C87E0,0x10  ,2,0,0,0},//0xFF
	{0x001C87F0,0x10  ,0,0,0,0},
	{0x001C8800,0x800 ,2,0,0,0},
	{0x001C9000,0x1   ,3,0xFF,0,0},//boot mode flags 1C9000->1C9020
	{0x001C9001,0x2   ,2,0,0,0},
	{0x001C9003,0x1   ,3,0xFF,0,0},//memory budget
	{0x001C9004,0x1   ,2,0,0,0},
	{0x001C9005,0x1   ,3,0xFF,0,0},//slow hdd mode
	{0x001C9006,0x1A  ,2,0,0,0},//0xFF??
	{0x001C9020,0x1   ,0,0,0,0},//init_safe_mode flag (e.g F1)
	{0x001C9021,0x1   ,0,0,0,0},//sysctl_machdep_cavern_dvt1_init_update
	{0x001C9022,0x1   ,1,0,0,0},//0x00??
	{0x001C9023,0x45  ,2,0,0,0},//0xFF??
	{0x001C9068,0x4   ,0,0,0,6},//firmware version
	{0x001C906C,0x14  ,2,0,0,0},
	{0x001C9080,0x6C  ,0,0,0,0},////acf token <- checked by sceSblDevActVerifyCheckExpire
	{0x001C90EC,0x14  ,2,0,0,0},
	{0x001C9100,0x100 ,0,0,0,0},//sce_cam_error_put
	{0x001C9200,0x60  ,0,0,0,18},//scrambled/obfuscated eap hdd key 1
	{0x001C9260,0xBF  ,2,0,0,0},//0xFF
	{0x001C931F,0x1   ,0,0,0,7},//uart 1 0xFF
	{0x001C9320,0x1   ,3,0xFF,0,0},//gddr5 clock speed
	{0x001C9321,0x1   ,3,0xFF,0,0},
	{0x001C9322,0x1   ,3,0xFF,0,0},//lvp_configure_tccds
	{0x001C9323,0x1   ,3,0xFF,0,0},//sam_boot_flags (anything other than FF for enabled)
	{0x001C9324,0x5   ,2,0,0,0},//0xFF??
	{0x001C9329,0x1   ,3,0xFF,0,0},//likely gddr5DebugFlag
	{0x001C932A,0xD6  ,2,0,0,0},//0xFF??
	{0x001C9400,0x210 ,0,0,0,0},//tokens?
	{0x001C9610,0x40  ,2,0,0,0},//0xFF??
	{0x001C9650,0x290 ,0,0,0,0},//qafutkn_ioctl?
	{0x001C98E0,0x20  ,2,0,0,0},//0xFF??
	{0x001C9900,0x100 ,2,0,0,0},//acf RSA signature
	{0x001C9A00,0x190 ,0,0,0,0},//tokens?
	{0x001C9B90,0x70  ,0,0,0,0},//0xFF??
	{0x001C9C00,0x3C  ,0,0,0,8},//HDD Info (Byte Reversed)
	{0x001C9C3C,0x4   ,0,0,0,0},//??
	{0x001C9C40,0x130 ,0,0,0,0},//setPupExpirationStatus
	{0x001C9D70,0x2   ,0,0,0,0},
	{0x001C9D72,0xE   ,2,0,0,0},//0xFF??
	{0x001C9D80,0x280 ,2,0,0,0},//0xFF
	//1CA000->0x1D0000
	{0x001CA000,0x9   ,0,0,0,0},
	{0x001CA009,0xC   ,0,0,0,0},
	{0x001CA015,0x2B  ,0,0,0,0},
	{0x001CA040,0x1   ,3,1,0,0},//swab O with X
	{0x001CA041,0x7   ,0,0,0,0},
	{0x001CA048,0x24  ,0,0,0,0},
	{0x001CA06C,0x9   ,0,0,0,0},
	{0x001CA075,0x28  ,0,0,0,0},
	{0x001CA09D,0x43  ,0,0,0,0},
	{0x001CA0E0,0xF   ,0,0,0,0},
	{0x001CA0EF,0x23  ,0,0,0,0},
	{0x001CA112,0x1E  ,0,0,0,0},
	{0x001CA130,0x1E  ,0,0,0,0},
	{0x001CA14E,0x1E  ,0,0,0,0},
	{0x001CA16C,0x29  ,0,0,0,0},
	{0x001CA195,0x15  ,0,0,0,0},
	{0x001CA1AA,0x426 ,1,0,0,0},
	{0x001CA5D0,0x1   ,0,0,0,11},//region byte
	{0x001CA5D1,0x1   ,0,0,0,12},//SKU byte
	{0x001CA5D2,0x6   ,0,0,0,0},//SKU Related
	{0x001CA5D8,0x4   ,0,0,0,0},
	{0x001CA5DC,0x24  ,1,0,0,0},
	{0x001CA600,0x1   ,3,0xFF,0,13},//IDU Mode Flag
	{0x001CA601,0x1   ,3,0x00,0,14},//Update Mode Flag
	{0x001CA602,0x1   ,3,0xFF,0,15},//Show Mode Flag (TestKit)
	{0x001CA603,0x1   ,3,0x00,0,16},//Registry Recover Mode Flag
	{0x001CA604,0x5   ,0,0,0,4},//Software Version (Pre 1.6)
	{0x001CA609,0x1   ,3,0xFF,0,17},//Arcade Mode Flag
	{0x001CA60A,0x2   ,2,0,0,0},//0xFF??
	{0x001CA60C,0x4   ,1,0,0,0},//0x00??
	{0x001CA610,0x15F0,2,0,0,0},
	{0x001CBC00,0x20  ,5,0,0,0},//per-console
	{0x001CBC20,0x20  ,2,0,0,0},//0xFF
	{0x001CBC40,0x20  ,5,0,0,0},
	{0x001CBC60,0x10  ,2,0,0,0},//0xFF??
	{0x001CBC70,0x50  ,2,0,0,0},//0xFF??
	{0x001CBCC0,0x20  ,0,0,0,0},//srtc_modevent
	{0x001CBCE0,0x520 ,2,0,0,0},//0xFF??
	{0x001CC200,0x60  ,0,0,0,19},//EAP Key 2
	{0x001CC260,0x1DA0,2,0,0,0},
	{0x001CE000,0x3   ,4,3,0,0},
	{0x001CE003,0x1   ,3,0x07,0,0},//sku byte = retail
	{0x001CE004,0xE   ,0,0,0,0},
	{0x001CE012,0x6   ,2,0,0,0},//0xFF??
	{0x001CE018,0xA   ,1,0,0,0},//0x00??
	{0x001CE022,0xE   ,2,0,0,0},
	{0x001CE030,0x34  ,0,0,0,0},
	{0x001CE064,0x19C ,2,0,0,0},
	{0x001CE200,0x18  ,0,0,0,0},
	{0x001CE218,0x8   ,2,0,0,0},
	{0x001CE220,0x18  ,0,0,0,0},
	{0x001CE238,0x8   ,2,0,0,0},
	{0x001CE240,0x18  ,0,0,0,0},
	{0x001CE258,0x8   ,2,0,0,0},
	{0x001CE260,0x18  ,0,0,0,0},
	{0x001CE278,0x8   ,2,0,0,0},
	{0x001CE280,0x18  ,0,0,0,0},
	{0x001CE298,0x8   ,2,0,0,0},
	{0x001CE2A0,0x18  ,0,0,0,0},
	{0x001CE2B8,0x8   ,2,0,0,0},
	{0x001CE2C0,0x18  ,0,0,0,0},
	{0x001CE2D8,0x8   ,2,0,0,0},
	{0x001CE2E0,0x18  ,0,0,0,0},
	{0x001CE2F8,0x8   ,2,0,0,0},
	{0x001CE300,0x18  ,0,0,0,0},
	{0x001CE318,0x8   ,2,0,0,0},
	{0x001CE320,0x18  ,0,0,0,0},
	{0x001CE338,0x8   ,2,0,0,0},
	{0x001CE340,0x18  ,0,0,0,0},
	{0x001CE358,0x8   ,2,0,0,0},
	{0x001CE360,0x18  ,0,0,0,0},
	{0x001CE378,0x8   ,2,0,0,0},
	{0x001CE380,0x18  ,0,0,0,0},
	{0x001CE398,0x8   ,2,0,0,0},
	{0x001CE3A0,0x18  ,0,0,0,0},
	{0x001CE3B8,0x8   ,2,0,0,0},
	{0x001CE3C0,0x18  ,0,0,0,0},
	{0x001CE3D8,0x8   ,2,0,0,0},
	{0x001CE3E0,0x18  ,0,0,0,0},
	{0x001CE3F8,0x8   ,2,0,0,0},
	{0x001CE400,0x18  ,0,0,0,0},
	{0x001CE418,0x8   ,2,0,0,0},
	{0x001CE420,0x18  ,0,0,0,0},
	{0x001CE438,0x8   ,2,0,0,0},
	{0x001CE440,0x18  ,0,0,0,0},
	{0x001CE458,0x8   ,2,0,0,0},
	{0x001CE460,0x18  ,0,0,0,0},
	{0x001CE478,0x8   ,2,0,0,0},
	{0x001CE480,0x18  ,0,0,0,0},
	{0x001CE498,0x8   ,2,0,0,0},
	{0x001CE4A0,0x18  ,0,0,0,0},
	{0x001CE4B8,0x8   ,2,0,0,0},
	{0x001CE4C0,0x18  ,0,0,0,0},
	{0x001CE4D8,0x8   ,2,0,0,0},
	{0x001CE4E0,0x18  ,0,0,0,0},
	{0x001CE4F8,0x8   ,2,0,0,0},
	{0x001CE500,0x18  ,0,0,0,0},
	{0x001CE518,0x8   ,2,0,0,0},
	{0x001CE520,0x18  ,0,0,0,0},
	{0x001CE538,0x8   ,2,0,0,0},
	{0x001CE540,0x18  ,0,0,0,0},
	{0x001CE558,0x8   ,2,0,0,0},
	{0x001CE560,0x18  ,0,0,0,0},
	{0x001CE578,0x8   ,2,0,0,0},
	{0x001CE580,0x18  ,0,0,0,0},
	{0x001CE598,0x8   ,2,0,0,0},
	{0x001CE5A0,0x18  ,0,0,0,0},
	{0x001CE5B8,0x8   ,2,0,0,0},
	{0x001CE5C0,0x18  ,0,0,0,0},
	{0x001CE5D8,0x8   ,2,0,0,0},
	{0x001CE5E0,0x18  ,0,0,0,0},
	{0x001CE5F8,0xA08 ,2,0,0,0},
	{0x001CF000,0x1   ,0,0,0,0},
	{0x001CF001,0xFFF ,2,0,0,0},
	//0x001D0000->0x00200000 sflash0s0x0: blank"
	{0x001D0000,0x30000,2,0,0,10}
};

#define TB_BIN_SIZE 0x000A

const char tb_binary[][TB_BIN_SIZE] =
{
	{0x00, 0x50, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00},
	{0x06, 0x01},
	{0x00, 0x07, 0xFF},
};

const char* tb_strings[] =
{
	"",//0
	"hard drive flag",//1
	"MAC address",//2
	"hard drive flag 2",//3
	"motherboard S/N",//4
	"console S/N",//5
	"Software Version (Pre 1.6)",//6
	"UART flag",//7
	"hard drive (Byte Reversed)",//8
	"SKU Model",//9
	"sflash0s0x0: blank",//10
	"region byte",//11
	"SKU byte",//12
	"IDU Mode Flag",//13
	"Update Mode Flag",//14
	"Show Mode Flag (TestKit)",//15
	"Registry Recover Mode Flag",//16
	"Arcade Mode Flag",//17
	"EAP Key 1",//18
	"EAP Key 2"//19
};

typedef struct
{
    const unsigned int* entries;
	const unsigned int size;
	const char* binary;
	const char** strings;
} FlashTable;

const FlashTable cid_table = { (unsigned int*)tb_sections,ARRAY_SIZE(tb_sections),(char*)tb_binary,tb_strings };

//these are the good entropy values..
const uint64_t entropy_entries[][2] =
{
	{0x3130303030303043,0},//C0000001
	{0x3130303830303043,1},//C0008001
	{0x3130303031303043,2},//C0010001
	{0x3130303831303043,3},//C0018001
	{0x3130303032303043,4},//C0020001
	{0x3130303832303043,5},//C0028001
};

//minimum values, allow -0.5% ~ -1% error
const double slb2_entropy_values[] =
{
	7.99, //7.999451
	0.23, //0.231872
	7.99, //7.998277
	0.38, //0.387995
	6.60, //6.645929
	0.57  //0.576938
};

//declaration private functions
void ps4_check_system_flags(PS4Nor* obj);
void ps4_print_result(PS4Nor* obj);
int ps4_check_padding(PS4Nor* obj, unsigned int offset, unsigned int len, const unsigned char padding);
int ps4_check_unencrypted_data(PS4Nor* obj, unsigned int offset, unsigned int len);
int ps4_check_encrypted_data(PS4Nor* obj, unsigned int offset, unsigned int len);

void ps4_check_x0000_x2000(PS4Nor* obj);
void ps4_checkMBR(PS4Nor* obj, const unsigned int offset);

void ps4_check_x2000_x3000(PS4Nor* obj);
void ps4_check_x3000_x4000(PS4Nor* obj);

int ps4_check_slb2_file_entropy(const char* filename, double entropy);
void ps4_check_slb2_entry(PS4Nor* obj, int address, const SceSlb2Entry* entry);
void ps4_check_slb2(PS4Nor* obj, const unsigned int offset, const unsigned int end);
void ps4_check_0x4000_0x1C4000(PS4Nor* obj);

void ps4_check_0x1C4000_0x200000(PS4Nor* obj);
void ps4_check_0x200000_0x380000(PS4Nor* obj);
void ps4_check_0x380000_0x3C0000(PS4Nor* obj);
void ps4_check_0x3C0000_0x2000000(PS4Nor* obj);


void ps4_compare_img_diff(PS4Nor* obj, PS4Nor* objDiff);
void ps4_print_statistics(PS4Nor* obj);


//implementation
int ps4_init(PS4Nor* delta, const char* filename)
{
    if (delta)
	{
        memset(delta, 0, sizeof(PS4Nor));

        delta->filename = filename;
        delta->deltaLevel = 3;

        FILE* f = fopen(delta->filename, "rb");

		if (!f)
		{
            ps4_out(delta, RED, -1, "ERROR: could not open file.");
			return 0;
		}

		fseek(f, 0, SEEK_END);
		long size = ftell(f);

		if (size != NOR_SIZE)
		{
            ps4_out(delta, RED, -1, "ERROR: file size is not the size of a PS4 NOR. filename: %s", delta->filename);
			fclose(f);
			return 0;
		}

        delta->isValid = 1;
		fseek(f, 0, SEEK_SET);
        fread(delta->buffer, size, 1, f);
		fclose(f);
	}

	return 1;
}

int ps4_validate(PS4Nor *obj, PS4Nor* delta, const char* outfilename)
{
	if (obj)
	{
		obj->countError = 0;
		obj->countOk = 0;
		obj->countWarning = 0;

		if (outfilename)
		{
			obj->outfile = fopen(outfilename, "wb+");
		}
		else
		{
			memset(obj->name, 0, FILENAME_MAX);
			strcpy(obj->name, obj->filename);
			int len = strlen(obj->name);
			obj->name[len - 3] = '\0';
			sprintf(obj->name, "%stxt", obj->name);
			obj->outfile = fopen(obj->name, "wb+");
		}

		ps4_info(obj);
		ps4_check_system_flags(obj);
		ps4_out(obj, PINK, -1, "STAGE 1: validating structural integrity..");
		ps4_check_x0000_x2000(obj);
		ps4_check_x2000_x3000(obj);
	    ps4_check_x3000_x4000(obj);
		ps4_check_0x4000_0x1C4000(obj);
		ps4_check_0x1C4000_0x200000(obj);
		ps4_check_0x200000_0x380000(obj);
		ps4_check_0x380000_0x3C0000(obj);
		ps4_check_0x3C0000_0x2000000(obj);
		ps4_out(obj, PINK, -1, "STAGE 2: validating MD5 checksum..");
		ps4_check_md5(obj);

        //useless..
        //if(delta)
        if (0)
		{
			ps4_out(obj, PINK, -1, "STAGE 3: validating against delta image..");
			ps4_compare_img_diff(obj,delta);
		}
		ps4_print_statistics(obj);
		ps4_print_result(obj);

		if (obj->outfile)
		{
			fclose(obj->outfile);
			obj->outfile = 0;
		}
		
		return obj->countError == 0;
	}

	return -1;
}
unsigned int system_flags_data[][9] =
{   //offset ,name,count, m1, m2 , m3 ,ms1,ms2,ms3
	{0x1C9000,0  ,     3, 0xFF,0xFB,0xFE, 2 ,3  ,4 },
	{0x1C9003,1  ,     2, 0xFF,0xFE,0   , 0 ,1  ,0 },
	{0x1C9005,2  ,     2, 0xFF,0x01,0   , 0 ,1  ,0 },
	{0x1C9010,3  ,     2, 0xFF,0xFE,0   , 0 ,1  ,0 },
	{0x1CA600,4  ,     2, 0xFF,0x01,0   , 0 ,1  ,0 },
	{0x1CA601,5  ,     2, 0x00,0x10,0   , 0 ,1  ,0 },
	{0x1CA603,6  ,     2, 0x00,0x01,0   , 0 ,1  ,0 },
	{0x1CA609,7  ,     2, 0xFF,0x01,0   , 0 ,1  ,0 },
	{0x1C9310,8  ,     2, 0xFF,0x01,0   , 0 ,1  ,0 },
	{0x1C9312,9  ,     2, 0xFF,0x01,0   , 0 ,1  ,0 },
	{0x1C9329,10 ,     2, 0xFF,0x3F,0   , 0 ,1  ,0 },
	{0x1CA040,11 ,     2, 0x01,0x00,0   , 0 ,1  ,0 },
	{0x1C931F,12 ,     2, 0xFF,0x01,0   , 0 ,1  ,0 },
	{0x1CC31F,13 ,     2, 0xFF,0x01,0   , 0 ,1  ,0 }
};

char* system_flags_names[] =
{
	"Boot Mode",//0
	"Memory Budget Mode",//1
	"Slow HDD",//2
	"Show Mode",//3
	"IDU Mode",//4
	"Update Mode",//5
	"Registry Recovery Mode",//6
	"Arcade Mode",//7
	"Memory Test Mode",//8
	"keystorage Test Mode",//9
	"GDDR5 Debug Flags",//10
	"Swap O with X buttons",//11
	"UART on Boot 1",//12
	"UART on Boot 2"//13
};

char* system_flags_mode_names[] =
{
	"disable",//0
	"enable",//1
	"Release Mode",//2
	"Assist Mode",//3
	"Development Mode",//4
};

void ps4_check_system_flags(PS4Nor *obj)
{
	ps4_out(obj,AQUA, -1, "System flags:");
	//GDDR5 clock speed
	const unsigned char* buffer = obj->buffer;
	unsigned offset = 0x1C9320;
	unsigned char x = buffer[offset];
	int r = (x == 0xFF) || (x > 0x17 && x < 0x5B);
	r = r ? VA_OK : VA_WARNING;

	if (x == 0xFF)
	{
		ps4_out(obj, WHITE, r, "%08X %-23s: default          ", offset, "GDDR5 Clock Speed");
	}
	else
	{
		ps4_out(obj, WHITE, r, "%08X %-23s: %4uMhz          ", offset, "GDDR5 Clock Speed", x * 0x19);
	}

	/*
	//manufacturing mode
	offset = 0x1CBC00;
	int count_00 = 0;
	int count_FF = 0;

	for (int i = 0; i < MANUFACTURING_MODE_LEN; ++i)
	{
		if (buffer[offset + i] == 0xFF)
		{
			++count_FF;
		}
	}

	if (count_FF == MANUFACTURING_MODE_LEN)
	{
		r = R_OK;
		s = "disabled";
	}
	else
	{
		r = R_WARNING;
		s = "uknown stat";
	}

	ps4_out(obj,WHITE, r, "%08X %-23s: %s", offset, "Manufacturing Mode", s);
	*/

	for (unsigned int i = 0; i < ARRAY_SIZE(system_flags_data); ++i)
	{
		offset = system_flags_data[i][0];
		char* flag_name = system_flags_names[system_flags_data[i][1]];
		unsigned int count = system_flags_data[i][2];
		r = VA_ERROR;

		for (unsigned int j = 0; j < count; ++j)
		{
			if (buffer[offset] == system_flags_data[i][3 + j])
			{
				r = j == 0 ? VA_OK : VA_WARNING;
				char* mode_name = system_flags_mode_names[system_flags_data[i][6 + j]];
				ps4_out(obj,WHITE, r, "%08X %-23s: value= %02X %s", offset, flag_name, buffer[offset], mode_name);
				break;
			}
		}

		if (r == VA_ERROR)
		{
			ps4_out(obj,WHITE, r, "%08X %-23s: value= %02X %s", offset, flag_name, buffer[offset], "unknown");
		}
	}
}

static const char* region_str[] =
{
	"Japan",//0
	"US, Canada (North America)",//1
	"Australia / New Zealand (Oceania)",//2
	"UK/Ireland",//3
	"Europe / Middle East / Africa",//4
	"Korea (South Korea)",//5
	"Southeast Asia / Hong Kong",//6
	"Taiwan",//7
	"Russia, Ukraine, India, Central Asia",//8
	"Mainland China",//9
	"",//10
	"Mexico, Central America, South America",//11
	"",//12
	"",//13
	"Mexico, Central America, South America",//14
	"US, Canada (North America)",//15
	"Europe / Middle East / Africa"//16
};

void ps4_info(PS4Nor *obj)
{
	const unsigned char* buffer = obj->buffer;
	unsigned int offset;
	int r = VA_OK;
	static unsigned char str[72];
	unsigned int len;

	ps4_out(obj,AQUA, -1, "Info:");

	//filename
	ps4_out(obj, WHITE, VA_OK, "%-32s: %-17s", "file", obj->filename);

	//sofware version
	if (*(buffer+ 0x1C9068+3)!= 0xFF)
	{
		offset = 0x1C9068;
	}
	else
	{
		offset = 0x1CA604;
	}

	char m = buffer[offset + 3];
	char n = buffer[offset + 2];
	obj->versionMajor = m;
	obj->versionMinor = n;
	obj->version = (m >> 4) * 10 + (m & 0x0F) + (((n >> 4) * 10 + (n & 0x0F)) / 100.0);
	r = m > 0 || n < 20;
	ps4_out(obj,WHITE, r, "%08X %-23s: %2X.%02X            ", offset, "firmware version", m, n);

	//region
	offset = 0x001C8047;
	obj->region = buffer[offset];
	obj->regionStr = "unlisted";
	r = -1;

	obj->region =  (buffer[0x001C8047]-'0') * 10 + buffer[0x001C8048] - '0';

	if (obj->region < sizeof(region_str))
	{
		r = VA_OK;
		obj->regionStr = region_str[obj->region];
	}
	else
	{
		r = VA_ERROR;
		obj->regionStr = "";
	}

	ps4_out(obj,WHITE, r, "%08X %-23s: %-17s",offset, "Region", obj->regionStr);

	//Console Type
	offset = 0x001CA5D1;
	char* s = "";

	static const unsigned char retail_bytes[] = { 0x76, 0xB3, 0x80, 0x02 };
	static const unsigned char dev_bytes[] = { 0x76, 0xB3, 0x80, 0x02 };
	r = VA_OK;

	if (0 == memcmp(buffer+offset, retail_bytes, 4))
	{
		obj->isRetail = 1;
		s = "Retail";
	}
	else if (0 == memcmp(buffer+offset, dev_bytes, 4))
	{
		obj->isRetail = 0;
		s = "Dev/Test";
	}
	else
	{
		s = "unknown";
		r = VA_ERROR;
	}
	

	ps4_out(obj,WHITE, r, "%08X %-23s: %-17s", offset, "Console Type", s);

	//HDD
	offset = 0x1C9C00;
	memset(str, '\0', 64);
	memcpy(str, buffer + offset, 64);
	
	for (int i = 0; i < 64; i += 2)
	{
	
		char c = str[i];
		str[i] = str[i + 1];
		str[i + 1] = c;
	}

	//s1
	int i = -1;
	while (i<64 && str[++i] != ' ');
	unsigned char* s1, *s2, *s3;

	if(i<64)
	{
		str[i] = '\0';
		s1 = str;

		//s2
		while (i < 64 && str[++i] == ' ');
		s2 = str + i;
		while (i < 64 && str[++i] != ' ');
		str[i] = '\0';

		//s3
		while (i < 64 && str[++i] == ' ');
		s3 = str + i;

		unsigned char c = str[i];

		while (i < 64 && ((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')))
		{
			c = str[++i];
		}
		str[i] = '\0';
	}
	else
	{
		s1 = s2 = s3 = "";
	}
	

	ps4_out(obj, WHITE, r, "%08X %-23s: %s / %s / %s ", offset, "HDD", s1,s2,s3);

	//MAC Address
	offset = 0x1C4021;
	len = 6;

	if (ps4_is_blank(buffer + offset, len))
	{
		r = VA_ERROR;
	}
	else
	{
		r = VA_OK;
	}

	memset(str, '\0', 72);
	sprintf(str, "%2X-%2X-%2X-%2X-%2X-%2X", buffer[0x1C4021], buffer[0x1C4022], buffer[0x1C4023], buffer[0x1C4024], buffer[0x1C4025], buffer[0x1C4026]);
	ps4_out(obj,WHITE, r, "%08X %-23s: %-17s", offset, "MAC Address", str);

	//Motherboard Serial
	offset = 0x1C8000;
	len = 0xE;
	memset(str, '\0', 72);
	memcpy(str, buffer + offset, len);

	if (strlen(str) == 0 || ps4_is_blank(buffer + offset, len))
	{
		r = VA_ERROR;
	}
	else
	{
		r = VA_OK;
	}

	ps4_out(obj,WHITE, r, "%08X %-23s: %-17s", offset, "Motherboard Serial", str);

	//Console Serial
	offset = 0x1C8030;
	len = 0xA;
	memset(str, '\0', 72);
	memcpy(str, buffer + offset, len);

	if (strlen(str) == 0 || ps4_is_blank(buffer + offset, len))
	{
		r = VA_ERROR;
	}
	else
	{
		r = VA_OK;
	}

	ps4_out(obj,WHITE, r, "%08X %-23s: %-17s", offset, "Console Serial", str);

	//SOCUID
	offset = 0x1C8010;
	len = 0x10;
	ps4_hex_to_str(str, buffer + offset, len);

	if (ps4_is_blank(buffer + offset, len))
	{
		r = VA_ERROR;
	}
	else
	{
		r = VA_OK;
	}

	ps4_out(obj,WHITE, r, "%08X %-23s: %-17s", offset, "SOCUID", str);

	//SKU ID - Model Type
	offset = 0x1C8041;
	len = 0xD;
	memset(str, '\0', 72);
	memcpy(str, buffer + offset, len);

	if (strlen(str)==0||ps4_is_blank(buffer + offset, len))
	{
		r = VA_ERROR;
	}
	else
	{
		r = VA_OK;
	}

	ps4_out(obj,WHITE, r, "%08X %-23s: %-17s", offset, "SKU ID - Model Type", str);

	//WIFI-Bluethooth Version
	offset = 0x014421E;
	len = 13;
	memcpy(str, buffer + offset, len);
	s = "";

	r = !memcmp(str, WIFIBT_TORUS_2, len);

	if (r)
	{
		obj->torusVersion = 2;
		s = "Torus 2";
	}
	else
	{
		r = !memcmp(str, WIFIBT_TORUS_1, len);

		if (r)
		{
			obj->torusVersion = 1;
			s = "Torus 1";
		}
		else
		{
			obj->torusVersion = -1;
			r = VA_WARNING;
			;			s = "unknown";
		}
	}

	ps4_out(obj,WHITE, r, "%08X %-23s: %-17s", offset, "WIFI-Bluethooth Version", s);

	

}

void ps4_add_result(PS4Nor* obj, int r)
{
	if (r == VA_OK)
	{
		++obj->countOk;
	}
	else if (r == VA_ERROR)
	{
		++obj->countError;
	}
	else if (r == VA_WARNING)
	{
		++obj->countWarning;
	}
}

void ps4_out(PS4Nor* obj, Color color,int r, const char* format,...)
{
	va_list argp;
	va_start(argp, format);

	if (obj->outfile)
	{
		vfprintf(obj->outfile, format, argp);
	}

	cc_set_color(color);
	vfprintf(stdout, format, argp);
	cc_set_color(WHITE);

	const char* s = "";

	ps4_add_result(obj,r);

	if (r == VA_OK)
	{
		cc_set_color(GREEN);
		s =  " OK!";
	}
	else if (r == VA_ERROR)
	{
		cc_set_color(RED);
		s = " ERROR";
	}
	else if (r == VA_WARNING)
	{
		cc_set_color(YELLOW);
		s = " WARNING!";
	}

	if (r > -1)
	{
		if (obj->outfile)
		{
			fprintf(obj->outfile,s);
			fprintf(obj->outfile, "\n");
		}

		printf(s);
		cc_set_color(WHITE);
	}
	else
	{
		if (obj->outfile)
		{
			fprintf(obj->outfile, "\n");
		}
	}

	printf("\n");
}

void ps4_print_result(PS4Nor *obj)
{
	ps4_out(obj,WHITE, -1, "\nRESULT:\nOK     :%4u\nWARNING:%4u\nERROR  :%4u", obj->countOk, obj->countWarning, obj->countError);
}

int ps4_check_padding(PS4Nor* obj, unsigned int offset, unsigned int len, const unsigned char padding)
{
	const unsigned char* buffer = obj->buffer + offset;
	int count = 0;

	for (unsigned int i = 0; i < len; ++i)
	{
		if (buffer[i] != padding)
		{
			ps4_out(obj,RED, -1,"%08X error: at %08X expected %2X but found %2X", offset,offset+i, padding, buffer[i]);
			++count;

			//could useful to find differences between nor files..
			if (count == 20)
			{
				ps4_out(obj,RED, -1, "%08X too many mismatches in this area..", offset);
				return VA_ERROR;
			}
		}
	}

	if (count == 0)
	{
		return VA_OK;
	}

	return VA_ERROR;
}

//there isn't much to do, other than comparing with actual good data
//which require comparing a lot of files..
int ps4_check_unencrypted_data(PS4Nor *obj, unsigned int offset, unsigned int len)
{
	const unsigned char* buffer = obj->buffer;
	//the value could be anything for the non-encrypted half..
	//it could be fully 0xFF but unlikely to be fully 0x00
	unsigned int count_00 = 0;

	for (unsigned int i = 0; i < len; ++i)
	{
		if (buffer[i] == 0x00)
		{
			++count_00;
		}
	}

	//avoid single byte sections..
	if (count_00>1 && count_00 == len)
	{
		ps4_out(obj,YELLOW, -1, "%08X->%08X length=%08X warning: region is unlikely to be fully zeroes", offset, offset + len, len);
		return VA_WARNING;
	}

	return VA_OK;
}

//it is unlikey to see any sequence of the same char in any encrypted file..
//this observation will be the basis for validating them.
int ps4_check_encrypted_data(PS4Nor *obj, unsigned int offset, unsigned int len)
{
	unsigned char *buffer = obj->buffer + offset;
	int r = VA_OK;
	int count;
	int limit = 0;

	for (unsigned int i = 0; i < len; ++i)
	{
		count = 1;

		while (i + 1 < len)
		{
			if (buffer[i] == buffer[i + 1])
			{
				++count;
				++i;
			}
			else
			{
				break;
			}
		}

		//.. unlikely to happen in encrypted data..
		if (count > 4)
		{
			++limit;
			ps4_out(obj, RED, -1, "%08X->%08X length=%08X error: unlikely consecutive (%02X) in encrypted data.", offset + i - count, offset + i, count, buffer[i]);
			r = VA_ERROR;

			if (limit > obj->deltaLevel)
			{
				return r;
			}
		}
	}

	return r;
}

void ps4_check_x0000_x2000(PS4Nor *obj)
{
	const char* buffer = obj->buffer;
	ps4_out(obj,AQUA, -1, "checking 0x0000->0x2000: PS4 Nor Flash Header");
	//static section: SONY COMPUTER ENTERTAINMENT INC. header magic
	int r = !memcmp(buffer, "SONY COMPUTER ENTERTAINMENT INC.", 32);
	ps4_out(obj,WHITE,r,"%08X  static section: Header Magic= SONY COMPUTER ENTERTAINMENT INC.:",0);

	//static section: 01000000100000001800000001000000010000000800000001
	static const char static_section[] = { 0x01, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01 };
	unsigned int offset = 0x20;
	unsigned int len = sizeof(static_section);
	r = !memcmp(buffer+offset, static_section, len);
	ps4_out(obj,WHITE,r, "%08X  %-21s length=%08X", offset,"static section:", len);

	//static section:
	offset = 0x39;
	unsigned int end = 0x1000;
	r = ps4_check_padding(obj, offset,end - offset,0);
	ps4_out(obj,WHITE,r, "%08X  %-21s length=%08X",offset,"static section:", 0x1000- 0x39);

	//dynamic section
	const unsigned char dynamic_Section_1[] = { 0x80,0x00 };
	unsigned char c = buffer[0x1000];
	r = VA_WARNING;

	for (unsigned int i = 0; i < (sizeof(dynamic_Section_1) / sizeof(unsigned char)); ++i)
	{
		if (c == dynamic_Section_1[i])
		{
			r = VA_OK;
			break;
		}
	}

	ps4_out(obj,WHITE,r, "%08X %-29s %08X", 0x1000,"dynamic section:",c);

	//static section
	offset = 0x1001;
	end = 0x2000;
	r = ps4_check_padding(obj, offset, end - offset, 0);
	ps4_out(obj,WHITE, r,"%08X  %-14s(0x00) length=%08X",offset,"static section:", end - offset);
}

static const unsigned char mbr1Data1arr[] = { 0x10, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x20, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x06, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x21, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0A, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x26, 0x01, 0x00, 0x00 };
static const unsigned char mbr2Data1arr[] = { 0x08, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x20, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x06, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x21, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x0A, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x26, 0x01, 0x00, 0x00 };
static const unsigned char mbr1Data2arr[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x22, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x0E, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00 };
static const unsigned char mbr2Data2arr[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x0E, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x22, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x0E, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00 };


void ps4_checkMBR(PS4Nor *obj,unsigned int offset)
{
	const unsigned char* buffer = obj->buffer;
    const master_block_v4_t* mbr = (master_block_v4_t*)(buffer + offset);
	//MBR magic: Sony Computer Entertainment Inc.
	int r = !memcmp(mbr->magic, "Sony Computer Entertainment Inc.", 32);
	ps4_out(obj,WHITE,r, "%08X  static section: mbr.magic       = Sony Computer Entertainment Inc.:", offset);
	offset +=sizeof(mbr->magic);

	//Dynamic Section 1:
	r = mbr->version == 4;
	ps4_out(obj,WHITE,r, "%08X dynamic section: mbr.version     = %08X", offset, mbr->version);
	offset +=sizeof(mbr->version);

	//TODO: check for other dynamic values..
	const unsigned int mbr_sizes[] = { 0x1000 };
	unsigned int c = mbr->size;

	r = VA_WARNING;

	for (unsigned int i = 0; i < (sizeof(mbr_sizes) / sizeof(unsigned int)); ++i)
	{
		if (c == mbr_sizes[i])
		{
			r = VA_OK;
			break;
		}
	}

	ps4_out(obj,WHITE,r, "%08X dynamic section: mbr.size        = %08X", offset, c);
	offset +=sizeof(mbr->size);

	//TODO: check for other dynamic values..
	const uint64_t mbr_reserved_values[] = { 0x0,0x51F5131B };
	uint64_t c2 = mbr->reserved;
	r = VA_WARNING;

	for (unsigned int i = 0; i < (sizeof(mbr_reserved_values) / sizeof(uint64_t)); ++i)
	{
		if (c2 == mbr_reserved_values[i])
		{
			r = VA_OK;
			break;
		}
	}

	ps4_out(obj,WHITE,r, "%08X dynamic section: mbr.reserved    = %08llX", offset, c2);
	offset +=sizeof(mbr->reserved);

	r = VA_ERROR;

	if ((mbr->loader_start == 0x00000011 && offset == 0x2030)
		|| (mbr->loader_start == 0x00000309 && offset == 0x3030))
	{
		r = VA_OK;
	}

	ps4_out(obj,WHITE,r, "%08X  static section: mbr.loader_start= %08X", offset, mbr->loader_start);
	offset +=sizeof(mbr->loader_start);

	r = mbr->loader_count <= 0x000002FF;
	ps4_out(obj,WHITE,r, "%08X dynamic section: mbr.loader_count= %08X", offset, mbr->loader_count);
	offset += sizeof(mbr->loader_count);

	r = mbr->reserved2 == 0;
	ps4_out(obj,WHITE,r, "%08X  static section: mbr.reserved2   = %08llX", offset, mbr->reserved2);
	offset +=sizeof(mbr->reserved2);

	//static section
	
	const unsigned char* sdata;
	unsigned int sdataLen = sizeof(mbr1Data1arr);

	if (offset == 0x2040)
	{
		sdata = mbr1Data1arr;
	}
	else
	{
		sdata = mbr2Data1arr;
	}

	r = !memcmp(buffer + offset, sdata, sdataLen);
	ps4_out(obj,WHITE,r, "%08X  static section: length          = %08X", offset, sdataLen);

	//dynamic section
	offset += sdataLen;
	unsigned char c3 = buffer[offset];

	//they should have the same value??
	if (c3 == 0 && mbr->reserved != 0)
	{
		r = VA_WARNING;
	}
	else
	{
		r = VA_OK;
	}

	ps4_out(obj,WHITE,r, "%08X dynamic section: value           = %08X", offset, c3);

	//static section
	offset += sizeof(char);
	
	sdataLen = sizeof(mbr1Data2arr);

	if (offset == 0x2089)
	{
		sdata = mbr1Data2arr;
	}
	else
	{
		sdata = mbr2Data2arr;
	}

	r = !memcmp(buffer + offset, sdata, sdataLen);
	ps4_out(obj,WHITE,r, "%08X  static section: length          = %08X", offset, sdataLen);

	offset += sdataLen;
	r = ps4_check_padding(obj, offset, 0xF50, 0);
	ps4_out(obj,WHITE,r, "%08X  static section: (0x00) length   = %08X", offset, 0xF50);
}

void ps4_check_x2000_x3000(PS4Nor *obj)
{
	ps4_out(obj,AQUA,-1,"checking 0x2000->0x3000: Master Block 1 (for sflash0s1.cryptx32)");
	ps4_checkMBR(obj, 0x2000);
}

void ps4_check_x3000_x4000(PS4Nor* obj)
{
	ps4_out(obj,AQUA,-1,"checking 0x3000->0x4000: Master Block 2 (for sflash0s1.cryptx32b)");
	ps4_checkMBR(obj, 0x3000);
}

int ps4_check_slb2_file_entropy(const char* filename, double entropy)
{
	//faster method on 64 bit systems..
	uint64_t name = *(uint64_t*)filename;

	for (unsigned int i = 0; i < ARRAY_SIZE(entropy_entries); ++i)
	{
		if (entropy_entries[i][0] == name)
		{
			double x = slb2_entropy_values[entropy_entries[i][1]];
			return entropy > x;
		}
	}

	//generic case.. possibly unreahable code.. 
	return entropy > 7.9;
}

void ps4_check_slb2_entry(PS4Nor *obj,int offset, const SceSlb2Entry* entry)
{
	int r = entry->fileStartSector > 0;
	ps4_out(obj,WHITE,r, "%08X  static section: entry.fileStartSector= %08X", offset, entry->fileStartSector);

	//dynamic section
	offset += sizeof(entry->fileStartSector);
	r = entry->fileSizeInByte > 0;
	ps4_out(obj,WHITE,r, "%08X dynamic section: entry.fileSizeInByte = %08X", offset, entry->fileSizeInByte);

	//static filler
	offset += sizeof(entry->fileSizeInByte);
	r = *(uint64_t*)entry->reserved == 0;
	ps4_out(obj,WHITE,r, "%08X  static section: entry.reserved=%016X", offset, *(uint64_t*)entry->reserved);

	//static section
	offset += sizeof(entry->reserved);
	int len = strlen(entry->fileName);
	r = len > 0;
	ps4_out(obj,WHITE,r, "%08X  static section: entry.fileName       = %s", offset, entry->fileName);

	//static filler.. the rest of entry1->fileName should be filled with 0
	offset += len;
	r = ps4_check_padding(obj, offset, sizeof(entry->fileName) - len, 0);
	ps4_out(obj,WHITE,r, "%08X  static section: entry.fileName padding (0x00)  ", offset);
}

void ps4_check_slb2(PS4Nor *obj,unsigned int offset, const unsigned int end)
{
	const unsigned char* buffer = obj->buffer;
	//static section: SLB2 
	ps4_out(obj,WHITE,-1, "%08X  checking slb2 header:", offset);
	unsigned int address = offset;
	const SceSlb2Header* slb2 =(const SceSlb2Header*) (buffer + offset);
	int r = slb2->magic == 0x32424C53; //"SBL2" in hex
	ps4_out(obj,WHITE,r, "%08X  static section: slb2.magic       = %08X", address, slb2->magic);

	//static section:
	address += sizeof(slb2->magic);
	r = slb2->version == 1;
	ps4_out(obj,WHITE,r, "%08X  static section: slb2.version     = %08X", address, slb2->version);

	//static section: 
	address += sizeof(slb2->version);
	r = slb2->flags == 0;
	ps4_out(obj,WHITE,r, "%08X  static section: slb2.flags       = %08X", address, slb2->flags);

	//static section:
	address += sizeof(slb2->flags);
	r = slb2->entryNum > 0;
	ps4_out(obj,WHITE,r, "%08X  static section: slb2.entryNum    = %08X", address, slb2->entryNum);

	//dynamic section
	address += sizeof(slb2->entryNum);
	r = slb2->sizeInSector > 0;
	ps4_out(obj,WHITE,r, "%08X dynamic section: slb2.sizeInSector= %08X", address, slb2->sizeInSector);

	//static filler
	address += sizeof(slb2->sizeInSector);
	r = (slb2->reserved[0] + slb2->reserved[1] + slb2->reserved[2]) == 0;
	ps4_out(obj,WHITE,r, "%08X  static section: slb2.reserved    = (0x00)  ", address);

	//static section
	address += sizeof(slb2->reserved);

	for (unsigned int i = 0; i < slb2->entryNum; ++i)
	{
		ps4_out(obj,PINK,-1,"%08X  checking slb2.entryList[%2u]:", address, i);
		ps4_check_slb2_entry(obj,address, &slb2->entryList[i]);
		address += sizeof(SceSlb2Entry);
	}

	//static filler, after sbl2 entries
	int len = SLB2_HEADER_SIZE - (address - offset);
	r = ps4_check_padding(obj, address, len, 0);
	ps4_out(obj,WHITE,r, "%08X  static section: SBL2 header padding    (0x00)  ", address);
	static const char deadbeef[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xCA, 0xFE, 0xBE, 0xBE, 0xDE, 0xAF, 0xBE, 0xEF, 0xCA, 0xFE, 0xBE, 0xBE };
	
	for (unsigned int i = 0; i < slb2->entryNum; ++i)
	{
		const SceSlb2Entry* entry = &slb2->entryList[i];
		address = offset + entry->fileStartSector * 512;
		double entropy = ps4_entropy(buffer + address, entry->fileSizeInByte);
		r = ps4_check_slb2_file_entropy(entry->fileName, entropy);

		ps4_out(obj,BLUE,r,"%08X checking file: %s entropy= %2.2f       ", address, entry->fileName, entropy);
		
		//the file is encrypted if these bytes exist..
		r = !memcmp(buffer + address + 0x18, deadbeef, sizeof(deadbeef));
		ps4_md5(obj->md5,buffer + address, entry->fileSizeInByte);
		ps4_out(obj,WHITE,-1,"%08X checking file: %s size=%08X encrypted=%u MD5= %s", address,entry->fileName, entry->fileSizeInByte, r, obj->md5);
		//calculating the padding length of the final sector of this file
		
		address += entry->fileSizeInByte;
		int remaining =entry->fileSizeInByte % 512;

		if (remaining > 0)
		{
			int padding = 512 - remaining;
			r = ps4_check_padding(obj, address, padding, 0);
			ps4_out(obj,WHITE,r,"%08X static  section: SLB2 file padding (0x00)   ", address);
			address += padding;
		}
	}
	r = ps4_check_padding(obj, address, end - address, 0);
	ps4_out(obj,WHITE,r,"%08X static  section: SLB2 padding      (0x00)   ", address);
}

void ps4_check_0x4000_0x1C4000(PS4Nor *obj)
{
	ps4_out(obj,AQUA,-1,"checking 0x00004000->0x00064000: sflash0s0x32 : emc_ipl");
	ps4_check_slb2(obj, 0x00004000, 0x00064000);

	ps4_out(obj,AQUA, -1, "checking 0x00064000->0x000C4000: sflash0s0x32b: emc_ipl");
	ps4_check_slb2(obj, 0x00064000, 0x000C4000);

	ps4_out(obj,AQUA, -1, "checking 0x000C4000->0x00144000: sflash0s0x33 : eap_kbl");
	ps4_check_slb2(obj, 0x000C4000, 0x00144000);

	ps4_out(obj,AQUA, -1, "checking 0x00144000->0x001C4000: sflash0s0x38: wifibt");
	ps4_check_slb2(obj, 0x00144000, 0x001C4000);
}

void ps4_check_table(PS4Nor *obj, const FlashTable *table)
{
	const unsigned char* buffer = obj->buffer;
	int r =-1;
	const char* s;
	char* msg = "";
	int value = 0;
	unsigned int offset= 0;

	for (unsigned int i = 0; i < table->size; ++i)
	{
		const unsigned int* ar = table->entries+TB_RECORD_SIZE*i;
		offset = ar[TB_OFFSET];

		switch (ar[TB_TYPE])
		{
			
		case TB_DYNAMIC:
			r = ps4_check_unencrypted_data(obj, offset, ar[TB_LEN]);
			msg = "%08X        dynamic section:        length= %08X %s";
			value = ar[TB_LEN];
			break;
		case TB_STATIC_00:
			r = ps4_check_padding(obj, offset, ar[TB_LEN], 0x00);
			msg = "%08X         static section: (0x00) length= %08X %s";
			value = ar[TB_LEN];
			break;
		case TB_STATIC_FF:
			r = ps4_check_padding(obj, offset, ar[TB_LEN], 0xFF);
			msg = "%08X         static section: (0xFF) length= %08X %s";
			value = ar[TB_LEN];
			break;
		case TB_STATIC_BYTE:
			r = ar[3] == buffer[offset];
			msg = "%08X         static   value:        value = %08X %s";
			value = buffer[offset];
			break;
		case TB_STATIC_STR:
			s = table->binary+ar[TB_N]* TB_BIN_SIZE;
			r = !strncmp(buffer + ar[TB_OFFSET], s, ar[TB_LEN]);
			msg = "%08X         static section:        length= %08X %s";
			value = ar[TB_LEN];
			break;
		case TB_PER_CONSOLE:
			r = ps4_check_unencrypted_data(obj, offset, ar[TB_LEN]);
			msg = "%08X            per-console:        length= %08X %s";
			value = ar[TB_LEN];
			break;

		case TB_PER_FIRMWARE:
			r = ps4_check_unencrypted_data(obj, offset, ar[TB_LEN]);
			msg = "%08X           per-firmware:        length= %08X %s";
			value = ar[TB_LEN];
			break;
		
		case TB_ENCRYPED:
			r = ps4_check_encrypted_data(obj, offset, ar[TB_LEN]);
			msg = "%08X         encrypted data:        length= %08X %s";
			value = ar[TB_LEN];
			break;

		case TB_PER_CONSOLE_ENCRYPED:
			r = ps4_check_encrypted_data(obj, offset, ar[TB_LEN]);
			msg = "%08X  encrypted per-console:        length= %08X %s";
			value = ar[TB_LEN];
			break;

		case TB_PER_FIRMWARE_ENCRYPED:
			r = ps4_check_encrypted_data(obj, offset, ar[TB_LEN]);
			msg = "%08X encrypted per-firmware:        length= %08X %s";
			value = ar[TB_LEN];
			break;
		default:
			break;
		}

		char* note = tb_strings[ar[TB_NOTE]];
		ps4_out(obj,WHITE,r,msg,offset, value, note);
	}

#ifdef DEBUG

	const unsigned int* a = table->entries;

	for (unsigned int i = 1; i < table->size; ++i)
	{
		unsigned int* b = table->entries + (i * TB_RECORD_SIZE);
		int offset1 = a[TB_OFFSET];
		int offset2 = b[TB_OFFSET];
		int len1 = a[TB_LEN];
		int len2 = b[TB_LEN];
		
		int checkedArea = offset1 + len1;
		int diff = offset2 - checkedArea;

		if (diff >0)
		{
			ps4_out(obj,GREEN, -1, "%08X->%08X section is    unlisted: length=%08X", checkedArea, offset2, diff);
		}
		else if (diff<0)
		{
			ps4_out(obj,RED  , -1, "%08X->%08X section is overlapping: length=%08X", offset1, offset2, -diff);
		}

		a = b;
	}
#endif //DEBUG
}

void ps4_check_0x1C4000_0x200000(PS4Nor *obj)
{
	ps4_out(obj,AQUA, -1, "checking 0x001C4000->0x00200000:");
	ps4_check_table(obj, &cid_table);
}

//these entries could be of variable length..
//the same thing could true about anything after this point..
const unsigned int segment1_entries[][TB_RECORD_SIZE] =
{
//0x200000 -> 0x380000:
{0x00200000,0x200 ,5,0,0,0},
{0x00200200,0xE00 ,2,0,0,0},
{0x00201000,0x200 ,5,0,0,0},
{0x00201200,0xE00 ,2,0,0,0},
{0x00202000,0x200 ,5,0,0,0},
{0x00202200,0xE00 ,2,0,0,0},
{0x00203000,0x200 ,5,0,0,0},
{0x00203200,0xE00 ,2,0,0,0},
{0x00204000,0x8C790 ,5,0,0,0},
{0x00290790,0x70 ,2,0,0,0},
{0x00290800,0x130 ,5,0,0,0},
{0x00290930,0xD0  ,2,0,0,0},
{0x00290A00,0xE0  ,5,0,0,0},
{0x00290AE0,0x120 ,2,0,0,0},
{0x00290C00,0x160 ,5,0,0,0},
{0x00290D60,0xA0  ,2,0,0,0},
{0x00290E00,0x40   ,5,0,0,0},
{0x00290E40,0x6F1C0,2,0,0,0},
{0x00300000,0x80000,7,0,0,0}
};

const FlashTable segment1_table = { (unsigned int*)segment1_entries,ARRAY_SIZE(segment1_entries),0,0 };

void ps4_check_0x200000_0x380000(PS4Nor *obj)
{
	ps4_out(obj,AQUA, -1, "checking 0x00200000->0x00380000:");
	ps4_check_table(obj, &segment1_table);

}

const unsigned int vtrm0_entries[][TB_RECORD_SIZE] =
{
	//0x00380000->0x003A0000 table
	{0x00380079,0xFF   ,2,0,0,0},
	{0x00380178,0x60   ,5,0,0,0},//vtrm1 digest
	{0x003801D8,0xE28  ,2,0,0,0},
	{0x00381000,0x9    ,0,0,0,0},
	{0x00381009,0x3F   ,2,0,0,0},
	{0x00381048,0x1    ,0,0,0,0},
	{0x00381049,0x3F   ,2,0,0,0},
	{0x00381088,0x9    ,0,0,0,0},
	{0x00381091,0x3F   ,2,0,0,0},
	{0x003810D0,0x1    ,0,0,0,0},
	{0x003810D1,0xF2F  ,2,0,0,0},
	{0x00382000,0x20   ,5,0,0,0},
	{0x00382020,0xFE0  ,2,0,0,0},
	{0x00383000,0x1000 ,7,0,0,0},
	{0x00384000,0x1C000,2,0,0,0}
};

const FlashTable vtrm0_table = { (unsigned int*)vtrm0_entries,ARRAY_SIZE(vtrm0_entries),0,0 };

const unsigned int vtrm1_entries[][TB_RECORD_SIZE] =
{
	//0x003A0000->0x003C0000 table
	{0x003A0079,0xFF   ,2,0,0,0},
	{0x003A0178,0x60   ,5,0,0,0},//vtrm2 digest
	{0x003A01D8,0xE28  ,2,0,0,0},
	{0x003A1000,0x9    ,0,0,0,0},
	{0x003A1009,0x3F   ,2,0,0,0},
	{0x003A1048,0x1    ,0,0,0,0},
	{0x003A1049,0x3F   ,2,0,0,0},
	{0x003A1088,0x9    ,0,0,0,0},
	{0x003A1091,0x3F   ,2,0,0,0},
	{0x003A10D0,0x1    ,0,0,0,0},
	{0x003A10D1,0xF2F  ,2,0,0,0},
	{0x003A2000,0x20   ,5,0,0,0},
	{0x003A2020,0xFE0  ,2,0,0,0},
	{0x003A3000,0x1000 ,7,0,0,0},
	{0x003A4000,0x1C000,2,0,0,0}
};

const FlashTable vtrm1_table = { (unsigned int*)vtrm1_entries,ARRAY_SIZE(vtrm1_entries),0,0 };

void ps4_check_VTRM_header(PS4Nor* obj, const unsigned int offset, const vtrmHeader* vtrm)
{
	unsigned int address = offset;
	uint8_t c = vtrm->flag[0];
	int r = c == 0xFC || c == 0x03 || c == 0x00;
	ps4_out(obj,WHITE, r, "%08X dynamic section: vtrm->flag          = %08X", address, c);

	address += sizeof(vtrm->flag);
	r = ps4_check_padding(obj, address, ARRAY_SIZE(vtrm->padding1), 0xFF);
	ps4_out(obj,WHITE, r, "%08X  static section: vtrm->padding1 (0xFF)         ", address);

	address += sizeof(vtrm->padding1);
	r = vtrm->constant == 1;
	ps4_out(obj,WHITE, r, "%08X  static section: vtrm->constant      = %08X", address, vtrm->constant);

	address += sizeof(vtrm->constant);
	r = ps4_check_padding(obj, address, ARRAY_SIZE(vtrm->padding2), 0xFF);
	ps4_out(obj,WHITE, r, "%08X  static section: vtrm->padding2 (0xFF)         ", address);

	address += sizeof(vtrm->padding2);
	r = 0 == strcmp(vtrm->magic, "SCEVTRM");
	ps4_out(obj,WHITE, r, "%08X  static section: vtrm->magic         = %8s", address, vtrm->magic);

	address += sizeof(vtrm->magic);
	r = vtrm->count1 == vtrm->count2;
	ps4_out(obj,WHITE, r, "%08X dynamic section: vtrm->count1        = %08X", address, vtrm->count1);

	address += sizeof(vtrm->count1);
	r = ps4_check_padding(obj, address, ARRAY_SIZE(vtrm->padding3), 0x00);
	ps4_out(obj,WHITE, r, "%08X  static section: vtrm->padding3 (0x00)         ", address);

	address += sizeof(vtrm->padding3);
	r = vtrm->count2 == vtrm->count1;
	ps4_out(obj,WHITE, r, "%08X dynamic section: vtrm->count2        = %08X", address, vtrm->count2);

	address += sizeof(vtrm->count2);
	r = ps4_check_padding(obj, address, ARRAY_SIZE(vtrm->padding4), 0x00);
	ps4_out(obj,WHITE, r, "%08X  static section: vtrm->padding4 (0x00)         ", address);

	static const uint8_t unknownBytes[] = { 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	address += sizeof(vtrm->padding4);
	const uint8_t* ar = vtrm->unknown;
	r = VA_OK;

	for (int i = 0; i < 16; ++i)
	{
		if (ar[i] != unknownBytes[i])
		{
			r = VA_ERROR;
			break;
		}
	}

	ps4_out(obj,WHITE, r, "%08X  static section: vtrm->unknown                 ", address);

	address += sizeof(vtrm->unknown);
	r = ps4_check_padding(obj, address, ARRAY_SIZE(vtrm->padding5), 0xFF);
	ps4_out(obj,WHITE, r, "%08X  static section: vtrm->padding5      (0xFF)    ", address);

	address += sizeof(vtrm->padding5);
	c = vtrm->ctrlFlag[0];
	r = c == 0xFF || c == 0xFE || c == 0xFC;
	ps4_out(obj,WHITE, r, "%08X dynamic section: vtrm->ctrlFlag      = %08X ", address, c);
}

void ps4_check_0x380000_0x3C0000(PS4Nor* obj)
{
	const unsigned char* buffer = obj->buffer;
	ps4_out(obj,AQUA, -1, "checking 0x00380000->0x003C0000: Virtual Table Rights Management");
	
	unsigned int offset = VTRM_REGION_1;
	vtrmHeader *vtrm = (vtrmHeader *)(buffer + offset);

	ps4_out(obj,BLUE, -1, "%08X checking VTRM Region 0", offset);
	ps4_check_VTRM_header(obj,offset, vtrm);
	ps4_check_table(obj, &vtrm0_table);

	offset = VTRM_REGION_2;
	vtrm = (vtrmHeader*)(buffer + offset);

	ps4_out(obj,BLUE, -1, "%08X checking VTRM Region 1", offset);
	ps4_check_VTRM_header(obj,offset, vtrm);
	ps4_check_table(obj, &vtrm1_table);
}

const unsigned int coreOS_entries[][TB_RECORD_SIZE] =
{
	//0x003C0000->0x02000000 table
	{0x003C0000,0x10   ,7,0,0,0},
	{0x003C0010,0x20   ,9,0,0,0},
	{0x003C0030,0x20   ,7,0,0,0},
	{0x003C0050,0x10   ,9,0,0,0},
	{0x003C0060,0x20   ,7,0,0,0},
	{0x003C0080,0x10   ,9,0,0,0},
	{0x003C0090,0x20   ,7,0,0,0},
	{0x003C00B0,0x10   ,9,0,0,0},
	{0x003C00C0,0x20   ,7,0,0,0},
	{0x003C00E0,0x10   ,9,0,0,0},
	{0x003C00F0,0x20   ,7,0,0,0},
	{0x003C0110,0x10   ,9,0,0,0},
	{0x003C0120,0x20   ,7,0,0,0},
	{0x003C0140,0x10   ,9,0,0,0},
	{0x003C0150,0xC0   ,7,0,0,0},
	{0x003C0210,0x10   ,9,0,0,0},
	{0x003C0220,0x30   ,7,0,0,0},
	{0x003C0250,0x10   ,9,0,0,0},
	{0x003C0260,0x20   ,7,0,0,0},
	{0x003C0280,0x10   ,9,0,0,0},
	{0x003C0290,0x10   ,7,0,0,0},
	{0x003C02A0,0x290  ,9,0,0,0},
	{0x003C0530,0x1FC0 ,7,0,0,0},
	{0x003C24F0,0x290  ,9,0,0,0},
	{0x003C2780,0x197D880,7,0,0,0},//unlisted
	{0x01D40000,0x2C0000,2,0,0,0}
};

const FlashTable coreOS_table = { (unsigned int*)coreOS_entries,ARRAY_SIZE(coreOS_entries),0,0 };

void ps4_check_0x3C0000_0x2000000(PS4Nor *obj)
{
	ps4_out(obj,AQUA, -1, "checking 0x003C0000->0x02000000: CoreOS");
	ps4_check_table(obj, &coreOS_table);
}

//0x00 = 3.39% 0xFF = 11.47% diff(0xFE) = 85.11% match = 0.04%
//a good nor dump should't have any zero in a non-zero area..
//the 0.03% bytes should also match..
//the imgdiff.bin accompanying this code is from 3 dumps fw=1.62
void ps4_compare_img_diff(PS4Nor *obj, PS4Nor* objDiff)
{
	const unsigned char* buffer = obj->buffer;
	const unsigned char* diff = objDiff->buffer;
	int count = 0;

	for (int i = 0; i < NOR_SIZE; ++i)
	{
		unsigned char c1 = buffer[i];
		unsigned char c2 = diff[i];

		if (c1 == 0x00 && c2 != 0x00)
		{
			int a = i;

			while (i < NOR_SIZE && (c1 == 0x00 && c2 != 0x00))
			{
				++i;
				c1 = buffer[i];
				c2 = diff[i];
			}

			int len = i - a;
			
			if (len > obj->deltaLevel)
			{
				ps4_out(obj,GREEN, VA_WARNING, "%08X->%08X unexpected 0x00 length =%08X", a, i, len);
			}
		}
		else if(c1 == 0xFF && !(c2==0xFF || c2 == 0xFE))
		{
			int a = i;

			while (i < NOR_SIZE && (c1 == 0xFF &&!(c2 == 0xFF || c2 == 0xFE)))
			{
				++i;
				c1 = buffer[i];
				c2 = diff[i];
			}

			int len = i - a;

			if (len > obj->deltaLevel)
			{
				ps4_out(obj,BLUE, VA_WARNING, "%08X->%08X unexpected 0xFF length =%08X", a, i, len);
			}
		}
		else if (c1 == 0xFE && c2 != 0xFE)
		{
			int a = i;

			while (i < NOR_SIZE && (c1 == 0xFE && c2 != 0xFE))
			{
				++i;
				c1 = buffer[i];
				c2 = diff[i];
			}

			int len = i - a;

			if (len > obj->deltaLevel)
			{
				ps4_out(obj,PINK, VA_WARNING, "%08X->%08X expected 0xFE length =%08X", a, i, len);
			}
		}
		else if (c1 != c2 && !(c2 == 0x00||c2==0xFE||c2==0xFF))
		{
			int a = i;

			while (i < NOR_SIZE && !(c2 == 0x00 || c2 == 0xFE || c2 == 0xFF))
			{
				++i;
				c1 = buffer[i];
				c2 = diff[i];
			}

			int len = i - a;

			if (len > obj->deltaLevel)
			{
				++count;
				ps4_out(obj,RED, VA_ERROR, "%08X->%08X unexpected bytes length =%08X", a, i, len);
			}
		}
	}

	int r = count == 0 ? VA_OK : VA_ERROR;
	ps4_out(obj,AQUA, r, "possible corrupted sections: count  =%4u",count);
}

void ps4_print_statistics(PS4Nor *obj)
{
	const unsigned char* buffer = obj->buffer;
	static double stats[256];
	memset(stats, 0, 256*sizeof(double));

	for (int i = 0; i < NOR_SIZE; ++i)
	{
		unsigned char c = buffer[i];
		double count = stats[c];
		stats[c] = ++count;
	}

	for (unsigned int i = 0; i < 256; ++i)
	{
		double count = stats[i];
		stats[i] = count/(NOR_SIZE/100);
	}

	ps4_out(obj,GREEN,-1, "bytes statistics:");

	unsigned char line[11];
	line[10] = '\0';

	for (int i = 0; i < 256; ++i)
	{
		double per = stats[i];
		
		if (per > 0.38)
		{
			memset(line, ' ', 10);

			for (int j = 0; j < per && j<10; ++j)
			{
				line[j] = '|';
			}

			int r;

			//0xFF < 11.9% 0x00 < 2.44% other <0.38%
			if (i == 0xFF) 
			{
				if (per> 11.8 && per < 12)
				{
					r = VA_OK;
				}
				else
				{
					r = VA_WARNING;
				}
				
			}

			else if (i == 0x00)
			{
				if (per > 2.4 && per < 2.6)
				{
					r = VA_OK;
				}
				else
				{
					r = VA_WARNING;
				}

			}
			else
			{
				r = VA_WARNING;
			}

			ps4_out(obj,YELLOW, r, "0x%02X %s %5.2f%%",i,line,per);
		}
	}
}
