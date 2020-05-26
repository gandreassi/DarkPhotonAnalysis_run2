#ifndef DOFIT_H
#define DOFIT_H

#include "HiggsAnalysis/CombinedLimit/interface/HZZ2L2QRooPdfs.h"

/*

Arguments to the doFit function : 

hist    : Pointer to a histogram that is to be fit. The histogram should to normalized to the expected yield in case of MC
fittype : Can be "signal", "bkgpol", "bkgpow" --> Use signal and bkgpol for now
order   : For signal order = 0 -> Gaussian, 1 -> one-sided Crystal Ball, 2-> two-sided crystal ball
        : For background, specifies the order of polynomial
draw    : If true, shows the fit

*/

std::map<std::string, double> fit(std::string name, TH1* hist, double mh, std::string fittype, int order, bool draw=false) {

    std::map<std::string, double> params;

    if (!hist) {
        std::cout << "Invalid input histogram" << std::endl;
        return params;
    }

    if (order < 0) {
        std::cout << "Invalid fit order" << std::endl;
        return params;
    }
    if (fittype == "bkgpol" && order == 0) {
        std::cout << "Invalid fit order" << std::endl;
        return params;
    }

    RooRealVar mass ("m", "m",  mh, hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax());
    RooDataHist datahist("datahist", "datahist", RooArgList(mass), hist);
    RooAbsPdf* pdf = NULL;

    RooArgList argl;
    std::vector<RooRealVar*> bargs;
    if (fittype == "bkgpol") {
        for (std::size_t i = 1; i <= order; i++) {
            std::stringstream argname_ss;
            argname_ss << "c" << i;
            bargs.push_back(new RooRealVar(argname_ss.str().c_str(), ""  ,  50.0, -100., 100.));
            argl.add(*bargs.back());
        }
        pdf = new RooBernstein("bpdf", "", mass, argl);
    }

    if (fittype == "bkgpow") {
        argl.add(mass);
        for (std::size_t i = 1; i <= 2* order; i++) {
            std::stringstream argname_ss;
            argname_ss << "c" << i;
            if      (i%2 == 1) bargs.push_back(new RooRealVar(argname_ss.str().c_str(), ""  ,  1.0, -1000. , 1000. ));
            else if (i%2 == 0) bargs.push_back(new RooRealVar(argname_ss.str().c_str(), ""  ,  0.0, -100.  , 100.  ));
            argl.add(*bargs.back());
        }
        std::stringstream pdf_ss;
        pdf_ss << "1+@1*pow(@0,@2)";
        for (std::size_t i = 2; i <= order; i++) pdf_ss << " + @" << 2*i-1 << "*pow(@0," << 2*i << ")";
        pdf = new RooGenericPdf("bpdf", "", pdf_ss.str().c_str(), argl);
    }

    RooRealVar m0   ("m0", ""  ,    mh        ,  mh-2., mh+2. );
    RooRealVar si   ("si", ""  ,    mh*0.012  , 0.0   , mh*0.1);
    RooRealVar aL   ("aL", ""  ,   2.0        , 0.0   , 10.0  );
    RooRealVar nL   ("nL", ""  ,   2.0        , 0.0   , 10.0  );
    RooRealVar aR   ("aR", ""  ,   2.0        , 0.0   , 10.0  );
    RooRealVar nR   ("nR", ""  ,   2.0        , 0.0   , 10.0  );
    if (fittype == "signal") {
        if (order == 0) pdf = new RooGaussian("spdf", "", mass, m0, si);
        if (order == 1) pdf = new RooCBShape ("spdf", "", mass, m0, si, aL, nL);
        if (order == 2) pdf = new RooDoubleCB("spdf", "", mass, m0, si, aL, nL, aR, nR);
    }

    if (pdf == NULL) {
        std::cout << "Unable to construct the PDF" << std::endl;
        return params;
    }

    pdf->fitTo(datahist, RooFit::SumW2Error(kTRUE));
    pdf->fitTo(datahist, RooFit::SumW2Error(kTRUE));
    pdf->fitTo(datahist, RooFit::SumW2Error(kTRUE));

    if (draw) {
        TCanvas* canvas = new TCanvas((string("canvas_")+name).c_str(), (string("canvas_")+name).c_str(), 600, 600);
        RooPlot* frame = mass.frame();
        datahist.plotOn(frame, RooFit::Binning(hist->GetNbinsX()), RooFit::DataError(RooAbsData::SumW2));
        pdf    ->plotOn(frame, RooFit::LineColor(kOrange+1));
        datahist.plotOn(frame, RooFit::Binning(hist->GetNbinsX()), RooFit::DataError(RooAbsData::SumW2));
        frame->Draw();
    }

    params["yield"] = hist->Integral();

    if (fittype == "bkgpol") {
        for (std::size_t i = 1; i <= order; i++) {
            std::stringstream argname_ss;
            argname_ss << "c" << i;
            params[argname_ss.str().c_str()] = bargs[i-1]->getVal();
        }
        params["nll2"]  = 2*pdf->createNLL (datahist)->getVal();
        params["chi2"]  =   pdf->createChi2(datahist)->getVal();
    }

    if (fittype == "signal") {
        params["m0"] = mh - m0.getVal();
        params["si"] = si.getVal();
        params["aL"] = aL.getVal();
        params["nL"] = nL.getVal();
        params["aR"] = aR.getVal();
        params["nR"] = nR.getVal();

        params["em0"] = m0.getError(); 
        params["esi"] = si.getError(); 
        params["eaL"] = aL.getError(); 
        params["enL"] = nL.getError(); 
        params["eaR"] = aR.getError(); 
        params["enR"] = nR.getError(); 
    }

    for (std::size_t i = 0; i < bargs.size(); i++) {
        if (bargs[i]) delete bargs[i];        
    }
    if (pdf != NULL) delete pdf;
    return params;
}


#endif 
