#pragma once

#include "Human.h"
#include "mathclass/transq.h"
#include "mathclass/vectorN.h"
#include "mathclass/matrixN.h"

class Skeleton;
class PoseData;
class PoseConstraint;

class PoseIK
{
public:
	PoseIK();
	~PoseIK();

	static void	initialize();

	static int	ik_body( Skeleton* skeleton, PoseData* source_pose, PoseData* target_pose, PoseConstraint* pose_constraint, double roll );

	static void	ik_limb( Skeleton* skeleton, PoseData* pose, const math::transq& end, unsigned int END, unsigned int LOWER, unsigned int UPPER, unsigned int BASE, double roll );
	static void	ik_right_hand( Skeleton* skeleton, PoseData* pose, const math::transq& rh_transq, double roll );
	static void	ik_left_hand( Skeleton* skeleton, PoseData* pose, const math::transq& lh_transq, double roll );
	static void	ik_right_foot( Skeleton* skeleton, PoseData* pose, const math::transq& rf_transq, double roll );
	static void	ik_left_foot( Skeleton* skeleton, PoseData* pose, const math::transq& lf_transq, double roll );

protected:
	static double	energyFunc( const math::vectorN& d );
	static double	gradientFunc( const math::vectorN& d, math::vectorN& dp );
	static double	incorporateDamping( const math::vectorN& d, math::vectorN& dp );

	static double	computeJacobian();
	static void		scalingJacobian( math::matrixN& J );
	static void		scalingCoefficients( math::vectorN& x );

	static Skeleton*		skeleton;
	static PoseData*		source_pose;
	static PoseData*		target_pose;
	static PoseConstraint*	pose_constraint;

	static unsigned int		num_dof;		// # of dof
	static unsigned int		num_equations;	// = max( # of constraints, # of dof )
	static unsigned int		num_unknowns;	// = # of dof

	static math::matrixN	J;				// num_equations * num_unknowns
	static math::vectorN	b;				// num_equations

	static double	ErrorBound;
	static double	Tolerance;

	static double	DampingCoeff[ NUM_JOINTS_IN_HUMAN+1 ];
	static double	RigidityCoeff[ NUM_JOINTS_IN_HUMAN+1 ];

	static bool	IsDampingEnabled;
};
