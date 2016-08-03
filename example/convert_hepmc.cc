#include "datamodel/MCParticleCollection.h"
#include "datamodel/GenVertexCollection.h"
#include "datamodel/EventInfoCollection.h"

#include <memory>
#include <unordered_map>

#include "HepMC/IO_GenEvent.h"

#include "tools/TranslateHepMC.h"

#include "podio/EventStore.h"
#include "podio/ROOTWriter.h"

int main(int argc, char** argv) {
  if( argc != 3) {
    std::cerr << "Usage: fcc-convert <hep mc file> <output file>" << std::endl;
    return 1;
  }
  const char* hepmcFile = argv[1];
  std::string output(argv[2]);
  // this function is to be used in conjunction with
  // the data service or some other memory manager
  auto file = std::unique_ptr<HepMC::IO_GenEvent>(new HepMC::IO_GenEvent(hepmcFile, std::ios::in));
  if ((nullptr == file) || (file->rdstate() == std::ios::failbit)) {
    std::cout <<  "Failure to read the file '" << hepmcFile << "'" << std::endl;
    return 1;
  }
  auto store  = podio::EventStore();
  auto writer = podio::ROOTWriter(output, &store);

  fcc::EventInfoCollection& evinfos = store.create<fcc::EventInfoCollection>("EventInfo");
  fcc::MCParticleCollection& particles = store.create<fcc::MCParticleCollection>("GenParticle");
  fcc::GenVertexCollection& vertices = store.create<fcc::GenVertexCollection>("GenVertex");

  writer.registerForWrite<fcc::EventInfoCollection>("EventInfo");
  writer.registerForWrite<fcc::MCParticleCollection>("GenParticle");
  writer.registerForWrite<fcc::GenVertexCollection>("GenVertex");

  int iev = 0;
  std::cout << "starting processing" << std::endl;
  std::cout << "output file: " << output << std::endl;
  bool validEvent = true;
  while(validEvent) {

    if (iev%100 == 0) std::cout << "processing event " << iev << std::endl;
    auto event = std::unique_ptr<HepMC::GenEvent>(new HepMC::GenEvent());
    event->use_units(HepMC::Units::GEV, HepMC::Units::MM);
    validEvent = file->fill_next_event(event.get());
    if (!validEvent) {
      break;
    }
    auto evinfo = evinfos.create();
    evinfo.Number(++iev);

    translateEvent(event.get(), particles, vertices);

    writer.writeEvent();
    store.clearCollections();
  }
  writer.finish();
  return 0;
}
