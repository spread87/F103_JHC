/*
 * core_Filter.h
 *
 *  Created on: 2017-6-2
 *      Author: Mxj
 */

#ifndef CORE_FILTER_H_
#define CORE_FILTER_H_

typedef struct {
	int32_t  Xk;  		// Input:
	int32_t  Sk;
	int32_t  Sk1;			//
	int32_t  Sk2;			//
	int32_t  Yk;			// Output
	int64_t	 a1;
	int64_t  a2;
	int64_t	 b0;
	int64_t	 b1;
	int64_t  b2;
	int32_t	 ws;
	void  (*init)();	// Pointer to calculation function
	void  (*calc)();	// Pointer to calculation function
} LPFILTER;

typedef LPFILTER *LPFILTER_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the LPFILTER object.
-----------------------------------------------------------------------------*/
#define LPFILTER_DEFAULTS { 0, \
                          0,0,0, \
                          0, \
                          0,0,0,0,0, \
                          0, \
                          (void (*)(uint32_t))lpfilter_init,\
              			  (void (*)(uint32_t))lpfilter_calc }

/*------------------------------------------------------------------------------
Prototypes for the functions in LPFILTER.C
------------------------------------------------------------------------------*/
void lpfilter_init(LPFILTER_handle);
void lpfilter_calc(LPFILTER_handle);


#endif /* CORE_FILTER_H_ */
