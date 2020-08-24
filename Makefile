# generic Makefile.(Linux only)
# Copyright 2006 - 2020 Eric Bachard
# This document is under GPL v2 license. See : http://www.gnu.org/licenses/gpl-2.0.html

# simple Makefile, to start with make ...
BUILD_DIR = .
VERSION_MAJOR = 0
VERSION_MINOR = 2
VERSION_MICRO = 0_devel
VERSION = ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_MICRO}

SOURCES_DIR = src
APPLICATION_NAME = alsa_record
BINARY_NAME = ${BUILD_DIR}/${APPLICATION_NAME}
CC = gcc
CXX = g++
CC_FLAGS = -Wall -Wextra
CC_STD = -std=c++11
OS_FLAGS =

LD_FLAGS += -lasound -lpthread

OUTBIN = ${BUILD_DIR}/${APPLICATION_NAME}
OUTBIN_DEBUG = ${BUILD_DIR}/${APPLICATION_NAME}_debug
OUTBIN_ALL = 	$(OUTBIN) 	$(OUTBIN_DEBUG)

INCLUDE_DIR = -Iinc

OBJS =  \
        ${SOURCES_DIR}/getch.c \
        ${SOURCES_DIR}/alsa_record.cpp

DEPS = $^

GCC_SECURITY_FLAGS = -fstack-protector -pie -fPIE -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security

CFLAGS = ${CC_FLAGS} ${CC_STD} ${OS_FLAGS}
CFLAGS_DEBUG = -g -DDEBUG

ARCHIVE_EXT = .tar.gz

TO_BE_ZIPPED = \
    inc \
    src \
    Makefile \
    README.md \
    wav_format.txt \
    wav_format.pdf

TO_BE_ZIPPED_BINARIES = \
    ${BUILD_DIR}


all : $(OUTBIN)

$(OUTBIN) : $(OBJS)
	$(CXX) $(CFLAGS) ${GCC_SECURITY_FLAGS} $(INCLUDE_DIR) -o $@ $(DEPS)  $(LD_FLAGS)

$(OUTBIN_DEBUG) : $(OBJS)
	$(CXX) $(CFLAGS) ${GCC_SECURITY_FLAGS} $(CFLAGS_DEBUG) $(INCLUDE_DIR) -o $(OUTBIN_DEBUG) $(DEPS) $(LD_FLAGS)

clean :
	rm -rf $(BUILD_DIR)/*.o
	rm -rf $(OUTBIN) $(OUTBIN_DEBUG) ${EXEC_NAME}_${VERSION}
	rm -rf ${APPLICATION_NAME}_${VERSION}
	echo Fichiers binaires supprimés.

devel: clean
	mkdir ${APPLICATION_NAME}_${VERSION}
	mkdir -p ${APPLICATION_NAME}_${VERSION}/build
	cp -R ${TO_BE_ZIPPED} ${APPLICATION_NAME}_${VERSION}
	tar cvzf ${APPLICATION_NAME}_${VERSION}${ARCHIVE_EXT} ${APPLICATION_NAME}_${VERSION}

#	rm -rf ${APPLICATION_NAME}_${VERSION}


archive : all
	mkdir ${APPLICATION_NAME}_${VERSION}
	cp -R ${TO_BE_ZIPPED} ${APPLICATION_NAME}_${VERSION}
	cp -R ${TO_BE_ZIPPED_BINARIES} ${APPLICATION_NAME}_${VERSION}
	tar cvzf ${APPLICATION_NAME}_${VERSION}${ARCHIVE_EXT} ${APPLICATION_NAME}_${VERSION}

real_clean : clean
	rm -rf ${APPLICATION_NAME}_*${ARCHIVE_EXT}

