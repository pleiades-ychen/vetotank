/// vetotankTrackInformation.hh
/// Author: Benton Pahlka <pahlka@physics.utexas.edu> , <pahlka@fnal.gov>
/// Copyright: 2012 (C) MINOS - Fermi National Accelerator Laboratory

#ifndef vetotankTrackInformation_h
#define vetotankTrackInformation_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4Allocator.hh"
#include "G4Track.hh"
#include "G4VUserTrackInformation.hh"

class vetotankTrackInformation : public G4VUserTrackInformation
{
 public:
  vetotankTrackInformation();
  vetotankTrackInformation(const G4Track *aTrack);
  ~vetotankTrackInformation();
  
  inline void *operator new(size_t);
  inline void operator delete(void *aTrackInfo);
  
 private:
  G4ThreeVector BirthPosition;
  G4double BirthLambda;
  G4double DistBeforeWLS;
  G4int Reflections;
  G4int ReflectBeforeWLS;
  G4int ReflectAfterWLS;
  G4double DistBefore;
  G4double DistAfter;
  G4int WLSCount;
  G4int TotalInternalReflections;
  G4double TimeBeforeWLS;
  G4double TimeAfterWLS;
  G4double WLSTime[60];
  G4double the_local_time;

  G4double my_time;

  G4double my_sum[30];
  
 public:

  G4ThreeVector GetBirthPosition() const {return BirthPosition;}
  G4double GetBirthLambda() const {return BirthLambda;}
  G4double GetDistBeforeWLS() const {return DistBeforeWLS;}
  G4int GetReflections() {return Reflections;}
  G4int GetReflectBeforeWLS() const {return ReflectBeforeWLS;}
  G4int GetReflectAfterWLS() const {return ReflectAfterWLS;}
  G4double GetTrackLength() const {return DistBefore;}
  G4int GetWLSCount() const {return WLSCount;}
  G4int GetTotalInternalReflections() const {return TotalInternalReflections;}
  G4double GetTime() {return my_time;}

  void CountWLS(){WLSCount++;}

  void CountTime(){TimeAfterWLS += the_local_time;}

  void CountReflections();
  void CountTotalInternalReflections();
  G4double ReturnTime(G4int wlscount);

  void SetDistBeforeWLS(G4double dist){DistBeforeWLS=dist;}
  void Print() const {;}
};

extern G4Allocator<vetotankTrackInformation> TrackInformationAllocator;

inline void* vetotankTrackInformation::operator new(size_t){ 
    void* aTrackInfo;
    aTrackInfo = (void*)TrackInformationAllocator.MallocSingle();
    return aTrackInfo;
}

inline void vetotankTrackInformation::operator delete(void *aTrackInfo){ 
    TrackInformationAllocator.FreeSingle((vetotankTrackInformation*)aTrackInfo);
}

#endif
