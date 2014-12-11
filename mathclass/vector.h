#ifndef VECTOR_H
#define VECTOR_H

#include "math_def.h"

namespace math
{
	class matrix;
	class transf;
	class position;
	class unit_vector;

	class vector
	{
	protected:
		double p[3];

	private:
		// negation
		friend vector      operator-( vector const& );
		friend unit_vector operator-( unit_vector const& );

		// addtion
		friend position& operator+=( position&, vector const& );
		friend vector&   operator+=( vector&, vector const& );
		friend vector    operator+( vector const&, vector const& );
		friend position  operator+( position const&, vector const& );
		friend position  operator+( vector const&, position const& );

		// subtraction
		friend vector    operator-( vector const&, vector const& );
		friend position  operator-( position const&, vector const& );
		friend position& operator-=( position&, vector const& );
		friend vector&   operator-=( vector&, vector const& );

		// dot product
		friend double    operator%( vector const&, vector const& );
		friend double    operator%( position const&, vector const& );
		friend double    operator%( vector const&, position const& );

		// cross product
		friend vector    operator*( vector const&, vector const& );
		friend position  operator*( position const&, unit_vector const& );
		friend position  operator*( unit_vector const&, position const& );

		// scalar Multiplication
		friend vector    operator*( vector const&, double );
		friend vector    operator*( double, vector const& );
		friend vector&   operator*=( vector&, double );

		// scalar Division
		friend vector    operator/( vector const&, double );
		friend double    operator/( vector const&, vector const& );
		friend vector&   operator/=( vector&, double );

		// matrix Multiplication
		friend vector    operator*( vector const&, matrix const& );
		friend vector    operator*( matrix const&, vector const& );

		friend vector&   operator*=( vector&, transf const& );
		friend vector    operator*( vector const&, transf const& );

		// functions
		friend double       len( vector const& );
		friend unit_vector  normalize( vector const& );

		friend vector       interpolate( double, vector const&, vector const& );

		friend double       angle( vector const&, vector const& );

		friend position     vector2position( vector const& );
		friend vector       position2vector( position const& );

		// stream
		friend std::ostream& operator<<( std::ostream&, vector const& );
		friend std::istream& operator>>( std::istream&, vector& );
		friend std::ostream& operator<<( std::ostream&, unit_vector const& );
		friend std::istream& operator>>( std::istream&, unit_vector& );

		//
		friend bool		operator==( const vector& a, const vector& b );
		friend bool		operator!=( const vector& a, const vector& b );

	public:
		// constructors
		vector() {}
		vector( const vector& v ) { 
			p[0] = v.p[0]; 
			p[1] = v.p[1]; 
			p[2] = v.p[2]; 
		}

		vector( double x, double y, double z ) { p[0]=x; p[1]=y; p[2]=z; }
		vector( double a[3] ) { p[0]=a[0]; p[1]=a[1]; p[2]=a[2]; }

		// inquiry functions
		inline double& operator[](int i) { return p[i]; }

		inline double x() const { return p[0]; };
		inline double y() const { return p[1]; };
		inline double z() const { return p[2]; };
		inline void   getValue( double d[3] ) { d[0]=p[0]; d[1]=p[1]; d[2]=p[2]; }
		inline void   setValue( double d[3] ) { p[0]=d[0]; p[1]=d[1]; p[2]=d[2]; }
		inline double getValue( int n ) const { return p[n]; }
		inline vector setValue( double x, double y, double z )
									{ p[0]=x, p[1]=y, p[2]=z; return *this; }
		inline double setValue( int n, double x )
									{ return p[n]=x; }

		double length() const;
		matrix cross() const;

		// change functions
		inline void set_x( double x ) { p[0]=x; };
		inline void set_y( double x ) { p[1]=x; };
		inline void set_z( double x ) { p[2]=x; };

		vector& operator=( const vector& v ) {
			p[0] = v.p[0]; 
			p[1] = v.p[1]; 
			p[2] = v.p[2];
			return ( *this );
		}

		/*
		bool operator==( const vector& v ) {
			return ( p[0]==v.p[0] && p[1]==v.p[1] && p[2]==v.p[2] );
		}
		*/
	};
};

#endif
