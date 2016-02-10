#include "MyAnalysis.h"

// podio specific includes
#include "podio/EventStore.h"
#include "podio/ROOTReader.h"

#include "datamodel/JetCollection.h"
#include "datamodel/JetParticleAssociationCollection.h"
#include "datamodel/ParticleCollection.h"

// Utility functions
#include "utilities/JetUtils.h"
#include "utilities/VectorUtils.h"
#include "utilities/ParticleUtils.h"

#include "TLorentzVector.h"

MyAnalysis::MyAnalysis() :
  m_hjetenergy("hjetenergy","hjetenergy", 500, 0, 500),
  m_hjetnparts("hjetnparts","hjetnparts", 50, 0, 50) {}


void MyAnalysis::loop(const char* filename) {
  m_hjetenergy.Reset();
  m_hjetnparts.Reset();
  auto reader = podio::ROOTReader();
  auto store = podio::EventStore();
  try {
    reader.openFile(filename);
  }
  catch(std::runtime_error& err) {
    std::cerr<<err.what()<<". Quitting."<<std::endl;
    exit(1);
  }
  store.setReader(&reader);

  bool verbose = true;

  // unsigned nEvents = 5;
  unsigned nEvents = reader.getEntries();
  for(unsigned i=0; i<nEvents; ++i) {
    if(i%1000==0)
      std::cout<<"reading event "<<i<<std::endl;
    if(i>2)
      verbose = false;
    processEvent(store, verbose, reader);
    store.clear();
    reader.endOfEvent();
  }
  return;
}


void MyAnalysis::processEvent(podio::EventStore& store, bool verbose,
                              podio::ROOTReader& reader) {

  // read jets
  const fcc::JetCollection* jrefs(nullptr);
  bool jets_available = store.get("GenJet",jrefs);
  std::vector<fcc::Particle> injets;
  
  if (jets_available){
    const fcc::JetParticleAssociationCollection* jprefs(nullptr);
    bool assoc_available = store.get("GenJetParticle",jprefs);
    if(verbose) {
      reader.getCollectionIDTable()->print();
      std::cout << "jet collection:" << std::endl;
    }
    for(const auto& jet : *jrefs){
      std::vector<fcc::Particle> jparticles = utils::associatedParticles(jet,
                                                                    *jprefs);
      TLorentzVector lv = utils::lvFromPOD(jet.Core().P4);
      m_hjetenergy.Fill(lv.E());
      m_hjetnparts.Fill(jparticles.size());
      if(verbose)
        std::cout << "\tjet: E=" << lv.E() << " "<<lv.Eta()<<" "<<lv.Phi()
                  <<" npart="<<jparticles.size()<<std::endl;
      if(assoc_available) {
        for(const auto& part : jparticles) {
          if(verbose)
            std::cout<<"\t\tassociated "<<part<<std::endl;
          injets.push_back(part);
        }
      }
    }
  }
}
