CXX=g++
CXXFLAGS=-O3 -fPIC -Wall
CFLAGS=`root-config --cflags`
LDFLAGS=`root-config --ldflags --glibs` -lRooFit -lRooFitCore -lMinuit

MACRO=mass_calibration

libRooDoubleCB.so: RooDoubleCBDict.cxx
	g++ -shared -o $@ $(CXXFLAGS) $(CFLAGS) -Iinclude $^

RooDoubleCBDict.o : RooDoubleCBDict.cxx
	g++ -c $^ -fPIC $(CXXFLAGS) $(CFLAGS) -o $@

RooDoubleCBDict.cxx : include/RooDoubleCB.h LinkDef.h
	rootcint -f $@ -c $(FLAGS)  -p include/RooDoubleCB.h LinkDef.h

RooDoubleCB.o : src/RooDoubleCB.cxx
	g++ -c $^ -fPIC $(CFLAGS) $(LDFLAGS) -o $@ 

clean:
	rm -f RooDoubleCB.o RooDoubleCBDict.cxx RooDoubleCBDict.o libRooDoubleCB.so.1.0 libRooDoubleCB.so RooDoubleCBDict_rdict.pcm 