{
  TChain tch("theRunTree");
  for(i=0;i<42;i++) 
    tch.AddFile(Form("mu10GeVstrdn_%d.root",i));
  
  tch.Draw("RunStatistics.Deposit>>h_edep(100,0,50)");
  new TCanvas;
  tch.Draw("RunStatistics.Detected");

  TH1F *h_edep = (TH1F*)gDirectory->Get("h_edep");
  int amp [100];
  double en [100];
  for(int i=0;i<100;i++) {
    en[i] = (double)(i+1)*0.5;
    amp[i] = h_edep->GetBinContent(i+1);
  }

  for(int i=0;i<100;i++)
    printf("%d, ",amp[i]);
  
}
