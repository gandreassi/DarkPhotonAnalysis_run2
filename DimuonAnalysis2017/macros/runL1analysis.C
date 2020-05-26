//#include "sumwgt.h"
#include "TFileCollection.h"
#include "TChain.h"
#include "TFile.h"
#include <TTreeReader.h>
#include "TH1D.h"
#include <TTreeReaderValue.h>
#include "TLorentzVector.h"

void runL1analysis(string treepath = "/eos/cms/store/group/comm_luminosity/ZCounting/ScoutingCaloMuon/crab_20180803_180709/180803_160717/0000/scout_174.root"/*"/eos/cms/store/user/jsalfeld/ScoutingCaloMuon/crab_20180516_052421/180516_032431/0000/scout_53.root"*/, const char* outfilename = "./scoutDataL1.root", bool isMC=false) {
//void trimscoutWithHist2local(const char* treepath = "/eos/cms/store/user/jsalfeld/QCD_Pt-15to20_MuEnrichedPt5_TuneCP5_13TeV_pythia8/crab_20180512_015229/180511_235241/0000/merged.root"/*"/eos/cms/store/user/jsalfeld/QCD_Pt-20to30_MuEnrichedPt5_TuneCP5_13TeV_pythia8/crab_20180512_015244/180511_235256/0000/merged.root"*/, const char* outfilename = "./scoutQCD2030x.root", bool isMC=false) {


  //TFileCollection fc("fc");
    //fc.Add(treepath);
   vector<string> filesToRun;
   vector<bool> isData;
   
   filesToRun.push_back(treepath);isData.push_back(true);
   TFile* outfile = new TFile(outfilename, "RECREATE");
   // TTree* outtree = new TTree("tree", "tree");
   TH1D* massLS_L1[21];
   TH1D* massOS_L1[21];
   TH1D* massLSPV_L1[21];
   TH1D* massOSPV_L1[21];

    float binedge=0.200;
    float binx[643];    
    for(unsigned int i=0; i<643; i++)
	{
  		binx[i] = float(binedge);
		binedge=binedge+0.01*binedge;
		//cout<<binedge<<endl;
	}

    double xbins[100000];
    xbins[0] = .2;
    int nbins = 0;
    double binWidth=0.004;//0.01; 
    for (int i=1; xbins[i-1]<200; i++) {
	  xbins[i] = xbins[i-1]*(1+binWidth);
	  nbins++;
	  cout << nbins << " "<< xbins[i] << endl;
	}


    

    // binx[393]=10;
    cout<<"binedge: "<<binedge<<endl;
      
     for(int nb=0; nb<21; nb++){
       
       

       massLS_L1[nb]        = new TH1D(Form("massLS_L1_%d",nb),        Form("massLS_L1_%d",nb),nbins,xbins);//  massLS_L1[nb]->Sumw2();
       massOS_L1[nb]        = new TH1D(Form("massOS_L1_%d",nb),        Form("massOS_L1_%d",nb),nbins,xbins);//  massOS_L1[nb]->Sumw2();
       massLSPV_L1[nb]      = new TH1D(Form("massLSPV_L1_%d",nb),      Form("massLSPV_L1_%d",nb),nbins,xbins);//massLSPV_L1[nb]->Sumw2();
       massOSPV_L1[nb]      = new TH1D(Form("massOSPV_L1_%d",nb),      Form("massOSPV_L1_%d",nb),nbins,xbins);//massOSPV_L1[nb]->Sumw2();
      
}

    //TH1 *bkgdM1p5
    
    TH1D* massOS_Inclusive = new TH1D("massOS_Inclusive","massOS_Inclusive",nbins,xbins);//  massOS_L1[nb]->Sumw2();   
    



   for(unsigned int f=0; f<filesToRun.size(); f++){


     cout<<filesToRun[f]<<endl;
    TChain* chain = new TChain("mmtree/tree");
    chain->Add((TString)filesToRun[f]);
    cout<<"here2"<<endl;
    TTreeReader reader(chain);
     cout<<"here3"<<endl;
    TTreeReaderValue<double>*                      wgt;
    TTreeReaderValue<double>*                      xsec;
    //TTreeReaderValue<std::vector<int>* >            pdgid   ;
    //TTreeReaderValue<std::vector<int>* >            motherid ;
    
    TTreeReaderValue<std::vector<float> >          mpt      (reader, "muonpt"     );
    TTreeReaderValue<std::vector<float> >          meta     (reader, "muoneta"    );
    TTreeReaderValue<std::vector<float> >          mphi     (reader, "muonphi"    );
    TTreeReaderValue<std::vector<float> >          mcharge  (reader, "muoncharge" );
    TTreeReaderValue<std::vector<char>  >          mid      (reader, "muonid"     );
    TTreeReaderValue<std::vector<float> >          chi2     (reader, "chi2"       );
    TTreeReaderValue<std::vector<float> >          dxy      (reader, "dxy"        );
    TTreeReaderValue<std::vector<float> >          dz       (reader, "dz"         );
    TTreeReaderValue<std::vector<float> >          cpiso    (reader, "cpiso"      );
    TTreeReaderValue<std::vector<float> >          chiso    (reader, "chiso"      );
    TTreeReaderValue<std::vector<float> >          phiso    (reader, "phiso"      );
    TTreeReaderValue<std::vector<float> >          nhiso    (reader, "nhiso"      );
    TTreeReaderValue<std::vector<float> >          puiso    (reader, "puiso"      );
    TTreeReaderValue<std::vector<bool> >           l1Result (reader, "l1Result"   );
    TTreeReaderValue<std::vector<unsigned char> >  nmhits   (reader, "nMuonHits"  );
    TTreeReaderValue<std::vector<unsigned char> >  nphits   (reader, "nPixelHits" );
    TTreeReaderValue<std::vector<unsigned char> >  ntklayers(reader, "nTkLayers"  );
    TTreeReaderValue<unsigned char>                hlt      (reader, "trig"       );
    TTreeReaderValue<unsigned>                     nverts   (reader, "nvtx"       );
    cout<<"here4"<<endl;
    TTreeReaderValue<std::vector<float> >          vtxX     (reader, "vtxX"       );
    TTreeReaderValue<std::vector<float> >          vtxY     (reader, "vtxY"       );
    TTreeReaderValue<std::vector<float> >          vtxZ     (reader, "vtxZ"       );
    // TTreeReaderValue<std::vector<float> >          vtxchi2  (reader, "vtxchi2"    );
    cout<<"here5"<<endl;
    TTreeReaderValue<double>                       rho      (reader, "rho"        );
    TTreeReaderValue<unsigned int>                 run      (reader, "run"        );
    TTreeReaderValue<unsigned int>                 lumSec   (reader, "lumSec"     );
  cout<<"here6"<<endl;
  //TTreeReaderValue<std::vector<int> >		   pdgid    (reader, "pdgid" );
  //TTreeReaderValue<std::vector<int> > 	   motherid (reader, "motherid");
     cout<<"here7"<<endl;
    
   if (isMC || !isData[f]) {
        wgt  = new TTreeReaderValue<double>(reader, "wgt" );
        xsec = new TTreeReaderValue<double>(reader, "xsec");
	
    }
	 cout<<"here8"<<endl;
 

    double wgtsum  = 1.0;//isMC ? sumwgt(treepath) : 1.0;
    float  puwgt   = 1.0;
    float  mcwgt   = 1.0;
    float  m1pt    = 0.0;        
    float  m1eta   = 0.0;        
    float  m1phi   = 0.0;        
    float  m1iso   = 0.0;        
    float  m2pt    = 0.0;        
    float  m2eta   = 0.0;        
    float  m2phi   = 0.0;        
    float  m2iso   = 0.0;        
    float  mass    = 0.0;        
    char   m1id    = 0;
    char   m2id    = 0;
    float  m1ch    = 0.; 
    float  m2ch    = 0.; 
    vector<bool> l1Bools;
    unsigned nvtx  = 0;
    unsigned Run   = 0;
    unsigned LumSec   = 0;

    int p=0;
    while(reader.Next()) {
      //if(!isData[f]) {if(pdgid->size()<2 || motherid->size()<2) continue;}
      if (((*hlt) & 2) == 0) continue;   

        double ea = (isMC ? 0.3 : 0.45);
        std::vector<unsigned> goodmuons;
        for (std::size_t i = 0; i < mpt->size(); i++) {
	  //if ((*nmhits)[i] == 0)     continue;
	  if ((*nphits)[i] == 0)     continue;
          if ((*ntklayers)[i] <= 5)  continue;
          if ((*chi2)[i] > 10.)      continue;
            double iso = (*cpiso)[i] + (*nhiso)[i] + (*phiso)[i] - ea*(*rho);
	    //	  if (iso > 10.0) continue;

            goodmuons.push_back(i);
        }

        if (goodmuons.size() < 2) continue;

        unsigned idx1 = goodmuons[0];
        unsigned idx2 = goodmuons[1];

        if ((*mpt)[goodmuons[0]] < (*mpt)[goodmuons[1]]) {
            idx1 = goodmuons[1];
            idx2 = goodmuons[0];
	    }
	
        TLorentzVector mm;
        TLorentzVector m1;
        TLorentzVector m2;
        m1.SetPtEtaPhiM((*mpt)[idx1], (*meta)[idx1], (*mphi)[idx1], 0.1057);
        m2.SetPtEtaPhiM((*mpt)[idx2], (*meta)[idx2], (*mphi)[idx2], 0.1057);
        mm += m1;
        mm += m2;

        m1pt   = m1.Pt();
        m1eta  = m1.Eta();
        m1phi  = m1.Phi();
        m1iso  = (*cpiso)[idx1] + (*phiso)[idx1] + (*nhiso)[idx1] - ea*(*rho);
        m1id   = (*mid)[idx1];
        m1ch   = (*mcharge)[idx1];

        m2pt   = m2.Pt();
        m2eta  = m2.Eta();
        m2phi  = m2.Phi();
        m2iso  = (*cpiso)[idx2] + (*phiso)[idx2] + (*nhiso)[idx2] - ea*(*rho);
        m2id   = (*mid)[idx2];
	m2ch   = (*mcharge)[idx2];

	Run=*run;
	LumSec=*lumSec;
	
		
 /*
HIGH MASS TRIGGER NO CORRELATION: ID  0 - 4 - 9
'L1_DoubleMu_12_5',
'L1_DoubleMu_12_8',
'L1_DoubleMu_13_6',
'L1_DoubleMu_15_5',
'L1_DoubleMu_15_7',

'L1_TripleMu_4_4_4',
'L1_TripleMu_5_0_0',
'L1_TripleMu_5_3_3',
'L1_TripleMu_5_5_3',
'L1_QuadMu0',

THE LOW MASS TRIGGER: ID 10
'L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4',

THE LOW MASS TRIGGER WITH CORR: 11 - 17
'L1_DoubleMu4p5er2p0_SQ_OS_Mass7to18',
'L1_DoubleMu4_SQ_OS_dR_Max1p2',
'L1_DoubleMu5_SQ_OS_Mass7to18',
'L1_DoubleMu_20_2_SQ_Mass_Max20',

'L1_DoubleMu0er1p4_SQ_OS_dR_Max1p4',

'L1_DoubleMu4p5_SQ_OS_dR_Max1p2',
'L1_DoubleMu6_SQ_OS',

THE LOW MASS TRIGGER TO MEASURE FAKE: ID 18 - 20 [ONLY IN FROM RUN 305405]
'L1_DoubleMu0er1p5_SQ_dR_Max1p4',
'L1_DoubleMu0er2_SQ_dR_Max1p4',
'L1_DoubleMu0_SQ'

 */
	//'L1_DoubleMu4_SQ_OS_dR_Max1p2', [12]
	//'L1_DoubleMu4p5_SQ_OS_dR_Max1p2', [16]
	//
        mass   = mm.M();
        nvtx = *nverts;
        //outtree->Fill();
	
        bool passPVconstraint = false;
	float BSx = 0.084;
	float BSy = -0.03;
	//float vchi2 = (*vtxchi2)[0];

	if(nvtx>0.){
	  if(  sqrt( ((*vtxX)[0] - BSx)*((*vtxX)[0] - BSx) + ((*vtxY)[0] - BSy)*((*vtxY)[0] - BSy) )  < 0.2 ) passPVconstraint = true;
	  }

	if(m1ch*m2ch<0. && passPVconstraint && goodmuons.size() >= 2)  massOS_Inclusive->Fill(mass);

	for(int bit=0; bit<21; bit++)
	{
		if(l1Result->at(bit) && m1pt > 3. && m2pt > 3.)
		{
		  if(m1ch*m2ch<0. && passPVconstraint && goodmuons.size() >= 2)  massOS_L1[bit]->Fill(mass);
		  if(m1ch*m2ch>0. && passPVconstraint && goodmuons.size() >= 2)  massLS_L1[bit]->Fill(mass);	
		  if(m1ch*m2ch<0. && passPVconstraint && goodmuons.size() == 2)  massOSPV_L1[bit]->Fill(mass);
		  if(m1ch*m2ch>0. && passPVconstraint && goodmuons.size() == 2)  massLSPV_L1[bit]->Fill(mass);		
		}       		
        }
   

   }}


   cout<<"START WRITING TO FILE"<<endl;

   //outtree->Write();
    
   massOS_Inclusive->Write();


    for(int bit = 0; bit<21; bit++){	
    massOS_L1[bit]->Write();
    massLS_L1[bit]->Write();
    massOSPV_L1[bit]->Write();
    massLSPV_L1[bit]->Write();
    }

    outfile->Close();

}
