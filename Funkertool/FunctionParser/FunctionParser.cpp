#include "FunctionParser.h"

FunctionParser::FunctionParser()
{
	CommandList[0] = Command("sin(",	ScriptFunction::SFTSine, 1);
	CommandList[1] = Command("cos(",	ScriptFunction::SFTCosine, 1);
	CommandList[2] = Command("min(",	ScriptFunction::SFTMin, 2);
	CommandList[3] = Command("max(",	ScriptFunction::SFTMax, 2);
	CommandList[4] = Command("rect(",	ScriptFunction::SFTRect, 1);
	CommandList[5] = Command("ramp(",	ScriptFunction::SFTRamp, 1);
	CommandList[6] = Command("rampup(", ScriptFunction::SFTRampUp, 1);
	CommandList[7] = Command("rampdown(", ScriptFunction::SFTRampDown, 1);
	CommandList[8] = Command("signum(", ScriptFunction::SFTSignum, 1);
	CommandList[9] = Command("abs(",	ScriptFunction::SFTAbs, 1);
	CommandList[10] = Command("rnd",	ScriptFunction::SFTRandom, 0);
	CommandList[11] = Command("noise(", ScriptFunction::SFTNoise, 2);
	CommandList[12] = Command("load(",	ScriptFunction::SFTLoad, 1);
	CommandList[13] = Command("store(", ScriptFunction::SFTStore, 2);
	CommandList[14] = Command("defsub(", ScriptFunction::SFTDefSub, 2);
	CommandList[15] = Command("gosub(",	ScriptFunction::SFTGoSub, 1);
}

//===============================================================================
// Parse a string
//===============================================================================
ScriptFunction* FunctionParser::Parse(const char* StringToParse)
{
	CurrentPosition = 0;
	char tmp = 0;

	int length = strlen(StringToParse);
	String = new char[length+1];

	for(int i = 0, j=0; i < length ; i++)
    {
		if(StringToParse[i] != ' ')
		{
			tmp = StringToParse[i];
			if(tmp >= 'A' && tmp <= 'Z')
				tmp += 'a' - 'A';
			String[j] = tmp;
			j++;
		}
	}
	String[j] = 0;

	ScriptFunction* result = Function();

	return result;
}

//===============================================================================
// Parse a function
//===============================================================================
ScriptFunction* FunctionParser::Function()
{
	SFBinary* result;
	SFBinary* tmp = 0;

	result = static_cast<SFBinary*>(Expression());

	while(1)
	{
		if(String[CurrentPosition] == '_')
		{
			CurrentPosition++;
			tmp = new SFBinary(ScriptFunction::SFTNOP);
			tmp->Parameter(static_cast<ScriptFunction*>(result), 0);
			tmp->Parameter(Expression(), 1);
		}
		else
		{
			break;
		}
		result = tmp;
	}

	return result;
}

//===============================================================================
// Parses an expression
//===============================================================================
ScriptFunction* FunctionParser::Expression()
{
	SFBinary* result;
	SFBinary* tmp = 0;
	SFConstant* Zero = 0;

	switch(String[CurrentPosition])
	{
	case('-'):
			CurrentPosition++;
			tmp = static_cast<SFBinary*>(Term());
			Zero = new SFConstant(ScriptFunction::SFTConstantValue);
			Zero->Value(0);
			result = new SFBinary(ScriptFunction::SFTSub);
			result->Parameter(Zero, 0);
			result->Parameter(tmp, 1);
			break;
	case('+'):
			CurrentPosition++;
			result = static_cast<SFBinary*>(Term());
			break;
	default:
			result = static_cast<SFBinary*>(Term());
			break;
	}

	while(1)
	{
		if(String[CurrentPosition] == '-')
		{
			CurrentPosition++;
			tmp = new SFBinary(ScriptFunction::SFTSub);
			tmp->Parameter(static_cast<ScriptFunction*>(result), 0);
			tmp->Parameter(Term(), 1);
		}
		else if(String[CurrentPosition] == '+')
		{
			CurrentPosition++;
			tmp = new SFBinary(ScriptFunction::SFTAdd);
			tmp->Parameter(static_cast<ScriptFunction*>(result), 0);
			tmp->Parameter(Term(), 1);
		}
		else
		{
			break;
		}
		result = tmp;
	}

	return result;
}

//===============================================================================
// Parses a term
//===============================================================================
ScriptFunction* FunctionParser::Term()
{
	SFBinary* result = static_cast<SFBinary*>(Pows());
	SFBinary* tmp = 0;

	while(1)
	{
		if(String[CurrentPosition] == '/')
		{
			CurrentPosition++;
			tmp = new SFBinary(ScriptFunction::SFTDiv);
			tmp->Parameter(result, 0);
			tmp->Parameter(Pows(), 1);
		}
		else if(String[CurrentPosition] == '*')
		{
			CurrentPosition++;
			tmp = new SFBinary(ScriptFunction::SFTMul);
			tmp->Parameter(result, 0);
			tmp->Parameter(Pows(), 1);
		}
		else if(String[CurrentPosition] == '%')
		{
			CurrentPosition++;
			tmp = new SFBinary(ScriptFunction::SFTMod);
			tmp->Parameter(result, 0);
			tmp->Parameter(Pows(), 1);
		}
		else
		{
			break;
		}
		result = tmp;
	}

	return result;
}

