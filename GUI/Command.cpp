#include "stdafx.h"
#include "Commands.h"

char *CommandGroups[GROUPS_CNT]=
{
	_T("Main Driver Parameters"),
	_T("Additional Driver Parameters"),
	_T("Current Control Parameters"),
  _T("Position Correction & Dumping"),
	_T("Detect Step Out Parameters")
};

char *MotionDirVal[]=
{
	"CW",
	"CCW",
	NULL
};

char *PulseModeVal[]=
{
	"Pulse-Dir",
  "CW-CCW",
	NULL
};

char *StepOutDetectionVal[]=
{
	"Disable",
  "Enable",
	NULL
};

char *PolarityRunSignalVal[]= 
{
	"Normal",
	"Inverse",
	NULL
};

char *PolarityAlarmSignalVal[]= 
{
	"Normal",
	"Inverse",
	NULL
};

char *PolarityResetSignalVal[]= 
{
	"Normal",
	"Inverse",
	NULL
};

BOARD_CMMND Cmmnd_1_1[]= 
{
	{0,			0,			"Resolution"},	
	{1,			0,			"Stop Current [%]"},
	{2,			0,			"Motion Direction",									MotionDirVal},
	{3,			0,			"Pulse Mode",												PulseModeVal},
	{4,			0,			"Step-Out Detection",								StepOutDetectionVal},
	{5,			0,			"Polarity Run Signal",							PolarityRunSignalVal},
	{6,			0,			"Polarity Alarm Signal",						PolarityAlarmSignalVal},
	{7,			0,			"Polarity Alarm Reset Signal",			PolarityResetSignalVal},
	{20,		0,			"Motor Type [No]"},
	{0xFF,	0,      NULL} 
};

BOARD_CMMND Cmmnd_1_2[]= 
{
	{8,			0,			"Voltage Measure Filter Time Constant"},
	{9,			0,			"Motor High Voltage Alarm On [V]"},
	{10,		0,			"Motor Voltage Resistor On [V]"},
	{11,		0,			"Motor Low Voltage Alarm On [V]"},
	{12,		0,			"Motor Low Voltage Switches Off [V]"},
	{13,		0,			"Motor Low Voltage Alarm On Delay [mS]"},
	{14,		0,			"Current Control Delta Current [%]"},
	{15,		0,			"Current Control Delay [mS]"},
	{65,		0,      "DC/DC Mode [0-Off; 1-On; 2-Control]"},
	{16,		0,			"DC/DC Off Time Delay [mS]"},
	{22,		0,			"DC/DC On Frequency [Hz]"},
	{17,		0,			"Start Delay Constant [mS]"},
	{18,		0,			"Hold Max Current Delay Constant [mS]"},
	{19,		0,			"Communication Baud Rate (5 - 9600 bps)"},
	{48,		0,			"Power Supply Voltage [V] (format xx.x)"},
	{0xFF,	0,      NULL} 
};

BOARD_CMMND Cmmnd_2_1[]= 
{
	{61,		0,			"Current Control Mode (0-FF&FB, 1-FF Only, 2-FB Only)"},
	{23,		0,			"I [mA] Motor Coils Current (Reference Current)"},
	{24,		0,			"R [mOhm] Coil and External Wires Resistance"},
	{25,		0,      "L [uH] Coil Inductance "},
	{26,		0,			"Ce=U/  [mV/RPS] Back EMF Constant"},
	{27,		0,			"Feed-Back Proportional Gain Min"},
	{59,		0,			"Feed-Back Proportional Gain Limit"},
	{60,		0,			"Feed-Back Proportional Gain Max 2500Hz"},
	{49,		0,			"Actual Feed-Back Proportional Gain"},
	{28,		0,			"Feed-Back Integrated Gain"},
	{0xFF,	0,      NULL} 
};
/*
BOARD_CMMND Cmmnd_2_1[]= 
{
	{61,		0,			"Current Control Mode [0-FF&FB, 1-FF Only, 2-FB Only]"},
	{23,		0,			"I [mA] Motor Coils Current (Reference Current)"},
	{24,		0,			"R [mOhm] Coil and External Wires Resistance (Feed-Forward parameter)"},
	{25,		0,      "L [uH] Coil Inductance (Feed-Forward parameter)"},
	{26,		0,			"Ce=U/  [mV/RPS] Back EMF Constant (Feed-Forward parameter)"},
	{27,		0,			"Feed-Back Proportional Gain Min (Feed-Back parameter)"},
	{59,		0,			"Feed-Back Proportional Gain Limit (Feed-Back parameter)"},
	{60,		0,			"Feed-Back Proportional Gain Max 2500Hz (Feed-Back parameter)"},
	{49,		0,			"Actual Feed-Back Proportional Gain (Feed-Back parameter)"},
	{28,		0,			"Feed-Back Integrated Gain (Feed-Back parameter)"},
	{0xFF,			0,      NULL} 
};
*/























/*


	{8,			0,			""},
	{9,			0,			""},
	{1,			0,			""},
	{1,			0,			""},
	{1,			0,			""},
	{1,			0,			""},
	{1,			0,			""},
	{1,			0,			""},
	{16,		0,			""},
	{22,		0,			""},
	{17,		0,			""},
	{18,		0,			""},
	{19,		0,			""},
	{48,		0,			""},

*/







