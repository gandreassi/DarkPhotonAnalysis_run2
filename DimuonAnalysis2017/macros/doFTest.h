#ifndef DOFTEST_H
#define DOFTEST_H

#include "doFit.h"

double doFTest(TH1* hist, double mh, int order1, int order2) {
    
    if (order1 <= order2) -1.0;

    std::map<std::string, double> p1 = fit("fit1", hist, mh, "bkgpol", order1, false);
    std::map<std::string, double> p2 = fit("fit2", hist, mh, "bkgpol", order2, false);

    double pvalue = ROOT::Math::chisquared_cdf_c(p2["nll2"] - p1["nll2"], 2*(order1-order2));
    double chi2   = p1["chi2"]/double(hist->GetNbinsX()-order1);
    std::cout << "P-value for order(" << order1 << ", " << order2 << ") : " << ROOT::Math::chisquared_cdf_c(p2["nll2"] - p1["nll2"], 2*(order1-order2)) << " ; Chi2/ndof : " << chi2 << std::endl;
   
    if (chi2 > 2.) pvalue = 0.; 
    return pvalue;
}

int getBestOrder(TH1* hist, double mh, int nbins, double xmin, double xmax) {

    RooMsgService::instance().setSilentMode(kTRUE);
    RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING) ;

    int order = 1;
    while (doFTest(hist, mh, order+1, order) < 0.05) order++;

    return order;
}

int getBestOrder(TH1* hist, double mh, double range, double binfact) {

    RooMsgService::instance().setSilentMode(kTRUE);
    RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING) ;

    int nbins = int(2.0*mh*range*binfact);
    double xmin = (1.-range)*mh;
    double xmax = (1.+range)*mh;

    return getBestOrder(hist, mh, nbins, xmin, xmax);

}

#endif
