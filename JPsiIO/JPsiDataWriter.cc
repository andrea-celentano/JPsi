#include <vector>
#include <cassert>

#include "JPsiIO/JPsiDataWriter.h"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TMath.h"

#include "JPsiIO/event.h"


JPsiDataWriter::JPsiDataWriter( const string& outFile,const string& treeName):
m_P(NULL),
m_outFile(NULL),
m_outTree(NULL)
{

	gROOT->ProcessLine("#include <vector>");
	TH1::AddDirectory( kFALSE );

	m_P = new vector<TLorentzVector>;
	m_outFile = new TFile( outFile.c_str(), "RECREATE" );
	m_outTree = new TTree(treeName.c_str(),treeName.c_str());


	m_outTree->Branch("P","vector<TLorentzVector>",&m_P);
	m_outTree->Branch("event",&m_e,"event_number/I:weight/F:v/F:W:t:Q2:Ee:The:m2ePeM:mm2ep:m2pEpEm:mm2e:m2eEpEm:mm2p:m2ep:mm2EpEm");
	m_outTree->Branch("nP",&m_nP,"nP/I");
	m_outTree->Branch("pid",m_pid,"pid[nP]/I");
	m_outTree->Branch("status",m_status,"status[nP]/I");
	m_eventCounter = 0;

	Ptarget.SetXYZT(0,0,0,MP);
}


JPsiDataWriter::~JPsiDataWriter(){

	m_outFile->cd();
	m_outTree->Write();
	m_outFile->Close();

}


void
JPsiDataWriter::writeEvent(const Kinematics& kin,int *pid,int *status){

	vector< TLorentzVector > particleList = kin.particleList();
	m_nP=particleList.size();

	double Ebeam=11.;
	Ptarget.SetXYZT(0,0,0,MP);
	Pbeam.SetXYZT(0,0,Ebeam,Ebeam);

	Peprime=particleList.at(0);
	Pproton=particleList.at(1);
	Peplus=particleList.at(2);
	Peminus=particleList.at(3);

	Ppsi=Peplus+Peminus;






	m_P->clear();

	m_P->push_back(Peprime);
	m_P->push_back(Pproton);
	m_P->push_back(Peplus);
	m_P->push_back(Peminus);


	m_e.v=(Pbeam-Peprime).E();
	m_e.Ee=Peprime.E();
	m_e.The=Peprime.Theta();

	m_e.Q2=-(Pbeam-Peprime).M2();
	m_e.t=-(Ptarget-Pproton).M2();
	m_e.W=((Pbeam-Peprime)+Ptarget).M();

	//invariant electron-positron and missing protons-scattered
	m_e.m2ePeM=(Peplus+Peminus).M2();m_e.mm2ep=(Pbeam+Ptarget-Peprime-Pproton).M2();

	//invariant proton e+ e- and missing scattered
	m_e.m2pEpEm=(Pproton+Peplus+Peminus).M2();m_e.mm2e=(Pbeam+Ptarget-Peprime).M2();

	//invariant scattered e+ e- and missing proton
	m_e.m2eEpEm=(Peprime+Peplus+Peminus).M2();m_e.mm2p=(Pbeam+Ptarget-Pproton).M2();

	//invariant scattered proton and missing e+e-
	m_e.m2ep=(Peprime+Pproton).M2();m_e.mm2EpEm=(Pbeam+Ptarget-Peplus-Peminus).M2();

	//weight
	m_e.weight=kin.weight();


	for (int ii=0;ii<m_nP;ii++){
		if (pid!=0) m_pid[ii]=pid[ii];
		else m_pid[ii]=0;

		if (status!=0) m_status[ii]=status[ii];
		else m_status[ii]=1;
	}




	m_outTree->Fill();

	m_eventCounter++;

} 


void
JPsiDataWriter::writeEvent(const vector<TLorentzVector> &P,double weight,int *pid,int *status){

	Kinematics kin(P,weight);
	this->writeEvent(kin,pid,status);

}
