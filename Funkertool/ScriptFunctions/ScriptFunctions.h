//===============================================================================
//
//	Name:
//		- -	ScriptFunctions.h
//
//	Type:
//		- -	Header für Utility-Quellcode
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

#ifndef	SCRIPT_FCUNTIONS_H
#define	SCRIPT_FCUNTIONS_H

#include	<iostream>
#include	"3DMath.h"

//===============================================================================
// The Memory-functions are special functions not really calculating
// anything, but loading from or storing values to the global
// Script-Function-Storage-Singleton.
// This can be used for saving values like variables
// SFStoreMemory stores the given value in the global Singleton-Storrage
// SFLoadMemory loads a value from the global Singleton-Storrage
// The passed parameter is ignored by the function like a
// constant function also behaves

//===============================================================================

class	ScriptFunction
{
public:

	//=====================================

	enum	ScriptFunctionType
	{
		SFTConstantValue	=	 0,		// ->	SFConstant
		SFTPassedValue		=	 1,		// ->	ScriptFunction
		SFTPI				=	 2,		// ->	ScriptFunction
		SFTE				=	 3,		// ->	ScriptFunction
		SFTAdd				=	 4,		// ->	SFBinary
		SFTSub				=	 5,		// ->	SFBinary
		SFTMul				=	 6,		// ->	SFBinary
		SFTDiv				=	 7,		// ->	SFBinary
		SFTPow				=	 8,		// ->	SFBinary
		SFTSine				=	 9,		// ->	SFUnary
		SFTCosine			=	10,		// ->	SFUnary
		SFTRect				=	11,		// ->	SFUnary
		SFTRamp				=	12,		// ->	SFUnary
		SFTRampUp			=	13,		// ->	SFUnary
		SFTRampDown			=	14,		// ->	SFUnary
		SFTSignum			=	15,		// ->	SFUnary
		SFTAbs				=	16,		// ->	SFUnary
		SFTRandom			=	17,		// ->	ScriptFunction
		SFTNoise			=	18,		// ->	SFBinary
		SFTLoad				=	19,		// ->	SFUnary
		SFTStore			=	20,		// ->	SFBinary
		SFTMin				=	21,		// ->	SFBinary
		SFTMax				=	22,		// ->	SFBinary
		SFTMod				=	23,		// ->	SFBinary
		SFTDefSub			=	24,		// ->	SFBinary
		SFTGoSub			=	25,		// ->	SFUnary
		SFTNOP				=	26,		// ->	SFBinary
		SFTConstantByte		=	27,		// ->	SFConstant
		SFTNone				=	-1
	};

	//=====================================

protected:
	ScriptFunctionType		sftType;
	virtual void			StreamIn	( std::istream& in );

public:
	ScriptFunction			( ScriptFunctionType sft );
	ScriptFunction();

	ScriptFunctionType		Type		( void );
	static	ScriptFunction*	Factory		( ScriptFunctionType sft );

	virtual	float			Evaluate	( const float& x );
	virtual void			Stream		( std::ostream& out );
	static	ScriptFunction*	Stream		( std::istream& in );
};

//===============================================================================

const	int	MaxSFStorage	=	10;
class	ScriptFunctionStorage
{
public:
	static ScriptFunctionStorage*	GetInstance( void );

	void					Memory	( float value, int Address );
	float					Memory	( int Address );
	void					Sub		( ScriptFunction*	Sub );
	ScriptFunction*			Sub		( );

protected:
	static ScriptFunctionStorage*	hInstance;
	ScriptFunction*			hSub;

	ScriptFunctionStorage	( );
	float					fMemory[MaxSFStorage];
};

//===============================================================================

class	SFConstant : public ScriptFunction
{
protected:
	float	fValue;

	void	StreamIn	( std::istream& in );

public:
	SFConstant			( ScriptFunctionType sft );

	void	Stream		( std::ostream& out );
	float	Evaluate	( const float& x );

	void	Value		( float value );
	float	Value		( void );
};

//===============================================================================

class	SFUnary : public ScriptFunction
{
protected:
	ScriptFunction*		lpParameter;
	SFUnary				( );
	~SFUnary			( );

	void				StreamIn	( std::istream& in );

public:
	SFUnary				( ScriptFunctionType sft );

	void				Stream		( std::ostream& out );
	float				Evaluate	( const float& x );

	void				Parameter	( ScriptFunction* Param );
	ScriptFunction*		Parameter	( void );
};

//===============================================================================

class	SFBinary : public ScriptFunction
{
protected:
	ScriptFunction*		lpParameter[2];

	void				StreamIn	( std::istream& in );

public:
	SFBinary			( ScriptFunctionType sft );
	~SFBinary			( );

	void				Stream		( std::ostream& out );
	float				Evaluate	( const float& x );

	void				Parameter	( ScriptFunction* Param, int Nr );
	ScriptFunction*		Parameter	( int Nr );
};

#endif
