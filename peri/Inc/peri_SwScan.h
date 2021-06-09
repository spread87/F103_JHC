//############################################################################
// FILE:	  peri_sw_scan.h
// CONTENT:	  IO filter treat
// AUTHOR:    LvSL
// DATE:      2013/07/03
//############################################################################

/*************************************************************************************/
/*
 * SW flags
 */
struct SW_FLAG_BITS
{
    uint16_t  DI_sta_F        		: 1;   // bit[0]   0 = pin off      1 = pin on
    uint16_t  DI_ON_DLY_F     	: 1;   // bit[1]   0 = no effect    1 = on  scan
    uint16_t  DI_OFF_DLY_F    	: 1;   // bit[2]   0 = no effect    1 = off scan
    uint16_t  DI_PIN_F        		: 1;   // bit[3]   0 = pin off      1 = pin on
    uint16_t  DI_OUT_F		  		: 1;
//    uint16_t  DI_MIN_F		  : 1;
};


union SW_STATUS_FLG
{
	uint16_t all;
    struct SW_FLAG_BITS   bit;
};

typedef struct
{
    union   SW_STATUS_FLG   flg;

    uint16_t  TIM_BASE;
    uint16_t  SCAN_CNT;
    uint16_t  TRUE_CNT;
    uint16_t  SET_TIME_DELAY;
    uint16_t  SET_SCAN_CNT;

}SW_SCAN_FUN;

extern SW_SCAN_FUN ButtonUp;
extern SW_SCAN_FUN ButtonDown;
extern SW_SCAN_FUN ButtonShift;
extern SW_SCAN_FUN ButtonMode;
extern SW_SCAN_FUN ButtonSet;

extern void InitScanDiSwitch(void);
extern void UpdateAllDiPort(void);

//############################################################################
// No More.
//############################################################################

