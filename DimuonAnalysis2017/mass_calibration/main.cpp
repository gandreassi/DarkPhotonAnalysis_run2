//#include "mass_calibration.cpp"
#include <TString.h>

void mass_calibration(TString year);

int main (int argc, char *argv[]) {
  
  if (argc > 1){
    return 0;
  }

  mass_calibration("2017");
  mass_calibration("2018");


  return 0;
}
