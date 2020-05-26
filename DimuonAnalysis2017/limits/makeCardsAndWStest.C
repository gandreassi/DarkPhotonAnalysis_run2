#include <iostream>
#include <TLegend.h>
#include <sstream>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include "TH1D.h"
#include "TH2D.h"
#include <THStack.h>
#include "TProfile.h"
#include "TGraph.h"
#include "TFile.h"
#include "TTree.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TFractionFitter.h"
#include <string>
#include <vector>
#include <math.h>
#include <TLatex.h>
#include <TLine.h>
#include <TMarker.h>
#include <TPave.h>
#include <TPaveStats.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <TString.h>
#include "TGraphErrors.h"
#include "TF1.h"
#include "TEfficiency.h"


#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <iostream>
#include <valarray>




#include <RooPlot.h>
#include <RooArgSet.h>
#include <RooArgList.h>
#include <RooDataSet.h>
#include <RooDataHist.h>
#include <RooGaussian.h>
#include <RooPolynomial.h>
#include <RooBernstein.h>
#include <RooRealVar.h> 
#include <RooFormulaVar.h> 
#include <RooWorkspace.h> 
#include <RooMsgService.h> 

#include "pdfs.h"


using namespace std;

void makeCardsAndWStest(){
  

  TH1F* histChi2_A=new TH1F("histChi2_A","histChi2_A",50,0.5,1.5);


  //WHICH YEAR
  	TString year="2017";
	TString suff="IterV3";
  //INPUT FILE WITH HISTOGRAMS TO FIT BACKGROUND
  	TFile* file=TFile::Open("/eos/cms/store/group/phys_exotica/darkPhoton/jakob/2017/ScoutingRunD/ScoutingCaloMuon/crab_20181212_141716/181212_131724/0000/mergedHists_two.root");

  //PREPARE EXPECTED NUMBER OF SIGNAL EVENTS PER CATEGORY
	//X-SECTION GRAPH
	//double m[17] 		= {0.25, 0.5, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 12.5, 14.0, 16.0, 18.0, 20.0};
	//double xSec[17] 	= {10,  10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,  10,  10,  10,  10,  10};//[pb]
	double m[11] 		= {2.0,  3.0,  4.0,  5.0,  6.0,  7.0,  8.0,   9.0,   10.0,  12.5,  20.0};
	double xSec[11] 	= {8541, 7514, 3323, 2055, 1422, 1043, 793.6, 621.1, 484.3, 292.8, 98.95};//[pb], model-dependent
	//TGraph* xsecgraph 	= new TGraph(17,m,xSec);
	TGraph* xsecgraph 	= new TGraph(11,m,xSec);

	//ACCEPTANCE
	TFile* acc_file = TFile::Open("acceptances.root");
	TEfficiency* acc_teff = (TEfficiency*)acc_file->Get("cmsacc");
	int nbins_acc=acc_teff->GetPassedHistogram()->GetNbinsX();
	double acceptances[nbins_acc];
	double m_acceptances[nbins_acc];
	for (int j=1; j<=nbins_acc; j++){
		acceptances[j-1] = acc_teff->GetEfficiency(j);
		m_acceptances[j-1] = acc_teff->GetPassedHistogram()->GetBinCenter(j);
	}
	TGraph* accgraph 	= new TGraph(nbins_acc,m_acceptances,acceptances);

	//LUMINOSITY
	double luminosity = 4000.;//pb-1

	//EFFICIENCY
	//get acceptance from hist
	TFile* eff_file = TFile::Open("efficiency.root");
	TH1F* eff_hist = (TH1F*)eff_file->Get("massSel");
	int nbins_eff=eff_hist->GetNbinsX();
	double effA[nbins_eff];
	double m_effA[nbins_eff];
	for (int j=1; j<=nbins_eff; j++){
		effA[j-1] = eff_hist->GetBinContent(j);
		m_effA[j-1] = eff_hist->GetBinCenter(j);
	}
	TGraph* effgraph 	= new TGraph(nbins_eff,m_effA,effA);
	

	effgraph->SaveAs("effgraph.root");
	accgraph->SaveAs("accgraph.root");
	xsecgraph->SaveAs("xsecgraph.root");

	
	//scale
	double eps2scale = 1; //this scales eps 0.02 -> 

	//*****----->>>>> nSignal = xsecgraph->Eval(mass)*eff*luminosity*acceptances[i]

	//define unfittable ranges
	//float unfittable_mins[9] = {0,    0.28, 0.5,   0.7,   0.95, 2.75,  3.55,  8.9};

	double unfittable_regions[8][2] = {{0,0.22}, {0.5,0.575}, {0.7,0.85}, {0.95,1.05}, {2.9,3.1}, {3.4,3.8}, {9.1,10.}};

   //LOOP OVER MASS INDICES AND MAKE THE CARDS/WORKSPACES
	double mass = -1.;
	for(int i=0; i<400; i++){
	  	//get the histograms
	  	TH1D* catA=(TH1D*)file->Get(Form("massforLimit_CatA%d",i));
	  	TH1D* catB=(TH1D*)file->Get(Form("massforLimit_CatB%d",i));
	  	TH1D* catC=(TH1D*)file->Get(Form("massforLimit_CatC%d",i));

	  	//we're using only one category, so we sum all histos
	  	catA->Add(catB);
	  	catA->Add(catC);
	  	delete catB;
	  	delete catC;

	  	double massLow  =  catA->GetXaxis()->GetXmin();
		double massHigh =  catA->GetXaxis()->GetXmax();
	  

		//compute mass point and define ROOFit variables
	  	mass = 0.5*(massLow+massHigh);
	  	bool dontfit=false;

	  	//reduce the mass bin width to avoid being inside the forbidden regions (resonances)
	  	for (auto fbin : unfittable_regions){
	  		if (massLow>fbin[0] && massHighBin<fbin[1]){ //the current bin is completely inside a }
	  		if ((massHigh-fbin[0])*(massHigh-fbin[1])<=0){ //high edge of our bin is in a forbidden region
	  			massHigh = fbin[0];
	  		}
	  		if ((massLow-fbin[0])*(massLow-fbin[1])<=0){ //low edge of our bin is in a forbidden region
	  			massLow = fbin[1];
	  		}
	  		if ((massLow<fbin[0]) && (massHigh>fbin[1])) dontfit=true; //this means that the unfittable region is right within our bin. In this case there's nothing to do.
	  		if ((massHigh-massLow)<0.2*massBinWidth) dontfit=true; //skip if the mass bin after this reduction is too small (or negative, which would mean the original mass bin was all inside a forbidden region)
	  	}
	  	if (dontfit) continue;



		//cout<<"Spline: "<<effAgraph->Eval(mass,0,"S")<<endl;
		//cout<<"Graph : "<<effAgraph->Eval(mass)<<endl;
		RooRealVar MH    ("MH"      , "MH"          , mass);	
		RooRealVar resA  ("resA"  	, "RFes. unc. " , mass*0.01);//to be checked
		RooRealVar m2mu  ("m2mu"  	, "m2mu " 	, massLow		, massHigh);

		//define the signal model
		RooGaussian signalModel_CatA("signalModel_CatA"	, "signalModel_CatA", m2mu,MH,resA);
		RooDataHist data_obs_CatA("data_obs_CatA", "", RooArgList(m2mu), catA);
		RooRealVar bkg_norm("bkg_norm_CatA", "",catA->Integral());


		RooRealVar par1("par1", "par1", 0.2, 0, 10);
		RooRealVar par2("par2", "par2", 1.5, 0, 10);
		RooRealVar par3("par3", "par3", 2.0, 0, 10);
		RooRealVar par4("par4", "par4", 2.0, 0, 10);
		RooArgList alist(par1, par2, par3, par4);
		RooBernstein bkg_mass_CatA("bkg_mass_CatA", "bkg_mass_CatA", m2mu, alist);

		bkg_mass_CatA.fitTo(data_obs_CatA);

		RooPlot *frame = m2mu.frame();
		data_obs_CatA.plotOn(frame);
		bkg_mass_CatA.plotOn(frame);
		TCanvas c_all("c_all", "c_all", 800, 500);
		frame->Draw("goff");
		c_all.SaveAs(Form("catA%d"+year+".png",i));

		//save into ROO workspace
		RooWorkspace dpworkspace("dpworkspace", "");
		dpworkspace.import(data_obs_CatA);
		dpworkspace.import(signalModel_CatA);
		dpworkspace.import(bkg_mass_CatA);
		dpworkspace.writeToFile(Form("dpWorkspace"+year+suff+"_%d.root",i));

		//write the datacard
		char inputShape[200];
		sprintf(inputShape,"dpCard_"+year+suff+"_%d.txt",i);
		ofstream newcardShape;
		newcardShape.open(inputShape);
		newcardShape << Form("imax * number of channels\n");
		newcardShape << Form("jmax * number of background\n");
		newcardShape << Form("kmax * number of nuisance parameters\n");
		newcardShape << Form("shapes data_obs	CatA dpWorkspace"+year+suff+"_%d.root dpworkspace:data_obs_CatA\n",i);
		newcardShape << Form("shapes bkg_mass	CatA dpWorkspace"+year+suff+"_%d.root dpworkspace:bkg_mass_CatA\n",i);
		newcardShape << Form("shapes signalModel	CatA dpWorkspace"+year+suff+"_%d.root dpworkspace:signalModel_CatA\n",i);
		newcardShape << Form("bin		CatA\n");
		newcardShape << Form("observation 	-1.0\n");
		newcardShape << Form("bin     		CatA		CatA		\n");
		newcardShape << Form("process 		signalModel  	bkg_mass	\n");
		newcardShape << Form("process 		0    		1	   	\n");
		newcardShape << Form("rate    		%f  		%f		\n",
				     eps2scale*xsecgraph->Eval(mass,0,"S")*effgraph->Eval(mass,0,"S")*luminosity*accgraph->Eval(mass,0,"S"), catA->Integral());
		//eps2scale*xsecgraph->Eval(mass,0,"S")*effgraph->Eval(mass,0,"S")*luminosity, catA->Integral());
		newcardShape << Form("lumi13TeV_2017 lnN 	1.023 	-\n");
		newcardShape << Form("eff_mu_13TeV_2017 lnN	1.015 	-\n");
		//newcardShape << Form("bkg_norm_cata rateParam CatA bkg_mass %f\n",catA->Integral());
		//newcardShape << Form("resA param %f %f\n",resA.getValV(),resA.getValV()*0.1);
		newcardShape.close();
		
	}
	histChi2_A->SaveAs("histAchi2_"+year+".root");

}
