//===============================================================================
//
//	Name:
//		- -	3DMath.h
//
//	Zweck:
//		- -	Mathematische 3D-Klassen und Funktionen
//
//	Erstellt von:
//		- - Sebastian Kohl (Blue Cobold)
//			Game + Effect Programmer
//			BlueCobold@gmx.de
//			Webmaster of www.MellowZ.de.vu
//
//	Erstellt am:
//		- - 05.04.2003
//
//===============================================================================


#ifndef _3DMATH_H
#define _3DMATH_H

#define PI			3.1415926535897932384626433832
#define PIf			3.1415926535897932384626433832f
#define PI2f		3.1415926535897932384626433832f*2.f
#define PI_2f		3.1415926535897932384626433832f/2.f
#define	_2_PIf		2.f/PIf
#define	_1_PIf		1.f/PIf
#define PI_4f		3.1415926535897932384626433832f/4.f
#define PI3_4f		3.1415926535897932384626433832f*3.f/4.f
#define PIf_180		3.1415926535897932384626433832f/180.0f
#define _180_PIf	180.f/PIf

#define NEARLY_ZERO		0.001f


// Standartisierte Random-Funktionen, System- und Compiler-unabhängig
void	m_srand (unsigned int seed);
int		m_rand	( );

//#define	SIGN(x)		(x < 0 ? (-1) : 1)
template<class T> int SIGN( T x)
{
	if ( x < 0)
		return -1;
	else
		return 1;
}

template<class x, class y> x MAX( x a, y b)
{
	if ( a > b )
		return a;
	else
		return b;
}
//#define	MAX(x,y)	(x > y ? x : y)
//#define	MIN(x,y)	(x < y ? x : y)
template<class x, class y> x MIN( x a, y b)
{
	if ( a < b )
		return a;
	else
		return b;
}
//#define MOD(x,y)	(x >= y ? x-y : x)
template<class x, class y> x MOD( x a, y b)
{
	while ( a >= b )
		a -= b;
	return a;
}

float	MODF( float x, float y);

//#pragma pack(push)
#pragma pack(push, 1)

struct CVector3
{
	float x, y, z;						
};


//=============================================================
// Dreieck mit Vertex- und TexCoord-Indices
struct	TRIANGLE
{
	unsigned short Vertex[3];	// die Nr des Vertexes in der Vertex-Liste
	unsigned short Coord[3];	// die Nr des Tex-Paares in der Tex-Coord-Liste
};

//=============================================================
// Zweidimensionaler Vektor, oder Koordinaten
struct CVector2
{
public:
	float x, y;
};

//=============================================================
// Vierdimensionaler Vektor z.B. für Farben oder Ort+Zeit
struct CVector4
{
public:
	float x, y, z, a;
};

//=============================================================
// Dreidimensionale Gerade
struct CLine2
{
public:
	CVector3	Direction;
	CVector3	Position;
};

//=============================================================
// Coordinaten im BYTE-Format
struct CCoord2
{
public:
	unsigned char x, y;
};

//=============================================================
// Ein Koordinatenachsen-paralleler Quader, definiert durch
// Mittepunkt und davon ausgehende Seitenmaße
struct CBox2
{
	CVector3	Middle;			// Mittelpunkt der Box
	CVector3	Size;			// Abmessungen zu den Seiten
};

//=============================================================
// Ein beliebiger Quader, definiert durch Mittepunkt und 
// davon ausgehende Seitenmaße
struct CBox4
{
	CVector3	Middle;			// Mittelpunkt der Box
	CVector3	Size[3];		// Vektoren zu den Seiten
};

//=============================================================
// Ein beliebiges 8-eckiges Objekt
struct CBox8
{
	CVector3	Vertices[8];
};

//=============================================================
// Eine Ebene definiiert durch Normale und Abstand zum Ursprung
struct CPlane4
{
	CVector3 Normal;
//	float	 A;					// x der Ebenen-Normalen
//	float	 B;					// y der Ebenen-Normalen
//	float	 C;					// z der Ebenen-Normalen
	float	 D;					// Abstand zum Ursprung
};

//=============================================================
// Eine Kugel, definiert durch Mittelpunkt und Radius
struct CSphere2
{
	CVector3	Middle;
	float		Radius;
};

//=============================================================
// ein komplettes Polygon mit Coordinates und Vertices
struct CPolygon3
{
	CVector3		Vertex[3];
	CVector2		Coord[3];
	CVector3		Normal[3];
	float			Alpha;
};

