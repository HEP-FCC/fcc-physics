#include "datamodel/ParticleCollection.h"
#include "datamodel/EventInfoCollection.h"
#include "datamodel/JetCollection.h"
#include "datamodel/JetParticleAssociationCollection.h"

// Utility functions
#include "utilities/JetUtils.h"
#include "utilities/VectorUtils.h"
#include "utilities/ParticleUtils.h"

// ROOT
#include "TBranch.h"
#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "TLorentzVector.h"

// STL
#include <vector>
#include <iostream>

// podio specific includes
#include "podio/EventStore.h"
#include "podio/ROOTReader.h"

void processEvent(podio::EventStore& store, bool verbose,
                  podio::ROOTReader& reader) {

  // read event information
  const EventInfoCollection* evinfocoll(nullptr);
  bool evinfo_available = store.get("EventInfo", evinfocoll);
  if(evinfo_available) {
    auto evinfo = evinfocoll->at(0);

    if(verbose)
      std::cout << "event number " << evinfo.Number() << std::endl;
  }

  // the following is commented out to test on-demand reading through Jet-Particle association,
  // see below
  // // read particles
  // ParticleCollection* ptcs(nullptr);
  // bool particles_available = store.get("GenParticle",ptcs);
  // if (particles_available){
  //   for(const auto& part : *ptcs) {
  //     std::cout<<part.containerID()<<" "<<part.index()<<std::endl;
  //   }
  // }

  // read jets
  const JetCollection* jrefs(nullptr);
  bool jets_available = store.get("GenJet",jrefs);
  std::vector<Particle> injets;

  if (jets_available){
    const JetParticleAssociationCollection* jprefs(nullptr);
    bool assoc_available = store.get("GenJetParticle",jprefs);
    if(verbose) {
      reader.getCollectionIDTable()->print();
      std::cout << "jet collection:" << std::endl;
    }
    for(const auto& jet : *jrefs){
      std::vector<Particle> jparticles = utils::associatedParticles(jet,
                                                                          *jprefs);
      TLorentzVector lv = utils::lvFromPOD(jet.Core().P4);
      if(verbose)
        std::cout << "\tjet: E=" << lv.E() << " "<<lv.Eta()<<" "<<lv.Phi()
                  <<" npart="<<jparticles.size()<<std::endl;
      if(assoc_available) {
        for(const auto& part : jparticles) {
          if(part.isAvailable()) {
            if(verbose)
              std::cout<<"\t\tassociated "<<part<<std::endl;
            injets.push_back(part);
          }
        }
      }
    }
  }

  // read particles
  const ParticleCollection* ptcs(nullptr);
  bool particles_available = store.get("GenParticle", ptcs);
  if (particles_available){
    std::vector<Particle> muons;
    // there is probably a smarter way to get a vector from collection?

    if(verbose)
      std::cout << "particle collection:" << std::endl;
    for(const auto& ptc : *ptcs){
      if(verbose)
        std::cout<<"\t"<<ptc<<std::endl;
      if( ptc.Core().Type == 4 ) {
        muons.push_back(ptc);
      }
    }
    // listing particles that are not used in a jet
    std::vector<Particle> unused = utils::unused(*ptcs, injets);
    if(verbose)
      std::cout<<"unused particles: "<<unused.size()<<"/"<<ptcs->size()<<" "<<injets.size()<<std::endl;

    // computing isolation for first muon
    if(not muons.empty()) {
      const Particle& muon = muons[0];
      float dRMax = 0.5;
      const std::vector<Particle> incone = utils::inCone( muon.Core().P4,
                                                          *ptcs,
                                                          dRMax);
      float sumpt = utils::sumPt(incone);
      if( verbose ) {
        std::cout<<"muon: "<<muon<<" sumpt "<<sumpt<<std::endl;
        std::cout<<"\tparticles in cone:"<<std::endl;
      }
      for(const auto& ptc : incone) {
        if( verbose )
          std::cout<<"\t"<<ptc<<std::endl;
      }
    }
  }
}


int main(){
  auto reader = podio::ROOTReader();
  auto store = podio::EventStore();
  try {
    reader.openFile("example.root");
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
    if(i%1000==0) {
      std::cout<<"reading event "<<i<<std::endl;
    }
    if(i>10) {
      verbose = false;
    }
    processEvent(store, verbose, reader);
    store.clear();
    reader.endOfEvent();
  }
  return 0;
}
