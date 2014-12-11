#ifndef MATRIX_H
#define MATRIX_H

#include "math_def.h"
#include "vector.h"

namespace math 
{
	class position;
	class quater;
	class transf;
	class unit_vector;

	class matrix
	{
	private:
		vector p[3];

		// addition & subtraction
		friend matrix    operator+( matrix const&, matrix const& );
		friend matrix&   operator+=( matrix&, matrix const& );
		friend matrix    operator-( matrix const&, matrix const& );
		friend matrix&   operator-=( matrix&, matrix const& );

		// multiplication
		friend matrix    operator*( double, matrix const& );
		friend matrix    operator*( matrix const&, double );
		friend matrix    operator*( matrix const&, matrix const& );
		friend vector    operator*( matrix const&, vector const& );
		friend vector    operator*( vector const&, matrix const& );
		friend position  operator*( matrix const&, position const& );
		friend position  operator*( position const&, matrix const& );
		friend unit_vector& operator*=( unit_vector&, matrix const& );
		friend unit_vector  operator*( unit_vector const&, matrix const& );

		// transform with quaternions
		friend matrix    Quater2Matrix( quater const& );
		friend quater    Matrix2Quater( matrix const& );

		// functions
		friend matrix    inverse( matrix const& );
		friend matrix    interpolate( double, matrix const&, matrix const& );
		friend matrix    mexp( vector const& );
		friend vector    mlog( matrix const& );

		// stream
		friend std::ostream& operator<<( std::ostream&, matrix const& );
		friend std::istream& operator>>( std::istream&, matrix& );

	public:
		// constructors
		matrix() {};
		matrix( vector const& a, vector const& b, vector const& c )
		{
			p[0][0]=a.x(); p[0][1]=a.y(); p[0][2]=a.z();
			p[1][0]=b.x(); p[1][1]=b.y(); p[1][2]=b.z();
			p[2][0]=c.x(); p[2][1]=c.y(); p[2][2]=c.z();
		};
		matrix( double a00, double a01, double a02,
				double a10, double a11, double a12,
				double a20, double a21, double a22 )
		{
			p[0][0]=a00; p[0][1]=a01; p[0][2]=a02;
			p[1][0]=a10; p[1][1]=a11; p[1][2]=a12;
			p[2][0]=a20; p[2][1]=a21; p[2][2]=a22;
		};
		matrix( const matrix& m )
		{
			p[0] = m.p[0];
			p[1] = m.p[1];
			p[2] = m.p[2];
		}

		// inquiry functions
		inline void   getValue( double d[3][3] )
				{ d[0][0]=p[0][0]; d[0][1]=p[0][1]; d[0][2]=p[0][2];
				d[1][0]=p[1][0]; d[1][1]=p[1][1]; d[1][2]=p[1][2];
				d[2][0]=p[2][0]; d[2][1]=p[2][1]; d[2][2]=p[2][2]; }
		inline void   setValue( double d[3][3] )
				{ p[0][0]=d[0][0]; p[0][1]=d[0][1]; p[0][2]=d[0][2];
				p[1][0]=d[1][0]; p[1][1]=d[1][1]; p[1][2]=d[1][2];
				p[2][0]=d[2][0]; p[2][1]=d[2][1]; p[2][2]=d[2][2]; }
		inline double getValue( int row, int col ) const { return p[row].getValue( col );}
		inline vector getValue( int row ) const { return p[row];}
		inline matrix setValue( double x00, double x01, double x02,
								double x10, double x11, double x12,
								double x20, double x21, double x22 )
				{ p[0][0]=x00, p[0][1]=x01, p[0][2]=x02,
				p[1][0]=x10, p[1][1]=x11, p[1][2]=x12,
				p[2][0]=x20, p[2][1]=x21, p[2][2]=x22;
				return *this; }

		inline vector setValue( int row, vector const& v )
				{ return p[row]=v; }
		inline double setValue( int row, int col, double x )
				{ return p[row][col]=x; }

		inline vector& operator[](int i) { return p[i]; };

		matrix transpose() const;
		matrix inverse() const;
		double det() const;

		matrix& operator=( const matrix& m ) {
			p[0] = m.p[0];
			p[1] = m.p[1];
			p[2] = m.p[2];

			return ( *this );
		}
	};

	extern matrix scaling( double );
	extern matrix scaling( double, double, double );
	extern matrix rotation( double, vector const& );
	extern matrix reflection( vector const& );
	extern vector Matrix2EulerAngle( const matrix& m );
	extern matrix EulerAngle2Matrix( const vector& m );
};

#endif
