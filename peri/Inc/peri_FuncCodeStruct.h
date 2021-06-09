/*
 *  peri_FuncCodeStruct.h
 *
 *  Created on: 2014-2-11
 *  Author: JH-LvSl
 */

#ifndef PERI_FUNCODESTRUCT_H_
#define PERI_FUNCODESTRUCT_H_

/*Function code struct define*/
typedef struct FuncCodeSingleF00	//Hide function code
{
	uint32_t F0000;
	uint32_t F0001;
	uint32_t F0002;
	uint32_t F0003;
	uint32_t F0004;
	uint32_t F0005;
	uint32_t F0006;
	uint32_t F0007;
	uint32_t F0008;
	uint32_t F0009;
	uint32_t F0010;
	uint32_t F0011;
	uint32_t F0012;
	uint32_t F0013;
	uint32_t F0014;
	uint32_t F0015;
	uint32_t F0016;
	uint32_t F0017;
	uint32_t F0018;
	uint32_t F0019;
	uint32_t F0020;
	uint32_t F0021;
	uint32_t F0022;
	uint32_t F0023;
	uint32_t F0024;
	uint32_t F0025;
	uint32_t F0026;
	uint32_t F0027;
	uint32_t F0028;
	uint32_t F0029;
	uint32_t F0030;
	uint32_t F0031;
	uint32_t F0032;
	uint32_t F0033;
	uint32_t F0034;
	uint32_t F0035;
	uint32_t F0036;
	uint32_t F0037;
	uint32_t F0038;
	uint32_t F0039;
	uint32_t F0040;
	uint32_t F0041;
	uint32_t F0042;
	uint32_t F0043;
	uint32_t F0044;
	uint32_t F0045;
	uint32_t F0046;
	uint32_t F0047;
	uint32_t F0048;
	uint32_t F0049;
	uint32_t F0050;
	uint32_t F0051;
	uint32_t F0052;
	uint32_t F0053;
	uint32_t F0054;
	uint32_t F0055;
	uint32_t F0056;
	uint32_t F0057;
	uint32_t F0058;
	uint32_t F0059;
	uint32_t F0060;
	uint32_t F0061;
	uint32_t F0062;
	uint32_t F0063;
	uint32_t F0064;
	uint32_t F0065;
	uint32_t F0066;
	uint32_t F0067;
	uint32_t F0068;
	uint32_t F0069;
	uint32_t F0070;
	uint32_t F0071;
	uint32_t F0072;
	uint32_t F0073;
	uint32_t F0074;
	uint32_t F0075;
	uint32_t F0076;
	uint32_t F0077;
	uint32_t F0078;
	uint32_t F0079;
	uint32_t F0080;
	uint32_t F0081;
	uint32_t F0082;
	uint32_t F0083;
	uint32_t F0084;
	uint32_t F0085;
	uint32_t F0086;
	uint32_t F0087;
	uint32_t F0088;
	uint32_t F0089;
	uint32_t F0090;
	uint32_t F0091;
	uint32_t F0092;
	uint32_t F0093;
	uint32_t F0094;
	uint32_t F0095;
	uint32_t F0096;
	uint32_t F0097;
	uint32_t F0098;
	uint32_t F0099;
}FuncCodeSingleF00;

typedef struct FuncCodeSingleF10
{
	uint32_t F1000;				//Manufacturers debugging function code
	uint32_t F1001;				//System Parameter Initialization
	uint32_t F1002;				//Control mode setup
	uint32_t F1003;				//Command pulse input selection
	uint32_t F1004;				//Command pulse rotational direction setup
	uint32_t F1005;				//Command pulse input mode setup
	uint32_t F1006;				//1st numerator of electronic gear
	uint32_t F1007;				//Denominator of electronic gear
	uint32_t F1008;				//Output pulse counts per one motor
	uint32_t F1009;				//Reversal of pulse output logic
	uint32_t F1010;				//1st torque limit
	uint32_t F1011;				//Position deviation excess setup
	uint32_t F1012;				//Servo On Delay Time
	uint32_t F1013;				//Servo Off Delay Time
	uint32_t F1014;				//External regenerative resistor setup
	uint32_t F1015;				//External regenerative resistor power
	uint32_t F1016;				//External regenerative resistor resistance
	uint32_t F1017;				//Encode setup
	uint32_t F1018;
	uint32_t F1019;
	uint32_t F1020;
	uint32_t F1021;
	uint32_t F1022;
	uint32_t F1023;
	uint32_t F1024;
	uint32_t F1025;
	uint32_t F1026;
	uint32_t F1027;
	uint32_t F1028;
	uint32_t F1029;
	uint32_t F1030;
	uint32_t F1031;
	uint32_t F1032;
	uint32_t F1033;
	uint32_t F1034;
	uint32_t F1035;
	uint32_t F1036;
	uint32_t F1037;
	uint32_t F1038;
	uint32_t F1039;
	uint32_t F1040;
	uint32_t F1041;
	uint32_t F1042;
	uint32_t F1043;
	uint32_t F1044;
	uint32_t F1045;
	uint32_t F1046;
	uint32_t F1047;
	uint32_t F1048;
	uint32_t F1049;

}FuncCodeSingleF10;

