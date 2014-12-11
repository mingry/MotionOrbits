#ifndef TRANSQ_H
#define TRANSQ_H

#include "math_def.h"

#include "vector.h"
#include "quater.h"

namespace math
{
	class unit_vector;
	class position;
	
	class transq
	{
	public:
		quater rotation;
		vector translation;

	private:
		// multiplication
		friend transq&      operator*=( transq &, transq const& );
		friend transq       operator* ( transq const&, transq const& );
		friend vector&      operator*=( vector&, transq const& );
		friend vector       operator* ( vector const&, transq const& );
		friend position&    operator*=( position&, transq const& );
		friend position     operator* ( position const&, transq const& );
		friend unit_vector& operator*=( unit_vector&, transq const& );
		friend unit_vector  operator* ( unit_vector const&, transq const& );

		// functions
		friend transq       interpolate( double, transq const&, transq const& );

		// stream
		friend std::ostream& operator<<( std::ostream&, transq const& );
		friend std::istream& operator>>( std::istream&, transq& );

		//
		friend bool	operator==( const transq& a, const transq& b );

	public:
		// constructors
		transq() {};
		transq( const transq& t ) {
			rotation = t.rotation;
			translation = t.translation;
		}
		transq( quater const& a, vector const& b ) { rotation=a; translation=b; }

		transq			inverse() const;

		transq& operator=( const transq& t ) {
			rotation = t.rotation;
			translation = t.translation;
			return ( *this );
		}

		/*
		bool operator==( const transq& t ) {
			return ( rotation==t.rotation && translation==t.translation );
		}
		*/
	};

	// identity transq
	extern transq identity_transq;

	// generator
	extern transq rotate_transq( double angle, vector const& axis );
	extern transq translate_transq( vector const& axis );
	extern transq translate_transq( double x, double y, double z );
};

#endif
