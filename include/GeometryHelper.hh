/// Author: Yu Chen <ychen87@syr.edu>

#ifndef GeometryHelper_h
#define GeometryHelper_h

#include "globals.hh"

class GeometryHelper 
{
public:
  GeometryHelper();
  ~GeometryHelper();
  G4double GetFiber_X (int iFiber);
  G4double GetFiber_Y (int iFiber);
  G4double GetSiPM_X (int iSiPM);
  G4double GetSiPM_Y (int iSiPM);

private:
  double x_local[4];
  double y_local[4];

  double x_bundle[4];
  double y_bundle[4];
};
#endif
