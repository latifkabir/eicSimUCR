# Filename: Makefile
# Description: Makefile for compiling eicSimUCR
# Author: Latif Kabir < kabir@bnl.gov >
# Created: Wed May 15 01:12:25 2019 (-0500)
# URL: jlab.org/~latif

ifndef EIC_SIM_UCR
  $(error $$EIC_SIM_UCR environment variable not defined. source setup.[c]sh first)
endif

ROOTCINT  = rootcint
ROOTFLAGS = $(shell root-config --cflags)
ROOTLIBS  = $(shell root-config --glibs)
CXXFLAGS  += -fPIC -std=c++11 -L ${EIC_SIM_UCR}/pythia8/lib -lpythia8 -L ${EIC_SIM_UCR}/fastjet/lib -lfastjet

LIB_INCLUDE  = -I${EIC_SIM_UCR}/root6/include -I${EIC_SIM_UCR}/pythia8/include -I${EIC_SIM_UCR}/fastjet/include

all: libeicSimUCR

clean: libeicSimUCR_clean
clear: libeicSimUCR_clear


libeicSimUCR: lib/libeicSimUCR.so
libeicSimUCR_base= $(wildcard src/*.cxx)  
libeicSimUCR_inc	= $(patsubst src/%.cxx, ./%.h,$(libeicSimUCR_base)) 
libeicSimUCR_obj	= $(patsubst src/%.cxx, src/%.o,$(libeicSimUCR_base))

lib/libeicSimUCR.so: $(libeicSimUCR_obj)  src/libeicSimUCR_Dict.o
	$(CXX) -o $@ $(CXXFLAGS) -shared -Wl,-soname,libeicSimUCR.so $^
	cp -u src/*.pcm lib/	
	@echo "                                                                        "
	@echo "-----------------------------------------------------------------------"
	@echo "Finished the compilation!"
	@echo "-----------------------------------------------------------------------"
src/%.o:src/%.cxx
	$(CXX) -c -o $@ $(CPPFLAGS) $(CXXFLAGS) $(LIB_INCLUDE) $(ROOTFLAGS) $<

src/libeicSimUCR_Dict.cxx:
	cd src/; $(ROOTCINT) -f libeicSimUCR_Dict.cxx -c $(libeicSimUCR_inc) LinkDef.h

libeicSimUCR_clean:
	rm -f $(libeicSimUCR_obj)
	rm -f src/*_Dict.*
	rm -f lib/*.pcm
	rm -f lib/libeicSimUCR.so

libeicSimUCR_clear:
	rm -f $(libeicSimUCR_obj)
	rm -f  src/*_Dict.*


