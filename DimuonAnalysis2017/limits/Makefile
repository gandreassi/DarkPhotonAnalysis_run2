CXX=g++
CXXFLAGS=-O3 -fPIC -Wall
CFLAGS=`root-config --cflags`
LDFLAGS=`root-config --ldflags --glibs` -lRooFit -lRooFitCore -lMinuit

ROODCBIR=../RooFit-pdfs
PDFSDIR=../mass_calibration/pdfs

MACRO=makeCardsAndWS

all: $(MACRO)

$(MACRO):  $(PDFSDIR)/src/pdfs.cpp $(ROODCBIR)/src/RooDoubleCB.cxx $(ROODCBIR)/RooDoubleCBDict.cxx $(ROODCBIR)/libRooDoubleCB.so  makeCardsAndWS.cpp 
	@echo "---> Making makeCardsAndWS.exe..."
	$(CXX) $(CXXFLAGS) $(CFLAGS) $(LDFLAGS) -I $(ROODCBIR)/include -I $(PDFSDIR)/include -I $(ROODCBIR) $(PDFSDIR)/src/pdfs.cpp $(ROODCBIR)/src/RooDoubleCB.cxx $(ROODCBIR)/RooDoubleCBDict.cxx  main.cpp $(MACRO).cpp -o $(MACRO).exe

clean:
	rm -f $(MACRO).exe

