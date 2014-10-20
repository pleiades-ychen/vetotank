/// Author: Yu Chen <ychen87@syr.edu>
/// 

#include "GeometryHelper.hh"
#include "G4SystemOfUnits.hh"

GeometryHelper::GeometryHelper() { 
  x_local[0] = -1.905*mm;
  x_local[1] = 1.905*mm;
  x_local[2] = -1.905*mm;
  x_local[3] = 1.905*mm;

  y_local[0] = 1.905*mm;
  y_local[1] = 1.905*mm;
  y_local[2] = -1.905*mm;
  y_local[3] = -1.905*mm;

  x_bundle[0] = -114.3*mm;
  x_bundle[1] = -38.1*mm;
  x_bundle[2] = 38.1*mm;
  x_bundle[3] = 114.3*mm;

  y_bundle[0] = 0.*mm;
  y_bundle[1] = 0.*mm;
  y_bundle[2] = 0.*mm;
  y_bundle[3] = 0.*mm;
}

GeometryHelper::~GeometryHelper() {

}


G4double GeometryHelper::GetFiber_X (int iFiber) {
  int iLocal = iFiber % 4;
  int iBundle = iFiber / 4;
  G4double x;
  x = x_local[iLocal] + x_bundle[iBundle];
  return x;
}

G4double GeometryHelper::GetFiber_Y (int iFiber) {
  int iLocal = iFiber % 4;
  int iBundle = iFiber / 4;
  G4double y;
  y = y_local[iLocal] + y_bundle[iBundle];
  return y;
}

G4double GeometryHelper::GetSiPM_X (int iSiPM) {
  return x_bundle[iSiPM];
}

G4double GeometryHelper::GetSiPM_Y (int iSiPM) {
  return y_bundle[iSiPM];
}
