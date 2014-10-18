{

gROOT->Reset();
gStyle->SetOptFit();
#include <fstream.h>
float x1[500], absorb1[500], absorb_length[500];
float x2[500], absorb2[500];

   ifstream in1;
   ifstream in2;

Int_t n1;
Int_t n2;

TString file1;
TString file2;

TString label1="Detector: ";
Int_t nlines1 = 0;
Int_t nlines2 = 0;

TFile *f = new TFile("fatesout.root","RECREATE");
TNtuple *ntuple = new TNtuple("ntuple","data from ascii file","x");
TString title;
TString top;
TString start="Histogram ";
TString filler;

  cout<<endl<<"Filename 1: "<<endl;
  cin>>file1;
  in1.open(file1);

  cout<<endl<<"Filename 2: "<<endl;
  cin>>file2;
  in2.open(file2);

  top=start;
  title=label1+file1;

int index1=0;
int index2=0;

c1 = new TCanvas("c1","Title", 200, 10, 600, 600);

while(!in1.eof()){in1 >> x1[index1] >> filler >> absorb1[index1]; index1++;}
in1.close();

while(!in2.eof()){in2 >> x2[index2] >> filler >> absorb2[index2]; index2++;}
in2.close();

  for (int i=0; i<500; i++){

       absorb_length[i] = (absorb1[i]*absorb2[i])/((absorb1[i]*0.0002)+(absorb2[i]*0.9998));
       //       absorb_length[i] = (absorb1[i]*absorb2[i])/((absorb1[i]*0.5)+(absorb2[i]*0.5));

    //absorb_length[i] = (absorb1[i]*absorb2[i])/((absorb1[i])+(absorb2[i]));
    //    absorb_length[i] = absorb1[i] - absorb2[i];

          cout << x1[i] << " " << absorb_length[i] << endl;

  }



graph=new TGraph(index1, x1, absorb_length);



   graph->Draw("AP");
   c1->Update();
   graph->GetHistogram()->SetXTitle("Wavelength (nm)");
   graph->GetHistogram()->SetYTitle("Counts");

   /* fill options */
   graph->SetFillColor(0);
   graph->SetFillStyle(1);

   /* line options */
   graph->SetLineColor(3);
   graph->SetLineWidth(10);
   graph->SetLineStyle(1);

   /* marker options */
   graph->SetMarkerColor(2);
   graph->SetMarkerStyle(4);
   graph->SetMarkerSize(1);



//   graph->Fit("landau","q");

   c1->Update();
//   TPaveStats *stats1 = (TPaveStats*)gr1->GetListOfFunctions()->FindObject("stats");
//   stats1->SetTextColor(kBlue); 
//   stats1->SetX1NDC(0.0); stats1->SetX2NDC(0.5); stats1->SetY1NDC(0.75);
   c1->Modified();

}
