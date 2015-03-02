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
	${OBJECTDIR}/include/ConfigReader/ConfigReader.o \
	${OBJECTDIR}/include/Database\ Connections/DBAdapter.o \
	${OBJECTDIR}/include/Logger/Logger.o \
	${OBJECTDIR}/include/Network\ Handler/NetworkHandler.o \
	${OBJECTDIR}/include/ServerMain/ServerMain.o \
	${OBJECTDIR}/include/SoundController/SoundController.o \
	${OBJECTDIR}/main.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f2 \
	${TESTDIR}/TestFiles/f1 \
	${TESTDIR}/TestFiles/f3

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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/serverapplication

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/serverapplication: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/serverapplication ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/include/ConfigReader/ConfigReader.o: include/ConfigReader/ConfigReader.cpp 
	${MKDIR} -p ${OBJECTDIR}/include/ConfigReader
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/include/ConfigReader/ConfigReader.o include/ConfigReader/ConfigReader.cpp

.NO_PARALLEL:${OBJECTDIR}/include/Database\ Connections/DBAdapter.o
${OBJECTDIR}/include/Database\ Connections/DBAdapter.o: include/Database\ Connections/DBAdapter.cpp 
	${MKDIR} -p ${OBJECTDIR}/include Connections
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/include/Database\ Connections/DBAdapter.o include/Database\ Connections/DBAdapter.cpp

${OBJECTDIR}/include/Logger/Logger.o: include/Logger/Logger.cpp 
	${MKDIR} -p ${OBJECTDIR}/include/Logger
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/include/Logger/Logger.o include/Logger/Logger.cpp

.NO_PARALLEL:${OBJECTDIR}/include/Network\ Handler/NetworkHandler.o
${OBJECTDIR}/include/Network\ Handler/NetworkHandler.o: include/Network\ Handler/NetworkHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/include Handler
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/include/Network\ Handler/NetworkHandler.o include/Network\ Handler/NetworkHandler.cpp

${OBJECTDIR}/include/ServerMain/ServerMain.o: include/ServerMain/ServerMain.cpp 
	${MKDIR} -p ${OBJECTDIR}/include/ServerMain
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/include/ServerMain/ServerMain.o include/ServerMain/ServerMain.cpp

${OBJECTDIR}/include/SoundController/SoundController.o: include/SoundController/SoundController.cpp 
	${MKDIR} -p ${OBJECTDIR}/include/SoundController
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/include/SoundController/SoundController.o include/SoundController/SoundController.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-conf ${TESTFILES}
${TESTDIR}/TestFiles/f2: ${TESTDIR}/tests/dbtest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f2 $^ ${LDLIBSOPTIONS} 

${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/networktest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS} 

${TESTDIR}/TestFiles/f3: ${TESTDIR}/tests/soundTest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f3 $^ ${LDLIBSOPTIONS} 


${TESTDIR}/tests/dbtest.o: tests/dbtest.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I. -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/dbtest.o tests/dbtest.cpp


${TESTDIR}/tests/networktest.o: tests/networktest.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I. -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/networktest.o tests/networktest.cpp


${TESTDIR}/tests/soundTest.o: tests/soundTest.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I. -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/soundTest.o tests/soundTest.cpp


${OBJECTDIR}/include/ConfigReader/ConfigReader_nomain.o: ${OBJECTDIR}/include/ConfigReader/ConfigReader.o include/ConfigReader/ConfigReader.cpp 
	${MKDIR} -p ${OBJECTDIR}/include/ConfigReader
	@NMOUTPUT=`${NM} ${OBJECTDIR}/include/ConfigReader/ConfigReader.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/include/ConfigReader/ConfigReader_nomain.o include/ConfigReader/ConfigReader.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/include/ConfigReader/ConfigReader.o ${OBJECTDIR}/include/ConfigReader/ConfigReader_nomain.o;\
	fi

.NO_PARALLEL:${OBJECTDIR}/include/Database\ Connections/DBAdapter.o
${OBJECTDIR}/include/Database\ Connections/DBAdapter_nomain.o: ${OBJECTDIR}/include/Database\ Connections/DBAdapter.o include/Database\ Connections/DBAdapter.cpp 
	${MKDIR} -p ${OBJECTDIR}/include/Database\ Connections
	@NMOUTPUT=`${NM} ${OBJECTDIR}/include/Database\ Connections/DBAdapter.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/include/Database\ Connections/DBAdapter_nomain.o include/Database\ Connections/DBAdapter.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/include/Database\ Connections/DBAdapter.o ${OBJECTDIR}/include/Database\ Connections/DBAdapter_nomain.o;\
	fi

${OBJECTDIR}/include/Logger/Logger_nomain.o: ${OBJECTDIR}/include/Logger/Logger.o include/Logger/Logger.cpp 
	${MKDIR} -p ${OBJECTDIR}/include/Logger
	@NMOUTPUT=`${NM} ${OBJECTDIR}/include/Logger/Logger.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/include/Logger/Logger_nomain.o include/Logger/Logger.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/include/Logger/Logger.o ${OBJECTDIR}/include/Logger/Logger_nomain.o;\
	fi

.NO_PARALLEL:${OBJECTDIR}/include/Network\ Handler/NetworkHandler.o
${OBJECTDIR}/include/Network\ Handler/NetworkHandler_nomain.o: ${OBJECTDIR}/include/Network\ Handler/NetworkHandler.o include/Network\ Handler/NetworkHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/include/Network\ Handler
	@NMOUTPUT=`${NM} ${OBJECTDIR}/include/Network\ Handler/NetworkHandler.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/include/Network\ Handler/NetworkHandler_nomain.o include/Network\ Handler/NetworkHandler.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/include/Network\ Handler/NetworkHandler.o ${OBJECTDIR}/include/Network\ Handler/NetworkHandler_nomain.o;\
	fi

${OBJECTDIR}/include/ServerMain/ServerMain_nomain.o: ${OBJECTDIR}/include/ServerMain/ServerMain.o include/ServerMain/ServerMain.cpp 
	${MKDIR} -p ${OBJECTDIR}/include/ServerMain
	@NMOUTPUT=`${NM} ${OBJECTDIR}/include/ServerMain/ServerMain.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/include/ServerMain/ServerMain_nomain.o include/ServerMain/ServerMain.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/include/ServerMain/ServerMain.o ${OBJECTDIR}/include/ServerMain/ServerMain_nomain.o;\
	fi

${OBJECTDIR}/include/SoundController/SoundController_nomain.o: ${OBJECTDIR}/include/SoundController/SoundController.o include/SoundController/SoundController.cpp 
	${MKDIR} -p ${OBJECTDIR}/include/SoundController
	@NMOUTPUT=`${NM} ${OBJECTDIR}/include/SoundController/SoundController.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/include/SoundController/SoundController_nomain.o include/SoundController/SoundController.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/include/SoundController/SoundController.o ${OBJECTDIR}/include/SoundController/SoundController_nomain.o;\
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
	    ${TESTDIR}/TestFiles/f2 || true; \
	    ${TESTDIR}/TestFiles/f1 || true; \
	    ${TESTDIR}/TestFiles/f3 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/serverapplication

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
