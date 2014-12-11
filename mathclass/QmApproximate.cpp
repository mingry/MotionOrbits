#include "QmApproximate.h"
#include "matrixN.h"
#include "vectorN.h"

#include <cassert>

using namespace math;

void
QmApproximate::setSize( int n )
{
	if ( n==num ) return;

	delete[] cp; cp = NULL;

	num = n;
	if ( n>0 ) cp = new vector[n+3];
}

void
QmApproximate::smoothBoundary()
{
	assert( num > 3 );

	cp[0] =
	cp[1] = 
	cp[2] =
	cp[num-1] = 
	cp[num-2] =
	cp[num-3] = vector(0,0,0);
}

vector
QmApproximate::evaluate( double t ) const
{
	int i = int(t);
	if ( i < 0     ) i = 0;
	if ( i > num-1 ) i = num-1;

	double f = t - i;

	return B0(f)*cp[i] + B1(f)*cp[i+1] + B2(f)*cp[i+2] + B3(f)*cp[i+3];
}

void
QmApproximate::discretize( int num_frames, vector *data )
{
    for( int i=0; i<num_frames; i++ )
    {
        double t = num * i;
        data[i] = evaluate( t/( num_frames ) );		// zoi: num_frames --> num_frames-1 (right?)
    }
}

void
QmApproximate::approximate( int num_frames, std::vector< QmScatteredData >& data_list )
{
	static vectorN weight; weight.setSize( num+3 );

	int i;
    for( i=0; i<num+3; i++ )
	{
		cp[i] = vector(0,0,0);
		weight[i] = 0.0;
	}

	std::vector< QmScatteredData >::iterator itor = data_list.begin();
	while( itor != data_list.end() )
    {
		double t = num * itor->position;
		t /= ( num_frames );

		i = int(t);
		if ( i < 0     ) i = 0;
		if ( i > num-1 ) i = num-1;

		double f = t - i;

		double r0 = B0(f); double s0 = r0*r0;
		double r1 = B1(f); double s1 = r1*r1;
		double r2 = B2(f); double s2 = r2*r2;
		double r3 = B3(f); double s3 = r3*r3;

		double sum = s0 + s1 + s2 + s3;

		cp[i  ] += s0 * itor->value * r0 / sum;
		cp[i+1] += s1 * itor->value * r1 / sum;
		cp[i+2] += s2 * itor->value * r2 / sum;
		cp[i+3] += s3 * itor->value * r3 / sum;

		weight[i  ] += s0;
		weight[i+1] += s1;
		weight[i+2] += s2;
		weight[i+3] += s3;

		itor ++;
    }

    for( i=0; i<num+3; i++ )
        if ( weight[i]>EPS ) cp[i] /= weight[i];
                        else cp[i] = vector(0,0,0);
}

void
QmApproximate::approximateLeastSquare( int num_frames, std::vector< QmScatteredData >& data_list )
{
	int num_data = data_list.size();

	static matrixN M;  M.setSize( num+3, num_data );
	static vectorN bx; bx.setSize( num_data );
	static vectorN by; by.setSize( num_data );
	static vectorN bz; bz.setSize( num_data );

	int i, j;

	for( i=0; i<num+3; i++ )
	for( j=0; j<num_data; j++ )
		M[i][j] = 0.0;

	std::vector< QmScatteredData >::iterator itor = data_list.begin();
	while( itor != data_list.end() )
    {
        double t = num * itor->position;
        t /= num_frames;

        i = int(t);
        if ( i < 0     ) i = 0;
        if ( i > num-1 ) i = num-1;

        double f = t - i;

		M[i  ][j] = B0(f);
		M[i+1][j] = B1(f);
		M[i+2][j] = B2(f);
		M[i+3][j] = B3(f);

		bx[j] = itor->value[0];
		by[j] = itor->value[1];
		bz[j] = itor->value[2];

		itor ++;
    }

	static matrixN Mt;  Mt.transpose( M );
	static matrixN Mp;  Mp.mult( Mt, M );
	static vectorN bxp; bxp.mult( Mt, bx );
	static vectorN byp; byp.mult( Mt, by );
	static vectorN bzp; bzp.mult( Mt, bz );

	static vectorN cx;	cx.solve( Mp, bxp );
	static vectorN cy;	cy.solve( Mp, byp );
	static vectorN cz;	cz.solve( Mp, bzp );

	/*
	static vectorN cx;	cx.solve( M, bx, 1e-3 );
	static vectorN cy;	cy.solve( M, by, 1e-3 );
	static vectorN cz;	cz.solve( M, bz, 1e-3 );
	*/

	for( i=0; i<num+3; i++ )
		cp[i] = vector( cx[i], cy[i], cz[i] );
}

