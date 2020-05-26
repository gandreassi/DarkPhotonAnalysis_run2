#include <iostream>
#include <TLegend.h>
#include <TFrame.h>
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
#include <RooAddPdf.h> 
#include <RooAddition.h> 
#include <RooMinuit.h> 
#include <RooFitResult.h> 
#include "pdfs.h"

using namespace std;

void mass_calibration(){

  //gROOT->ProcessLine(".L ../RooFit-pdfs/CB.cc+");
 

  //WHICH YEAR
  	TString year="2017";
  //INPUT FILE WITH HISTOGRAMS
  	TFile* file=TFile::Open("root://eoscms.cern.ch//eos/cms/store/group/phys_exotica/darkPhoton/jakob/2017/ScoutingRunD/ScoutingCaloMuon/crab_20181212_141716/181212_131724/0000/mergedHists_two.root");


	std::map<string,int> resonance_hist_IDs; //map resonance name to histogram number
	resonance_hist_IDs["omega"] = 136;
	resonance_hist_IDs["phi"] = 163;
	resonance_hist_IDs["jpsi"] = 274;
	resonance_hist_IDs["psi2s"] = 293;
	resonance_hist_IDs["upsilon"] = 390;
	std::map<string, RooDataHist*> hist_map; //map resonance name to RooDataHist
	std::map<string, RooAddPdf*> pdf_map; //map resonance name to pdf

	std::vector <float> mass, massError, width, widthError;
	std::vector <string> resonances;
	resonances.push_back("omega");
	resonances.push_back("phi");
	resonances.push_back("jpsi");
	resonances.push_back("psi2s");
	resonances.push_back("upsilon1s");
	resonances.push_back("upsilon2s");
	resonances.push_back("upsilon3s");

	reso_pdfs pdfs;
	RooWorkspace *w = pdfs.w;

   //LOOP OVER MASS INDICES AND MAKE THE CARDS/WORKSPACES

	RooArgSet chi2s;
	for (const auto &name_ID : resonance_hist_IDs ){
	  	//get the histograms
	  	TH1D* catA=(TH1D*)file->Get(Form("massforLimit_CatA%d", name_ID.second));
	  	TH1D* catB=(TH1D*)file->Get(Form("massforLimit_CatB%d", name_ID.second));
	  	TH1D* catC=(TH1D*)file->Get(Form("massforLimit_CatC%d", name_ID.second));

	  	//we're using only one category, so we sum all histos
	  	catA->Add(catB);
	  	catA->Add(catC);
	  	delete catB;
	  	delete catC;

	  	RooRealVar* m2mu = w->var(("m2mu_"+name_ID.first).c_str());
	  	m2mu->setRange(catA->GetXaxis()->GetXmin(),catA->GetXaxis()->GetXmax());
	  	RooDataHist* rdh = new RooDataHist(("rdh_"+name_ID.first).c_str(), ("rdh_"+name_ID.first).c_str(), RooArgSet(*m2mu), catA);
	  	RooAddPdf* model = (RooAddPdf*) w->pdf(("model_"+name_ID.first).c_str());
	  	hist_map[name_ID.first] = rdh;
	  	pdf_map[name_ID.first] = model;
	  	RooAbsReal* chi2 = model->createChi2(*rdh, RooFit::Range(("fitRange_"+name_ID.first).c_str()));
	  	chi2s.add(*chi2);
	 }

	RooAddition totchi2 ("totchi2", "totchi2", chi2s);

	RooMinuit m(totchi2);
	RooFitResult* Ares = new RooFitResult();

	//while (Ares->covQual()<3){ //famo a mano che famo prima, tanto pe cambia'
		m.migrad();
		m.hesse();
		Ares = m.save();
		cout<<"$$$$"<<Ares->covQual()<<endl;
	//}	

	for (const auto &name_ID : resonance_hist_IDs ){
		RooPlot *frame = w->var(("m2mu_"+name_ID.first).c_str())->frame(RooFit::Range(("fitRange_"+name_ID.first).c_str()));
		hist_map[name_ID.first]->plotOn(frame);
		TIterator* pdfIter = pdf_map[name_ID.first]->pdfList().createIterator();
		RooAbsPdf* signal = (RooAbsPdf*)pdfIter->Next();
		while (signal){
			pdf_map[name_ID.first]->plotOn(frame, RooFit::NormRange(("fitRange_"+name_ID.first).c_str()), RooFit::Components(signal->GetName()), RooFit::LineColor(kGreen));
			signal = (RooAbsPdf*)pdfIter->Next();
		}
		pdf_map[name_ID.first]->plotOn(frame, RooFit::NormRange(("fitRange_"+name_ID.first).c_str()), RooFit::Components(("bkgModel_"+name_ID.first).c_str()), RooFit::LineColor(kRed));
		pdf_map[name_ID.first]->plotOn(frame, RooFit::NormRange(("fitRange_"+name_ID.first).c_str()));
		TCanvas c_all("c_all", "c_all", 800, 500);
		frame->Draw("goff");
		frame->SetTitle("");
		frame->GetXaxis()->SetTitle("dimuon mass [GeV]");
		c_all.SaveAs(("plots/"+name_ID.first+(string)year+".png").c_str());
	} 

	for (auto reso : resonances){
		//fill graph
		mass.push_back(w->var(("M_"+reso).c_str())->getVal());
		massError.push_back(w->var(("M_"+reso).c_str())->getError());
		width.push_back(w->var(("res_"+reso+"_rel").c_str())->getVal());
		widthError.push_back(w->var(("res_"+reso+"_rel").c_str())->getError());
	}


TCanvas* c1 = new TCanvas("c1","c1",200,10,700,500);
   c1->SetGrid();
   c1->GetFrame()->SetFillColor(21);
   c1->GetFrame()->SetBorderSize(12);
   const Int_t n = resonances.size();
   TGraphErrors* gr = new TGraphErrors(n,&mass[0],&width[0],&massError[0],&widthError[0]);
   gr->SetTitle("Relative resonance width vs mass");
   gr->SetMarkerColor(4);
   gr->SetMarkerStyle(21);
   gr->GetYaxis()->SetRangeUser(0,0.05);
   gr->Draw("AP");
   c1->SaveAs(("plots/graph"+(string)year+".png").c_str());
}
