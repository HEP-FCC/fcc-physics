#include "MyAnalysis.h"

// albers specific includes
#include "albers/EventStore.h"
#include "albers/Reader.h"
#include "albers/Registry.h"

#include "datamodel/JetCollection.h"
#include "datamodel/JetParticleAssociationCollection.h"

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
  albers::Reader reader;
  albers::EventStore store(nullptr);
  store.setReader(&reader);
  try {
    reader.openFile(filename);
  }
  catch(std::runtime_error& err) {
    std::cerr<<err.what()<<". Quitting."<<std::endl;
    exit(1);
  }
  bool verbose = true;

  // unsigned nEvents = 5;
  unsigned nEvents = reader.getEntries();
  for(unsigned i=0; i<nEvents; ++i) {
    if(i%1000==0)
      std::cout<<"reading event "<<i<<std::endl;
    if(i>10)
      verbose = false;
    processEvent(store, verbose, reader);
    store.endOfEvent();
    reader.endOfEvent();
  }
  return;
}


void MyAnalysis::processEvent(albers::EventStore& store, bool verbose,
			      albers::Reader& reader) {

  // read jets
  JetCollection* jrefs(nullptr);
  bool jets_available = store.get("GenJet",jrefs);
  std::vector<ParticleHandle> injets;

  if (jets_available){
    JetParticleAssociationCollection* jprefs(nullptr);
    bool assoc_available = store.get("GenJetParticle",jprefs);
    if(verbose) {
      reader.getRegistry()->print();
      std::cout << "jet collection:" << std::endl;
    }
    for(const auto& jet : *jrefs){
      std::vector<ParticleHandle> jparticles = utils::associatedParticles(jet,
									  *jprefs);
      TLorentzVector lv = utils::lvFromPOD(jet.read().Core.P4);
      m_hjetenergy.Fill(lv.E());
      m_hjetnparts.Fill(jparticles.size());
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
}
