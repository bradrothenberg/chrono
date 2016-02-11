

# Arithmetic configuration
SET(MUMPS_ARITHMETIC "double" CACHE STRING "MUMPS arithmetic")
SET_PROPERTY(CACHE MUMPS_ARITHMETIC PROPERTY STRINGS float double complex complex-double)


IF (MUMPS_ARITHMETIC MATCHES "float")
	SET(ARITH_PREFIX "s")
	MESSAGE("No float library implemented yet")
ELSEIF (MUMPS_ARITHMETIC MATCHES "double")
	SET(ARITH_PREFIX "d")
ELSEIF (MUMPS_ARITHMETIC MATCHES "complex")
	SET(ARITH_PREFIX "c")
	MESSAGE("No complex-float library implemented yet")
ELSEIF (MUMPS_ARITHMETIC MATCHES "complex-double")
	SET(ARITH_PREFIX "z")
	MESSAGE("No complex-double library implemented yet")
ENDIF ()

MESSAGE(STATUS "Arithmetic set to '${MUMPS_ARITHMETIC}'. Added '${ARITH_PREFIX}' prefix...")


##########################

SET(MUMPS_ROOT_DIRECTORY "D:/SourceTreeWS/MumpsWin/Mumps5.0.0" CACHE PATH "Mumps root directory" FORCE)
SET(MUMPS_USE_MPI FALSE CACHE BOOL "Select if MPI is needed" FORCE)
SET(MUMPS_USE_DEFAULT_ORDERINGS TRUE CACHE BOOL "Select if default ordering library should be used" FORCE)
SET(USE_MKL_BLAS TRUE CACHE BOOL "Select if BLAS libraries should be used" FORCE)

MARK_AS_ADVANCED(MUMPS_USE_MPI
				MUMPS_USE_DEFAULT_ORDERINGS
				USE_MKL_BLAS)

SET(MUMPS_INCLUDE
	"${MUMPS_ROOT_DIRECTORY}/include")
	
SET(MUMPS_LIBRARIES
	"${MUMPS_ROOT_DIRECTORY}/lib/lib${ARITH_PREFIX}mumps.lib"
	"${MUMPS_ROOT_DIRECTORY}/lib/libmumps_common.lib")


IF (MUMPS_USE_MPI)
	MESSAGE("MPI is not implemented yet.")
ELSE (MUMPS_USE_MPI)
	SET(LIBSEQ_INCLUDE "${MUMPS_ROOT_DIRECTORY}/libseq")
	SET(LIBSEQ_LIBRARIES "${MUMPS_ROOT_DIRECTORY}/libseq/libmpiseq.lib")
ENDIF (MUMPS_USE_MPI)

IF (MUMPS_USE_DEFAULT_ORDERINGS)
	SET(MUMPS_ORDERINGS_LIBRARIES	"${MUMPS_ROOT_DIRECTORY}/lib/libpord.lib")
ELSE (MUMPS_USE_DEFAULT_ORDERINGS)
	MESSAGE("Non-default orderings is not implemented yet.")
	# SET(MUMPS_ORDERINGS_LIBRARIES "" CACHE PATH "Path to ordering library" FORCE)
ENDIF (MUMPS_USE_DEFAULT_ORDERINGS)

IF (USE_MKL_BLAS)

	find_package(mkl)
	
	SET(BLAS_LIBRARIES
		"C:/Program Files (x86)/Intel/Composer XE 2015/mkl/lib/intel64/mkl_intel_lp64.lib"
		"C:/Program Files (x86)/Intel/Composer XE 2015/mkl/lib/intel64/mkl_intel_thread.lib"
		"C:/Program Files (x86)/Intel/Composer XE 2015/mkl/lib/intel64/mkl_core.lib"
		"C:/Program Files (x86)/Intel/Composer XE 2015/compiler/lib/intel64/libiomp5md.lib"
		)
		
	SET(BLAS_INCLUDE
		"C:/Program Files (x86)/Intel/Composer XE 2015/mkl/include"
		"C:/Program Files (x86)/Intel/Composer XE 2015/compiler/include"
		)
		
ELSE (USE_MKL_BLAS)
	MESSAGE("Non-default BLAS library is not implemented yet.")
	# SET(BLAS_ROOT "" CACHE PATH "Path to BLAS directory" FORCE)
	# SET(BLAS_LIBRARIES "${BLAS_ROOT}/lib/lib.lib")
	# SET(BLAS_INCLUDE "${BLAS_ROOT}/include")
ENDIF (USE_MKL_BLAS)

################à
SET (CH_MUMPS_LIBRARIES
						"${MUMPS_LIBRARIES}"
						"${LIBSEQ_LIBRARIES}" #aka LIBS
						"${MUMPS_ORDERINGS_LIBRARIES}" #aka LORDERINGS
						"${BLAS_LIBRARIES}" #aka LIBBLAS
						)
						
SET (CH_MUMPS_INCLUDES
						"${LIBSEQ_INCLUDE}"
						"${MUMPS_INCLUDE}"
						)
						
SET(MUMPS_FOUND TRUE)