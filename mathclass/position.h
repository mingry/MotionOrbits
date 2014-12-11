#ifndef POSITION_H
#define POSITION_H

#include "math_def.h"
#include <cmath>

namespace math 
{
	class vector;
	class unit_vector;
	class matrix;
	class transf;

	class position
	{
	private:
		double p[3];
	    
		// addtion
		friend position& operator+=( position&, math::vector const& );
		friend position  operator+( position const&, math::vector const& );
		friend position  operator+( math::vector const&, position const& );

		// subtraction
		friend position& operator-=( position&, math::vector const& );
		friend vector    operator-( position const&, position const& );
		friend position  operator-( position const&, math::vector const& );

		// multiplication
		friend position  operator*( matrix const&, position const& );
		friend position  operator*( position const&, matrix const& );

		friend position& operator*=( position&, transf const& );
		friend position  operator*( position const&, transf const& );

		// multiplication by scalar
		friend position& operator*=( position&, double );
		friend position  operator*( position const&, double );
		friend position  operator*( double, position const& );

		// dot product
		friend double    operator%( position const&, math::vector const& );
		friend double    operator%( math::vector const&, position const& );

		// cross product
		friend position  operator*( position const&, unit_vector const& );
		friend position  operator*( unit_vector const&, position const& );

		// functions

		friend position  interpolate( double, position const&, position const& );
		friend double    distance( position const&, position const& );
		friend double    distance( position const&, position const&, position const& );

		friend position	affineCombination( position const&, position const&, double );
		friend position	affineCombination( position const&, position const&, position const&,
										double, double );
		friend position	affineCombination( position const&, position const&, position const&, position const&,
										double, double, double );
		friend position	affineCombination( int, position[], double[] );

		friend position     vector2position( math::vector const& );
		friend math::vector       position2vector( position const& );

		// stream
		friend std::ostream& operator<<( std::ostream&, position const& );
		friend std::istream& operator>>( std::istream&, position& );

		friend bool operator==( const position& a, const position& b );
		friend bool operator!=( const position& a, const position& b );

	public:
		// constructors
		position() {};
		position( double x, double y, double z ) { p[0]=x; p[1]=y; p[2]=z; };
		position( double a[3] ) { p[0]=a[0]; p[1]=a[1]; p[2]=a[2]; };
		position( const position& pos ) { p[0] = pos.p[0]; p[1] = pos.p[1]; p[2] = pos.p[2]; }
		position& operator=( const position& pos ) { 
			p[0] = pos.p[0]; p[1] = pos.p[1]; p[2] = pos.p[2];
			return *this;
		}

		// inquiry functions
		double& operator[] (int i) { return p[i]; }

		double x() const { return p[0]; }
		double y() const { return p[1]; }
		double z() const { return p[2]; }
		void   getValue( double d[3] ) { d[0]=p[0]; d[1]=p[1]; d[2]=p[2]; }
		void   setValue( double d[3] ) { p[0]=d[0]; p[1]=d[1]; p[2]=d[2]; }
		double getValue( int n ) const { return p[n]; }

		double coordinate( int i ) const { return p[i]; };
		double norm() const { return (double)sqrt( p[0]*p[0] + p[1]*p[1] + p[2]*p[2] ); };

		// chang functions
		void set_x( double x ) { p[0]=x; };
		void set_y( double x ) { p[1]=x; };
		void set_z( double x ) { p[2]=x; };
	};
};

#endif
