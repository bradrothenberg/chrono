//
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2010 Alessandro Tasora
// All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file at the top level of the distribution
// and at http://projectchrono.org/license-chrono.txt.
//


/// Class for interfacing with Mumps

#ifndef CHMUMPSENGINE_H
#define CHMUMPSENGINE_H

///////////////////////////////////////////////////
//
//   ChMumpsEngine.h
//
//   Use this header if you want to exploit Intel®
//	 MUMPS Library from Chrono::Engine programs.
//
//   HEADER file for CHRONO,
//  Multibody dynamics engine
//
// ------------------------------------------------
//             www.deltaknowledge.com
// ------------------------------------------------
///////////////////////////////////////////////////

#include "chrono_mumps/ChApiMumps.h"
#include "chrono_mumps/ChAssembledMatrix.h"

#include <stdio.h>
#include <mpi.h>
#include <dmumps_c.h>
#define JOB_INIT -1
#define JOB_END -2
#define USE_COMM_WORLD -987654

#define ICNTL(I) icntl[(I)-1] /* macro s.t. indices match documentation */
#define INFOG(I) infog[(I)-1] /* macro s.t. indices match documentation */
#define INFO(I) info[(I)-1] /* macro s.t. indices match documentation */

namespace chrono
{

	class ChApiMumps ChMumpsEngine
	{
	public:
		ChMumpsEngine(){};
		virtual ~ChMumpsEngine();

		void SetProblem(ChAssembledMatrix& Z, ChMatrix<>& rhs);
		void Initialize();
		int MumpsCall(int job_call = 6);
 
		//void GetResidual(ChMatrix<> res);

		void PrintINFO();

	private:
		DMUMPS_STRUC_C mumps_id;
		int n = 0;
		int nz = 0;
		double* rhs = nullptr;
		int myid = 0;
		int ierr = 0;
	};
} // end of namespace chrono


#endif