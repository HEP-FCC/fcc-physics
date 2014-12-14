{
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
