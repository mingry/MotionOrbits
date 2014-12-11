#ifndef QUATER_H
#define QUATER_H

#include "math_def.h"
#include "vector.h"
#include "unit_vector.h"

#include <cmath>

namespace math
{
	class vector;
	class matrix;
	class position;
	class unit_vector;

	class quater
	{
	private:
		double p[4];

		// negation
		friend quater    operator-( quater const& );

		// addtion
		friend quater    operator+( quater const&, quater const& );

		// subtraction
		friend quater    operator-( quater const&, quater const& );

		// dot product
		friend double    operator%( quater const&, quater const& );

		// Multiplication
		friend quater    operator*( quater const&, quater const& );

		// scalar Multiplication
		friend quater    operator*( quater const&, double );
		friend quater    operator*( double, quater const& );

		// scalar Division
		friend quater    operator/( quater const&, double );

		// transform with matrix
		matrix	getMatrix();
		void	setMatrix( matrix const& );

		friend matrix    Quater2Matrix( quater const& );
		friend quater    Matrix2Quater( matrix const& );
		friend vector    Quater2EulerAngle( quater const& );
		friend quater    EulerAngle2Quater( vector const& );

		// functions
		friend quater    exp( vector const& );
		friend quater    pow( vector const&, double );
		friend quater    inverse( quater const& );
		friend double    len( quater const& );
		friend vector    ln ( quater const& );
		friend position  rotate( quater const&, position const& );
		friend vector    rotate( quater const&, vector const& );
		friend unit_vector rotate( quater const&, unit_vector const& );
		friend quater    slerp( quater const&, quater const&, double );
		friend quater    interpolate( double, quater const&, quater const& );
		friend double    distance( quater const&, quater const& );
		friend vector    difference( quater const&, quater const& );

		// stream
		friend std::ostream& operator<<( std::ostream&, quater const& );
		friend std::istream& operator>>( std::istream&, quater& );

		//
		friend bool operator==( const quater& a, const quater& b );

	public:
		//
		// constructors
		//
		quater() {};
		quater( const quater& q ) { 
			p[0]=q.p[0]; 
			p[1]=q.p[1]; 
			p[2]=q.p[2]; 
			p[3]=q.p[3]; 
		}
		quater( double w, double x, double y, double z )
							{ p[0]=w; p[1]=x; p[2]=y; p[3]=z; }
		quater( double a[4] ) { p[0]=a[0]; p[1]=a[1]; p[2]=a[2]; p[3]=a[3]; }

		quater( const unit_vector& vn1, const unit_vector& vn2 ) {
			*this = exp( atan2f( (float)len(vn1*vn2), (float)( vn1%vn2 ) ) * math::normalize(vn1*vn2) / 2 ); 
		}
		quater( const vector& v1, const vector& v2 ) {
			unit_vector vn1 = math::normalize(v1), vn2 = math::normalize(v2);
			*this = exp( atan2f( (float)len(vn1*vn2), (float)( vn1%vn2 ) ) * math::normalize(vn1*vn2) / 2 ); 
		}

		//
		// inquiry functions
		//
		inline double	real() const { return p[0]; }
		inline vector	imaginaries() const { return vector( p[1], p[2], p[3] ); }
		inline quater	inverse() const { return quater( p[0], -p[1], -p[2], -p[3] ); }
		inline quater	normalize() const;
		inline double	length() const;
		inline double&	operator[] (int i) { return p[i]; }

		inline void getValue( double d[4] ) { d[0]=p[0]; d[1]=p[1]; d[2]=p[2]; d[2]=p[2]; }
		inline void setValue( double d[4] ) { p[0]=d[0]; p[1]=d[1]; p[2]=d[2]; p[2]=d[2]; }
		inline void setValue( double w, double x, double y, double z ) {
			p[0] = w; p[1] = x; p[2] = y; p[3] = z;
		}

		inline double w() const { return p[0]; }
		inline double x() const { return p[1]; }
		inline double y() const { return p[2]; }
		inline double z() const { return p[3]; }

		//
		// Set parameters
		//
		inline void set_w( double x ) { p[0]=x; }
		inline void set_x( double x ) { p[1]=x; }
		inline void set_y( double x ) { p[2]=x; }
		inline void set_z( double x ) { p[3]=x; }

		quater& operator=( const quater& q ) {
			p[0] = q.p[0]; 
			p[1] = q.p[1]; 
			p[2] = q.p[2]; 
			p[3] = q.p[3];
			return ( *this );
		}

		bool operator==( const quater& q ) {
			return ( p[0]==q.p[0] && p[1]==q.p[1] && p[2]==q.p[2] && p[3]==q.p[3] );
		}
	};
};

#endif
