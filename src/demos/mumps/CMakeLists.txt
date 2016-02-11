# Additional include paths
INCLUDE_DIRECTORIES( ${CH_MUMPS_INCLUDES} )
 
MESSAGE(STATUS "Demo programs for Mumps module...")

# ---------------------------------------------------------------------

MESSAGE(STATUS "...add demo_mumps")
ADD_EXECUTABLE(demo_mumps demo_mumps)
SOURCE_GROUP("" FILES demo_mumps)
SET_TARGET_PROPERTIES(demo_mumps PROPERTIES 
                      FOLDER demos
                      LINK_FLAGS "${CH_LINKERFLAG_EXE}")
TARGET_LINK_LIBRARIES(demo_mumps ChronoEngine ChronoEngine_mumps)
ADD_DEPENDENCIES(demo_mumps ChronoEngine ChronoEngine_mumps)
INSTALL(TARGETS demo_mumps DESTINATION bin)

IF (ENABLE_MODULE_MKL)
	MESSAGE(STATUS "...add demo_matrix_comparison")
	ADD_EXECUTABLE(demo_matrix_comparison demo_matrix_comparison)
	SOURCE_GROUP("" FILES demo_matrix_comparison)
	SET_TARGET_PROPERTIES(demo_matrix_comparison PROPERTIES 
						  FOLDER demos
						  LINK_FLAGS "${CH_LINKERFLAG_EXE}")
	TARGET_LINK_LIBRARIES(demo_matrix_comparison ChronoEngine ChronoEngine_mumps ChronoEngine_mkl)
	TARGET_INCLUDE_DIRECTORIES(demo_matrix_comparison PUBLIC ${CH_MKL_INCLUDES} )
	ADD_DEPENDENCIES(demo_matrix_comparison ChronoEngine ChronoEngine_mumps ChronoEngine_mkl)
	INSTALL(TARGETS demo_matrix_comparison DESTINATION bin)
ENDIF(ENABLE_MODULE_MKL)




