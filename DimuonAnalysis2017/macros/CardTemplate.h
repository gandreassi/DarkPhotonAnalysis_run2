#ifndef CARDTEMPLETE_H
#define CARDTEMPLETE_H

#include <sstream>

std::string createCardTemplate(double mh, std::string cat, std::string workspacefilename) {

    std::stringstream binss;
    binss << "   bin" << cat << "      ";
    std::string bin = binss.str();

    std::string spdfstart = "w:sig_mass_";
    std::string bpdfstart = "w:bkg_mass_";

    std::stringstream card;
    card << "imax *                                                                                                                        \n";
    card << "jmax *                                                                                                                        \n";
    card << "kmax *                                                                                                                        \n";

    card << "------------                                                                                                                  \n";
    card << "shapes sig      " << bin << "     " << workspacefilename << "     " << spdfstart << cat << "_pdf                              \n";
    card << "shapes bkg      " << bin << "     " << workspacefilename << "     " << bpdfstart << cat << "_pdf                              \n";
    card << "shapes data_obs " << bin << "     " << workspacefilename << "     w:data_obs                                                  \n";
    card << "------------                                                                                                                  \n";
    card << "bin             " << bin                                                                                                 <<  "\n";
    card << "observation        -1                                                                                                         \n";
    card << "------------                                                                                                                  \n";

    card << "bin                               " << bin     <<   bin     <<   "\n";
    card << "process                              sig            bkg           \n";
    card << "process                              -1             1             \n";
    card << "rate                                  1             1             \n";
    card << "------------\n";
    card << "## Mass : "       << mh << std::endl;
    
    card << "lumi_13TeV                lnN        1.026          -             \n";
    card << "QCDscale                  lnN        1.20           -             \n";
    card << "pdf                       lnN        1.05           -             \n";
    card << "CMS_eff_m                 lnN        1.015          -             \n";
    card << "CMS_darkphoton_scale      param      0              0.001                                                                     \n";
    card << "CMS_darkphoton_res        param      0              0.2                                                                       \n";

    return card.str();
}

#endif
