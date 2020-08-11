#include <TString.h>

void makeCardsAndWS(TString year);

int main (int argc, char *argv[]) {
  
  if (argc > 1){
    return 0;
  }

  makeCardsAndWS("2017");
  makeCardsAndWS("2018"); //not combined yet! TODO


  return 0;
}
