CXX=g++
CXXFLAGS=-O3 -fPIC -Wall

ROOT_FLAGS=$(shell $(ROOTSYS)/bin/root-config --cflags)
ROOT_INCLUDE=$(shell $(ROOTSYS)/bin/root-config  --incdir)

BUILDDIR=.
OBJ_DIR=.


ROOT_LIBS    = $(shell $(ROOTSYS)/bin/root-config --ldflags --glibs) -lTreePlayer -lMinuit -lXMLIO -lMLP -lRIO -lTMVA 
GLIBS := $(shell root-config --glibs)
FLAGS = $(CXXFLAGS)
FLAGS += ${ROOT_FLAGS}
FLAGS_ROOFIT = $(FLAGS)
FLAGS_ROOFIT += -lRooFitCore -lRooFit -lRooStats -lFoam

ROODCBIR=../RooFit-pdfs
PDFSDIR=./pdfs

MACRO=mass_calibration

all: $(MACRO)


$(MACRO): $(PDFSDIR)/src/pdfs.cpp $(ROODCBIR)/src/RooDoubleCB.cxx RooDoubleCBDict.cxx  main.cpp mass_calibration.cpp libRooDoubleCB.so
	@echo "---> Making mass_calibration.exe..."
	$(CXX) $(CXXFLAGS) $(ROOT_LIBS) $(GLIBS) $(FLAGS_ROOFIT) -I $(ROODCBIR)/include -I $(PDFSDIR)/include -I $(ROOT_INCLUDE) $(PDFSDIR)/src/pdfs.cpp $(ROODCBIR)/src/RooDoubleCB.cxx RooDoubleCBDict.cxx  main.cpp mass_calibration.cpp -o $(MACRO).exe

libRooDoubleCB.so : libRooDoubleCB.so.1.0
	ln -sf $^ $@

libRooDoubleCB.so.1.0 : RooDoubleCB.o RooDoubleCBDict.o
	gcc -shared -Wl,-soname,libRooDoubleCB.s

RooDoubleCBDict.o : RooDoubleCBDict.cxx
	g++ -c $^ -fPIC $(CXXFLAGS) $(ROOT_LIBS) $(GLIBS) $(FLAGS_ROOFIT) -o $@

RooDoubleCBDict.cxx : $(ROODCBIR)/include/RooDoubleCB.h $(ROODCBIR)/LinkDef.h
	rootcint -f RooDoubleCBDict.cxx -c $(ROODCBIR)/include/RooDoubleCB.h $(ROODCBIR)/LinkDef.h 

RooDoubleCB.o : $(ROODCBIR)/src/RooDoubleCB.cxx
	g++ -c $^ -fPIC $(CXXFLAGS) $(ROOT_LIBS) $(GLIBS) $(FLAGS_ROOFIT) -I $(ROODCBIR)/include -o $@ 


clean:
	rm -f $(BUILDDIR)/$(MACRO).exe RooDoubleCB.o RooDoubleCBDict.cxx RooDoubleCBDict.o libRooDoubleCB.so.1.0 libRooDoubleCB.so RooDoubleCBDict_rdict.pcm 


