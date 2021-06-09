//############################################################################
// FILE:	  peri_TimerCtl.h
// CONTENT:
// AUTHOR:    LvSL
// DATE:      2014/02/24
//############################################################################

#ifndef PERI_TIMER_CTL_H_
#define PERI_TIMER_CTL_H_

//timer struct_function define
typedef void (*TIMER_OVER_FUNC)(uint16_t id);

//timer Property
#define TIMER_MODE_ONESHOT 0       /**< one time      */
#define TIMER_MODE_PRIODIC 1       /**< period treat  */

//timer run property
#define TIMER_STOP         0       /**< stop timer  */
#define TIMER_START        1       /**< start timer */

// struct define
struct TimerConfig {
    uint16_t mode;                   /**< timer property             */
    uint32_t init_ms;                /**< init timer count           */
    TIMER_OVER_FUNC func;          /**< treat func after timer     */
};

struct TimerState
{
    int16_t   state;             		/**< run state     		*/
    uint32_t  ms;                		/**< run timer 			*/
    struct TimerConfig config; 		/**< timer/func define	*/
};

//Timer ID
#define TIMER_ID_OPE_KEYSCAN_PERIOD  	0 /**< example */
#define PIGAINSWITCH					1 /*PI Parameter Gain Switch*/
#define POSIOKTIMER					    2 /*Position Complete Hold Time*/
#define PULSEOUTZPHASE					3 /*Pulse Output Z Phase Valid Timer*/
#define POSIEASYPLC					    4 /*Position Easy PLC Wait Timer*/
#define VELOEASYPLC					    5 /*Velocity Easy PLC Run Timer*/
#define RESETDELAY                      6/*复位延时*/
#define INERTIADELAY                    7/*惯量间隙时间*/
#define TIMER_ID_COUNT               	8 /**< Timer ID Count           */


//extern function
extern uint16_t 	TimerInit(uint16_t, struct TimerConfig *);
extern uint16_t 	TimerStart(uint16_t);
extern void 	TimerReStart(uint16_t);
extern void 	TimerStop(uint16_t);

#endif /* PERI_TIMER_CTL_H_ */
