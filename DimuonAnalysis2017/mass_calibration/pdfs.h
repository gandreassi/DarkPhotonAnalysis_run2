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
#include "DSCB.h"


using namespace std;


class reso_pdfs {
	public:

		RooWorkspace* w;

		reso_pdfs(){
			w = new RooWorkspace("dpworkspace", "");
			// mass
			
			//// common params
			RooRealVar alpha1  ("alpha1"  	, "alpha1 " 	, 1.,0.,2.5);
			RooRealVar alpha2  ("alpha2"  	, "alpha2 " 	, 1.,0.,2.5);
			RooRealVar n1  ("n1"  	, "n1 " 	, 1,0.5,5);
			//RooRealVar n2  ("n2"  	, "n2 " 	, 1,0.5,5);
			RooRealVar gau_reso_scale ("gau_reso_scale", "gau_reso_scale", 0.5, 0.2, 1.5);
			//////



			///// define all pdfs
			RooRealVar m2mu_omega  ("m2mu_omega"  	, "m2mu_omega" 	, 0,15);
			RooRealVar M_omega    ("M_omega"      , "M_omega"          , 0.782, 0.77, 0.795);
			RooRealVar res_omega_rel ("res_omega_rel", "res_omega_rel", 0.01, 0.005, 0.1);
			RooFormulaVar res_CB_omega  ("res_CB_omega"  	, "M_omega*res_omega_rel" , RooArgList(M_omega, res_omega_rel));
			RooDoubleCB signalModel_CB_omega("signalModel_CB_omega"	, "signalModel_CB_omega", m2mu_omega,M_omega,res_CB_omega, alpha1,n1,alpha2,n1);
			RooFormulaVar res_gau_omega("res_gau_omega"  	, "gau_reso_scale*M_omega*res_omega_rel" , RooArgList(gau_reso_scale, M_omega, res_omega_rel));
			RooGaussian signalModel_gau_omega("signalModel_gau_omega", "signalModel_gau_omega", m2mu_omega,M_omega,res_gau_omega);
			RooRealVar frac_gau_omega("frac_gau_omega", "frac_gau_omega", 0.5,0,1);
			RooAddPdf signalModel_omega("signalModel_omega", "signalModel_omega", RooArgList(signalModel_CB_omega, signalModel_gau_omega), RooArgList(frac_gau_omega));

			RooRealVar sig_fraction_omega("sig_fraction_omega", "",0.5,0.02,0.99);
			RooRealVar par1_omega("par1_omega", "par1_omega", 0.2, 0, 10);
			RooRealVar par2_omega("par2_omega", "par2_omega", 1.5, 0, 10);
			RooRealVar par3_omega("par3_omega", "par3_omega", 2.0, 0, 10);
			RooRealVar par4_omega("par4_omega", "par4_omega", 2.0, 0, 10);
			RooArgList parlist_omega(par1_omega, par2_omega, par3_omega, par4_omega);
			RooBernstein bkgModel_omega("bkgModel_omega", "bkgModel_omega", m2mu_omega, parlist_omega);
			RooAddPdf model_omega("model_omega", "model_omega", RooArgList(signalModel_omega, bkgModel_omega), RooArgList(sig_fraction_omega));
			w->import(model_omega);



			// //////////

			RooRealVar m2mu_phi  ("m2mu_phi"  	, "m2mu_phi" 	, 0,15);
			RooRealVar M_phi    ("M_phi"      , "M_phi"          , 1.02, 0.95, 1.1);	
			RooRealVar res_phi_rel ("res_phi_rel", "res_phi_rel", 0.01, 0.005, 0.1);
			RooFormulaVar res_CB_phi  ("res_CB_phi"  	, "M_phi*res_phi_rel" , RooArgList(M_phi, res_phi_rel));
			RooDoubleCB signalModel_CB_phi("signalModel_CB_phi"	, "signalModel_CB_phi", m2mu_phi,M_phi,res_CB_phi, alpha1,n1,alpha2,n1);
			RooFormulaVar res_gau_phi("res_gau_phi"  	, "gau_reso_scale*M_phi*res_phi_rel" , RooArgList(gau_reso_scale, M_phi, res_phi_rel));
			RooGaussian signalModel_gau_phi("signalModel_gau_phi", "signalModel_gau_phi", m2mu_phi,M_phi,res_gau_phi);
			RooRealVar frac_gau_phi("frac_gau_phi", "frac_gau_phi", 0.5,0,1);
			RooAddPdf signalModel_phi("signalModel_phi", "signalModel_phi", RooArgList(signalModel_CB_phi, signalModel_gau_phi), RooArgList(frac_gau_phi));

			RooRealVar sig_fraction_phi("sig_fraction_phi", "",0.5,0.02,0.99);
			RooRealVar par1_phi("par1_phi", "par1_phi", 0.2, 0, 10);
			RooRealVar par2_phi("par2_phi", "par2_phi", 1.5, 0, 10);
			RooRealVar par3_phi("par3_phi", "par3_phi", 2.0, 0, 10);
			RooRealVar par4_phi("par4_phi", "par4_phi", 2.0, 0, 10);
			RooArgList parlist_phi(par1_phi, par2_phi, par3_phi, par4_phi);
			RooBernstein bkgModel_phi("bkgModel_phi", "bkgModel_phi", m2mu_phi, parlist_phi);
			RooAddPdf model_phi("model_phi", "model_phi", RooArgList(signalModel_phi, bkgModel_phi), RooArgList(sig_fraction_phi));
			w->import(model_phi);


			//////////

			RooRealVar m2mu_jpsipsi  ("m2mu_jpsipsi"  	, "m2mu_jpsipsi" 	, 0,15);

			RooRealVar M_jpsi    ("M_jpsi"      , "M_jpsi"          , 3.1, 3.0, 3.2);	
			RooRealVar res_jpsi_rel ("res_jpsi_rel", "res_jpsi_rel", 0.01, 0.005, 0.1);
			RooFormulaVar res_CB_jpsi  ("res_CB_jpsi"  	, "M_jpsi*res_jpsi_rel" , RooArgList(M_jpsi, res_jpsi_rel));
			RooDoubleCB signalModel_CB_jpsi("signalModel_CB_jpsi"	, "signalModel_CB_jpsi", m2mu_jpsipsi,M_jpsi,res_CB_jpsi, alpha1,n1,alpha2,n1);
			RooFormulaVar res_gau_jpsi("res_gau_jpsi"  	, "gau_reso_scale*M_jpsi*res_jpsi_rel" , RooArgList(gau_reso_scale, M_jpsi, res_jpsi_rel));
			RooGaussian signalModel_gau_jpsi("signalModel_gau_jpsi", "signalModel_gau_jpsi", m2mu_jpsipsi,M_jpsi,res_gau_jpsi);
			RooRealVar frac_gau_jpsi("frac_gau_jpsi", "frac_gau_jpsi", 0.5,0,1);
			RooAddPdf signalModel_jpsi("signalModel_jpsi", "signalModel_jpsi", RooArgList(signalModel_CB_jpsi, signalModel_gau_jpsi), RooArgList(frac_gau_jpsi));

			RooRealVar M_psi2s    ("M_psi2s"      , "M_psi2s"          , 3.65, 3.6, 3.8);	
			RooRealVar res_psi2s_rel ("res_psi2s_rel", "res_psi2s_rel", 0.01, 0.005, 0.1);
			RooFormulaVar res_CB_psi2s  ("res_CB_psi2s"  	, "M_psi2s*res_psi2s_rel" , RooArgList(M_psi2s, res_psi2s_rel));
			RooDoubleCB signalModel_CB_psi2s("signalModel_CB_psi2s"	, "signalModel_CB_psi2s", m2mu_jpsipsi,M_psi2s,res_CB_psi2s, alpha1,n1,alpha2,n1);
			RooFormulaVar res_gau_psi2s("res_gau_psi2s"  	, "gau_reso_scale*M_psi2s*res_psi2s_rel" , RooArgList(gau_reso_scale, M_psi2s, res_psi2s_rel));
			RooGaussian signalModel_gau_psi2s("signalModel_gau_psi2s", "signalModel_gau_psi2s", m2mu_jpsipsi,M_psi2s,res_gau_psi2s);
			RooRealVar frac_gau_psi2s("frac_gau_psi2s", "frac_gau_psi2s", 0.5,0,1);
			RooAddPdf signalModel_psi2s("signalModel_psi2s", "signalModel_psi2s", RooArgList(signalModel_CB_psi2s, signalModel_gau_psi2s), RooArgList(frac_gau_psi2s));

			RooRealVar sig_fraction_jpsi("sig_fraction_jpsi", "",0.3,0.1,0.9);
			RooRealVar sig_fraction_psi2s("sig_fraction_psi2s", "",0.1,0.02,0.5);

			RooRealVar par1_jpsipsi("par1_jpsipsi", "par1_jpsipsi", 0.2, 0, 10);
			RooRealVar par2_jpsipsi("par2_jpsipsi", "par2_jpsipsi", 1.5, 0, 10);
			RooRealVar par3_jpsipsi("par3_jpsipsi", "par3_jpsipsi", 2.0, 0, 10);
			// RooRealVar par4_jpsipsi("par4_jpsipsi", "par4_jpsipsi", 2.0, 0, 10);
			RooArgList parlist_jpsipsi(par1_jpsipsi, par2_jpsipsi, par3_jpsipsi);
			RooBernstein bkgModel_jpsipsi("bkgModel_jpsipsi", "bkgModel_jpsipsi", m2mu_jpsipsi, parlist_jpsipsi);


			RooAddPdf model_jpsipsi("model_jpsipsi", "model_jpsipsi", RooArgList(signalModel_jpsi, signalModel_psi2s, bkgModel_jpsipsi), RooArgList(sig_fraction_jpsi,sig_fraction_psi2s));
			w->import(model_jpsipsi);



			//////////

			RooRealVar m2mu_upsilon  ("m2mu_upsilon"  	, "m2mu_upsilon" 	, 0,15);
			RooRealVar M_upsilon1s    ("M_upsilon1s"      , "M_upsilon1s"          , 9.46, 9.36, 9.56);	
			RooRealVar res_upsilon1s_rel ("res_upsilon1s_rel", "res_upsilon1s_rel", 0.01, 0.005, 0.1);
			RooFormulaVar res_CB_upsilon1s  ("res_CB_upsilon1s"  	, "M_upsilon1s*res_upsilon1s_rel" , RooArgList(M_upsilon1s, res_upsilon1s_rel));
			RooDoubleCB signalModel_CB_upsilon1s("signalModel_CB_upsilon1s"	, "signalModel_CB_upsilon1s", m2mu_upsilon,M_upsilon1s,res_CB_upsilon1s, alpha1,n1,alpha2,n1);
			RooFormulaVar res_gau_upsilon1s("res_gau_upsilon1s"  	, "gau_reso_scale*M_upsilon1s*res_upsilon1s_rel" , RooArgList(gau_reso_scale, M_upsilon1s, res_upsilon1s_rel));
			RooGaussian signalModel_gau_upsilon1s("signalModel_gau_upsilon1s", "signalModel_gau_upsilon1s", m2mu_upsilon,M_upsilon1s,res_gau_upsilon1s);
			RooRealVar frac_gau_upsilon1s("frac_gau_upsilon1s", "frac_gau_upsilon1s", 0.5,0,1);
			RooAddPdf signalModel_upsilon1s("signalModel_upsilon1s", "signalModel_upsilon1s", RooArgList(signalModel_CB_upsilon1s, signalModel_gau_upsilon1s), RooArgList(frac_gau_upsilon1s));

			RooRealVar M_upsilon2s    ("M_upsilon2s"      , "M_upsilon2s"          , 10.05, 9.95, 10.1);	
			RooRealVar res_upsilon2s_rel ("res_upsilon2s_rel", "res_upsilon2s_rel", 0.01, 0.005, 0.1);
			RooFormulaVar res_CB_upsilon2s  ("res_CB_upsilon2s"  	, "M_upsilon2s*res_upsilon2s_rel" , RooArgList(M_upsilon2s, res_upsilon2s_rel));
			RooDoubleCB signalModel_CB_upsilon2s("signalModel_CB_upsilon2s"	, "signalModel_CB_upsilon2s", m2mu_upsilon,M_upsilon2s,res_CB_upsilon2s, alpha1,n1,alpha2,n1);
			RooFormulaVar res_gau_upsilon2s("res_gau_upsilon2s"  	, "gau_reso_scale*M_upsilon2s*res_upsilon2s_rel" , RooArgList(gau_reso_scale, M_upsilon2s, res_upsilon2s_rel));
			RooGaussian signalModel_gau_upsilon2s("signalModel_gau_upsilon2s", "signalModel_gau_upsilon2s", m2mu_upsilon,M_upsilon2s,res_gau_upsilon2s);
			RooRealVar frac_gau_upsilon2s("frac_gau_upsilon2s", "frac_gau_upsilon2s", 0.5,0,1);
			RooAddPdf signalModel_upsilon2s("signalModel_upsilon2s", "signalModel_upsilon2s", RooArgList(signalModel_CB_upsilon2s, signalModel_gau_upsilon2s), RooArgList(frac_gau_upsilon2s));

			RooRealVar M_upsilon3s    ("M_upsilon3s"      , "M_upsilon3s"          , 10.3, 10.25, 10.4);	
			RooRealVar res_upsilon3s_rel ("res_upsilon3s_rel", "res_upsilon3s_rel", 0.01, 0.005, 0.1);
			RooFormulaVar res_CB_upsilon3s  ("res_CB_upsilon3s"  	, "M_upsilon3s*res_upsilon3s_rel" , RooArgList(M_upsilon3s, res_upsilon3s_rel));
			RooDoubleCB signalModel_CB_upsilon3s("signalModel_CB_upsilon3s"	, "signalModel_CB_upsilon3s", m2mu_upsilon,M_upsilon3s,res_CB_upsilon3s, alpha1,n1,alpha2,n1);
			RooFormulaVar res_gau_upsilon3s("res_gau_upsilon3s"  	, "gau_reso_scale*M_upsilon3s*res_upsilon3s_rel" , RooArgList(gau_reso_scale, M_upsilon3s, res_upsilon3s_rel));
			RooGaussian signalModel_gau_upsilon3s("signalModel_gau_upsilon3s", "signalModel_gau_upsilon3s", m2mu_upsilon,M_upsilon3s,res_gau_upsilon3s);
			RooRealVar frac_gau_upsilon3s("frac_gau_upsilon3s", "frac_gau_upsilon3s", 0.5,0,1);
			RooAddPdf signalModel_upsilon3s("signalModel_upsilon3s", "signalModel_upsilon3s", RooArgList(signalModel_CB_upsilon3s, signalModel_gau_upsilon3s), RooArgList(frac_gau_upsilon3s));

			RooRealVar sig_fraction_upsilon1s("sig_fraction_upsilon1s", "",0.2,0.02,0.8);
			RooRealVar sig_fraction_upsilon2s("sig_fraction_upsilon2s", "",0.2,0.02,0.8);
			RooRealVar sig_fraction_upsilon3s("sig_fraction_upsilon3s", "",0.2,0.02,0.8);

			RooRealVar par1_upsilon("par1_upsilon", "par1_upsilon", 0.2, 0, 10);
			RooRealVar par2_upsilon("par2_upsilon", "par2_upsilon", 1.5, 0, 10);
			RooRealVar par3_upsilon("par3_upsilon", "par3_upsilon", 2.0, 0, 10);
			RooRealVar par4_upsilon("par4_upsilon", "par4_upsilon", 2.0, 0, 10);

			RooArgList parlist_upsilon(par1_upsilon, par2_upsilon, par3_upsilon, par4_upsilon);
			RooBernstein bkgModel_upsilon("bkgModel_upsilon", "bkgModel_upsilon", m2mu_upsilon, parlist_upsilon);
			RooAddPdf model_upsilon("model_upsilon", "model_upsilon", RooArgList(signalModel_upsilon1s, signalModel_upsilon2s, signalModel_upsilon3s, bkgModel_upsilon), RooArgList(sig_fraction_upsilon1s, sig_fraction_upsilon2s, sig_fraction_upsilon3s));
			w->import(model_upsilon);

		};

};


