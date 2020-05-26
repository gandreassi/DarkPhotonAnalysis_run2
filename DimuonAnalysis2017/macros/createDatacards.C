#include "HiggsAnalysis/CombinedLimit/interface/HZZ2L2QRooPdfs.h"
#include "doFit.h"
#include "doFTest.h"
#include "CardTemplate.h"

#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>

#include <RooArgSet.h>
#include <RooArgList.h>
#include <RooDataSet.h>
#include <RooRealVar.h> 
#include <RooFormulaVar.h> 
#include <RooWorkspace.h> 
#include <RooMsgService.h> 

void makeWorkspace(double mh, TH1* data, std::map<std::string, double> sparams, std::map<std::string, double> bparams, std::string cat, bool maketoy=true, bool useSignalInterpol=false) {

    RooMsgService::instance().setSilentMode(kTRUE);
    RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING) ;

    stringstream mh_ss;
    mh_ss << mh;
    
    std::cout << "Creating datacard for " << mh_ss.str() << " GeV mass point, category " << cat << " ... " << std::endl;
   
    std::stringstream card_name_ss;
    card_name_ss << "card_";
    card_name_ss << "m" << mh_ss.str() << "_";
    card_name_ss << cat;
    std::string card_name = card_name_ss.str();

    std::string workspace = card_name+"_workspace.root";

    /* Dark photon mass and dimuon mass variables */

    const char* massvarstr  = "CMS_darkphoton_mass";
    const char* scalevarstr = "CMS_darkphoton_scale";
    const char* resvarstr   = "CMS_darkphoton_res";

    int    fitbins  = data->GetNbinsX();
    double massLow  = data->GetXaxis()->GetXmin();
    double massHigh = data->GetXaxis()->GetXmax();

    std::cout << "Will perform a binned fit with " << fitbins << " bins in the mass range " << massLow << " to " << massHigh << std::endl; 

    RooRealVar rmh  ("MH"       , "MH"         , mh);
    RooRealVar m2mu (massvarstr , "Dimuon mass", mh  , massLow, massHigh, "GeV/c^{2}");
    RooRealVar scale(scalevarstr, "Scale unc. ", 0.0 , 0.0    , 1.0     , "GeV/c^{2}");
    RooRealVar res  (resvarstr  , "RFes. unc. ", 0.0 , 0.0    , 1.0);
    m2mu.setBins(data->GetNbinsX());   

    /* Extract shape parameters */

    std::string spdf = "sig_mass_";
    std::string bpdf = "bkg_mass_";

    spdf += cat;
    bpdf += cat;

    RooRealVar sig_norm((spdf+"_pdf_norm").c_str(), "", sparams["yield"]);
    RooRealVar bkg_norm((bpdf+"_pdf_norm").c_str(), "", bparams["yield"]);

    std::cout << "Expected signal     yield : " << sig_norm.getVal() << std::endl;
    std::cout << "Expected background yield : " << bkg_norm.getVal() << std::endl;

    sig_norm.setConstant(kTRUE);
    bkg_norm.setConstant(kTRUE);

    /* Define PDFs */

    // Background
    int bkgorder = bparams.size() - 3;
    RooArgList argl;
    std::vector<RooRealVar*> bargs;
    for (std::size_t i = 1; i <= bkgorder; i++) {
        std::stringstream argname_ss;
        argname_ss << "c" << i;
        double argval = bparams[argname_ss.str().c_str()];
        bargs.push_back(new RooRealVar(argname_ss.str().c_str(), "", argval, argval-500., argval+500.));
        argl.add(*bargs.back());
    }
    RooBernstein bkg_mass_pdf(("bkg_mass_"+cat+"_pdf" ).c_str(), "", m2mu, argl);
   
    // Signal
    std::stringstream meanss;
    std::stringstream sigmass;
    double aLval = 0.0;
    double aRval = 0.0;
    double nLval = 0.0;
    double nRval = 0.0;

    if (!useSignalInterpol) {
        meanss  << "@0 - " << sparams["m0"]  << " + " << "@0*@1";
        sigmass << sparams["si"]   << " * " << "(1+@1)";
        aLval = sparams["aL"];
        aRval = sparams["aR"];
        nLval = sparams["nL"];
        nRval = sparams["nR"];
    }
    else {
        meanss  << "35 + 0.99785*(@0-35)"        << " + " << "@0*@1";
        sigmass << "(0.3762 + 0.012223*(@0-35))" << " * " << "(1+@1)";
        aLval = 1.26833918722;
        aRval = 1.2945031338;
        nLval = 2.76027985241;
        nRval = 9.59850913168;
    }

    RooFormulaVar fmean ((spdf+"_fmean" ).c_str(), "", meanss .str().c_str(), RooArgList(rmh, scale));
    RooFormulaVar fsigma((spdf+"_fsigma").c_str(), "", sigmass.str().c_str(), RooArgList(rmh, res  ));
    RooRealVar    raL   ((spdf+"_aL"    ).c_str(), "", aLval);
    RooRealVar    rnL   ((spdf+"_nL"    ).c_str(), "", nLval);
    RooRealVar    raR   ((spdf+"_aR"    ).c_str(), "", aRval);
    RooRealVar    rnR   ((spdf+"_nR"    ).c_str(), "", nRval);

    RooDoubleCB sig_mass_pdf(("sig_mass_"+cat+"_pdf" ).c_str(), "", m2mu, fmean, fsigma, raL, rnL, raR, rnR);

    /* RooDataSet of the observed data */

    TH1F dhist("dhist", "", fitbins, massLow, massHigh);

    if (maketoy) {
        std::cout << "Generating toy data with " << int(bparams["yield"]) << " events\n";
        RooDataSet* dset = bkg_mass_pdf.generate(m2mu, int(bparams["yield"]));
        for (int i = 0; i < dset->numEntries(); i++) {
            const RooArgSet* aset = dset->get(i);
            double mass = aset->getRealValue(massvarstr);
            dhist.Fill(mass);
        }
    }

    RooDataHist data_obs("data_obs", "", RooArgList(m2mu), (maketoy ? &dhist : data));

    RooWorkspace w("w", "");

    w.import(data_obs);
    w.import(sig_norm);
    w.import(bkg_norm);
    w.import(sig_mass_pdf);
    w.import(bkg_mass_pdf);

    w.writeToFile(workspace.c_str());

    /* Create the data card text file */

    std::string card = createCardTemplate(mh, cat, workspace);
    std::ofstream ofile;
    ofile.open ((card_name +".txt").c_str());
    ofile << card;
    ofile.close();

    for (std::size_t i = 0; i < bargs.size(); i++) {
        if (bargs[i]) delete bargs[i];        
    }
}

