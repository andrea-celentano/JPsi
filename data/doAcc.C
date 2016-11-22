doAcc(){
  TChain ch("ACC");ch.Add("acc*.root");
  TFile f("accOUT.root","recreate");
  ch.CloneTree(-1,"fast");
  f.Write();


}