//=============================================================
// ein komplettes Listen-Polygon mit Coordinates und Vertices
class CListPolygon3
{
public:
	CListPolygon3	();
	CListPolygon3	( CPolygon3 Polygon);
	CListPolygon3	( CPolygon3 Polygon, CListPolygon3* Next);

	void	SplitPolygonsIntoList	( CPolygon3 Polygon);

	CPolygon3		Polygon;
	CListPolygon3*	Next;
};

//=============================================================
// ein komplettes Quad mit Coordinates und Vertices
struct CQuad4
{
	CVector3		Vertex[4];
	CVector2		Coord[4];
	float			Alpha;		// Alpha-Wert des Quads
};

//=============================================================
// ein komplettes Listen-Quad mit Coordinates und Vertices
struct CListQuad4
{
	CQuad4			Quad;
	CListQuad4*		Next;
};

#pragma pack(pop)

//=================================
// Richtungskonstanten für Vektoren
#define	SAME_VECTOR_DIRECTION		 0
#define OPPOSIT_VECTOR_DIRECTION	 1
#define NO_SIMILAR_DIRECTION		 2


//======================== Cross ===================================
//
// Kreuzt zwei Vektoren und gibt einen senkrechten zurück
//
//==================================================================
CVector3 Cross(const CVector3& Vector1, const CVector3& Vector2);

//======================== Magnitude ===============================
//
// Gibt die Länge des Vektors zurück, auch bekannt als
// euklidische Norm
//
//==================================================================
float Magnitude(const CVector3& Vector);

//======================== Normalize ===============================
//
// Normalisiert den Vektor
//
//==================================================================
CVector3 Normalize(const CVector3& Normal);

//========== etwas passender:
inline CVector3 operator !(const CVector3& Vector)
{
	return Normalize( Vector);
}

//======================== Normal ==================================
//
// Gibt die Normale eines Polygons zurück
//
//==================================================================
CVector3 Normal(const CVector3* Triangle);


//======================== ToVector ================================
//
// Gibt einen Vektor zurück
//
//==================================================================
CVector3 ToVector( float x, float y, float z);


//======================== PolygonNormal ===========================
//
// Normale, NICHT normalisiert
//
//==================================================================
CVector3 PolygonNormal(const CVector3* Triangle);


//======================== AddVectors ==============================
//
// Addiert zwei Vektoren
//
//==================================================================
CVector3 AddVectors(const CVector3& Vector1, const CVector3& Vector2);

inline CVector3 operator +(const CVector3& V1, const CVector3& V2)
{
	return AddVectors( V1, V2);
}

inline CVector3 operator *(const CVector3& V1, const CVector3& V2)
{
	CVector3 V = {	V1.x * V2.x,
					V1.y * V2.y,
					V1.z * V2.z};
	return V;
}

inline CVector2 operator +(const CVector2& V1, const CVector2& V2)
{
	CVector2 V = { V1.x + V2.x, V1.y + V2.y};
	return V;
}

inline CVector2 operator *(const CVector2& V1, const CVector2& V2)
{
	CVector2 V = { V1.x * V2.x, V1.y * V2.y};
	return V;
}

//======================== SubVectors ==============================
//
// Subtrahiert zwei Vektoren und gibt somit einen Vektor von einem
// Punkt zu einem anderen zurück
//
//==================================================================
CVector3 SubVectors(const CVector3& To, const CVector3& From);


inline CVector3 operator -(const CVector3& V1, const CVector3& V2)
{
	return SubVectors( V1, V2);
}

//======================== SubVectors ==============================
//
// Subtrahiert zwei Vektoren
//
//==================================================================
CVector2 SubVectors(const CVector2& Vector1, const CVector2& Vector2);


//======================== ScaleVector==============================
//
// Multipliziert einen Vektor mit einer Zahl [Skalieren]
//
//==================================================================
CVector3 ScaleVector(const CVector3& Vector, float Scale);

inline CVector3 operator *( float Scale, const CVector3& V1)
{
	return ScaleVector( V1, Scale);
}

inline CVector3 operator *( const CVector3& V1, float Scale)
{
	return ScaleVector( V1, Scale);
}

inline CVector2 operator *( float Scale, const CVector2& V1)
{
	CVector2 V = {	V1.x * Scale,
					V1.y * Scale};
	return V;
}


inline CPlane4 operator *(float s, CPlane4 Plane)
{
	Plane.Normal = s*Plane.Normal;
	Plane.D *= s;
	return Plane;
}

