
// STL
#include <iostream>
#include <fstream>
#include <vector>


#include "Pythia8/Pythia.h"
#include "Pythia8/ParticleData.h"

std::string outputFilename(const char* cardFilename, const std::string extension = ".root") {
  std::string output(cardFilename);
  size_t dot = output.find_last_of(".");
  if(dot != std::string::npos){
    output.replace(dot, output.size()-dot, extension);
  }
  else {
    output += extension;
  }
  size_t slash = output.find_last_of("/");
  if(slash != std::string::npos){
    output.replace(0, slash+1, "");
  }
  return output;
}


int main(int argc, char** argv) {
  if( argc != 2) {
    std::cerr<<"Usage <command> <pythia card file>"<<std::endl;
    return 1;
  }
  const char* card_file = argv[1];
  std::cout<<"start processing..."<<std::endl;

  Pythia8::Pythia pythia;
  pythia.readFile(card_file);
  pythia.init();
  
  unsigned nevents = pythia.mode("Main:numberOfEvents");
  std::cout<<"generating "<<nevents<<" events ..."<<std::endl;

  std::vector<int> chargedMultiplicities(nevents);
  std::vector<double> minBiasEtaVec(1000 * nevents);
  std::vector<double> topEtaVec(nevents);
  unsigned int chargedTracksCounter;

  for(unsigned iev = 0; iev < nevents; ++iev) {
    // generate
    if (!pythia.next()) continue;
    // loop over particles in event
    for (int i = 0; i < pythia.event.size(); ++i) {
      chargedTracksCounter = 0;
      // select final state charged particles
      if (pythia.event[i].isFinal() && pythia.event[i].isCharged()) {
        minBiasEtaVec.push_back(pythia.event[i].eta());
        ++chargedTracksCounter;
      }
      // select top
      if (pythia.event[i].id() == 6) { 
        topEtaVec.push_back(pythia.event[i].eta());
      }
    chargedMultiplicities.push_back(chargedTracksCounter);
    }
  }

  Pythia8::Hist minBiasEta("Min Bias Tracks - Pseudorapidity", 100, -16, 16);
  for (auto e: minBiasEtaVec) {
    minBiasEta.fill(e);
  }
  std::cout<<"Min Bias Tracks - Pseudorapidity" << std::endl;

  std::ofstream ofs_minbiastracks_eta(outputFilename(card_file, "_alltracks_eta.dat"), std::ofstream::out);
  minBiasEta.table(ofs_minbiastracks_eta);
  ofs_minbiastracks_eta.close();
      
  Pythia8::Hist topEta("Top Tracks - Pseudorapidity", 100, -16, 16);
  for (auto e: topEtaVec) {
    topEta.fill(e);
  }

  std::cout<<"Top Bias Tracks - Pseudorapidity"<<std::endl;
  std::ofstream ofs_toptracks_eta(outputFilename(card_file, "_toptracks_eta.dat"), std::ofstream::out);
  topEta.table(ofs_toptracks_eta);
  ofs_toptracks_eta.close();

  return 0;
}
