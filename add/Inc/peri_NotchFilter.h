//############################################################################
// FILE:	  peri_notch_filter.h
// CONTENT:	  2-step notch filter
// AUTHOR:    LvSL
// DATE:      2013/08/08
//############################################################################


typedef struct
{
	int32_t 		X;						/*input	 		Format:Q24*/
	int32_t 		X_n1;					/*input(n-1)	*/
	int32_t 		X_n2;					/*input(n-2)	*/
	int32_t 		Y;						/*output	 	*/
	int32_t 		Y_n1;					/*output(n-1)	*/
	int32_t 		Y_n2;					/*output(n-2)	*/
	int32_t 		NotchCoef_a0;			/*Denominator coefficients*/
	int32_t 		NotchCoef_a1;
	int32_t 		NotchCoef_a2;
	int32_t 		NotchCoef_b0;			/*Numerator coefficients*/
	int32_t 		NotchCoef_b1;
	int32_t 		NotchCoef_b2;
	uint16_t		Bandwidth;
	uint16_t		NotchFreq;
	uint16_t		SampleFreq;
	void        (*CoefCalc)();			/*pointer to filter coefficients calculation*/
	void  		(*Filtercalc)();		/*Pointer to filter treat function */
}NOTCHFILTER;

typedef NOTCHFILTER *NOTCHFILTER_Handle;

#define NOTCHFILTER_DEFAULTS { 0, 0, 0, \
                          	   0, 0, 0, \
                          	   0x1000000, \
                          	   0, 0, \
                          	   0, 0, 0, \
                          	   0, 0, 8000, \
                          	   (void (*)(uint32_t))NotchCoef_Calc,\
                          	   (void (*)(uint32_t))NotchFilter_Calc }


void NotchCoef_Calc(NOTCHFILTER_Handle);
void NotchFilter_Calc(NOTCHFILTER_Handle);




