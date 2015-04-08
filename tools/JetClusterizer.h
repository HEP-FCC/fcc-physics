#ifndef __ANALYSISCPP_TOOLS_JETCLUSTERIZER_H__
#define __ANALYSISCPP_TOOLS_JETCLUSTERIZER_H__

#include "TObject.h"

#include <vector>
#include <iostream>
#include <cmath>
#include <TLorentzVector.h>
#include <TMath.h>

#include "fastjet/ClusterSequence.hh"
#include "fastjet/PseudoJet.hh"

class JetClusterizer {
    
 public:

  JetClusterizer(const std::vector<TLorentzVector> & objects, double ktpower, double rparam);

  /// get grouping (inclusive jets)
  std::vector<TLorentzVector> getGrouping(double ptMin = 0.0);

  /// get grouping (exclusive jets, until n are left)
  std::vector<TLorentzVector> getGroupingExclusive(int njets);
  
  /// get grouping (exclusive jets, up to cut dcut)
  std::vector<TLorentzVector> getGroupingExclusive(double dcut);

  /// get pruned 4-vector
  TLorentzVector getPruned(double zcut, double rcutFactor) ;

  /// get pruned 4-vector for a given subject (must be called after getGroupingExclusive)
  TLorentzVector getPrunedSubjetExclusive(unsigned int isubjet, double zcut, double rcutFactor) ;

  /// get pruned 4-vector for a given subject (must be called after getGroupingInclusive)
  TLorentzVector getPrunedSubjetInclusive(unsigned int isubjet, double zcut, double rcutFactor) ;


 private:
  // pack the returns in a fwlite-friendly way
  std::vector<TLorentzVector> makeP4s(const std::vector<fastjet::PseudoJet> &jets) ;

  // prune and return in fa fwlite-friendly way
  TLorentzVector getPruned(const fastjet::PseudoJet & jet, double zcut, double rcutFactor) ;

  // used to handle the inputs
  std::vector<fastjet::PseudoJet> fjInputs_;        // fastjet inputs

  double ktpower_;
  double rparam_;

  /// fastjet outputs
  fastjet::ClusterSequence* fjClusterSeq_;    
  std::vector<fastjet::PseudoJet> inclusiveJets_; 
  std::vector<fastjet::PseudoJet> exclusiveJets_; 
};

#endif   
