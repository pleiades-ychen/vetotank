{
  gStyle->SetOptStat(0);
  
  TFile *fin = new TFile("ChHist_ba133_cw.root");
  TH1D *ch[8];
  for(int i=0;i<8;i++) {
    ch[i] = (TH1D*) fin->Get(Form("PMT%d",i));
    ch[i]->SetLineColor(i+1);
    ch[i]->SetLineWidth(2);
    if(i==0){
      ch[i]->Draw();
      //see post-comments
      ch[i]->SetTitle("Ba133 at West;"
		      "Photoelectrons; Events / PE");
    }
    else
      ch[i]->Draw("same");
    ch[i]->GetXaxis()->SetRange(2,101);
    //ba133 (2,51)
    //cs137 (2,101)
    
    gPad->SetLogy();
  }
  
  TLegend *leg = new TLegend(0.6,0.3,0.9,0.9);
  for(int i=0;i<8;i++)
    leg->AddEntry(ch[i],Form("Channel %d",i),"l");
  leg->Draw();



}

/*
Ba133 at Center
Ba133 at East
Ba133 at West
Cs137 at Center
Cs137 at East
Cs137 at West

*/
