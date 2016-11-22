#include <iostream>
#include <string>

//ROOT Stuffs
#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TParticle.h"
#include "TLorentzVector.h"
#include "TRandom3.h"
#include "TSystem.h"
//Read and write
#include "JPsiIO/JPsiDataWriter.h"
#include "JPsiIO/dict.h"
//MonteCarlo Stuff
extern "C"{
#include "TFastMC.hxx"
}


using namespace std;

Float_t Track(TLorentzVector* vec, Int_t ID, string CONFIGFILE);




int main(int argc, char** argv){


	// ************************
	// usage
	// ************************



	if (argc != 4){
		cout << "Usage:" << endl << endl;
		cout << "\tdoMonteCarlo> <Input file name> <Output file name> <Configuration File>" << endl << endl;
		return 1;
	}
	cout << endl << " *** Do MonteCarlo with FastMC *** " << endl << endl;
	gSystem->Load("libEG");
	// ************************
	// parse the command line parameters
	// ************************

	string infilename(argv[1]);
	string outfilename(argv[2]);
	string configfile(argv[3]);

	// ************************
	// create a DataWriter
	// ************************

	cout << "Creating Data Writer..." << endl;
	vector<string> dataWriterNames;
	dataWriterNames.push_back("FinalREC");
	JPsiDataWriter dataWriter(outfilename,"ACC");
	cout << "... Finished creating Data Writer" << endl << endl;


	// ************************
	// reader
	// ************************
	vector<TLorentzVector> *m_P=0;
	int status[20];
	int pid[20];
	int nP;
	TFile *f=new TFile(infilename.c_str(),"READ");
	TTree *t=(TTree*)f->Get("GEN");
	t->SetBranchAddress("P",&m_P);
	t->SetBranchAddress("nP",&nP);
	t->SetBranchAddress("pid",pid);
	//*********************
	//Read the number of events
	//********************
	int nevents=t->GetEntries();

	cout<<"There are: "<<nevents<<" events in input file"<<endl;

	// ************************
	// Read events and do the MonteCarlo
	// ************************
	Double_t weight=0; //Is detected?
	TLorentzVector *lv=new TLorentzVector(0,0,0,0);
	Int_t pdg=0;
	vector<TLorentzVector> out;
	int size;

	for (int ii=0;ii<nevents;ii++){
		if ((ii%10000)==0) cout<<"Event "<<ii<<endl;
		t->GetEntry(ii);
		out.clear();

		for (int jj=0;jj<m_P->size();jj++){
			//Get the Particle 4-vector and ID
			lv->SetXYZT(m_P->at(jj).X(),m_P->at(jj).Y(),m_P->at(jj).Z(),m_P->at(jj).T());
			pdg=pid[jj];
			weight=0;//If the particle is tracked (0-> NO, 1->YES)
			weight=Track(lv,pdg,configfile.c_str());
			status[jj]=weight;
			out.push_back(*lv);
		}
		dataWriter.writeEvent(out,1,pid,status);
	} //end cicle on events
}






Float_t Track(TLorentzVector* vec, Int_t ID,string CONFIGFILE){
	double THETAD0=vec->Theta()*TMath::RadToDeg();
	double P0=vec->Rho();
	double PHID0=vec->Phi()*TMath::RadToDeg();
	float THETAD1=THETAD0;
	float P1=P0;
	float PHID1=PHID0;


	float TORCUR=-844;  /*Max is 3375*/
	float D_PHI,WEIGHT=0;
	char* CFILE;
	double Mass=vec->M(); //nominal mass of the particle
	//char* CFILE="conf5.dat";
	CFILE=&CONFIGFILE[0];

	//PHIS1 HAS TO BE ABSOLUTE PHI IN DEGREES!!!


	TFastMC::clas_at12g(ID,P1,THETAD1,PHID1,TORCUR,D_PHI,WEIGHT,CFILE); //THIS INCLUDE ALSO THE FT!!



	//need double precision
	P0=P1;
	THETAD0=THETAD1;
	PHID0=PHID1;

	vec->SetXYZT(0,0,0,0);

	vec->SetZ(P0);
	vec->SetE(sqrt(P0*P0+Mass*Mass));
	vec->SetTheta(THETAD0*TMath::DegToRad());
	vec->SetPhi(PHID0*TMath::DegToRad());

	return WEIGHT;
}



