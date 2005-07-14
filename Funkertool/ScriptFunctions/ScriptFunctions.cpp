//===============================================================================
//
//	Name:
//		- -	ScriptFunctions.cpp
//
//	Type:
//		- -	Utility-Quellcode
//
//	Zweck:
//		- -	Klassen für dynamische Script-Funktionen
//
//	Remarks:
//		- - All trigonometric functions or otherwise likely alternating
//			ones are calculated in gon. This will mean a range of 0..400
//			not of 0..360 or 0..2PI. Be carefull with that! All
//			these alternating functions will repeat at 400. Also they
//			will have a range from -1 to 1 in their evaluted value.
//
//	Erstellt von:
//		- - Blue Cobold
//
//	Erstellt am:
//		- - 15.06.2005
//
//	Erstellt für:
//		- - "MellowZ"
//
//===============================================================================

#include "ScriptFunctions.h"
#include <cmath>

const	float			SFC_MAIN_CODE_PI			= 3.141592653f;
const	float			SFC_MAIN_CODE_E				= 2.718281828f;
const	float			SFC_MAIN_CODE_2_GON			= SFC_MAIN_CODE_PI / 200.f;

ScriptFunctionStorage* ScriptFunctionStorage::hInstance = 0;

//===============================================================================
//
//===============================================================================

ScriptFunctionStorage::ScriptFunctionStorage( )
{
	for ( int i=0; i<MaxSFStorage; i++)
		this->fMemory[i] = 0;
	this->hSub	= 0;
}

//===============================================================================

ScriptFunctionStorage*	ScriptFunctionStorage::GetInstance( void )
{
	if ( !ScriptFunctionStorage::hInstance )
		ScriptFunctionStorage::hInstance = new ScriptFunctionStorage;
	return ScriptFunctionStorage::hInstance;
}

//===============================================================================

float	ScriptFunctionStorage::Memory( int Address )
{
	if ( (Address<0) || (Address>=MaxSFStorage) )
		return 0;
	return this->fMemory[Address];
}

//===============================================================================

void	ScriptFunctionStorage::Memory( float value, int Address )
{
	if ( (Address<0) || (Address>=MaxSFStorage) )
		return;
	this->fMemory[Address] = value;
}

//===============================================================================

void	ScriptFunctionStorage::Sub( ScriptFunction* Sub )
{
	this->hSub	= Sub;
}

//===============================================================================

ScriptFunction*	ScriptFunctionStorage::Sub( )
{
	return this->hSub;
}

//===============================================================================
//
//===============================================================================
ScriptFunction::ScriptFunction()
{
}

ScriptFunction::ScriptFunction( ScriptFunctionType sft )
{
	this->sftType	= sft;
}

//===============================================================================

ScriptFunction*	ScriptFunction::Factory( ScriptFunctionType sft )
{
	ScriptFunction* sf = 0;
	switch (sft)
	{
		case SFTConstantValue:
		case SFTConstantByte:
			sf	= new SFConstant( sft );
			break;

		case SFTPassedValue:
		case SFTPI:
		case SFTE:
		case SFTRandom:
			sf	= new ScriptFunction( sft );
			break;

		case SFTAdd:
		case SFTSub:
		case SFTMul:
		case SFTDiv:
		case SFTPow:
		case SFTNoise:
		case SFTStore:
		case SFTMin:
		case SFTMax:
		case SFTMod:
		case SFTNOP:
			sf	= new SFBinary( sft );
			break;

		case SFTSine:
		case SFTCosine:
		case SFTRect:
		case SFTRamp:
		case SFTRampUp:
		case SFTRampDown:
		case SFTSignum:
		case SFTAbs:
		case SFTLoad:
		case SFTDefSub:
		case SFTGoSub:
			sf	= new SFUnary( sft );
			break;
	}
	return sf;
}

//===============================================================================

ScriptFunction::ScriptFunctionType	ScriptFunction::Type( void )
{
	return this->sftType;
}

//===============================================================================

float	ScriptFunction::Evaluate( const float& x )
{
	switch (this->sftType)
	{
		case SFTPassedValue:
			return x;

		case SFTPI:
			return SFC_MAIN_CODE_PI;

		case SFTE:
			return SFC_MAIN_CODE_E;

		case SFTRandom:
			return (m_rand()/16384.f)-1;

	}
	return 0;
}

//===============================================================================

void	ScriptFunction::Stream( std::ostream& out )
{
	char csft = static_cast<char>(this->sftType);
	out.write( reinterpret_cast<char*>(&csft), sizeof(csft) );
}

//===============================================================================

void	ScriptFunction::StreamIn( std::istream& in )
{
}

//===============================================================================

ScriptFunction*	ScriptFunction::Stream( std::istream& in )
{
	char csft;
	in.read( reinterpret_cast<char*>(&csft), sizeof(csft) );
	ScriptFunction* sf = ScriptFunction::Factory( static_cast<ScriptFunctionType>(csft) );
	if ( sf )
		sf->StreamIn( in );
	return sf;
}

//===============================================================================
//
//===============================================================================

SFConstant::SFConstant( ScriptFunctionType sft )
{
	this->sftType = sft;
}

//===============================================================================

float	SFConstant::Value( void )
{
	return this->fValue;
}

//===============================================================================

void	SFConstant::Value( float value )
{
	this->fValue = value;
}

//===============================================================================

float	SFConstant::Evaluate( const float& x )
{
	return this->fValue;
}

//===============================================================================

void	SFConstant::Stream( std::ostream& out )
{
	// use only 24 bits. The Sign is not needed, because the interpreter
	// for the functions does no allow a negativ value.
	// To high values are also denied.
	unsigned long export = *reinterpret_cast<unsigned long*>(&this->fValue);
	export <<= 1;
	this->ScriptFunction::Stream( out );
	out.write( reinterpret_cast<char*>(&export), 3 );
}

