//
//
//
//
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "TFile.h"
#include "TH1.h"
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

void generate_histograms(){

    const Int_t num_files;
    cout << "Enter number of files: " << endl;
    cin >> num_files;

    const Int_t num_bins;
    cout << "Enter number of bins: " << endl;
    cin >> num_bins;

    TString file_name_out;
    file_name_out = "compiled_data.root";
    TFile *outputdata = new TFile(file_name_out,"RECREATE");

    char FileName[80];

    TH1F *hBirthX = new TH1F("hBirthX", "Starting X-Position",num_bins, -200, 200);
    TH1F *hBirthY = new TH1F("hBirthY", "Starting Y-Position",num_bins,-100, 100);
    TH1F *hBirthZ = new TH1F("hBirthZ", "Starting Z-Position",num_bins,-1000, 1000);
    TH1F *hBirthLambda = new TH1F("hBirthLambda", "Starting Wavelength", num_bins, 200, 500);
    TH1F *hDeathLambda = new TH1F("hDeathLambda", "Final Wavelength of Detected Photons", num_bins, 350, 700);
    TH1F *hDistBefore = new TH1F("hDistBefore", "Dist Before",num_bins, 0, 10000);
    TH1F *hDistAfter = new TH1F("hDistAfter", "Dist After",num_bins, 0, 10000);
    TH1F *hReflections = new TH1F("hReflections", "Total Number of Reflections",num_bins, 0, 100);
    TH1F *hReflectBeforeWLS = new TH1F("hReflectBeforeWLS", "Reflect Before", 10, 0, 10);
    TH1F *hReflectAfterWLS = new TH1F("hReflectAfterWLS", "Reflect After", 50, 0, 50);
    TH1F *hTotalInternalReflections = new TH1F("hTotalInternalReflections", "Total Internal Reflections", 400, 0, 400);
    TH1F *hWLSZ = new TH1F("hWLSZ", "WLSZ", num_bins, 0, 1000);
    TH1F *hWLSCount = new TH1F("hWLSCount", "",30, 0, 30);
    TH1F *hPMTnumber = new TH1F("hPMTnumber", "",2, 0, 2);
    TH1F *hExitAngle = new TH1F("hExitAngle", "Exit Angle of Detected Photons", num_bins, -1.6, 1.6);
    TH1F *hTotTrackLen = new TH1F("hTotTrackLen", "TTL", 200, 0, 50000);
    TH1F *hDeathX = new TH1F("hDeathX", "",num_bins, -200, 200);
    TH1F *hDeathY = new TH1F("hDeathY", "",num_bins, -100, 100);
    TH1F *hDeathZ = new TH1F("hDeathZ", "",num_bins, -1100, 1100);
    TH1F *hTotalTOF = new TH1F("hTotalTOF", "",num_bins, 0, 100);
    TH1F *hGlobalTOF = new TH1F("hGlobalTOF", "",num_bins, 0, 100);

    TH1F *hFirstTubeTime = new TH1F("hFirstTubeTime", "First Tube Time",num_bins, 0, 50);
    TH1F *hSecondTubeTime = new TH1F("hSecondTubeTime", "Second Tube Time",num_bins, 0, 50);

    TH2F *hExitvsDeathLambda = new TH2F("hExitvsDeath","ExitvsDeath of Detected Photons",num_bins, -1.6, 1.6, num_bins, 300, 700 );
    TH2F *hBirthPosition = new TH2F("hBirthPosition", "Final Positions", num_bins, -0.6, 0.6, num_bins, -0.6, 0.6);
    TH2F *hDeathPosition = new TH2F("hDeathPosition", "Final Positions", num_bins, -200, 200, num_bins, -200, 200);
    TH2F *hTTLvsTIR = new TH2F("hTTLvsTIR", "TTLvsTIR", 200, 0, 10000, 200, 0, 100000 );



    TH1F *hTimeDiff = new TH1F("hTimeDiff", "",200, -20, 20);

    float  MaxE = 0;
    float  MinE = 0;
    
    float Max1 = 0;
    float Min1 = 1000;
    
    float Max2 = 0;
    float Min2 = 1000;
    
    //    for (int k = 0; k < num_files; k++){
    //      sprintf(FileName,"250k_1m_offaxis_%d.root",k+1);

    //   sprintf(FileName,"d.root");
          sprintf(FileName,"test_deathX.root");

      cout << "File: " << FileName << endl;

      TFile event_file(FileName);


  
  Int_t numevents = event_file.GetNkeys()-1;
  //  Int_t MaxExitAngle=0,MaxDist=0,MaxDistZ=0;
  //  Int_t MaxDetSpec=0,MaxScintSpec=0;
  //  Int_t MaxRefls=0,MaxWLSCount=0;
  for(int i = 0; i < numevents; i++){

      
      Tally OnePhoton;
      char EventName[12];
      sprintf(EventName,"Event%d",i);
      TTree *theTree = (TTree*)event_file.Get(EventName);
      if(!theTree) break;
      theTree->SetBranchAddress(EventName,&OnePhoton);
      
      // simple test for max and min
      MaxE = theTree->GetMaximum("GlobalTOF");
      MinE = theTree->GetMinimum("GlobalTOF");
      cout << "Maximum = " << MaxE << "     Minimum = " << MinE << endl; 
      
      Int_t numphotons = (Int_t)theTree->GetEntries();
      cout << "The number of photons is " << numphotons << endl;
      
      for (Int_t j = 0; j < numphotons; j++){
	theTree->GetEntry(j);

	hDeathLambda->Fill(OnePhoton.DeathLambda);
	hExitAngle->Fill(OnePhoton.ExitAngle);
	hBirthX->Fill(OnePhoton.BirthX);
	hBirthY->Fill(OnePhoton.BirthY);
	hBirthZ->Fill(OnePhoton.BirthZ);
	hBirthLambda->Fill(OnePhoton.BirthLambda);
	hDeathLambda->Fill(OnePhoton.DeathLambda);
	hDistBefore->Fill(OnePhoton.DistBefore);
	hDistAfter->Fill(OnePhoton.DistAfter);
	hReflections->Fill(OnePhoton.Reflections);
	hReflectBeforeWLS->Fill(OnePhoton.ReflectBeforeWLS);
	hReflectAfterWLS->Fill(OnePhoton.ReflectAfterWLS);
	hTotalInternalReflections->Fill(OnePhoton.TotalInternalReflections);
	hWLSZ->Fill(OnePhoton.WLSZ);
	hWLSCount->Fill(OnePhoton.WLSCount);
	hPMTnumber->Fill(OnePhoton.PMTnumber);
	hExitAngle->Fill(OnePhoton.ExitAngle);
	hTotTrackLen->Fill(OnePhoton.TotTrackLen + OnePhoton.DistBefore);
	hDeathX->Fill(OnePhoton.DeathX);
	hDeathY->Fill(OnePhoton.DeathY);
	hDeathZ->Fill(OnePhoton.DeathZ);

	hTotalTOF->Fill(OnePhoton.TotalTOF); 
	hGlobalTOF->Fill(OnePhoton.GlobalTOF);

	hExitvsDeathLambda->Fill(OnePhoton.ExitAngle, OnePhoton.DeathLambda);
	hDeathPosition->Fill(OnePhoton.DeathX, OnePhoton.DeathY);
	hBirthPosition->Fill(OnePhoton.BirthX, OnePhoton.BirthY);
	hTTLvsTIR->Fill(OnePhoton.TotTrackLen, OnePhoton.TotalInternalReflections);

	// analyze the individual tubes

	if (OnePhoton.PMTnumber == 0){
	  hFirstTubeTime->Fill(OnePhoton.GlobalTOF);
	  if (OnePhoton.GlobalTOF > Max1){
	    Max1 = OnePhoton.GlobalTOF;
	    cout << "Max1 = " << Max1 << endl;
	  }
	  if (OnePhoton.GlobalTOF < Min1){
	    Min1 = OnePhoton.GlobalTOF;
	    cout << "Min1 = " << Min1 << endl;

	  }


	  
	  //	  hFirstTubeTime->GetXaxis()->SetRangeUser(hFirstTubeTime->GetMean()-5*hFirstTubeTime->GetRMS(), hFirstTubeTime->GetMean()+5*hFirstTubeTime->GetRMS());

	}







	if (OnePhoton.PMTnumber == 1){
	  hSecondTubeTime->Fill(OnePhoton.GlobalTOF);
	  if (OnePhoton.GlobalTOF > Max2){
	    Max2 = OnePhoton.GlobalTOF;
	    cout << "Max2 = " << Max2 << endl;
	  }
	  if (OnePhoton.GlobalTOF < Min2){
	    Min2 = OnePhoton.GlobalTOF;
	    cout << "Min2 = " << Min2 << endl;

	  }




	  //	  hSecondTubeTime->GetXaxis()->SetRangeUser(h->GetMean()-5*h->GetRMS(), h->GetMean()+5*h->GetRMS());

	}

      }

      // end counting the photons, now we can fit the histogram
      /*
	  hFirstTubeTime->Fit("landau","R", "", Min1, Max1);
	  TF1 *fitfunc1 = hFirstTubeTime->GetFunction("landau");
	  Double_t a1 = fitfunc1->GetParameter(0);
	  Double_t b1 = fitfunc1->GetParameter(1);
	  Double_t c1 = fitfunc1->GetParameter(2);
	  cout << "First parameter =  " << a1 << endl;
	  cout << "Second parameter = " << b1 << endl;
	  cout << "Third parameter =  " << c1 << endl;
	  Double_t delta1 = fitfunc1->GetParError(1);
	  char label1[50];
	  sprintf(label1,"Mean Value = %f +/- %f", b1, delta1);
	  cout<<label1<<endl;

	  hSecondTubeTime->Fit("landau","R", "", Min2, Max2);
	  TF1 *fitfunc2 = hSecondTubeTime->GetFunction("landau");
	  Double_t a2 = fitfunc2->GetParameter(0);
	  Double_t b2 = fitfunc2->GetParameter(1);
	  Double_t c2 = fitfunc2->GetParameter(2);
	  cout << "First parameter =  " << a2 << endl;
	  cout << "Second parameter = " << b2 << endl;
	  cout << "Third parameter =  " << c2 << endl;
	  Double_t delta2 = fitfunc2->GetParError(1);
	  char label2[50];
	  sprintf(label2,"Mean Value = %f +/- %f", b2, delta2);
	  cout<<label2<<endl;

	  hTimeDiff->Fill(b1-b2);
      */
      
  } //end loop on events
    
    outputdata->Write();
    
}
