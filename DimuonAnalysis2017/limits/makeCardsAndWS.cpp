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

#include <RooAbsPdf.h> 
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
#include <RooAddPdf.h> 
#include <TROOT.h> 

//#include "pdfs.h"
#include "RooDoubleCB.h"


using namespace std;

void makeCardsAndWS(){
  

  TH1F* histChi2_A=new TH1F("histChi2_A","histChi2_A",50,0.5,1.5);


  //WHICH YEAR
  	TString year="2017";
	TString suff="IterV3";
  //INPUT FILE WITH HISTOGRAMS TO FIT BACKGROUND
  	TFile* file=TFile::Open("/mnt/hadoop/scratch/gandreas/DP_histos/mergedHists_three.root");

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
	

	effgraph->SaveAs("output/effgraph.root");
	accgraph->SaveAs("output/accgraph.root");
	xsecgraph->SaveAs("output/xsecgraph.root");

	
	//scale
	double eps2scale = 1; //this scales eps 0.02 -> 

	//*****----->>>>> nSignal = xsecgraph->Eval(mass)*eff*luminosity*acceptances[i]

	//define unfittable ranges
	//float unfittable_mins[9] = {0,    0.28, 0.5,   0.7,   0.95, 2.75,  3.55,  8.9};

	double unfittable_regions[8][2] = {{0,0.22}, {0.5,0.575}, {0.7,0.85}, {0.95,1.05}, {2.9,3.1}, {3.4,3.8}, {9.1,10.}};

   //LOOP OVER MASS INDICES AND MAKE THE CARDS/WORKSPACES
	double mass = -1.;
	TFile* f_ws = TFile::Open("../mass_calibration/pdfs.root", "READ");
	RooWorkspace *w = (RooWorkspace*)f_ws->Get("dpworkspace");
	w->loadSnapshot("calibrated");
	w->var("alpha1")->setConstant(true);//all this should actually be automatic. Check!!
	w->var("alpha2")->setConstant(true);
	w->var("n1")->setConstant(true);
	w->var("frac_gau")->setConstant(true);
	w->var("gau_reso_scale")->setConstant(true);

	w->Print();

	for(int i=0; i<400; i++){
	  	//get the histograms
	  	TH1D* catA=(TH1D*)file->Get(Form("massforLimit_CatA%d",i));
	  	TH1D* catB=(TH1D*)file->Get(Form("massforLimit_CatB%d",i));
	  	//TH1D* catC=(TH1D*)file->Get(Form("massforLimit_CatC%d",i));

	  	//we're using only one category, so we sum all histos
	  	catA->Add(catB);
	  	//catA->Add(catC);
	  	delete catB;
	  	//delete catC;

	  	double massLow  =  catA->GetXaxis()->GetXmin();
		double massHigh =  catA->GetXaxis()->GetXmax();
		double massBinWidth = massHigh-massLow;
	  

		//compute mass point and define ROOFit variables
	  	mass = 0.5*(massLow+massHigh);
	  	bool dontfit=false;

	  	//reduce the mass bin width to avoid being inside the forbidden regions (resonances)
	  	for (auto fbin : unfittable_regions){
	  		if (massLow>fbin[0] && massHigh<fbin[1]) dontfit=true; //the current bin is completely inside a forbidden region
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
		RooRealVar* m2mu = w->var("m2mu");
		m2mu->setMin(massLow);
		m2mu->setMax(massHigh);

		RooAddPdf* signalModel = (RooAddPdf*)w->pdf("signalModel_generic");

		//define the signal model
		w->var("M_generic")->setVal(mass);
		w->var("M_generic")->setConstant(true);
		w->var("res_rel_generic")->setVal(0.013);
		w->var("res_rel_generic")->setConstant(true);
		//in pdf.h aggiungi una pdf generica e salvala nel workspace con tutti i param già fissati. poi riprendila da qui, e usa dirett
		// la sua variabile massa osservabile come massa qui, semplicemente cambiandogli il range.

		RooDataHist data_obs("data_obs", "", RooArgList(*m2mu), catA);
		RooRealVar bkg_norm("bkg_norm", "",catA->Integral());


		RooRealVar par1("par1", "par1", 0.2, 0, 10);
		RooRealVar par2("par2", "par2", 1.5, 0, 10);
		RooRealVar par3("par3", "par3", 2.0, 0, 10);
		RooRealVar par4("par4", "par4", 2.0, 0, 10);
		RooArgList alist(par1, par2, par3, par4);
		RooBernstein bkg_model("bkg_model", "bkg_model", *m2mu, alist);

		bkg_model.fitTo(data_obs);

		RooPlot *frame = m2mu->frame();
		data_obs.plotOn(frame);
		bkg_model.plotOn(frame);
		TCanvas c_all("c_all", "c_all", 800, 500);
		frame->Draw("goff");
		c_all.SaveAs(Form("output/catA%d"+year+".png",i));

		//save into ROO workspace
		RooWorkspace dpworkspace("dpworkspace", "");
		dpworkspace.import(data_obs);
		dpworkspace.import(*signalModel);
		dpworkspace.import(bkg_model);
		dpworkspace.writeToFile(Form("output/dpWorkspace"+year+suff+"_%d.root",i));

		//write the datacard
		char inputShape[200];
		sprintf(inputShape,"output/dpCard_"+year+suff+"_%d.txt",i);
		ofstream newcardShape;
		newcardShape.open(inputShape);
		newcardShape << Form("imax * number of channels\n");
		newcardShape << Form("jmax * number of background\n");
		newcardShape << Form("kmax * number of nuisance parameters\n");
		newcardShape << Form("shapes data_obs	CatAB dpWorkspace"+year+suff+"_%d.root dpworkspace:data_obs\n",i);
		newcardShape << Form("shapes bkg_mass	CatAB dpWorkspace"+year+suff+"_%d.root dpworkspace:bkg_model\n",i);
		newcardShape << Form("shapes signalModel_generic	CatAB dpWorkspace"+year+suff+"_%d.root dpworkspace:signalModel\n",i);
		newcardShape << Form("bin		CatAB\n");
		newcardShape << Form("observation 	-1.0\n");
		newcardShape << Form("bin     		CatAB		CatAB		\n");
		newcardShape << Form("process 		signalModel_generic  	bkg_mass	\n");
		newcardShape << Form("process 		0    		1	   	\n");
		newcardShape << Form("rate    		%f  		%f		\n",
				     eps2scale*xsecgraph->Eval(mass,0,"S")*effgraph->Eval(mass,0,"S")*luminosity*accgraph->Eval(mass,0,"S"), catA->Integral());
		//eps2scale*xsecgraph->Eval(mass,0,"S")*effgraph->Eval(mass,0,"S")*luminosity, catA->Integral());
		newcardShape << Form("lumi13TeV_2017 lnN 	1.023 	-\n");
		newcardShape << Form("eff_mu_13TeV_2017 lnN	1.015 	-\n");
		//newcardShape << Form("bkg_norm rateParam CatA bkg_mass %f\n",catA->Integral());
		//newcardShape << Form("resA param %f %f\n",resA.getValV(),resA.getValV()*0.1);
		newcardShape.close();
		
	}
	histChi2_A->SaveAs("output/histAchi2_"+year+".root");
	f_ws->Close();

}
