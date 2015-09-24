# Additional include paths
INCLUDE_DIRECTORIES( ${CH_MUMPS_INCLUDES} )
 
MESSAGE(STATUS "Demo programs for Mumps module...")

# ---------------------------------------------------------------------

MESSAGE(STATUS "...add demo_mumps")
ADD_EXECUTABLE(demo_mumps demo_mumps.cpp)
SOURCE_GROUP("" FILES demo_mumps.cpp)
SET_TARGET_PROPERTIES(demo_mumps PROPERTIES 
                      FOLDER demos
                      LINK_FLAGS "${CH_LINKERFLAG_EXE}")
TARGET_LINK_LIBRARIES(demo_mumps ChronoEngine ChronoEngine_mumps)
ADD_DEPENDENCIES(demo_mumps ChronoEngine ChronoEngine_mumps)
INSTALL(TARGETS demo_mumps DESTINATION bin)



