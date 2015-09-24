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

#ifndef CHMUMPSSOLVER_H
#define CHMUMPSSOLVER_H

///////////////////////////////////////////////////
//
//   ChLcpMumpsSolver.h
//
//   Use this header if you want to exploit Intel®
//	 MUMPS Library
//   from Chrono::Engine programs.
//
//   HEADER file for CHRONO,
//  Multibody dynamics engine
//
// ------------------------------------------------
//             www.deltaknowledge.com
// ------------------------------------------------
///////////////////////////////////////////////////



#include "chrono_mumps/ChApiMumps.h"
#include "core/ChMatrix.h"
#include "lcp/ChLcpSystemDescriptor.h"
#include "lcp/ChLcpSolver.h"
#include "chrono_mumps/ChMumpsEngine.h"

//#include <process.h>



namespace chrono {


    /// Class that wraps the Intel MUMPS 'PARDISO' parallel direct solver.
    /// It can solve linear systems. It cannot solve VI and complementarity problems.
    
   class ChApiMumps ChLcpMumpsSolver : public ChLcpSolver {

   private:
	   long int solver_call = 0;
	   ChAssembledMatrix ass_mat = ChAssembledMatrix(1, 1);
	   ChMatrixDynamic<double> rhs;
	   ChMatrixDynamic<double> res;
	   double res_norm = 0;
	   ChMumpsEngine mumps_engine;

	   bool size_lock = true;
	   bool sparsity_pattern_lock = true;

   public:

	   ChLcpMumpsSolver() {};
	   virtual ~ChLcpMumpsSolver(){};


         /// Solve using the MUMPS
		virtual double Solve(ChLcpSystemDescriptor& sysd) override; ///< system description with constraints and variables

		
		
	   


    };

}  // END_OF_NAMESPACE____

#endif