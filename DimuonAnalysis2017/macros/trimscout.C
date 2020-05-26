#include "sumwgt.h"

void trimscout(const char* treepath = "/media/Disk1/avartak/CMS/Data/Dileptons/ScoutingPFMuons/scout_RunD.root", const char* outfilename = "/media/Disk1/avartak/CMS/Data/Dileptons/ScoutingPFMuons/trimscout_RunD.root", bool isMC=false) {

    TFile purwtfile("../data/purwt.root");
    TH1F* purwthist = (TH1F*)purwtfile.Get("puhist");

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

    TTreeReaderValue<std::vector<char>  >          mid      (reader, "muonid"     );
    TTreeReaderValue<std::vector<float> >          chi2     (reader, "chi2"       );
    TTreeReaderValue<std::vector<float> >          dxy      (reader, "dxy"        );
    TTreeReaderValue<std::vector<float> >          dz       (reader, "dz"         );
    TTreeReaderValue<std::vector<float> >          cpiso    (reader, "cpiso"      );
    TTreeReaderValue<std::vector<float> >          chiso    (reader, "chiso"      );
    TTreeReaderValue<std::vector<float> >          phiso    (reader, "phiso"      );
    TTreeReaderValue<std::vector<float> >          nhiso    (reader, "nhiso"      );
    TTreeReaderValue<std::vector<float> >          puiso    (reader, "puiso"      );
    TTreeReaderValue<std::vector<unsigned char> >  nmhits   (reader, "nMuonHits"  );
    TTreeReaderValue<std::vector<unsigned char> >  nphits   (reader, "nPixelHits" );
    TTreeReaderValue<std::vector<unsigned char> >  ntklayers(reader, "nTkLayers"  );
    TTreeReaderValue<unsigned char>                hlt      (reader, "trig"       );
    TTreeReaderValue<unsigned>                     nverts   (reader, "nvtx"       );
    TTreeReaderValue<double>                       rho      (reader, "rho"        );

    if (isMC) {
        wgt  = new TTreeReaderValue<double>(reader, "wgt" );
        xsec = new TTreeReaderValue<double>(reader, "xsec");
    }

    TFile* outfile = new TFile(outfilename, "RECREATE");
    TTree* outtree = new TTree("tree", "tree");

    double wgtsum  = isMC ? sumwgt(treepath) : 1.0;
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

    while(reader.Next()) {

        if (((*hlt) & 2) == 0) continue;

        if (isMC) {
            mcwgt = (**wgt) * (**xsec) / (400. * wgtsum);
            puwgt = purwthist->GetBinContent(purwthist->FindBin(nvtx));
        }            

        double ea = (isMC ? 0.3 : 0.45);
        std::vector<unsigned> goodmuons;
        for (std::size_t i = 0; i < mpt->size(); i++) {
            if ((*nmhits)[i] == 0)     continue;
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
        

        m2pt   = m2.Pt();
        m2eta  = m2.Eta();
        m2phi  = m2.Phi();
        m2iso  = (*cpiso)[idx2] + (*phiso)[idx2] + (*nhiso)[idx2] - ea*(*rho);
        m2id   = (*mid)[idx2];

        mass   = mm.M();

        nvtx = *nverts;

        outtree->Fill();

    }

    outtree->Write();

    outfile->Close();
}
