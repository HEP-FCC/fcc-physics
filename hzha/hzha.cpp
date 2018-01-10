#include <iostream>

#include "HepMC/GenEvent.h"
#include "HepMC/HEPEVT_Wrapper.h"
#include "HepMC/IO_HEPEVT.h"


extern "C" {
  void readcards_(const char*);
  //void vzero_(int&,int&);
  //void ffread_(int&, char*, char*, char*);
  //void ucopy_(char*, char*, int&);
  void hzha_init_(void);
  void hzha_event_(void);
  //void timel_(int&);
  void hzha_close_(void);
  void pyhepc_(int&);
}

int main(int argc, char* args[]) {
  if(argc != 2) {
    std::cerr<<"usage: hzha <cards_file>"<<std::endl;
    return 1;
  }
  const char* filename = args[1];
  readcards_(filename);
  hzha_init_();

  HepMC::IO_HEPEVT hepevtio;
  for(unsigned i=0; i<1000; ++i) {
    hzha_event_();
    int mode=1;
    pyhepc_(mode);
    HepMC::GenEvent* evt = hepevtio.read_next_event();
    // std::cout<<(*evt)<<std::endl;
    delete evt;
  }
  hzha_close_();
  return 0;
}