#include "getTrimScoutHist.h"

void makeDatacard(double mh, int nbins, double xmin, double xmax, std::string cat, int order) {

    std::string bkgpath;
    std::string sigpath;
    
    bkgpath = "/media/Disk1/avartak/CMS/Data/Dileptons/ScoutingPFMuons/trimscout_RunD.root";

    std::stringstream sigfname_ss;
    sigfname_ss << "/media/Disk1/avartak/CMS/Data/Dileptons/DarkPhoton/trimscout_M" << int(mh) << ".root";
    sigpath = sigfname_ss.str();
    
    TH1F* sighist = new TH1F("sighist", "", nbins, xmin, xmax);
    TH1F* bkghist = new TH1F("bkghist", "", nbins, xmin, xmax);
    
    getTrimScoutHist(bkgpath, bkghist, true , false, false);
    getTrimScoutHist(sigpath, sighist, true , true , false);
    
    if (order < 0) order = getBestOrder(bkghist, mh, nbins, xmin, xmax);
 
    std::map<std::string, double> sparams = fit("sig", sighist, mh, "signal", 2    , false);
    std::map<std::string, double> bparams = fit("bkg", bkghist, mh, "bkgpol", order, false);
    
    std::cout << "Best polynomial order : " << order << std::endl;

    makeWorkspace(mh, bkghist, sparams, bparams, cat, false);

    delete sighist;
    delete bkghist;
}


void makeDatacard(double mh, double range, double binfact, std::string cat, int order) {

    int nbins = int(2*binfact);
    double xmin = (1.-range)*mh;
    double xmax = (1.+range)*mh;

    makeDatacard(mh, nbins, xmin, xmax, cat, order);

}


void createDatacards() {

    makeDatacard(12., 0.10, 50., "ZZ", -1);    
    makeDatacard(15., 0.10, 50., "ZZ", -1);    
    makeDatacard(20., 0.10, 50., "ZZ", -1);    
    makeDatacard(25., 0.10, 50., "ZZ", -1);    
    makeDatacard(30., 0.10, 50., "ZZ", -1);    
    makeDatacard(35., 0.10, 50., "ZZ", -1);    
    makeDatacard(40., 0.10, 50., "ZZ", -1);    
    makeDatacard(45., 0.10, 50., "ZZ", -1);    
    makeDatacard(50., 0.10, 50., "ZZ", -1);    
    makeDatacard(55., 0.10, 50., "ZZ", -1);    
    makeDatacard(60., 0.10, 50., "ZZ", -1);    
    makeDatacard(65., 0.10, 50., "ZZ", -1);    
    makeDatacard(70., 0.10, 50., "ZZ", -1);    

}
