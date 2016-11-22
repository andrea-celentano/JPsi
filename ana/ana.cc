#include <iostream>
#include <vector>

#include "TLorentzVector.h"
#include "../JPsiIO/event.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TEfficiency.h"
#include "TObjArray.h"
#include "TLatex.h"
#ifdef __MAKECINT__
#pragma extra_include "vector";
#pragma extra_include "TLorentzVector.h";

#pragma link C++ class vector< int >+;
#pragma link C++ class std::vector<TLorentzVector>;
#endif

#define MP 0.9383
using namespace std;

int ana(){
  TFile *f1=new TFile("genOUT.root");
  TFile *f2=new TFile("accOUT.root");

  TTree *t1=(TTree*)f1->Get("GEN");
  TTree *t2=(TTree*)f2->Get("ACC");

  event eGen,eAcc;
  int N;
  int status[20];

  vector<TLorentzVector> *Pgen=0;
  vector<TLorentzVector> *Pacc=0;

  TLorentzVector PpsiGen,PpsiAcc;
  TLorentzVector PgGen,PgAcc;
  TLorentzVector Pbeam,Ptarget;
  int ePclas,eMclas;

  if (t1->GetEntries()!=t2->GetEntries()){
      cout<<"Error with number of entries"<<endl;
      exit(1);
  }
  N=t1->GetEntries();
  t1->SetBranchAddress("event",&eGen);
  t2->SetBranchAddress("event",&eAcc);
  t2->SetBranchAddress("status",status);
  t1->SetBranchAddress("P",&Pgen);
  t2->SetBranchAddress("P",&Pacc);

  cout<<"There are: "<<N<<" events "<<endl;
  //N/=10;
  /*Define here histograms*/
  TH2D *hThetaVsPprotonGen=new TH2D("hThetaVsPprotonGen","ThetaVsPproton GEN;P (GeV);Theta (deg)",100,0,5,100,0,50);
  TH2D *hThetaVsPprotonAcc=new TH2D("hThetaVsPprotonAcc","ThetaVsPproton ACC;P (GeV);Theta (deg)",100,0,5,100,0,50);

  TH2D *hThetaVsPleptonsGen=new TH2D("hThetaVsPleptonsGen","ThetaVsPleptons GEN;P (GeV);Theta (deg)",100,0,10,100,0,50);
  TH2D *hThetaVsPleptonsAcc=new TH2D("hThetaVsPleptonsAcc","ThetaVsPleptons ACC;P (GeV);Theta (deg)",100,0,10,100,0,50);

  TH2D *hThetaVsPhiprotonAcc=new TH2D("hThetaVsPhiprotonAcc","ThetaVsPhiproton ACC;Phi (deg);Theta (deg)",200,-180,180,100,0,50);
  TH2D *hThetaVsPhielectronAcc=new TH2D("hThetaVsPhielectronAcc","ThetaVsPhielectron ACC;Phi (deg);Theta (deg)",200,-180,180,100,0,50);
  TH2D *hThetaVsPhipositronAcc=new TH2D("hThetaVsPhipositronAcc","ThetaVsPhipositron ACC;Phi (deg);Theta (deg)",200,-180,180,100,0,50);



  TH2D *hQ2VsNu=new TH2D("hQ2VsNu","Q2VsNu;Q2(GeV**2);vu(GeV)",100,0,.25,100,7.5,11);
  TH2D *hresNu=new TH2D("hresNu","NuRes;nu(GeV);#delta vu(GeV)",100,7.5,11,100,-.2,.2);
  
  TH1D *hMMeGen=new TH1D("hMMeGen","p e+ e- mass GEN ; Events/5 MeV",180,3.8,4.7);

  //case 1: all particles detected
  TH1D *hMePeM_1=new TH1D("hMePeM_1","e+e- mass;M(GeV)",250,2.8,3.3);
  TH1D *hMMeePeM_1=new TH1D("hMMeePeM_1","(e e+e-)missing mass;MM(GeV)",200,0.5,1.5);
  TH1D *hMpePeMAcc_1=new TH1D("hMpePeMAcc_1","p e+ e- mass",180,3.8,4.7);
  TH1D *hMMeAcc_1=new TH1D("hMMeAcc_1","e missing mass",180,3.8,4.7);
  TH1D *heffMMe_1=new TH1D("heffMMe_1","Acceptance;electron MM (GeV)",180,3.8,4.7);
  TH2D *hresMMe_1=new TH2D("hresMMe_1","MMe resolution; MMe(GeV); #delta MMe(GeV)",90,4.1,4.55,200,-.05,.05);
  //case 2: only e+ e- detected
  TH1D *hMePeM_2=new TH1D("hMePeM_2","e+e- mass;M(GeV)",250,2.8,3.3);
  TH1D *hMMeePeM_2=new TH1D("hMMeePeM_2","(e e+e-)missing mass;MM(GeV)",200,0.5,1.5);
  TH1D *hMpePeMAcc_2=new TH1D("hMpePeMAcc_2","p e+ e- mass",180,3.8,4.7);
  TH1D *hMMeAcc_2=new TH1D("hMMeAcc_2","e missing mass (GeV)",180,3.8,4.7);
  TH1D *heffMMe_2=new TH1D("heffMMe_2","Acceptance;electron MM (GeV)",180,3.8,4.7);
  TH2D *hresMMe_2=new TH2D("hresMMe_2","MMe resolution; MMe(GeV); #delta MMe(GeV)",90,4.1,4.55,200,-.05,.05);
 
  //start loop on events
  
  for (int ievent=0;ievent<N;ievent++){
    if (ievent%10000==0) cerr<<ievent<<endl;
    Pbeam.SetXYZT(0,0,11,11);
    Ptarget.SetXYZT(0,0,0,MP);
    t1->GetEntry(ievent);
    t2->GetEntry(ievent);
      //ordine delle particelle: e- proton ePlus eMinus
    PpsiGen=Pgen->at(2)+Pgen->at(3);
    PpsiAcc=Pacc->at(2)+Pacc->at(3);
    
    //condizione per FT: theta<4.5 deg
    //per definizione della mia simulazione, e- diffuso da beam sempre misurato.
    ePclas=(Pgen->at(2).Theta()>4.5*TMath::DegToRad());
    eMclas=(Pgen->at(3).Theta()>4.5*TMath::DegToRad());

    hThetaVsPprotonGen->Fill(Pgen->at(1).P(),Pgen->at(1).Theta()*TMath::RadToDeg());
    hThetaVsPleptonsGen->Fill(Pgen->at(2).P(),Pgen->at(2).Theta()*TMath::RadToDeg());
    hThetaVsPleptonsGen->Fill(Pgen->at(3).P(),Pgen->at(3).Theta()*TMath::RadToDeg());
    hQ2VsNu->Fill(eGen.Q2,eGen.v);
    hresNu->Fill(eGen.v,eGen.v-eAcc.v);

    hMMeGen->Fill(eGen.W);
   
    if (status[1]==1)  {hThetaVsPprotonAcc->Fill(Pgen->at(1).P(),Pgen->at(1).Theta()*TMath::RadToDeg()); hThetaVsPhiprotonAcc->Fill(Pgen->at(1).Phi()*TMath::RadToDeg(),Pgen->at(1).Theta()*TMath::RadToDeg());}
    if ((status[2]==1)&&ePclas)   {hThetaVsPleptonsAcc->Fill(Pgen->at(2).P(),Pgen->at(2).Theta()*TMath::RadToDeg()); hThetaVsPhipositronAcc->Fill(Pgen->at(2).Phi()*TMath::RadToDeg(),Pgen->at(2).Theta()*TMath::RadToDeg());}
    if ((status[3]==1)&&eMclas)   {hThetaVsPleptonsAcc->Fill(Pgen->at(3).P(),Pgen->at(3).Theta()*TMath::RadToDeg()); hThetaVsPhielectronAcc->Fill(Pgen->at(3).Phi()*TMath::RadToDeg(),Pgen->at(3).Theta()*TMath::RadToDeg());}

    /*Here is the case for all 4 particles measured (escattered,p,e+,e-)*/
    if ((status[1]==1)&&(status[2]==1)&&ePclas&&(status[3]==1)&&(eMclas)){
      hMePeM_1->Fill(PpsiAcc.M());
      hMMeePeM_1->Fill((Pbeam+Ptarget-Pacc->at(0)-Pacc->at(2)-Pacc->at(3)).M());
      hMMeAcc_1->Fill(eAcc.W);
      hMpePeMAcc_1->Fill(sqrt(eAcc.m2pEpEm));
      heffMMe_1->Fill(eAcc.W); //I do not want bin migration
      hresMMe_1->Fill(eGen.W,eGen.W-eAcc.W);
    }  
    /*Here is the case for 3 particles measured (escattered,e+,e-)*/
    if ((status[2]==1)&&ePclas&&(status[3]==1)&&(eMclas)){
      hMePeM_2->Fill(PpsiAcc.M());
      hMMeePeM_2->Fill((Pbeam+Ptarget-Pacc->at(0)-Pacc->at(2)-Pacc->at(3)).M());
      hMMeAcc_2->Fill(eAcc.W);
      hMpePeMAcc_2->Fill(sqrt(eAcc.m2pEpEm));
      heffMMe_2->Fill(eAcc.W); //I do not want bin migration
      hresMMe_2->Fill(eGen.W,eGen.W-eAcc.W);  
    }  

  }//end loop events
  /*efficiency*/
  hMMeGen->Sumw2();
  heffMMe_1->Sumw2();heffMMe_1->Divide(hMMeGen);
  heffMMe_2->Sumw2();heffMMe_2->Divide(hMMeGen);
  /*Draw*/
  TCanvas *c1=new TCanvas("c1","Clas12 acceptance single particles");
  c1->Divide(3,3);
  c1->cd(1);hThetaVsPprotonGen->Draw("colz");
  c1->cd(2);hThetaVsPprotonAcc->Draw("colz");
  c1->cd(4);hThetaVsPleptonsGen->Draw("colz");
  c1->cd(5);hThetaVsPleptonsAcc->Draw("colz");

  c1->cd(7);hThetaVsPhiprotonAcc->Draw("colz");
  c1->cd(8);hThetaVsPhielectronAcc->Draw("colz");
  c1->cd(9);hThetaVsPhipositronAcc->Draw("colz");

   TCanvas *c2=new TCanvas("c2","Scattered e- in FT");c2->Divide(2,2);
  c2->cd(1);hQ2VsNu->Draw("colz");
  c2->cd(2);hresNu->Draw("colz");

  TCanvas *c3=new TCanvas("c3","all detected");c3->Divide(2,3);
  c3->cd(1);hMePeM_1->Draw();
  c3->cd(2);hMMeePeM_1->Draw();
  c3->cd(3);hMMeGen->Draw();hMpePeMAcc_1->SetLineColor(2);hMpePeMAcc_1->Draw("SAME");hMMeAcc_1->SetLineColor(3);hMMeAcc_1->Draw("SAME");
  TLatex l1;l1.DrawLatex(3.9,35E4,"Generated");l1.Draw("SAME");
  TLatex l2;l2.SetTextColor(2);l2.DrawLatex(3.9,25E4,"M(p e+ e-)");
  TLatex l3;l3.SetTextColor(3);l3.DrawLatex(3.9,15E4,"MM(e)");
  c3->cd(4);heffMMe_1->Draw();heffMMe_1->GetXaxis()->SetRangeUser(4.1,4.53);
  c3->cd(5);hresMMe_1->Draw("colz");
  c3->cd(6);
  TObjArray *ahresMMe_1=new TObjArray;
  hresMMe_1->FitSlicesY(0,0,-1,5,"",ahresMMe_1);
  ((TH1F*)((*ahresMMe_1)[2]))->Draw();
 

  TCanvas *c4=new TCanvas("c4","e+ e- detected");c4->Divide(2,3);
  c4->cd(1);hMePeM_2->Draw();
  c4->cd(2);hMMeePeM_2->Draw();
  c4->cd(3);hMMeGen->Draw();hMpePeMAcc_2->SetLineColor(2);hMpePeMAcc_2->Draw("SAME");hMMeAcc_2->SetLineColor(3);hMMeAcc_2->Draw("SAME");
  
  TLatex ll1;ll1.DrawLatex(3.9,35E4,"Generated");ll1.Draw("SAME");
  TLatex ll2;ll2.SetTextColor(2);ll2.DrawLatex(3.9,25E4,"M(p e+ e-)");
  TLatex ll3;ll3.SetTextColor(3);ll3.DrawLatex(3.9,15E4,"MM(e)");
  c4->cd(4);heffMMe_2->Draw();heffMMe_2->GetXaxis()->SetRangeUser(4.1,4.53);
  c4->cd(5);hresMMe_2->Draw("colz");
  c4->cd(6);
  TObjArray *ahresMMe_2=new TObjArray;
  hresMMe_2->FitSlicesY(0,0,-1,5,"",ahresMMe_2);
  ((TH1F*)((*ahresMMe_2)[2]))->Draw();
}
















