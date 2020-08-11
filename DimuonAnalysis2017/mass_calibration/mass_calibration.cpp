#include <iostream>
#include <TLegend.h>
#include <TFrame.h>
#include <sstream>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include "TH1F.h"
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
#include <cctype>
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
#include <TROOT.h> 
#include "pdfs.h"

using namespace std;

void mass_calibration(TString year="2017"){ 

  //INPUT FILE WITH HISTOGRAMS
  	TFile* file;
  	if (year.CompareTo("2017")) file=TFile::Open("/mnt/hadoop/scratch/gandreas/DP_histos/mergedHists_three.root");
  	else if (year.CompareTo("2018")) file=TFile::Open("/mnt/hadoop/scratch/gandreas/DP_histos/2018/mergedHistos_v1.root");

	std::map<string, RooDataHist*> hist_map; //map resonance name to RooDataHist
	std::map<string, RooAddPdf*> pdf_map; //map resonance name to pdf

	std::map<string, vector<float>> hist_ranges; 
	hist_ranges["OmegaPhi"] = {{0.65, 1.3}};
	hist_ranges["JPsiPsi"] =  {{2.7, 4.}};
	hist_ranges["Upsilon"] =  {{8.5 ,11}};

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
	TH1F* histo=(TH1F*)file->Get("massforLimitFull");

	RooArgSet chi2s;
	for (const auto &name_range : hist_ranges){
		string name = name_range.first;
	  	//get the histograms

	  	float xmin = name_range.second[0];
	  	float xmax = name_range.second[1];
	  	if (name.compare("JPsiPsi") == 0) xmax = 4.;

	  	string name_lower = name;
	  	std::for_each(name_lower.begin(), name_lower.end(), [](char & c) {c = ::tolower(c);});
	  	RooRealVar* m2mu = w->var(("m2mu_"+name_lower).c_str());
	  	cout<<name_lower<<m2mu<<endl;
	  	m2mu->setRange(xmin, xmax);
	  	RooDataHist* rdh = new RooDataHist(("rdh_"+name_lower).c_str(), ("rdh_"+name_lower).c_str(), RooArgSet(*m2mu), histo);
	  	RooAddPdf* model = (RooAddPdf*) w->pdf(("model_"+name_lower).c_str());
	  	hist_map[name_lower] = rdh;
	  	pdf_map[name_lower] = model;
	  	RooAbsReal* chi2 = model->createChi2(*rdh, RooFit::Range(("fitRange_"+name_lower).c_str()));
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
	// }	

	pdfs.freeze();
	pdfs.saveAfterCalibration(("pdfs"+(string)year+".root").c_str());


	for (const auto &name_range : hist_ranges){
		string name = name_range.first;
	  	string name_lower = name;
	  	std::for_each(name_lower.begin(), name_lower.end(), [](char & c) {c = ::tolower(c);});

	  	RooPlot *frame = w->var(("m2mu_"+name_lower).c_str())->frame(RooFit::Range(("fitRange_"+name_lower).c_str()));
		hist_map[name_lower]->plotOn(frame);
		TIterator* pdfIter = pdf_map[name_lower]->pdfList().createIterator();
		RooAddPdf* signal = (RooAddPdf*)pdfIter->Next();
		while (signal){
			pdf_map[name_lower]->plotOn(frame, RooFit::NormRange(("fitRange_"+name_lower).c_str()), RooFit::Components(signal->GetName()), RooFit::LineColor(kGreen));
			signal = (RooAddPdf*)pdfIter->Next();
		}
		pdf_map[name_lower]->plotOn(frame, RooFit::NormRange(("fitRange_"+name_lower).c_str()), RooFit::Components(("bkgModel_"+name_lower).c_str()), RooFit::LineColor(kRed));
		pdf_map[name_lower]->plotOn(frame, RooFit::NormRange(("fitRange_"+name_lower).c_str()));
		TCanvas c_all("c_all", "c_all", 800, 500);
		frame->Draw("goff");
		frame->SetTitle("");
		frame->GetXaxis()->SetTitle("dimuon mass [GeV]");
		c_all.SaveAs(("plots/"+name_lower+(string)year+".png").c_str());
		c_all.SaveAs(("plots/"+name_lower+(string)year+".pdf").c_str());
	} 

	for (auto reso : resonances){
		//fill graph
		mass.push_back(w->var(("M_"+reso).c_str())->getVal());
		massError.push_back(w->var(("M_"+reso).c_str())->getError());
		width.push_back(w->var(("res_rel_"+reso).c_str())->getVal());
		widthError.push_back(w->var(("res_rel_"+reso).c_str())->getError());
	}


	TCanvas* c1 = new TCanvas("c1","c1",200,10,700,500);
	c1->SetGrid();
	c1->GetFrame()->SetFillColor(21);
	c1->GetFrame()->SetBorderSize(12);
	const Int_t n = resonances.size();
	TGraphErrors* gr = new TGraphErrors(n,&mass[0],&width[0],&massError[0],&widthError[0]);
	gr->SetName("resos");
	gr->SetTitle("Relative resonance width vs mass");
	gr->SetMarkerColor(4);
	gr->SetMarkerStyle(21);
	gr->GetYaxis()->SetRangeUser(0,0.05);
	gr->Draw("AP");
	c1->SaveAs(("plots/graph"+(string)year+".png").c_str());

	TFile *outf = new TFile(("mass_resolutions"+(string)year+".root").c_str(), "RECREATE");
	gr->Write();
	outf->Close();


}