//======================== VectorsAreEqual ==============================
//
// Prüft, ob zwei Vectoren gleich sind. Ist geeignet, um 2 Vertices
// daraufhin zu testen, ob sie die selben sind.
//
//=======================================================================
bool VectorsAreEqual( CVector3 Vector1, CVector3 Vector2);

inline bool operator ==( CVector3 V1, CVector3 V2)
{
	return VectorsAreEqual( V1, V2);
}


//======================== VectorsAreEqual ==============================
//
// Prüft, ob zwei Vectoren gleich sind. Ist geeignet, um 2 Vertices
// daraufhin zu testen, ob sie die selben sind.
//
//=======================================================================
bool VectorsAreEqual( CVector2 Vector1, CVector2 Vector2);

inline bool operator ==( CVector2 V1, CVector2 V2)
{
	return VectorsAreEqual( V1, V2);
}


//======================== VectorsAreParallel ===========================
//
// Prüft, ob zwei Vectoren Parallel sind
//
//=======================================================================
bool VectorsAreParallel( CVector3 Vector1, CVector3 Vector2);


//========================== RotateVectorY ==============================
//
// Rotiert einen Vektor um die y-Achse
//
//=======================================================================
CVector3 RotateVectorY( CVector3 Vector, float Angle);


//========================== RotateVectorZ ==============================
//
// Rotiert einen Vektor um die z-Achse
//
//=======================================================================
CVector3 RotateVectorZ( CVector3 Vector, float Angle);


//=========================== RotateVector ==============================
//
// Rotiert einen Vektor um eine beliebige Achse
//
//=======================================================================
CVector3 RotateVector( CVector3 Vector, CVector3 Achsis, float Angle);


//======================== MakePlane ====================================
//
// Berechnet für ein Poligon die Ebene, in der es liegt
//
//=======================================================================
CPlane4 MakePlane( CVector3 Normal, CVector3 Point);
CPlane4 MakePlane( CVector3 Vector[3]);


//======================== MakePlane ====================================
//
// Berechnet ebenfalls für ein Poligon die Ebene, in der es liegt,
// aber es wird dabei NICHT die Richtung der Normalen angepasst
// und dadurch kann Polygon-Spezifisch entschieden werden, ob ein
// Punkt IN_FRONT_OF oder IN_BACK_OF liegt
//
//=======================================================================
CPlane4 MakePolyPlane( CVector3 Normal, CVector3 Point);
CPlane4 MakePolyPlane( CVector3 Vector[3]);



//======================== NormalizePlane ====================================
//
// Normalisiert die Daten einer Ebene
//
//=======================================================================
CPlane4	NormalizePlane(CPlane4 Plane);




#define IN_FRONT_OF		0
#define IN_BACK_OF		1
#define PLANAR			2

//======================== ClassifyPoint ================================
//
// Auf welcher Seite einer Ebene liegt ein Punkt?
// Die Genauigkeit beträgt NEARLY_ZERO.
//
// Es gibt 3 Möglichkeiten für einen Punkt:
// IN_FRONT_OF		Der Abstand zwischen Punkt und Ebene ist größer
//					als NEARLY_ZERO und kleiner 0
// IN_BACK_OF		Der Abstand zwischen Punkt und Ebene ist größer
//					als NEARLY_ZERO und größer 0
// PLANAR			Der Abstand zwischen Punkt und Ebene ist kleiner
//					als NEARLY_ZERO und der Punkt liegt somit IN der Ebene
//
//=======================================================================
int ClassifyPoint( CVector3 Point, CPlane4 Plane);


#define CROSSING			3
//#define	CROSSING_WITH_TOUCH	4
//======================== ClassifyPolygon ==============================
// Gibt zurück, wie sich ein Polygon zu einer Ebene verhält
//
// 3 Möglichkeiten, wie bei einem Punkt.
// Zusätzlich:
// CROSSING				Das Polygon schneidet sich mit der Ebene
// CROSSING_WITH_TOUCH	CROSSING + ein Punkt ist planar
//=======================================================================
int ClassifyPolygon( CVector3* Polygon, CPlane4 Plane, bool* OnePlanar, CVector3* vPlanar);
int ClassifyPolygon( CVector3* Polygon, CPlane4 Plane);
int ClassifyPolygon( CPolygon3 Polygon, CPlane4 Plane);

//======================== SplitPolygon =================================
//
// Splittet ein Polygon auf, wenn es sich mit einer Ebene schneidet.
// Verlangt werden dabei zwei NULL-Pointer
//
//=======================================================================
void SplitPolygon( CPolygon3 Polygon, CPlane4 Plane,
					   CListPolygon3** Front, CListPolygon3** Back );


