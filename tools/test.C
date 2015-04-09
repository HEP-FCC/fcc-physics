{
  gSystem.Load("libanalysiscpp-tools");
  JetClusterizer jc;
  jc.add_p4(TLorentzVector(10,0,0,10));
  jc.add_p4(TLorentzVector(20,0,0,20));
  jc.add_p4(TLorentzVector(0,40,0,40));
  jc.clusterize();
  std::cout<<"njets "<<jc.n_jets()<<std::endl;
  TLorentzVector jet = jc.jet(0);
  std::cout<<"jet E "<<jet.E()<<std::endl;
  gApplication.Terminate();
}
