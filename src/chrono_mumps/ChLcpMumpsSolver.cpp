#include "ChLcpMumpsSolver.h"


namespace chrono
{
	
	
	
	
	double ChLcpMumpsSolver::Solve(ChLcpSystemDescriptor& sysd) ///< system description with constraints and variables
	{
		sysd.ConvertToMatrixForm(&ass_mat, &rhs);

		mumps_engine.Initialize();
		mumps_engine.SetProblem(ass_mat, rhs);
		mumps_engine.MumpsCall();
		solver_call++;
		printf("Call: %d\n", solver_call);

		sysd.FromVectorToUnknowns(rhs);

		return 0.0;
	}

} // namespace chrono