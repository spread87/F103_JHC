/*
 * peri_FuncCodeEnum.h
 *
 * Created on: 2014-11-26
 * Author: JH-LvSL
 */

#ifndef PERI_FUNCCODEENUM_H_
#define PERI_FUNCCODEENUM_H_

enum
{
	POSITIONCTRLMODE = 0,	//Position Control Mode
	VELOCITYCTRLMODE,		//Velocity Control Mode
	TORQUECTRLMODE,			//Torque Control Mode
	POSIVELOCTRLMODE,		//Position/Velocity Control Mode
	POSITORQCTRLMODE,		//Position/Torque Control Mode
	VELOTORQCTRLMODE,		//Velocity/Torque Control Mode
	FULLCOSEDCTRLMODE,		//Full_Cosed Control Mode
	JOGCTRLMODE,
	//PIDVELOCITYMODE,
	//PIDTORQUEMODE,
}F1002; /*Control Mode Select*/




enum
{
	LOWPULSEINPUT = 0,	//Low Speed Pulse Input
	HIGHPULSEINPUT,		//High Speed Pulse Input
}F1003;	/*Command pulse input selection*/

enum
{
	PULSEAB = 0,		//Pulse Mode:A+B
	PULSEDIR,			//Pulse + Dir
	PULSECCWCW,			//CCW+CW
}F1005;/*Command pulse input mode setup*/

enum
{
	InnerVeloCMD = 0,	// 0 £∫Internal Velocity CMD
	AI1VeloCMD,			// 1£∫ AI1 Velocity CMD
	AI2VeloCMD,			// 2£∫ AI2 Velocity CMD
	AI3VeloCMD,			// 3£∫ AI3 Velocity CMD
	VelocityPLCMode,		// 4£∫ Velocity PLC Mode
	VelocityPIDMode,		// 5£∫ Velocity PID Mode
	VelocityCOMMode,		// 6£∫
	VelocityPIMode,			// 7£∫
	VelocityCANMode,		// 8£∫
}F4000;/*Velocity CMD Source Setup*/

enum
{
	PosiPLCOneShotModeSetup = 0,//Easy Position PLC One Shot Mode
	PosiPLCCycleModeSetup,		//Easy Position PLC Cycle Mode
	PosiPLCDIModeSetup,			//Easy Position DI Mode
//	PosiPLCQueueModeSetup,		//Easy Position Queue Mode
}F7000;/*Muti-position Operation Mode Setup*/

enum    //Ãÿ ‚¥Ê»°
{
 Normal=0,
 ParameterInit1,
 ParameterInit2,
 ParameterInit3,
 ParameterInit4,
 ParameterInit5,
 ParameterInit6,
 ParameterInit7,
 ParameterInit8,
 ParameterInit9,
 ParameterInit10,
 InflexibilityChange,
 InertiaChange,
}SPECIALE2PROMSAVE;
#endif /* PERI_FUNCCODEENUM_H_ */