typedef struct FuncCodeSingleF50
{
	uint32_t F5000;				//DI1 input selection
	uint32_t F5001;				//DI2 input selection
	uint32_t F5002;				//DI3 input selection
	uint32_t F5003;				//DI4 input selection
	uint32_t F5004;				//DI5 input selection
	uint32_t F5005;				//DI6 input selection
	uint32_t F5006;				//DI7 input selection
	uint32_t F5007;				//DI8 input selection
	uint32_t F5008;				//DI9 input selection
	uint32_t F5009;				//DI10 input selection
	uint32_t F5010;				//DO1 output selection
	uint32_t F5011;				//DO2 output selection
	uint32_t F5012;				//DO3 output selection
	uint32_t F5013;				//DO4 output selection
	uint32_t F5014;				//DO5 output selection
	uint32_t F5015;				//DO6 output selection
	uint32_t F5016;				//Filter time constant of AI1
	uint32_t F5017;				//Low Limit Voltage of AI1
	uint32_t F5018;				//Correspond ratio of Low Limit Voltage of AI1
	uint32_t F5019;				//High Limit Voltage of AI1
	uint32_t F5020;				//Correspond ratio of High Limit Voltage of AI1
	uint32_t F5021;				//Filter time constant of AI2
	uint32_t F5022;				//Low Limit Voltage of AI2
	uint32_t F5023;				//Correspond ratio of Low Limit Voltage of AI2
	uint32_t F5024;				//High Limit Voltage of AI2
	uint32_t F5025;				//Correspond ratio of High Limit Voltage of AI2
	uint32_t F5026;				//Filter time constant of AI3
	uint32_t F5027;				//Low Limit Voltage of AI3
	uint32_t F5028;				//Correspond ratio of Low Limit Voltage of AI3
	uint32_t F5029;				//High Limit Voltage of AI3
	uint32_t F5030;				//Correspond ratio of High Limit Voltage of AI3
	uint32_t F5031;				//Mode selection of analog output
	uint32_t F5032;				//Output function selection of AO1
	uint32_t F5033;				//Low Limit Voltage of AO1
	uint32_t F5034;				//Correspond ratio of Low Limit Voltage of AO1
	uint32_t F5035;				//High Limit Voltage of AO1
	uint32_t F5036;				//Correspond ratio of High Limit Voltage of AO1
	uint32_t F5037;				//Output function selection of AO2
	uint32_t F5038;				//Low Limit Voltage of AO2
	uint32_t F5039;				//Correspond ratio of Low Limit Voltage of AO2
	uint32_t F5040;				//High Limit Voltage of AO2
	uint32_t F5041;				//Correspond ratio of High Limit Voltage of AO2
	uint32_t F5042;				//Range of positioning complete(In-position)
	uint32_t F5043;				//Hold time of INP
	uint32_t F5044;				//Zero-speed value range
	uint32_t F5045;				//Speed coincidence range
	uint32_t F5046;				//At-speed(speed arrival)
	uint32_t F5047;				//Selection-1 of alarm output
	uint32_t F5048;				//Selection-2 of alarm output
	uint32_t F5049;
	uint32_t F5050;				//High Limit Voltage of AO2
	uint32_t F5051;				//Correspond ratio of High Limit Voltage of AO2
	uint32_t F5052;				//Range of positioning complete(In-position)
	uint32_t F5053;				//Hold time of INP
	uint32_t F5054;				//Zero-speed value range
	uint32_t F5055;				//Speed coincidence range
	uint32_t F5056;				//At-speed(speed arrival)
	uint32_t F5057;				//Selection-1 of alarm output
	uint32_t F5058;				//Selection-2 of alarm output
	uint32_t F5059;


}FuncCodeSingleF50;

