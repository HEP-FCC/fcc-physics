{ 
  //Does not work in root6... 
  //seems we can do that from the command line but not
  //in an unnamed macro.
  //anyway, the python version is better, check it out.
  gSystem.Load("libanalysiscpp-myanalysis");
  MyAnalysis ma;
  ma.loop("example.root");
  TCanvas c1;
  c1.Divide(2,1);
  c1.cd(1);
  ma.m_hjetenergy.Draw();
  c1.cd(2);
  ma.m_hjetnparts.Draw();
}
