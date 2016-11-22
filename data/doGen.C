doGen(){
  TChain ch("GEN");ch.Add("gen*.root");
  TFile f("genOUT.root","recreate");
  ch.CloneTree(-1,"fast");
  f.Write();


}