typedef struct FuncCodeSingleF60
{
	uint32_t F6000;				//2nd numerator of electronic gear
	uint32_t F6001;				//3rd numerator of electronic gear
	uint32_t F6002;				//4th numerator of electronic gear
	uint32_t F6003;				//Denominator of pulse output division
	uint32_t F6004;				//Selection of torque limit
	uint32_t F6005;				//2nd torque limit
	uint32_t F6006;				//Torque limit switching setup
	uint32_t F6007;				//Positive direction torque limit of external input
	uint32_t F6008;				//Negative direction torque limit of external input
	uint32_t F6009;				//Input gain of analog torque limit
	uint32_t F6010;				//High/Low Input Pulse setup
	uint32_t F6011;				//Sequence at Servo-off
	uint32_t F6012;				//Sequence at alarm
	uint32_t F6013;				//Torque setup for emergency stop
	uint32_t F6014;				//Over-speed level setup
	uint32_t F6015;				//LED initial status
	uint32_t F6016;				//Origin homing mode
	uint32_t F6017;				//Speed value of JOG
	uint32_t F6018;				//Display lock of operation panel
	uint32_t F6019;				//485 communication address
	uint32_t F6020;				//485/232 communication baud rate setup
	uint32_t F6021;				//485/232 communication parity bit
	uint32_t F6022;				//485/232 communication stop bit
	uint32_t F6023;				//Reserved
	uint32_t F6024;				//Reserved
	uint32_t F6025;				//Reserved
	uint32_t F6026;				//Reserved
	uint32_t F6027;				//Reserved
	uint32_t F6028;				//Reserved
	uint32_t F6029;				//Reserved
	uint32_t F6030;				//Reserved
	uint32_t F6031;				//Reserved
	uint32_t F6032;				//Reserved
	uint32_t F6033;				//Reserved
	uint32_t F6034;				//Reserved
	uint32_t F6035;				//Reserved
	uint32_t F6036;				//Reserved
	uint32_t F6037;				//Reserved
	uint32_t F6038;				//Reserved
	uint32_t F6039;				//Reserved

}FuncCodeSingleF60;

typedef struct FuncCodeSingleFA0	//Hide function code
{
	uint32_t FA000;
	uint32_t FA001;
	uint32_t FA002;
	uint32_t FA003;
	uint32_t FA004;
	uint32_t FA005;
	uint32_t FA006;
	uint32_t FA007;
	uint32_t FA008;
	uint32_t FA009;
	uint32_t FA010;
	uint32_t FA011;
	uint32_t FA012;
	uint32_t FA013;
	uint32_t FA014;
	uint32_t FA015;
	uint32_t FA016;
	uint32_t FA017;
	uint32_t FA018;
	uint32_t FA019;
	uint32_t FA020;
	uint32_t FA021;
	uint32_t FA022;
	uint32_t FA023;
	uint32_t FA024;
	uint32_t FA025;
	uint32_t FA026;
	uint32_t FA027;
	uint32_t FA028;
	uint32_t FA029;
	uint32_t FA030;
	uint32_t FA031;
	uint32_t FA032;
	uint32_t FA033;
	uint32_t FA034;
	uint32_t FA035;
	uint32_t FA036;
	uint32_t FA037;
	uint32_t FA038;
	uint32_t FA039;
	uint32_t FA040;
	uint32_t FA041;
	uint32_t FA042;
	uint32_t FA043;
	uint32_t FA044;
	uint32_t FA045;
	uint32_t FA046;
	uint32_t FA047;
	uint32_t FA048;
	uint32_t FA049;
	uint32_t FA050;
	uint32_t FA051;
	uint32_t FA052;
	uint32_t FA053;
	uint32_t FA054;
	uint32_t FA055;
	uint32_t FA056;
	uint32_t FA057;
	uint32_t FA058;
	uint32_t FA059;
	uint32_t FA060;
	uint32_t FA061;
	uint32_t FA062;
	uint32_t FA063;
	uint32_t FA064;
	uint32_t FA065;
	uint32_t FA066;
	uint32_t FA067;
	uint32_t FA068;
	uint32_t FA069;
	uint32_t FA070;
	uint32_t FA071;
	uint32_t FA072;
	uint32_t FA073;
	uint32_t FA074;
	uint32_t FA075;
	uint32_t FA076;
	uint32_t FA077;
	uint32_t FA078;
	uint32_t FA079;
	uint32_t FA080;
	uint32_t FA081;
	uint32_t FA082;
	uint32_t FA083;
	uint32_t FA084;
	uint32_t FA085;
	uint32_t FA086;
	uint32_t FA087;
	uint32_t FA088;
	uint32_t FA089;
	uint32_t FA090;
	uint32_t FA091;
	uint32_t FA092;
	uint32_t FA093;
	uint32_t FA094;
	uint32_t FA095;
	uint32_t FA096;
	uint32_t FA097;
	uint32_t FA098;
	uint32_t FA099;
}FuncCodeSingleFA0;

/**Function Code Group Struct define**/
typedef struct FuncCodeGroup
{
	FuncCodeSingleF00 F00;
	FuncCodeSingleF10 F10;
	FuncCodeSingleF50 F50;
	FuncCodeSingleF60 F60;
	FuncCodeSingleFA0 FA0;
}FuncCodeGroup;

/**All Function Code define**/
typedef struct
{
	FuncCodeGroup Init;
	FuncCodeGroup Ram;
	FuncCodeGroup E2prom;
	FuncCodeGroup Min;
	FuncCodeGroup Max;
	FuncCodeGroup Att;
}FuncCode;

//#define	FuncCode_Base	 0x20001000UL
//#define	FuncCode_Handle 	(FuncCode *)FuncCode_Base

struct Fcode_FuncTbl
{
	uint16_t value;
	uint16_t (*func)(uint16_t, uint32_t, FuncCode*);
};
#endif /* PERI_FUNCODESTRUCT_H_ */
