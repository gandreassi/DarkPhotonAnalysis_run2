#ifndef GETTRIMSCOUTHIST_H
#define GETTRIMSCOUTHIST_H

void getTrimScoutHist(std::string treepath, TH1* hist, bool os, bool isMC = false, bool offline = false) {

    TChain chain("tree");

    TFileCollection fc("fc");
    fc.Add(treepath.c_str());
    chain.AddFileInfoList(fc.GetList());

    TTreeReader reader(&chain);

    TTreeReaderValue<float>* wgt;
    if (isMC) wgt  = new TTreeReaderValue<float>(reader, "mcwgt" );

    TTreeReaderValue<float>     m1pt      (reader, "m1pt"    );
    TTreeReaderValue<float>     m1eta     (reader, "m1eta"   );
    TTreeReaderValue<float>     m1phi     (reader, "m1phi"   );
    TTreeReaderValue<float>     m1iso     (reader, "m1iso"   );
    TTreeReaderValue<char>      m1id      (reader, "m1id"    );

    TTreeReaderValue<float>     m2pt      (reader, "m2pt"    );
    TTreeReaderValue<float>     m2eta     (reader, "m2eta"   );
    TTreeReaderValue<float>     m2phi     (reader, "m2phi"   );
    TTreeReaderValue<float>     m2iso     (reader, "m2iso"   );
    TTreeReaderValue<char>      m2id      (reader, "m2id"    );

    TTreeReaderValue<float>     mass      (reader, "mass"    );
    TTreeReaderValue<unsigned>  nvtx      (reader, "nvtx"    );

    bool blind = true;
    double lumi = 4.25;
    if (blind) lumi /= 10.;

    double isocut = 5.;
    if (!isMC) isocut = 5.;

    int counter = 0;
    while(reader.Next()) {
   
        counter++;
        if (!isMC && blind && counter%10 != 0) continue;

        if ((*m1id)  * (*m2id) > 0 &&  os) continue;         
        if ((*m1id)  * (*m2id) < 0 && !os) continue;         
        if (!offline && ((*m1pt)  < (*mass)/3.0 || (*m2pt) < (*mass)/4.0)) continue;
        if ( offline && ((*m1pt)  < 12.0        || (*m2pt) < 5.0       )) continue;
        if ((*m1iso) > isocut || (*m2iso) > isocut) continue;

        hist->Fill(*mass, weight);

    }
}

#endif
