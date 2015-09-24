#include "ChMumpsEngine.h"
#include <algorithm>

namespace chrono
{
	
	
	
	
	
	
	ChMumpsEngine::~ChMumpsEngine()
	{	
		mumps_id.job = JOB_END;
		dmumps_c(&mumps_id); /* Terminate instance */
		ierr = MPI_Finalize();
	}	
	
	
	void ChMumpsEngine::SetProblem(ChAssembledMatrix& Z, ChMatrix<>& rhs)	
	{
		Z.Trim();
		n = Z.GetRows();
		nz = Z.GetArraysLength();	
	
	
		Z.SetOneIndexedArray(true);	
		/* Define the problem on the host */
		if (myid == 0) {
			mumps_id.n = n;
			mumps_id.nz = nz;
			mumps_id.irn = Z.GetRowIndexAddress();
			mumps_id.jcn = Z.GetColIndexAddress();
			mumps_id.a = Z.GetValuesAddress();
			mumps_id.rhs = rhs.GetAddress();
		}
	}	


	void ChMumpsEngine::Initialize()
	{
		ierr = MPI_Init(nullptr, nullptr);
		ierr = MPI_Comm_rank(MPI_COMM_WORLD, &myid);

		/* Initialize a MUMPS instance. Use MPI_COMM_WORLD */
		mumps_id.job = JOB_INIT;
		mumps_id.par = 1;
		mumps_id.sym = 0;
		mumps_id.comm_fortran = USE_COMM_WORLD;
		
		dmumps_c(&mumps_id);	
		/* Output messages */
		mumps_id.ICNTL(1) = 6; // Error
		mumps_id.ICNTL(2) = 0; // Diagnostic and warnings
		mumps_id.ICNTL(3) = 0; // Global information
		mumps_id.ICNTL(4) = 1; // Error, warning and diagnostic control	
		/* Matrix control */
		mumps_id.ICNTL(5) = 0; // Assembled Matrix
		mumps_id.ICNTL(18) = 0; // Matrix centralized on the host
	}	
	
	int ChMumpsEngine::MumpsCall(int job_call)
	{	
		/* Call the MUMPS package. */
		mumps_id.job = job_call;
		dmumps_c(&mumps_id);
		return mumps_id.INFOG(1);
	}	
	
	
	void ChMumpsEngine::PrintINFO()
	{
		printf("INFO(1)=");
		switch (mumps_id.INFO(1))
		{
		case (0) :
			printf("0: Mumps is succesful!\n");
			break;
		case(-1) :
			printf("-1: Error on processor %d\n", mumps_id.INFO(2));
			break;
		case(-2) :
			printf("-2: Number of nonzeros out of range NZ=%d\n", mumps_id.INFO(2));
			break;
		case(-3) :
			printf("-3: Mumps called with wrong JOB. JOB=%d\n", mumps_id.INFO(2));
			break;
		case(-4) :
			printf("-4: Error in user-provided permutation array PERM_IN at position: %d\n", mumps_id.INFO(2));
			break;
		case(-5) :
			printf("-5: Problem of real workspace allocation of size %d during analysis\n", mumps_id.INFO(2));
			break;
		case(-6) :
			printf("-6: Matrix is singular in structure. Matrix rank: %d\n", mumps_id.INFO(2));
			break;
		case(-7) :
			printf("-7: Problem of integer workspace allocation of size %d during analysis\n", mumps_id.INFO(2));
			break;
		case(-10) :
			printf("-10: Matrix is numerically singular.\n");
			break;
		case(-16) :
			printf("-16: N is out of range. N=%d\n", mumps_id.INFO(2));
			break;
		case(-21) :
			printf("-21: PAR=1 not allowed because only one processor is available.\n");
			break;
		case(-22) :
			printf("-22: Array pointers have problems. INFO(2)=%d\n", mumps_id.INFO(2));
			break;
		default:
			printf("%d: See the user guide. INFO(2)=%d\n", mumps_id.INFO(1), mumps_id.INFO(2));
		}
			
	}

}