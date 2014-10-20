{
  TChain tch("theRunTree");
  for(int i=0;i<10;i++) 
    tch.AddFile(Form("steelbox_ba133cc_%d.root",i)); //!!!
  
  tch.Draw("RunStatistics.Deposit>>h_ba_edep","RunStatistics.Deposit!=0","");
  TH1F *h_edep = (TH1F*)gDirectory->Get("h_ba_edep"); 
  h_edep->SetTitle("Collimated Ba133 at Center; Deposited (MeV);"); //!!!
  h_edep->SetLineWidth(2);
  h_edep->SetLineColor(4);
  new TCanvas;
  tch.Draw("RunStatistics.Detected>>h_ba_pe","RunStatistics.Detected!=0","");
  TH1F *h_pe = (TH1F*)gDirectory->Get("h_ba_pe");
  h_pe->SetTitle("Collimated Ba133 at Center; Photoelectrons;"); //!!!
  h_pe->SetLineWidth(2);
  h_pe->SetLineColor(2);

  TFile *outfile = new TFile("mc_spectra_20140917_ba133_cc.root","update"); //!!!
  h_edep->Write();
  h_pe->Write();
  outfile->Close();
 
}

/*
  cc, Center
  ce, East
  cw, West

*/
