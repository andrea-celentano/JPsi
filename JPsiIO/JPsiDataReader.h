#if !defined(JPSIDATAREADER)
#define JPSIDATAREADER

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "TString.h"
#include "TChain.h"
#include "TTree.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "IUAmpTools/DataReader.h"
#include "IUAmpTools/UserDataReader.h"
#include "JPsiIO/event.h"

using namespace std;

class JPsiDataReader : public UserDataReader< JPsiDataReader >{

public:
	JPsiDataReader() : UserDataReader< JPsiDataReader >() { }
	JPsiDataReader( const vector< string >& args );
	string name() const { return "JPsiDataReader"; }
	virtual Kinematics* getEvent();
	virtual void resetSource();
	virtual unsigned int numEvents() const;
	int eventCounter() const { return m_eventCounter; }


private:
	

	string m_treeName;
	string m_fileName;
	//event-counters
	int m_eventCounter;	
	unsigned int m_Nevents;
	int m_eventIDX;

	//the chain to handle multiple TTres
	TTree *m_t;
	TFile *m_f;
	vector <TLorentzVector> *m_P;
	event m_event;
	//the kinematics object to return	
	Kinematics *m_kin;


	

	
	
};

#endif