//======================== CrossPlaneWithLine ===========================
//
// Gibt den Schnittpunkt einer Line mit einer Ebene zurück
//
// Parameter:	CPlane4		für die Ebene
//				CVector3	für den Startpunkt der Linie
//				CVector3	für den Endpunkt der Linie
//				CVector3*	für die Übergabe eines Schnittpunktes
//=======================================================================
bool CrossPlaneWithLine( CPlane4 Plane, CVector3 Start, CVector3 End, CVector3* Cross);


//======================== CrossPlaneWithLine ===========================
//
// Gibt den Schnittpunkt einer Geraden mit einer Ebene zurück
//
// Parameter:	CPlane4		für die Ebene
//				CLine2		für die Geraden
//				CVector3*	für die Übergabe eines Schnittpunktes
//=======================================================================
bool CrossPlaneWithLine( CPlane4 Plane, CLine2 Line, CVector3* Cross);
// das gleiche, guckt allerdings nur in "Vorwärts-Richtung"
bool CrossPlaneWithRay( CPlane4 Plane, CLine2 Line, CVector3* Cross);



//======================== CrossSphereWithLine ===========================
//
// Gibt die Schnittpunkte einer Geraden mit einer Kugel zurück
//
// Parameter:	CPlane4		für die Ebene
//				CLine2		für die Geraden
//				CVector3*	für die Übergabe der Schnittpunkte
//				CVector3*	für die Übergabe der Schnittpunkte
// Rückgabe:	0			keine Treffer
//				1			ein Berührpunkt
//				2			zwei Schnittpunkte
//=======================================================================
int CrossSphereWithLine( CSphere2 Sphere, CLine2 Line,
						 CVector3* CrossNear,
						 CVector3* CrossFar);


//======================== PointInPolygon ===============================
//
// Testet, ob ein Punkt in einem Polygon liegen kann
//
// Parameter:	CVector3*	für das Polygon
//				CVector3	für den Testpunkt
// Rückgabe:	-1		bei FALSE
//				0,1,2	bei speziellem Vertex
//				3,4,5	bei spezieller Edge (Werte sind Edge+3)
//				6		bei TRUE
//=======================================================================
int PointInPolygon( CVector3 Polygon[3], CVector3 Point);


//======================== PolygonIntersection ==========================
//
// Gibt zurück, wie sich zwei Polygone zueinander verhalten.
// Werte sind wie bei ClassifyPolygon, nur INDEPENDENT kommt dazu
//
// Parameter:	CVector3*	für Polygon1
//				CVector3*	für Polygon2
//=======================================================================
#define TOUCHING		4
#define	INDEPENDENT		5
int PolygonIntersection( CVector3 vPoly1[3], CVector3 vPoly2[3]);


//======================== GetAngleBetweenVectors =======================
//
// Gibt den Winkel zwischen 2 Vektoren zurück
//
// Parameter:	CVector3	Vector1
//				CVector3	Vector2
//=======================================================================
float GetAngleBetweenVectors( CVector3 Vector1, CVector3 Vector2);


//======================== CalculateTextureCoord ========================
//
// Gibt die Textur-Koordinaten eines Punktes auf einer Texturierten Linie zurück
//
// Parameter:	CVector3	für den Startpunkt der Linie
//				CVector3	für den Endpunkt der Linie
//				CVector3	für den gesuchten Punkt
//				CVector2	Textur-Koordinaten für Start
//				CVector2	Textur-Koordinaten für End
//=======================================================================
CVector2 CalculateTextureCoord( CVector3 Start, CVector3 End, CVector3 Point, 
							    CVector2 TStart, CVector2 TEnd);


//======================== CrossPlaneWithPlane ==============================
//
// Schneidet zwei Ebenen und gibt die Schnittgerade zurück
//
//=======================================================================
bool CrossPlaneWithPlane( CPlane4 Plane1, CPlane4 Plane2, CLine2* Line);

//=======================================================================

CVector3 CosInterpolate(CVector3 a, CVector3 b, float x);

//=======================================================================

CVector3 PerlinNoise( float x, float y, float z, float a, unsigned char octaves);
float	 PerlinNoise1DBy4D( float x, float y, float z, float a, unsigned char octaves, float wrap);


//=========================== MirrorVector ==============================
//
// Spiegelt einen Vektor an einer Ebene, gegeben durch Punkt und Normale
//
//=======================================================================
CVector3 MirrorVector( const CVector3& Target, const CVector3& Normal);


#endif 
