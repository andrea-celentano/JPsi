
#include <vector>
#include <cassert>
#include <fstream>
#include <map>
#include "TH1.h"
#include "TFile.h"
#include "TTree.h"
#include "IUAmpTools/Kinematics.h"
#include "JPsiIO/JPsiDataReader.h"


JPsiDataReader::JPsiDataReader( const vector< string >& args ) :
UserDataReader< JPsiDataReader >(args),
m_eventCounter( 0 ),

m_P(NULL),
m_treeName("GEN"),
m_fileName("file.root")
{
	assert(args.size() == 2); // The file name, tree name

	m_fileName=args.at(0);
	m_treeName=args.at(1);

	TH1::AddDirectory( kFALSE );

	m_P=new vector<TLorentzVector>;
	cout<<"JPsiDataReader: connecting to the TTree"<<endl;
	m_f=new TFile(m_fileName.c_str());
	m_t=(TTree*)m_f->Get(m_treeName.c_str());
	m_t->SetBranchAddress("P",&m_P);
	m_t->SetBranchAddress("event",&m_event);

	m_Nevents = ( unsigned int )( m_t->GetEntries() );
	cout<<"JPsiDataReader: ";
	cout<<"--> "<<m_Nevents<<" events"<<endl;
}


void
JPsiDataReader::resetSource(){

	m_eventCounter = 0;
}


Kinematics*
JPsiDataReader::getEvent(){
	double weight=1;
	//order is proton, pip, pi0, pim. But in the amplitude we need everything!
	double Ebeam=11;
	double Mp=0.9383;
	TLorentzVector Pbeam(0,0,Ebeam,Ebeam);
	TLorentzVector Ptarget(0,0,0,Mp);
	TLorentzVector Pboost;
	TLorentzVector Pgamma;
	double theta;

	if( m_eventCounter < numEvents() ){

		m_t->GetEntry( m_eventCounter);

		vector< TLorentzVector > particleList; /*order is: eprime, proton, ePlus,eMinus*/
		particleList.push_back(Pbeam);
		particleList.push_back(Ptarget);
		particleList.push_back(m_P->at(0)); //eprime
		particleList.push_back(m_P->at(1)); //proton
		particleList.push_back(m_P->at(2)); //ePlus
		particleList.push_back(m_P->at(3)); //eMinus

		/*Now go to the CM frame of the g* P -> J/psi P reaction*/
		//1: define the boost vector and boost
		Pgamma=(Pbeam-m_P->at(0));
		Pboost=(Pgamma)+m_P->at(1);
		for (int ii=0;ii<particleList.size();ii++) particleList.at(ii).Boost(-(Pboost.BoostVector()));
		Pgamma.Boost(-(Pboost.BoostVector()));

		//2: perform two rotations to have Pgamma along z
		//2a: First rotate along z to have Pgamma in the xz plane
		theta=atan2(-Pgamma.Y(),Pgamma.X());
		for (int ii=0;ii<particleList.size();ii++) particleList.at(ii).RotateZ(theta);
		Pgamma.RotateZ(theta);
		//2b: rotate along y to have Pgamma along z plane
		theta=atan2(-Pgamma.X(),Pgamma.Z());
		for (int ii=0;ii<particleList.size();ii++) particleList.at(ii).RotateY(theta);
		Pgamma.RotateY(theta);

		//3: last rotation to have target and proton in xz
		theta=atan2(-particleList.at(3).Y(),particleList.at(3).X());
		for (int ii=0;ii<particleList.size();ii++) particleList.at(ii).RotateZ(theta);
		Pgamma.RotateZ(theta);


		m_kin=new Kinematics( particleList );
		//Set the weight
		weight=m_event.weight;
		m_kin->setWeight(weight);
		m_eventCounter++;
		return m_kin;
	}

	else{
		return NULL;
	}

}


unsigned int
JPsiDataReader::numEvents() const{
	if (!m_t) return 0;
	return m_Nevents;
}

