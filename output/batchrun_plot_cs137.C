{
  TChain tch("theRunTree");
  for(int i=0;i<10;i++) 
    tch.AddFile(Form("steelbox_cs137_cc_%d.root",i));//!!!
  
  tch.Draw("RunStatistics.Deposit>>h_cs_edep","RunStatistics.Deposit!=0","");
  TH1F *h_edep = (TH1F*)gDirectory->Get("h_cs_edep");
  h_edep->SetTitle("Collimated Cs137 at Center; Deposited (MeV);"); //!!!
  h_edep->SetLineWidth(2);
  h_edep->SetLineColor(4);
  new TCanvas;
  tch.Draw("RunStatistics.Detected>>h_cs_pe","RunStatistics.Detected!=0","");
  TH1F *h_pe = (TH1F*)gDirectory->Get("h_cs_pe");
  h_pe->SetTitle("Collimated Cs137 at Center; Photoelectrons;"); //!!!
  h_pe->SetLineWidth(2);
  h_pe->SetLineColor(2);

  TFile *outfile = new TFile("mc_spectra_20140917_cs137_cc.root","update");//!!!
  h_edep->Write();
  h_pe->Write();
  outfile->Close();

  /*
  TH1F *h_edep = (TH1F*)gDirectory->Get("h_edep");
  int amp [100];
  double en [100];
  for(int i=0;i<100;i++) {
    en[i] = (double)(i+1)*0.5;
    amp[i] = h_edep->GetBinContent(i+1);
  }

  for(int i=0;i<100;i++)
    printf("%d, ",amp[i]);
  */
}
