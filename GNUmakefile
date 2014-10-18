##### GNUmakefile #####
# ----------------------------------------------------------
# Script defining rules and paths for making binaries.
# ----------------------------------------------------------
# Automatic creation of GNUmakefile for Momo environment.
# with users selection of G4TARGET, UI and VIS variables
#  Momo is the name of legendary samurai who conquered bad geants. ----   H. Yoshida.

name := vetotank
G4TARGET := $(name)
G4EXLIB := true

# Vis/GUI used
G4VIS_USE_VRMLFILE := 1
# Vis/GUI built
G4VIS_BUILD_VRMLFILE_DRIVER := 1

# You must provide these four variables!  

#G4SYSTEM:=Darwin-g++
#G4INSTALL:=/usr/local/GEANT4/geant4.9.4.p02
#G4WORKDIR:=/usr/local/GEANT4/geant4.9.4.p02

#G4SYSTEM:=Linux-g++
#G4INSTALL:=${HOME}/GEANT4/geant4.7.0.p01
#G4WORKDIR:=${HOME}/GEANT4/geant4.7.0.p01

#G4LEVELGAMMADATA:=${HOME}/GEANT4/data/PhotonEvaporation2.0

ifndef G4INSTALL
   G4INSTALL = ../../..
endif

.PHONY: all
all: lib bin

  include $(G4INSTALL)/config/binmake.gmk

########################### ROOT #################################


ifdef ROOTSYS
ifndef G4UI_USE_ROOT
  ROOTCPPFLAGS   = $(shell $(ROOTSYS)/bin/root-config --cflags)
  #ROOTCPPFLAGS  := $(filter-out -D_REENTRANT,$(ROOTCPPFLAGS))
  #ROOTCPPFLAGS  := $(filter-out -pthread,$(ROOTCPPFLAGS))
  CPPFLAGS      += -DG4ANALYSIS_USE_ROOT $(ROOTCPPFLAGS)
  ROOTLIBS       = $(shell $(ROOTSYS)/bin/root-config --nonew --glibs)
  ROOTLIBS      := $(filter-out -lNew,$(ROOTLIBS))
  #ROOTLIBS      := $(filter-out -lThread,$(ROOTLIBS))
  #ROOTLIBS      := $(filter-out -lpthread,$(ROOTLIBS))
  #ROOTLIBS      := $(filter-out -pthread,$(ROOTLIBS))
  ROOTLIBS      += -lMinuit -lHtml
  LDLIBS        += $(ROOTLIBS)
endif
endif
