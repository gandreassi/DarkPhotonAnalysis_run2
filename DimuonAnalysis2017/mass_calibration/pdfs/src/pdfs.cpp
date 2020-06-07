
#include "pdfs.h"
using namespace std;


		reso_pdfs::~reso_pdfs(){
			w->Delete();
		};

		reso_pdfs::reso_pdfs(){

			w = new RooWorkspace("dpworkspace", "dpworkspace");
			w->addClassDeclImportDir("../RooFit-pdfs/");
			w->addClassImplImportDir("../RooFit-pdfs/");
			w->importClassCode(RooDoubleCB::Class(),kTRUE);

			
			//// common params
			alpha1 = new RooRealVar("alpha1"  	, "alpha1 " 	, 1.,0.,2.5);
			alpha2 = new RooRealVar("alpha2"  	, "alpha2 " 	, 1.,0.,2.5);
			n1 = new RooRealVar("n1"  	, "n1 " 	, 1,0.5,5);
			//n2 = RooRealVar("n2"  	, "n2 " 	, 1,0.5,5);
			frac_gau = new RooRealVar("frac_gau", "frac_gau", 0.5,0,1);
			gau_reso_scale = new RooRealVar ("gau_reso_scale", "gau_reso_scale", 0.5, 0.2, 1.5);
			//////


			///// define all pdfs
			RooRealVar* m2mu_omegaphi = new RooRealVar("m2mu_omegaphi"  	, "m2mu_omegaphi" 	, 0,15);
			RooAddPdf* signalModel_omega = reso_pdfs::make_signal_model("omega", m2mu_omegaphi, 0.782, 0.77, 0.795);
			RooAddPdf* signalModel_phi = reso_pdfs::make_signal_model("phi", m2mu_omegaphi, 1.02, 0.95, 1.1);

			RooRealVar sig_fraction_omega("sig_fraction_omega", "",0.2,0.02,0.8);
			RooRealVar sig_fraction_phi("sig_fraction_phi", "",0.2,0.02,0.8);

			RooRealVar par1_omegaphi("par1_omegaphi", "par1_omegaphi", 0.2, 0, 15);
			RooRealVar par2_omegaphi("par2_omegaphi", "par2_omegaphi", 1.5, 0, 15);
			RooRealVar par3_omegaphi("par3_omegaphi", "par3_omegaphi", 2.0, 0, 15);
			RooRealVar par4_omegaphi("par4_omegaphi", "par4_omegaphi", 2.0, 0, 15);
			RooRealVar par5_omegaphi("par5_omegaphi", "par5_omegaphi", 2.0, 0, 15);
			RooArgList parlist_omegaphi(par1_omegaphi, par2_omegaphi, par3_omegaphi, par4_omegaphi, par5_omegaphi);
			RooBernstein bkgModel_omegaphi("bkgModel_omegaphi", "bkgModel_omegaphi", *m2mu_omegaphi, parlist_omegaphi);
			RooAddPdf model_omegaphi("model_omegaphi", "model_omegaphi", RooArgList(*signalModel_omega, *signalModel_phi, bkgModel_omegaphi), RooArgList(sig_fraction_omega, sig_fraction_phi));
			w->import(model_omegaphi);


			//////////

			RooRealVar* m2mu_jpsipsi = new RooRealVar("m2mu_jpsipsi"  	, "m2mu_jpsipsi" 	, 0,15);
			RooAddPdf* signalModel_jpsi = reso_pdfs::make_signal_model("jpsi", m2mu_jpsipsi, 3.1, 3.0, 3.2);
			RooAddPdf* signalModel_psi2s = reso_pdfs::make_signal_model("psi2s", m2mu_jpsipsi, 3.65, 3.6, 3.8);

			RooRealVar sig_fraction_jpsi("sig_fraction_jpsi", "",0.3,0.1,0.9);
			RooRealVar sig_fraction_psi2s("sig_fraction_psi2s", "",0.1,0.02,0.5);

			RooRealVar par1_jpsipsi("par1_jpsipsi", "par1_jpsipsi", 0.2, 0, 10);
			RooRealVar par2_jpsipsi("par2_jpsipsi", "par2_jpsipsi", 1.5, 0, 10);
			RooRealVar par3_jpsipsi("par3_jpsipsi", "par3_jpsipsi", 2.0, 0, 10);
			// RooRealVar par4_jpsipsi("par4_jpsipsi", "par4_jpsipsi", 2.0, 0, 10);
			RooArgList parlist_jpsipsi(par1_jpsipsi, par2_jpsipsi, par3_jpsipsi);
			RooBernstein bkgModel_jpsipsi("bkgModel_jpsipsi", "bkgModel_jpsipsi", *m2mu_jpsipsi, parlist_jpsipsi);

			RooAddPdf model_jpsipsi("model_jpsipsi", "model_jpsipsi", RooArgList(*signalModel_jpsi, *signalModel_psi2s, bkgModel_jpsipsi), RooArgList(sig_fraction_jpsi,sig_fraction_psi2s));
			w->import(model_jpsipsi);



			//////////

			RooRealVar* m2mu_upsilon = new RooRealVar("m2mu_upsilon"  	, "m2mu_upsilon" 	, 0,15);
			RooAddPdf* signalModel_upsilon1s = reso_pdfs::make_signal_model("upsilon1s", m2mu_upsilon, 9.46, 9.36, 9.56);
			RooAddPdf* signalModel_upsilon2s = reso_pdfs::make_signal_model("upsilon2s", m2mu_upsilon, 10.05, 9.95, 10.1);
			RooAddPdf* signalModel_upsilon3s = reso_pdfs::make_signal_model("upsilon3s", m2mu_upsilon, 10.3, 10.25, 10.4);

			RooRealVar sig_fraction_upsilon1s("sig_fraction_upsilon1s", "",0.2,0.02,0.8);
			RooRealVar sig_fraction_upsilon2s("sig_fraction_upsilon2s", "",0.2,0.02,0.8);
			RooRealVar sig_fraction_upsilon3s("sig_fraction_upsilon3s", "",0.2,0.02,0.8);

			RooRealVar par1_upsilon("par1_upsilon", "par1_upsilon", 0.2, 0, 10);
			RooRealVar par2_upsilon("par2_upsilon", "par2_upsilon", 1.5, 0, 10);
			RooRealVar par3_upsilon("par3_upsilon", "par3_upsilon", 2.0, 0, 10);
			RooRealVar par4_upsilon("par4_upsilon", "par4_upsilon", 2.0, 0, 10);

			RooArgList parlist_upsilon(par1_upsilon, par2_upsilon, par3_upsilon, par4_upsilon);
			RooBernstein bkgModel_upsilon("bkgModel_upsilon", "bkgModel_upsilon", *m2mu_upsilon, parlist_upsilon);

			RooAddPdf model_upsilon("model_upsilon", "model_upsilon", RooArgList(*signalModel_upsilon1s, *signalModel_upsilon2s, *signalModel_upsilon3s, bkgModel_upsilon), RooArgList(sig_fraction_upsilon1s, sig_fraction_upsilon2s, sig_fraction_upsilon3s));
			w->import(model_upsilon);


			// add another signal pdf, which will not participate in the fit but will serve as base pdf for the limit
			RooRealVar* m2mu = new RooRealVar("m2mu"  	, "m2mu" 	, 0,15);
			RooAddPdf* generic_signal_model = reso_pdfs::make_signal_model("generic", m2mu, 5, 0, 15);
			w->import(*generic_signal_model);

		};

		RooAddPdf* reso_pdfs::make_signal_model(string name, RooRealVar* mass, double m_init, double m_min, double m_max){
			RooRealVar* M = new RooRealVar(("M_"+name).c_str()      , ("M_"+name).c_str()          , m_init, m_min, m_max);
			RooRealVar* res_rel = new RooRealVar(("res_rel_"+name).c_str(), ("res_rel_"+name).c_str(), 0.01, 0.005, 0.1);
			RooFormulaVar* res_CB = new RooFormulaVar(("res_CB_"+name).c_str(), ("M_"+name+"*res_rel_"+name).c_str() , RooArgList(*M, *res_rel));
			RooDoubleCB* signalModel_CB = new RooDoubleCB(("signalModel_CB_"+name).c_str(), ("signalModel_CB_"+name).c_str(), *mass, *M,*res_CB,*alpha1,*n1,*alpha2,*n1);
			RooFormulaVar* res_gau = new RooFormulaVar(("res_gau_"+name).c_str()  	, ("gau_reso_scale*M_"+name+"*res_rel_"+name).c_str() , RooArgList(*gau_reso_scale, *M,*res_rel));
			RooGaussian* signalModel_gau = new RooGaussian(("signalModel_gau_"+name).c_str(), ("signalModel_gau_"+name).c_str(), *mass, *M,*res_gau);
			RooAddPdf* signalModel = new RooAddPdf(("signalModel_"+name).c_str(), ("signalModel_"+name).c_str(), RooArgList(*signalModel_CB, *signalModel_gau), RooArgList(*frac_gau));
			return signalModel;
		};

		void reso_pdfs::freeze(){
			alpha1->setConstant(kTRUE);
			alpha2->setConstant(kTRUE);
			n1->setConstant(kTRUE);
			//n2->setConstant(kTRUE);
			frac_gau->setConstant(kTRUE);
			gau_reso_scale->setConstant(kTRUE);
		};

		void reso_pdfs::saveAfterCalibration(const char* fname){
			TFile* f_ws = TFile::Open(fname,"RECREATE");
			w->saveSnapshot("calibrated", w->allVars());
			w->Write();
			w->Print();
			f_ws->Close();
		};




