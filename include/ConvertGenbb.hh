/// Author:  Benton Pahlka , <pahlka@fnal.gov>
/// Copyright: 2012 (C) MINOS - Fermi National Accelerator Laboratory

#ifndef ConvertGenbb_h
#define ConvertGenbb_h

#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <math.h>

#define ARRAY_INDEX 100000

using std::cout;
using std::cin;
using std::ios;
using std::cerr;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;

class ConvertGenbb{

public:

  ConvertGenbb();
  ~ConvertGenbb();

private:

  int first_event;
  int num_events;
  string filler;
  int int_filler;
  double dbl_filler;
  
public:
  
  struct _Event {
    
    int event_number;
    float event_start_time;
    int ptles_emitted;
    
    struct _Particle{
      int G_ptle_num;
      float px, py, pz;
      double time_shift;
      float energy;
    } Particle[30];
    
    inline int operator ==(const _Event& right) const
    {return (this==&right);}
    
    
  } Event[ARRAY_INDEX];
  

public:
  
  void ReadEvents(char* infile_name);
  void DumpSingleEvent(char* outfile_name, int evt);
  void DumpSingleEventStdOut(int evt);
  void DumpEventEnergies(char* outfile_name, int evt);
  
};
#endif
