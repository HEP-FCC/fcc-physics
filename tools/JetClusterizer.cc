#include "JetClusterizer.h"

#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"

#include <iostream>


JetClusterizer::JetClusterizer(Mode mode) : m_mode(mode) {
  switch(mode) {
  case PP:
    std::cout<<"Jet clusterizer in pp mode, antikt cone 0.4, pt sorting"<<std::endl;
    m_definition = fastjet::JetDefinition(fastjet::JetAlgorithm::antikt_algorithm,
					  0.4,
					  fastjet::RecombinationScheme::E_scheme);
    break;
  case EE:
    std::cout<<"Jet clusterizer in ee mode, ee_kt, E sorting"<<std::endl;
    m_definition = fastjet::JetDefinition(fastjet::JetAlgorithm::ee_kt_algorithm);
    break;
  default:
    assert(false);
  }
}

void JetClusterizer::prepare_input(PseudoJets& inputs) const {
  for(unsigned i=0; i<m_inputs.size(); ++i) {
    const TLorentzVector& p4 = m_inputs[i];
    inputs.emplace_back( p4.Px(), p4.Py(), p4.Pz(), p4.E());
    inputs.back().set_user_index(i);
  } 
}

void JetClusterizer::prepare_output(const PseudoJets& outputs) {
  m_outputs.clear();
  m_constituents.clear();
  for(const fastjet::PseudoJet& pjet : outputs) {
    m_outputs.emplace_back(pjet.px(), pjet.py(), pjet.pz(), pjet.E());
    const PseudoJets& constituents = pjet.constituents();
    m_constituents.emplace_back(std::vector<unsigned>());
    for(const auto& constit : constituents) {
      m_constituents.back().push_back(constit.user_index());
    }
  }
}

JetClusterizer::PseudoJets JetClusterizer::sort(const PseudoJets& pjets) const {
  switch(m_mode) {
  case PP:
    return fastjet::sorted_by_pt(pjets);
    break;
  case EE:
    return fastjet::sorted_by_E(pjets);
    break;
  default:
    assert(false);
  }
}
  

void JetClusterizer::make_inclusive_jets(float min_pt) {
  PseudoJets inputs;
  prepare_input(inputs);
  
  fastjet::ClusterSequence sequence(inputs, m_definition);
  PseudoJets outputs
    = sort(sequence.inclusive_jets(min_pt));

  prepare_output(outputs);
}

void JetClusterizer::make_exclusive_jets(int njets) {
  PseudoJets inputs;
  prepare_input(inputs);
  
  fastjet::ClusterSequence sequence(inputs, m_definition);
  PseudoJets outputs
    = sort(sequence.exclusive_jets(njets));

  prepare_output(outputs);
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

