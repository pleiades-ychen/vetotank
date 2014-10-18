{
//   example of macro to read data from an ascii file and
//   create a root file with an histogram and an ntuple.
   gROOT->Reset();
#include "Riostream.h"

   ifstream in;

   in.open("full_popop_emission.cfg");

   Float_t x;
   Float_t y;
   Int_t nlines = 0;
   TFile *f = new TFile("file1.root","RECREATE");
   TH1F *h1 = new TH1F("h1","x distribution",500,200,700);
   TNtuple *ntuple = new TNtuple("ntuple","data from ascii file","x:y");
   TString filler;


   while (!in.eof()) {

          in >> x >> y;

      if (!in.good()) break;
      if (nlines < 5) printf("x=%8f, y=%8f",x,y);
      h1->Fill(x,y);
      ntuple->Fill(x,y);
      nlines++;
   }
h1->Draw();
   printf(" found %d pointsn",nlines);

   in.close();

   f->Write();
}
