CXX=g++
CXXFLAGS=-O3 -fPIC -Wall
CFLAGS=`root-config --cflags`
LDFLAGS=`root-config --ldflags --glibs` -lRooFit -lRooFitCore -lMinuit

ROODCBIR=../RooFit-pdfs
PDFSDIR=./pdfs

MACRO=mass_calibration

all: $(MACRO)


$(MACRO): $(PDFSDIR)/src/pdfs.cpp $(ROODCBIR)/src/RooDoubleCB.cxx $(ROODCBIR)/RooDoubleCBDict.cxx  main.cpp mass_calibration.cpp $(ROODCBIR)/libRooDoubleCB.so
	@echo "---> Making mass_calibration.exe..."
	$(CXX) $(CXXFLAGS) $(CFLAGS) $(LDFLAGS) -I $(ROODCBIR)/include -I $(PDFSDIR)/include -I $(ROODCBIR) $(PDFSDIR)/src/pdfs.cpp $(ROODCBIR)/src/RooDoubleCB.cxx $(ROODCBIR)/RooDoubleCBDict.cxx  main.cpp mass_calibration.cpp -o $(MACRO).exe


clean:
	rm -f $(MACRO).exe


