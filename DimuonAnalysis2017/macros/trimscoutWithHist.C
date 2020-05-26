//#include "sumwgt.h"

void trimscoutWithHist(const char* treepath = "/afs/cern.ch/work/j/jsalfeld/CMSSW_9_2_1/src/DarkPhotonAnalysis/DimuonAnalysis2017/scout.root", const char* outfilename = "./trimscout.root", bool isMC=false) {

  //TFile purwtfile("../data/purwt.root");
  //TH1F* purwthist = (TH1F*)purwtfile.Get("puhist");

    TFileCollection fc("fc");
    fc.Add(treepath);

    TChain* chain = new TChain("mmtree/tree");
    chain->AddFileInfoList(fc.GetList());

    TTreeReader reader(chain);

    TTreeReaderValue<double>*                      wgt;
    TTreeReaderValue<double>*                      xsec;

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
    TTreeReaderValue<double>                       rho      (reader, "rho"        );
    TTreeReaderValue<unsigned int>                 run      (reader, "run"        );

    if (isMC) {
        wgt  = new TTreeReaderValue<double>(reader, "wgt" );
        xsec = new TTreeReaderValue<double>(reader, "xsec");
    }

    TFile* outfile = new TFile(outfilename, "RECREATE");

    TH1F* highMassOS = new TH1F("highMassOS","highMassOS",2.*1996.,0.2,100);
    TH1F* lowMassOS = new TH1F("lowMassOS","lowMassOS",2.*1996.,0.2,100);
    TH1F* yMassOS = new TH1F("yMassOS","yMassOS",2.*1996.,0.2,100);
    TH1F* specMassOS = new TH1F("specMassOS","specMassOS",2.*1996.,0.2,100);
    TH1F* allMassOS = new TH1F("allMassOS","allMassOS",2.*1996.,0.2,100);
    TH1F* lowOrHighMassOS = new TH1F("lowOrHighMassOS","lowOrHighMassOS",2.*1996.,0.2,100);
    TH1F* lowOrYOrHighMassOS = new TH1F("lowOrYOrHighMassOS","lowOrYOrHighMassOS",2.*1996.,0.2,100);
    TH1F* specOrHighMassOS = new TH1F("specOrHighMassOS","specOrHighMassOS",2.*1996.,0.2,100);
    TH1F* specOrYOrHighMassOS = new TH1F("specOrYOrHighMassOS","specOrYOrHighMassOS",2.*1996.,0.2,100);
    
    TH1F* highMassLS = new TH1F("highMassLS","highMassLS",2.*1996.,0.2,100);
    TH1F* lowMassLS = new TH1F("lowMassLS","lowMassLS",2.*1996.,0.2,100);
    TH1F* yMassLS = new TH1F("yMassLS","yMassLS",2.*1996.,0.2,100);
    TH1F* specMassLS = new TH1F("specMassLS","specMassLS",2.*1996.,0.2,100);
    TH1F* allMassLS = new TH1F("allMassLS","allMassLS",2.*1996.,0.2,100);
    TH1F* lowOrHighMassLS = new TH1F("lowOrHighMassLS","lowOrHighMassLS",2.*1996.,0.2,100);
    TH1F* lowOrYOrHighMassLS = new TH1F("lowOrYOrHighMassLS","specOrYOrHighMassLS",2.*1996.,0.2,100);
    TH1F* specOrHighMassLS = new TH1F("specOrHighMassLS","specOrHighMassLS",2.*1996.,0.2,100);
    TH1F* specOrYOrHighMassLS = new TH1F("specOrYOrHighMassLS","specOrYOrHighMassLS",2.*1996.,0.2,100);

    TTree* outtree = new TTree("tree", "tree");

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

    if (isMC) {
    outtree->Branch("mcwgt" , &mcwgt , "mcwgt/F" );
    outtree->Branch("puwgt" , &puwgt , "puwgt/F" );
    }
    outtree->Branch("m1pt"  , &m1pt  , "m1pt/F"  );
    outtree->Branch("m1eta" , &m1eta , "m1eta/F" );
    outtree->Branch("m1phi" , &m1phi , "m1phi/F" );
    outtree->Branch("m1iso" , &m1iso , "m1iso/F" );
    outtree->Branch("m1id"  , &m1id  , "m1id/B"  );
    outtree->Branch("m2pt"  , &m2pt  , "m2pt/F"  );
    outtree->Branch("m2eta" , &m2eta , "m2eta/F" );
    outtree->Branch("m2phi" , &m2phi , "m2phi/F" );
    outtree->Branch("m2iso" , &m2iso , "m2iso/F" );
    outtree->Branch("m2id"  , &m2id  , "m2id/B"  );
    outtree->Branch("mass"  , &mass  , "mass/F"  );
    outtree->Branch("nvtx"  , &nvtx  , "nvtx/i"  );
    //outtree->Branch("l1Bools"  , &l1Bools , "l1Bools" );

    while(reader.Next()) {

      if (((*hlt) & 2) == 0) continue;

        /*if (isMC) {
            mcwgt = (**wgt) * (**xsec) / (400. * wgtsum);
            puwgt = purwthist->GetBinContent(purwthist->FindBin(nvtx));
	    } */           

        double ea = (isMC ? 0.3 : 0.45);
        std::vector<unsigned> goodmuons;
        for (std::size_t i = 0; i < mpt->size(); i++) {
	  //if ((*nmhits)[i] == 0)     continue;
	  if ((*nphits)[i] == 0)     continue;
          if ((*ntklayers)[i] <= 5)  continue;
          if ((*chi2)[i] > 10.)      continue;
            double iso = (*cpiso)[i] + (*nhiso)[i] + (*phiso)[i] - ea*(*rho);
	  if (iso > 10.0) continue;

            goodmuons.push_back(i);
        }

        if (goodmuons.size() != 2) continue;

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
	

	//cout<<m1ch<<"   "<<m2ch<<endl;

	//for (int i=0; i<l1Result->size();i++){
	//  cout<<l1Result->at(i)<<endl;	
	//}


	bool hHighTrig = ( l1Result->at(0) || l1Result->at(1) || l1Result->at(2) || l1Result->at(3) || l1Result->at(4) );
	bool hLowTrig = ( l1Result->at(10) );
	bool hYTrig = ( l1Result->at(11) );
	bool hSpec = ( l1Result->at(18) );
	bool hHighOrSpec = ( hSpec || hHighTrig  );
	bool hHighOrLow = ( hLowTrig || hHighTrig  );
	

	
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



        mass   = mm.M();
        nvtx = *nverts;
        outtree->Fill();


	
	if( hHighTrig ){
		if(m1ch*m2ch<0. ) highMassOS->Fill(mass);
		if(m1ch*m2ch>0. ) highMassLS->Fill(mass);
		}
	if( hLowTrig ){
		if(m1ch*m2ch<0. ) lowMassOS->Fill(mass);
		if(m1ch*m2ch>0. ) lowMassLS->Fill(mass);
		}
	if( hLowTrig || hHighTrig ){
		if(m1ch*m2ch<0. ) lowOrHighMassOS->Fill(mass);
		if(m1ch*m2ch>0. ) lowOrHighMassLS->Fill(mass);
		}
	if( hLowTrig || hHighTrig || hYTrig){
		if(m1ch*m2ch<0. ) lowOrYOrHighMassOS->Fill(mass);
		if(m1ch*m2ch>0. ) lowOrYOrHighMassLS->Fill(mass);
		}
	if( hYTrig ){
		if(m1ch*m2ch<0. ) yMassOS->Fill(mass);
		if(m1ch*m2ch>0. ) yMassLS->Fill(mass);
		}
	if( hSpec ){
		if(m1ch*m2ch<0. ) specMassOS->Fill(mass);
		if(m1ch*m2ch>0. ) specMassLS->Fill(mass);
		}
	if( hSpec || hHighTrig ){
		if(m1ch*m2ch<0. ) specOrHighMassOS->Fill(mass);
		if(m1ch*m2ch>0. ) specOrHighMassLS->Fill(mass);
		}
	if( hSpec || hHighTrig || hYTrig){
		if(m1ch*m2ch<0. ) specOrYOrHighMassOS->Fill(mass);
		if(m1ch*m2ch>0. ) specOrYOrHighMassLS->Fill(mass);
		}

	if( 1 ){
		if(m1ch*m2ch<0. ) allMassOS->Fill(mass);
		if(m1ch*m2ch>0. ) allMassLS->Fill(mass);		
	     	}



	
    }

    outtree->Write();

    highMassOS->Write();    
    highMassLS->Write();
    lowMassOS->Write();    
    lowMassLS->Write();
    specMassOS->Write();    
    specMassLS->Write();
    yMassOS->Write();    
    yMassLS->Write();
    lowOrYOrHighMassOS->Write();    
    lowOrYOrHighMassLS->Write();
    lowOrHighMassOS->Write();    
    lowOrHighMassLS->Write();
    specOrHighMassOS->Write();    
    specOrHighMassLS->Write();
    specOrYOrHighMassOS->Write();    
    specOrYOrHighMassLS->Write();
    allMassOS->Write();    
    allMassLS->Write();

    outfile->Close();
}
