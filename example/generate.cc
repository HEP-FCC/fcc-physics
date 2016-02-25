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
    std::cerr<<"Usage: pythiafcc-generate <pythia card file>"<<std::endl;
    return 1;
  }
  const char* card_file = argv[1]; 
  
  std::cout<<"start processing"<<std::endl;

  auto store  = podio::EventStore();
  auto writer = podio::ROOTWriter("example.root", &store);


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
    // fill event information
    // here, asking the store for the collection.
    // could also just reuse the reference obtained at the time of the creation
    // of the collection
    auto& evinfocoll = store.create<fcc::EventInfoCollection> ("EventInfo");
    fcc::EventInfo evinfo = evinfocoll.create();
    evinfo.Number(iev);

    if (!pythia.next()) continue;
    HepMC::GenEvent* hepmcevt = new HepMC::GenEvent();
    ToHepMC.fill_next_event( pythia, hepmcevt );

    //    std::cout<<"Nvtx = "<<hepmcevt->vertices_size()<<std::endl;
    typedef std::map<HepMC::GenVertex*, fcc::GenVertex > VertexMap;
    VertexMap vtx_map;
    for ( HepMC::GenEvent::vertex_iterator iv = hepmcevt->vertices_begin();
	  iv != hepmcevt->vertices_end(); ++iv ) {
      const HepMC::FourVector& vpos = (*iv)->position();
      fcc::GenVertex vtx = vcoll.create();
      vtx.Position().X = vpos.x();
      vtx.Position().Y = vpos.y();
      vtx.Position().Z = vpos.z();
      vtx.Ctau(vpos.t());
      vtx_map.emplace(*iv, vtx);
    }
    input_particles.clear();
    for ( HepMC::GenEvent::particle_iterator ip = hepmcevt->particles_begin();
	  ip != hepmcevt->particles_end(); ++ip ) {
      HepMC::GenParticle* hepmcptc = *ip;
      fcc::MCParticle ptc = pcoll.create();
      fcc::BareParticle& core = ptc.Core();
      core.Type = hepmcptc->pdg_id();
      core.Charge = pythia.particleData.charge(core.Type);
      core.Status = hepmcptc->status();
      core.P4.Px = hepmcptc->momentum().px();
      core.P4.Py = hepmcptc->momentum().py();
      core.P4.Pz = hepmcptc->momentum().pz();
      core.P4.Mass = hepmcptc->momentum().m();

      if(core.Status==1) {
	input_particles.push_back( fastjet::PseudoJet(hepmcptc->momentum().px(),
						      hepmcptc->momentum().py(),
						      hepmcptc->momentum().pz(),
						      hepmcptc->momentum().e() ));
      }
      std::vector<fastjet::PseudoJet> input_particles;

      typedef VertexMap::const_iterator IVM;
      IVM prodvtx = vtx_map.find(hepmcptc->production_vertex());
      if(prodvtx!=vtx_map.end()) {
	ptc.StartVertex(prodvtx->second);
      }

      IVM endvtx = vtx_map.find(hepmcptc->end_vertex());
      if(endvtx!=vtx_map.end()) {
	ptc.EndVertex(endvtx->second);
      }

    }
    fastjet::ClusterSequence clust_seq(input_particles, jet_def);
    double ptmin = 10;
    std::vector<fastjet::PseudoJet> jets = clust_seq.inclusive_jets(ptmin);
    for(unsigned i=0; i<jets.size(); ++i) {
      fcc::GenJet genjet = genjetcoll.create();
      fcc::BareJet& core = genjet.Core();
      core.P4.Px = jets[i].px();
      core.P4.Py = jets[i].py();
      core.P4.Pz = jets[i].pz();
      core.P4.Mass = jets[i].m();
    }

    writer.writeEvent();
    store.clearCollections();
    delete hepmcevt;
  }

  writer.finish();

  return 0;
}
