#=============================================================================
# CMake configuration file for the Chrono MUMPS module
# 
# Cannot be used stand-alone (it's loaded by CMake config. file in parent dir.)
#=============================================================================

option(ENABLE_MODULE_MUMPS "Enable the Chrono MUMPS module" OFF)

find_package(mumps)

# ------------------------------------------------------------------------------
# Collect all additional include directories necessary for the Mumps module
# ------------------------------------------------------------------------------
INCLUDE_DIRECTORIES(${ChronoEngine_UNIT_MUMPS_INCLUDE})
SET(ChronoEngine_UNIT_MUMPS_INCLUDE
    ${CH_MUMPS_INCLUDES}
)

SET(CH_MUMPS_INCLUDES "${CH_MUMPS_INCLUDES}" PARENT_SCOPE)
MESSAGE(STATUS "${CH_MUMPS_INCLUDES}")

# ------------------------------------------------------------------------------
# List all files in the Chrono mumps module
# ------------------------------------------------------------------------------

SET(ChronoEngine_MUMPS_HEADERS
  ChApiMumps.h
  ChLcpMumpsSolver.h
  ChMumpsEngine.h
  ChAssembledMatrix.h
)

SET(ChronoEngine_MUMPS_SOURCES
  ChLcpMumpsSolver.cpp
  ChMumpsEngine.cpp
  ChAssembledMatrix.cpp
)

SOURCE_GROUP("" FILES ${ChronoEngine_MUMPS_HEADERS} ${ChronoEngine_MUMPS_SOURCES})

# ------------------------------------------------------------------------------
# Add the ChronoEngine_mumps library
# ------------------------------------------------------------------------------
LINK_DIRECTORIES("C:/Program Files (x86)/Intel/Composer XE 2015/compiler/lib/intel64")

ADD_LIBRARY(ChronoEngine_mumps SHARED
            ${ChronoEngine_MUMPS_SOURCES}
            ${ChronoEngine_MUMPS_HEADERS})

SET_TARGET_PROPERTIES(ChronoEngine_mumps PROPERTIES
                      COMPILE_FLAGS "${CH_CXX_FLAGS}"
                      LINK_FLAGS "${CH_LINKERFLAG_SHARED}"
                      COMPILE_DEFINITIONS "CH_API_COMPILE_MUMPS")

TARGET_LINK_LIBRARIES(ChronoEngine_mumps
                      ChronoEngine
                      ${CH_MUMPS_LIBRARIES})
					  
TARGET_INCLUDE_DIRECTORIES(ChronoEngine_mumps PUBLIC
							${CH_MUMPS_INCLUDES})

INSTALL(TARGETS ChronoEngine_mumps
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

INSTALL(FILES ${ChronoEngine_MUMPS_HEADERS} 
        DESTINATION include/chrono_mumps)