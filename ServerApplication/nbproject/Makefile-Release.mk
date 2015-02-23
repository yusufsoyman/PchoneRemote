#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Database\ Connections/DBAdapter.o \
	${OBJECTDIR}/Logger/Logger.o \
	${OBJECTDIR}/Network\ Handler/NetworkHandler.o \
	${OBJECTDIR}/SoundController.o \
	${OBJECTDIR}/main.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f1

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/soundcontroller

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/soundcontroller: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/soundcontroller ${OBJECTFILES} ${LDLIBSOPTIONS}

.NO_PARALLEL:${OBJECTDIR}/Database\ Connections/DBAdapter.o
${OBJECTDIR}/Database\ Connections/DBAdapter.o: Database\ Connections/DBAdapter.cpp 
	${MKDIR} -p ${OBJECTDIR} Connections
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Database\ Connections/DBAdapter.o Database\ Connections/DBAdapter.cpp

${OBJECTDIR}/Logger/Logger.o: Logger/Logger.cpp 
	${MKDIR} -p ${OBJECTDIR}/Logger
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Logger/Logger.o Logger/Logger.cpp

.NO_PARALLEL:${OBJECTDIR}/Network\ Handler/NetworkHandler.o
${OBJECTDIR}/Network\ Handler/NetworkHandler.o: Network\ Handler/NetworkHandler.cpp 
	${MKDIR} -p ${OBJECTDIR} Handler
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Network\ Handler/NetworkHandler.o Network\ Handler/NetworkHandler.cpp

${OBJECTDIR}/SoundController.o: SoundController.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SoundController.o SoundController.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-conf ${TESTFILES}
${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/soundTest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS} 


${TESTDIR}/tests/soundTest.o: tests/soundTest.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I. -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/soundTest.o tests/soundTest.cpp


.NO_PARALLEL:${OBJECTDIR}/Database\ Connections/DBAdapter.o
${OBJECTDIR}/Database\ Connections/DBAdapter_nomain.o: ${OBJECTDIR}/Database\ Connections/DBAdapter.o Database\ Connections/DBAdapter.cpp 
	${MKDIR} -p ${OBJECTDIR}/Database\ Connections
	@NMOUTPUT=`${NM} ${OBJECTDIR}/Database\ Connections/DBAdapter.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Database\ Connections/DBAdapter_nomain.o Database\ Connections/DBAdapter.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/Database\ Connections/DBAdapter.o ${OBJECTDIR}/Database\ Connections/DBAdapter_nomain.o;\
	fi

${OBJECTDIR}/Logger/Logger_nomain.o: ${OBJECTDIR}/Logger/Logger.o Logger/Logger.cpp 
	${MKDIR} -p ${OBJECTDIR}/Logger
	@NMOUTPUT=`${NM} ${OBJECTDIR}/Logger/Logger.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Logger/Logger_nomain.o Logger/Logger.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/Logger/Logger.o ${OBJECTDIR}/Logger/Logger_nomain.o;\
	fi

.NO_PARALLEL:${OBJECTDIR}/Network\ Handler/NetworkHandler.o
${OBJECTDIR}/Network\ Handler/NetworkHandler_nomain.o: ${OBJECTDIR}/Network\ Handler/NetworkHandler.o Network\ Handler/NetworkHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/Network\ Handler
	@NMOUTPUT=`${NM} ${OBJECTDIR}/Network\ Handler/NetworkHandler.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Network\ Handler/NetworkHandler_nomain.o Network\ Handler/NetworkHandler.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/Network\ Handler/NetworkHandler.o ${OBJECTDIR}/Network\ Handler/NetworkHandler_nomain.o;\
	fi

${OBJECTDIR}/SoundController_nomain.o: ${OBJECTDIR}/SoundController.o SoundController.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/SoundController.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SoundController_nomain.o SoundController.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/SoundController.o ${OBJECTDIR}/SoundController_nomain.o;\
	fi

${OBJECTDIR}/main_nomain.o: ${OBJECTDIR}/main.o main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/main.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main_nomain.o main.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/main.o ${OBJECTDIR}/main_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f1 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/soundcontroller

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
