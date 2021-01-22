#if !defined __GIS__DEFINE__H__
#define  __GIS__DEFINE__H__

#include <math.h>
#include <float.h>

namespace MapEngine
{
	const double Epsilon_High      = 1.0E-15;
	const double Epsilon_Medium    = 1.0E-10;
	const double Epsilon_Low       = 1.0E-05;
	const double Epsilon           = Epsilon_Medium;

	#define PI	3.14159265358979323846
	#define DOUBLE_MAX	DBL_MAX
	#define DOUBLE_MIN	-DBL_MAX

	#define H_UNDEFINED             0x00
	#define	H_PIXEL_UNIT			0x01
	#define H_MM_UNIT				0x02
	#define	H_CM_UNIT				0x03
	#define	H_M_UNIT				0x04
	#define	H_KM_UNIT				0x05
	#define	H_INCH_UNIT				0x06
	#define	H_LINK_UNIT				0x07
	#define	H_USFOOT_UNIT			0x08
	#define	H_FOOT_UNIT				0x09
	#define	H_YARD_UNIT				0x0A
	#define	H_ROD_UNIT				0x0B
	#define	H_CHAIN_UNIT			0x0C
	#define	H_MILE_UNIT				0x0D
	#define	H_NAUTICAL_MILE_UNIT	0x0E
	#define	H_DEGREE_UNIT			0x0F
	
	#define GEOMETRY_LAYER	1
	#define TOPOLOGY_LAYER	2
} //namespace MapEngine

#endif  //  __GIS__DEFINE__H__