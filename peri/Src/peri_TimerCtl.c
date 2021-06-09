//############################################################################
// FILE:	  peri_TimerCtl.c
// CONTENT:
// AUTHOR:    LvSL
// DATE:      2014/02/24
//############################################################################

#include <main.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include <peri_GlobalVariablesExtern.h>
#include <peri_TimerCtl.h>
static struct TimerState  timerStateTable[TIMER_ID_COUNT];

/**
 * @brief timer treat
 * @note  period:1ms
 */
void TimerControlThread(void)
{
    uint16_t i;
    struct TimerState *ts;

    for(i = 0; i < TIMER_ID_COUNT; i++)
    {
        ts = &timerStateTable[i];

        if(ts->state == TIMER_START)
        {
        	//ts->ms--;

            if(ts->ms == 0)
            {
                ts->config.func(i);

                if(ts->config.mode == TIMER_MODE_PRIODIC)
                {
                    ts->ms = ts->config.init_ms;
                }
                else
                {
                    ts->state = TIMER_STOP;
                }
            }
            else
            {
                 ts->ms--;
            }
        }
    }
}

/**
 * @brief init timer
 * @param[in] id timer ID
 * @param[in] *config
 */
uint16_t TimerInit(uint16_t id, struct TimerConfig *config)
{
    if(timerStateTable[id].state == TIMER_START)
    {
        return 0;
    }
    timerStateTable[id].ms = config->init_ms;
    timerStateTable[id].state = TIMER_STOP;
    timerStateTable[id].config = *config;

    return 1;
}

/**
 * @brief start timer
 * @param[in] id timer ID
 */
uint16_t TimerStart(uint16_t id)
{
    if(timerStateTable[id].state == TIMER_START)
    {
        return 0;
    }
    timerStateTable[id].ms = timerStateTable[id].config.init_ms;
    timerStateTable[id].state = TIMER_START;

    return 1;
}

/**
 * @brief restart timer
 * @param[in] id timer ID
 */
void TimerReStart(uint16_t id)
{
    timerStateTable[id].ms = timerStateTable[id].config.init_ms;
    timerStateTable[id].state = TIMER_START;
}

/**
 * @brief stop timer
 * @param[in] id timer ID
 */
void TimerStop(uint16_t id)
{
    if(timerStateTable[id].state == TIMER_STOP)
    {
        return;
    }
    timerStateTable[id].state = TIMER_STOP;
}