//===============================================================================

void	SFConstant::StreamIn( std::istream& in )
{
	this->fValue = 0;
	in.read( reinterpret_cast<char*>(&this->fValue), 3 );
	(*reinterpret_cast<unsigned long*>(&this->fValue)) >>= 1;
}

//===============================================================================
//
//===============================================================================

SFUnary::SFUnary( ScriptFunctionType sft )
{
	this->sftType		= sft;
	this->lpParameter	= 0;
}
SFUnary::SFUnary()
{
	this->lpParameter	= 0;
}
SFUnary::~SFUnary( )
{
	if ( this->lpParameter )
	{
		delete[] this->lpParameter;
		this->lpParameter	= 0;
	}
}

//===============================================================================

void	SFUnary::Parameter( ScriptFunction* Param )
{
	this->lpParameter = Param;
}

//===============================================================================

ScriptFunction*	SFUnary::Parameter( void )
{
	return this->lpParameter;
}

//===============================================================================

float	SFUnary::Evaluate( const float& x )
{
	float x2 = this->lpParameter->Evaluate( x );
	float t;
	switch ( this->sftType )
	{
		case SFTSine:
			return sinf( SFC_MAIN_CODE_2_GON * x2 );

		case SFTCosine:
			return cosf( SFC_MAIN_CODE_2_GON * x2 );

		case SFTRect:
			if ( x2 < 200 )
				return 1;
			else
				return -1;

		case SFTRamp:
			t = MODF(fabsf(x2)+100,400);
			if ( t < 200 )
				return SIGN(x)*(t/100.f-1);
			else
				return SIGN(x)*(3-t/100.f);

		case SFTRampUp:
			t = MODF(fabsf(x2),400);
			return SIGN(x2)*(t/200-1);

		case SFTRampDown:
			t = MODF(fabsf(x2),400);
			return SIGN(x2)*(1-t/200);

		case SFTSignum:
			return SIGN(x2);

		case SFTAbs:
			return fabsf(x2);

		case SFTLoad:
			return ScriptFunctionStorage::GetInstance()->Memory( static_cast<int>(x2) );

		case SFTGoSub:
			{
				ScriptFunction* sf = ScriptFunctionStorage::GetInstance()->Sub();
				if ( sf )
					return sf->Evaluate( x2 );
				else
					return x2;
			}
	}
	return 0;
}

//===============================================================================

void	SFUnary::Stream( std::ostream& out )
{
	this->ScriptFunction::Stream( out );
	this->lpParameter->Stream( out );
}

//===============================================================================

void	SFUnary::StreamIn( std::istream& in )
{
	this->lpParameter = ScriptFunction::Stream( in );
}

//===============================================================================
//
//===============================================================================

SFBinary::SFBinary( ScriptFunctionType sft )
{
	this->sftType = sft;
}

SFBinary::~SFBinary( )
{
	if ( this->lpParameter[0] )
	{
		delete this->lpParameter[0];
		this->lpParameter[0] = 0;
	}
	if ( this->lpParameter[1] )
	{
		delete this->lpParameter[1];
		this->lpParameter[1] = 0;
	}
}
//===============================================================================

void	SFBinary::Parameter( ScriptFunction* Param, int Nr )
{
	if ( (Nr<0) || (Nr>1) )
		return;
	this->lpParameter[Nr] = Param;
}

//===============================================================================

ScriptFunction*	SFBinary::Parameter( int Nr )
{
	if ( (Nr<0) || (Nr>1) )
		return 0;
	return this->lpParameter[Nr];
}

//===============================================================================

float	SFBinary::Evaluate( const float& x )
{
	float x1, x2;
	if ( this->sftType != SFTDefSub )
	{
		x1 = this->lpParameter[0]->Evaluate( x );
		x2 = this->lpParameter[1]->Evaluate( x );
	}
	switch ( this->sftType )
	{
		case SFTAdd:
			return x1+x2;

		case SFTSub:
			return x1-x2;

		case SFTMul:
			return x1*x2;

		case SFTDiv:
			if ( x2!=0 )
				return x1/x2;
			else
				return 1000000000;

		case SFTPow:
			return powf( x1, x2 );

		case SFTNoise:
			return PerlinNoise1DBy4D( x1, 0, 0, 0, static_cast<int>(x2), 100000000 );

		case SFTStore:
			ScriptFunctionStorage::GetInstance()->Memory( x1, static_cast<int>(x2) );
			return x1;

		case SFTMin:
			return MIN( x1, x2 );

		case SFTMax:
			return MAX( x1, x2 );

		case SFTMod:
			return MOD( x1, x2 );

		case SFTDefSub:
			{
				ScriptFunction* sf	= ScriptFunctionStorage::GetInstance()->Sub( );
				ScriptFunctionStorage::GetInstance()->Sub( this->lpParameter[0] );
				x2 = this->lpParameter[1]->Evaluate( x );
				ScriptFunctionStorage::GetInstance()->Sub( sf );
			}
			return x2;

		case SFTNOP:
			return x2;
	}
	return 0;
}

//===============================================================================

void	SFBinary::Stream( std::ostream& out )
{
	this->ScriptFunction::Stream( out );
	this->lpParameter[0]->Stream( out );
	this->lpParameter[1]->Stream( out );
}

//===============================================================================

void	SFBinary::StreamIn( std::istream& in )
{
	this->lpParameter[0] = ScriptFunction::Stream( in );
	this->lpParameter[1] = ScriptFunction::Stream( in );
}
