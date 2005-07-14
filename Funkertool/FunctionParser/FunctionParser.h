//===============================================================================
//
//	Name:
//		- -	FunctionParser.h
//
//	Type:
//		- -	Header f�r Umwandlung eines String in eine Function
//
//	Zweck:
//		- -	Klassen f�r dynamische Script-Funktionen
//
//	Remarks:
//		- - String muss Nullterminiert sein!
//
//	Erstellt von:
//		- - da_collega aka. Andres N�tzli
//
//	Erstellt am:
//		- - 15.06.2005
//
//	�nderungen:
//		- - Blue Cobold aka Sebastian Kohl
//		  - - 03.07.2005
//		  - - Umbenannt in FunctionParser
//
//	Erstellt f�r:
//		- - "MellowZ"
//
//===============================================================================

#include "../ScriptFunctions/ScriptFunctions.h"
#include <iostream>

struct Command
{
	char* String;
	ScriptFunction::ScriptFunctionType SFT;
	int Parameters;

	Command() {};

	Command(char* Str, ScriptFunction::ScriptFunctionType sft, int cntParameter)
	{
		String = Str;
		SFT = sft;
		Parameters = cntParameter;
	}
};

class FunctionParser
{
private:
	char* String;
	int CurrentPosition;
	Command CommandList[16];

	ScriptFunction* Expression();
	ScriptFunction* Term();
	ScriptFunction* Pows();
	ScriptFunction* Factor();
	ScriptFunction* Number();
	ScriptFunction* Digits();
	ScriptFunction* Function();
	double Digit();
	bool isNext(char* command);

public:
	FunctionParser();
	ScriptFunction* Parse(const char* StringToParse);
	~FunctionParser();
};
