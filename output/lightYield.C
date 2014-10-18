struct RunTally {
  float IncidentE;
  float Deposit;
  int Generated;
  int Absorbed;
  int BulkAbs;
  int Escaped;
  int Failed;
  int Detected;
  int WLS;
  
  inline int operator ==(const RunTally& right) const
  {return (this==&right);}
};

void lightYield(){
  TH2F* ly = new TH2F("ly","Photonelectrons v.s. Deposited Energy;Deposited (keV);PE",
		      1500,0,1500,
		      500,0,500);
  TH1F* edep = new TH1F("edep","Deposited Energy;Edep (keV);Count/keV",1500,0,1500);
  TH1F* pe = new TH1F("pe","Photoelectrons;PE);Count/PE",500,0,500);
  RunTally runStat;
  //double edep;
  //int pe;
  TFile* datafile = new TFile("test_0801_lastrun.root");
  TTree* T = (TTree*)datafile->Get("theRunTree");
  T->SetBranchAddress("RunStatistics",&runStat);
  
  int Entries = T->GetEntries();
  for(int n=0;n<Entries;n++){
    T->GetEntry(n);
    ly->Fill(1E3*runStat.Deposit,runStat.Detected);
    if(runStat.Deposit!=0.) edep->Fill(1E3*runStat.Deposit);
    if(runStat.Detected!=0.) pe->Fill(runStat.Detected);
  }
  TCanvas* c1 = new TCanvas("c1");
  ly->SetMarkerStyle(1);
  ly->Draw();
  
  TCanvas* c2 = new TCanvas("c2");
  edep->Draw();
  c2->SetLogy();

  TCanvas* c3 = new TCanvas("c3");
  pe->Draw();
  c3->SetLogy();
}
