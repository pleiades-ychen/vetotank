#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <TStyle.h>
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2D.h"
#include "TPad.h"
#include "TAxis.h"

using namespace std;

struct Tally {

  // ABSOLUTELY must correspond to the order
  // in which variables were set in the G4 code

  float BirthX;
  float BirthY;
  float BirthZ;
  float BirthLambda;
  float DeathLambda;
  int   Reflections;
  int   ReflectBeforeWLS;
  int   ReflectAfterWLS;
  float Distance_in_Z;
  int   TotalInternalReflections;
  int   WLSCount;
  float WLSZ;
  int   PMTnumber;
  float ExitAngle;
  float DistBefore;
  float DistAfter;
  float TotTrackLen;
  float DeathX;
  float DeathY;
  float DeathZ;
  float TotalTOF;
  float GlobalTOF;

};

class Channel_PE_Info {
public:
  Channel_PE_Info();
  ~Channel_PE_Info();
public:
  double GetNpeFracByID(int chID){return npe_frac[chID];};
  int GetNpeTot(){return npe_tot;};
  void UpdateNpeFrac();
  void Fill(int);
private:
  int npe_ch[8];
  int npe_tot;
  double npe_frac[8];
};

Channel_PE_Info::Channel_PE_Info()
{
  for(int i=0;i<8;i++)
    npe_ch[i] = 0;
  npe_tot = 0;
}

Channel_PE_Info::~Channel_PE_Info() 
{
  /*~~~~~~~~~*/
}

void Channel_PE_Info::UpdateNpeFrac()
{
  for (int i=0; i<8; i++)
    npe_tot += npe_ch[i];
  if(npe_tot != 0) {
    for (int i=0; i<8; i++)
      npe_frac[i] = (double)npe_ch[i]/npe_tot;
  }
}

void Channel_PE_Info::Fill(int chID)
{
  npe_ch[chID]++;
}

void channels_scatters(int numFiles, int numEvents) 
{
  TH2D *hSC = new TH2D("hSC","Ba133 at Center;npe fraction;channels", //!!!
		       100,0,1.,
		       8,0,8);
  //set file loop
  for(int n = 0; n < numFiles; n++){
    char FileName[20];
    sprintf(FileName,"ba133cc_0%d.root",n); //!!!
    TFile fin(FileName,"READ");
    cout<< FileName <<" has been opened.\n";
    //set event loop
    for (int i = 0; i < numEvents; i++) {
      /********Event*****************/
      Tally OnePhoton;
      Channel_PE_Info peOneEvent;
      char EventName[12];
      sprintf(EventName,"Event%d",i);
      TTree *theTree = (TTree*)fin.Get(EventName);
      if(!theTree) break;
      theTree->SetBranchAddress(EventName,&OnePhoton);
      int numphotons = (int) theTree->GetEntries();
      
      for(int j=0; j<numphotons; j++) {
	theTree->GetEntry(j);    
	peOneEvent.Fill(OnePhoton.PMTnumber);
      }
      peOneEvent.UpdateNpeFrac();
      if(peOneEvent.GetNpeTot()!=0) {
	for(int k=0;k<8;k++) {
	  hSC->Fill(peOneEvent.GetNpeFracByID(k),k);
	}
      }
      /********Event*****************/
    }
  }
  
  //gStyle->SetOptStat(0);
  //gStyle->SetPalette(1,0);
  hSC->Draw("colz");
  gPad->SetLogz();
  
  new TCanvas;
  TProfile *pr = hSC->ProfileY("channels_profile",0,100,"d");
  //pr->Draw();
}

/*
Ba133 at Center
Ba133 at East
Ba133 at West
Cs137 at Center
Cs137 at East
Cs137 at West

*/

/*
  
  
*/