//===============================================================================
// Parses pows
//===============================================================================
ScriptFunction* FunctionParser::Pows()
{
	SFBinary* result = static_cast<SFBinary*>(Factor());
	SFBinary* tmp = 0;

	while(1)
	{
		if(String[CurrentPosition] == '^')
		{
			CurrentPosition++;
			tmp = new SFBinary(ScriptFunction::SFTPow);
			tmp->Parameter(result, 0);
			tmp->Parameter(Factor(), 1);
		}
		else
		{
			break;
		}
		result = tmp;
	}

	return result;
}

//===============================================================================
// Parses factors
//===============================================================================
ScriptFunction* FunctionParser::Factor()
{
	ScriptFunction* result;
	ScriptFunction* tmp = 0;

	for(int i = 0; i < sizeof(CommandList) / sizeof(Command); i++)
	{
		if(isNext(CommandList[i].String))
		{
			if(CommandList[i].Parameters == 1)
			{
				CurrentPosition += strlen(CommandList[i].String);
				result = new SFUnary(CommandList[i].SFT);
				static_cast<SFUnary*>(result)->Parameter(Function());
				CurrentPosition++;
				return result;
			}
			else if(CommandList[i].Parameters == 2)
			{
				CurrentPosition += strlen(CommandList[i].String);
				tmp = Function();
				CurrentPosition++;
				result = new SFBinary(CommandList[i].SFT);
				static_cast<SFBinary*>(result)->Parameter(tmp, 0);
				static_cast<SFBinary*>(result)->Parameter(Function(), 1);
				CurrentPosition++;
				return result;
			}
			else
			{
				CurrentPosition += strlen(CommandList[i].String);
				result = new ScriptFunction(CommandList[i].SFT);
				return result;
			}
		}
	}

	if(String[CurrentPosition] == '(')
	{
		CurrentPosition++;
		result = Expression();
		CurrentPosition++;
	}
	else
	{
		result = Number();
	}
	return result;
}

//===============================================================================
// Parses numbers
//===============================================================================
ScriptFunction* FunctionParser::Number()
{
	SFBinary* resultScript;
	ScriptFunction* tmp = 0;
	SFConstant* Zero = 0;

	switch(String[CurrentPosition])
	{
	case('-'):
			CurrentPosition++;
			tmp = Digits();
			Zero = new SFConstant(ScriptFunction::SFTConstantValue);
			Zero->Value(0);
			resultScript = new SFBinary(ScriptFunction::SFTSub);
			resultScript->Parameter(Zero, 0);
			resultScript->Parameter(tmp, 1);
			break;
	case('+'):
			CurrentPosition++;
			resultScript = static_cast<SFBinary*>(Digits());
			break;
	default:
			resultScript = static_cast<SFBinary*>(Digits());
			break;
	}

	return resultScript;
}

//===============================================================================
// Parses digits and constants like PI
//===============================================================================
ScriptFunction* FunctionParser::Digits()
{
	float number, count;
	number = 0;
	count = 1;

	if(String[CurrentPosition] == 'x')
	{
		CurrentPosition++;
		return new ScriptFunction(ScriptFunction::SFTPassedValue);
	}
	else if(String[CurrentPosition] == 'e')
	{
		CurrentPosition++;
		return new ScriptFunction(ScriptFunction::SFTE);
	}
	else if(String[CurrentPosition] == 'p' && String[CurrentPosition + 1] == 'i')
	{
		CurrentPosition += 2;
		return new ScriptFunction(ScriptFunction::SFTPI);
	}

	SFConstant* result;

	while(isdigit(String[CurrentPosition]))
	{
		number = number * 10 + Digit();
		CurrentPosition++;
	}

	if(String[CurrentPosition] == '.')
	{
		CurrentPosition++;
		while(isdigit(String[CurrentPosition]))
		{
			count *= 10;
			number = number + (Digit() / count);
			CurrentPosition++;
		}
	}

	if(number / 2.0f == static_cast<int>(number / 2.0f) && number > 0 && number < 257)
	{
		result = new SFConstant(ScriptFunction::SFTConstantByte);
		result->Value(number);
	}
	else
	{
		result = new SFConstant(ScriptFunction::SFTConstantValue);
		result->Value(number);
	}

	return result;
}

//===============================================================================
// Makes a ASCII Digit to a number
//===============================================================================
double FunctionParser::Digit()
{
	return String[CurrentPosition] - '0';
}

//===============================================================================
// Checks, if the next command is
//===============================================================================
bool FunctionParser::isNext(char* command)
{
	int counter = 0;
	bool result = true;

	while(command[counter] != '\0')
	{
		if(command[counter] != String[CurrentPosition + counter])
		{
			result = false;
			break;
		}
		counter++;
	}
	return result;
}

FunctionParser::~FunctionParser()
{
}