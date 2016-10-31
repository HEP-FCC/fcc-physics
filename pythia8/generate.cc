// Data model
#include "datamodel/EventInfo.h"
#include "datamodel/EventInfoCollection.h"
#include "datamodel/MCParticle.h"
#include "datamodel/MCParticleCollection.h"
#include "datamodel/GenJet.h"
#include "datamodel/GenJetCollection.h"
#include "datamodel/GenVertex.h"
#include "datamodel/GenVertexCollection.h"
#include "datamodel/LorentzVector.h"

// STL
#include <iostream>
#include <vector>

// albers specific includes
#include "podio/EventStore.h"
#include "podio/ROOTWriter.h"

#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/HepMC2.h"
#include "Pythia8/ParticleData.h"

#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"

using namespace std;

int main(int argc, char** argv) {

  if( argc != 2) {
    std::cerr << "Usage: pythiafcc-generate <pythia card file>" << std::endl;
    return 1;
  }
  const char* card_file = argv[1];
  std::string output(card_file);
  size_t dot = output.find_last_of(".");
  if(dot != std::string::npos){
    output.replace(dot, output.size()-dot, ".root");
  }
  else {
    output += ".root";
  }
  size_t slash = output.find_last_of("/");
  if(slash != std::string::npos){
    output.replace(0, slash+1, "");
  }
  std::cout << "start processing" << std::endl;
  std::cout << "output file: " << output << std::endl;

  auto store  = podio::EventStore();
  auto writer = podio::ROOTWriter(output, &store);


  fcc::EventInfoCollection& evinfocoll = store.create<fcc::EventInfoCollection>("EventInfo");
  fcc::MCParticleCollection& pcoll = store.create<fcc::MCParticleCollection>("GenParticle");
  fcc::GenVertexCollection& vcoll = store.create<fcc::GenVertexCollection>("GenVertex");
  fcc::GenJetCollection& genjetcoll = store.create<fcc::GenJetCollection>("GenJet");

  writer.registerForWrite<fcc::EventInfoCollection>("EventInfo");
  writer.registerForWrite<fcc::MCParticleCollection>("GenParticle");
  writer.registerForWrite<fcc::GenVertexCollection>("GenVertex");
  writer.registerForWrite<fcc::GenJetCollection>("GenJet");

  // Generator. Process selection. LHC initialization. Histogram.
  Pythia8::Pythia pythia;
  pythia.readFile(card_file);
  pythia.init();

  unsigned nevents = pythia.mode("Main:numberOfEvents");

  // Interface for conversion from Pythia8::Event to HepMC event.
  HepMC::Pythia8ToHepMC ToHepMC;
  Pythia8::ParticleData PDG;

  // Fastjet

  fastjet::Strategy strategy = fastjet::Best;
  fastjet::RecombinationScheme recomb_scheme = fastjet::E_scheme;
  double rparam = 0.5;
  fastjet::JetDefinition jet_def(fastjet::antikt_algorithm, rparam,
                                 recomb_scheme, strategy);

  std::vector<fastjet::PseudoJet> input_particles;

  for(unsigned iev=0; iev<nevents; ++iev) {
    auto evinfo = evinfocoll.create();
    evinfo.number(iev);

    if (!pythia.next()) continue;
    auto hepmcevt = new HepMC::GenEvent();
    hepmcevt->use_units(HepMC::Units::GEV, HepMC::Units::MM);
    ToHepMC.fill_next_event( pythia, hepmcevt );

    std::map<HepMC::GenVertex*, fcc::GenVertex> vtx_map;
    for (auto iv = hepmcevt->vertices_begin();
         iv != hepmcevt->vertices_end(); ++iv) {
      const HepMC::FourVector& vpos = (*iv)->position();
      fcc::GenVertex vtx = vcoll.create();
      vtx.x(vpos.x());
      vtx.y(vpos.y());
      vtx.z(vpos.z());
      vtx.ctau(vpos.t());
      vtx_map.emplace(*iv, vtx);
    }
    input_particles.clear();
    for (auto ip = hepmcevt->particles_begin();
         ip != hepmcevt->particles_end(); ++ip) {
      auto hepmcptc = *ip;
      auto ptc = pcoll.create();
      auto& core = ptc.core();
      core.pdgId = hepmcptc->pdg_id();
      core.charge = pythia.particleData.charge(core.pdgId);
      core.status = hepmcptc->status();
      core.p4.px = hepmcptc->momentum().px();
      core.p4.py = hepmcptc->momentum().py();
      core.p4.pz = hepmcptc->momentum().pz();
      core.p4.mass = hepmcptc->momentum().m();

      if(core.status==1) {
        input_particles.push_back( fastjet::PseudoJet(hepmcptc->momentum().px(),
                                                      hepmcptc->momentum().py(),
                                                      hepmcptc->momentum().pz(),
                                                      hepmcptc->momentum().e() ));
      }
      std::vector<fastjet::PseudoJet> input_particles;

      auto prodvtx = vtx_map.find(hepmcptc->production_vertex());
      if(prodvtx != vtx_map.end()) {
        ptc.startVertex(prodvtx->second);
      }

      auto endvtx = vtx_map.find(hepmcptc->end_vertex());
      if(endvtx != vtx_map.end()) {
        ptc.endVertex(endvtx->second);
      }

    }
    fastjet::ClusterSequence clust_seq(input_particles, jet_def);
    double ptmin = 10;
    std::vector<fastjet::PseudoJet> jets = clust_seq.inclusive_jets(ptmin);
    for(unsigned i = 0; i < jets.size(); ++i) {
      auto genjet = genjetcoll.create();
      auto& core = genjet.core();
      core.p4.px = jets[i].px();
      core.p4.py = jets[i].py();
      core.p4.pz = jets[i].pz();
      core.p4.mass = jets[i].m();
    }

    writer.writeEvent();
    store.clearCollections();
    delete hepmcevt;
  }

  writer.finish();

  return 0;
}
