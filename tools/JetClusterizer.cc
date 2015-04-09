#include "JetClusterizer.h"

#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"

#include <iostream>

JetClusterizer::JetClusterizer() {
  std::cout<<"building jet clusterizer"<<std::endl;
  fastjet::JetAlgorithm algorithm = fastjet::JetAlgorithm::antikt_algorithm;
  fastjet::RecombinationScheme recomb_scheme = fastjet::RecombinationScheme::E_scheme;
  float radius = 0.5;
  m_definition = fastjet::JetDefinition(algorithm, radius, recomb_scheme); 

}


void JetClusterizer::clusterize() {
  std::vector<fastjet::PseudoJet> inputs;
  for(const TLorentzVector& p4 : m_inputs) {
    inputs.emplace_back( p4.Px(), p4.Py(), p4.Pz(), p4.E());
  }
  // fastjet::JetDefinition   definition(m_algorithm, m_R, m_recomb_scheme); 
  fastjet::ClusterSequence sequence(inputs, m_definition);
  std::vector<fastjet::PseudoJet> outputs = fastjet::sorted_by_E(sequence.inclusive_jets(1.));
  m_outputs.clear();
  for(const fastjet::PseudoJet& pjet : outputs) {
    m_outputs.emplace_back(pjet.px(), pjet.py(), pjet.pz(), pjet.E());
  }
}


JetClusterizer::P4 JetClusterizer::jet(unsigned i) const {
  if( i >= m_outputs.size()) {
    std::cerr<<"no such jet"<<std::endl;
    return TLorentzVector();
    //TODO throw exception
  }
  else
    return m_outputs[i];
}

