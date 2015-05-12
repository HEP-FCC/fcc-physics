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
  for(unsigned i=0; i<m_inputs.size(); ++i) {
    const TLorentzVector& p4 = m_inputs[i];
    inputs.emplace_back( p4.Px(), p4.Py(), p4.Pz(), p4.E());
    inputs.back().set_user_index(i);
  }
  // fastjet::JetDefinition   definition(m_algorithm, m_R, m_recomb_scheme); 
  fastjet::ClusterSequence sequence(inputs, m_definition);
  std::vector<fastjet::PseudoJet> outputs = fastjet::sorted_by_E(sequence.inclusive_jets(1.));
  m_outputs.clear();
  m_constituents.clear();
  // m_constituents.reserve(outputs.size());
  for(const fastjet::PseudoJet& pjet : outputs) {
    m_outputs.emplace_back(pjet.px(), pjet.py(), pjet.pz(), pjet.E());
    const std::vector<fastjet::PseudoJet>& constituents = pjet.constituents();
    m_constituents.emplace_back(std::vector<unsigned>());
    for(const auto& constit : constituents) {
      m_constituents.back().push_back(constit.user_index());
    }
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

unsigned JetClusterizer::n_constituents(unsigned i) const {
  if( i >= m_constituents.size()) {
    std::cerr<<"no such jet"<<std::endl;
    return -1;
    //TODO throw exception
  }
  else
    return m_constituents[i].size();
}

unsigned JetClusterizer::constituent_index(unsigned ijet,
					   unsigned iconst) const {
  if( ijet >= m_constituents.size()) {
    std::cerr<<"no such jet"<<std::endl;
    return -1;
    //TODO throw exception
  }
  if( iconst >= m_constituents[ijet].size()) {
    std::cerr<<"no such constituent"<<std::endl;
    return -1;
    //TODO throw exception
  }
  return m_constituents[ijet][iconst];
}


