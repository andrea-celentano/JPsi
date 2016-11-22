#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "TString.h"
#include "TH1F.h"
#include "TFile.h"
#include "TRandom3.h"
#include "TGenPhaseSpace.h"
#include "TLorentzVector.h"
#include "TRandom.h"
#include "TTree.h"
#include "TMath.h"
#include "TApplication.h"

#include "IUAmpTools/Kinematics.h"
#include "IUAmpTools/AmplitudeManager.h"
#include "IUAmpTools/AmpToolsInterface.h"
#include "IUAmpTools/ConfigFileParser.h"
#include "IUAmpTools/ConfigurationInfo.h"





#include "JPsiIO/JPsiDataWriter.h"
#include "JPsiIO/dict.h"



//here goes our amplitudes
#include "JPsiAmp/BreitWigner.h"
#include "JPsiAmp/Constant.h"


#ifdef __MAKECINT__
#pragma link C++ class std::vector<TLorentzVector>+;
#endif

#define Me 0.511E-3 //get the mass of the electron in GeV
#define Mp 0.9383
#define MJPsi 3.09691
#define Ebeam 11.000510999

//define here the beam (e-) and the target(p) in the LAB frame, standard CLAS12 reference frame.
//z along beam, y up from the earth surface, x=y vector z
TLorentzVector Pbeam(0,0,sqrt(Ebeam*Ebeam-Me*Me),Ebeam);
TLorentzVector Ptarget(0,0,0,Mp);

using namespace std;

void print_help(){
	cout << "Usage:" << endl << endl;
	cout << "\tgeneratePS -N number of events -s seed -o <Data file out>"<< endl << endl;
	cout<<" Pseudo data file out: file where we save the pseudo-events"<<endl;

}






int main(int argc, char** argv){


	// ************************
	// usage
	// ************************

	cout << endl << " *** Generating Events According to Amplitudes *** " << endl << endl;

	if (argc == 1){
		print_help();
		return 0;
	}


	// ************************
	// parse the command line parameters
	// ************************




	string outfilename="null.root";
	int N;
	double seed=0;
	for (int ii=0;ii<argc;ii++){

		if (strcmp(argv[ii],"-N")==0) N=atoi(argv[ii+1]);
		if (strcmp(argv[ii],"-o")==0) outfilename=argv[ii+1];
		if (strcmp(argv[ii],"-s")==0) seed=atof(argv[ii+1]);
	}


	// string outfilename(argv[2]);



	cout << "Output file = "<<outfilename<<endl;
	cout << "Number of events = " <<N<<endl;
	gRandom->SetSeed(seed);




	/*Output*/
	JPsiDataWriter Writer(outfilename);
	vector < TLorentzVector > m_P;


	/*Generate PS events*/
	TGenPhaseSpace generator;
	TLorentzVector P0=Pbeam+Ptarget;
	TLorentzVector Ppsi,Pe,Pp,Peplus,Peminus;



	double daughterMasses[3] = {Me,Mp,MJPsi};
	generator.SetDecay(P0, 3, daughterMasses);

	double maxWeight = generator.GetWtMax();
	double weight;
	double intensity;
	double Ee,The;
	double sThetaPsi,cThetaPsi,PhiPsi,PPsi;

	Double_t Eemin=0.5;//min scattered electron energy
	Double_t Eemax=4.0;//max scattered electron energy
	Double_t thmin=2.5*TMath::DegToRad(); //min scatter angle convert to rad
	Double_t thmax=4.5*TMath::DegToRad(); //min scatter angle convert to rad


	int pid[4]={11,2212,-11,11};
	int status[4]={1,1,1,1};
	cout << "doing phase space..." << endl;
	for (int i = 0; i < N; i++){
		weight=generator.Generate();
		if (weight < gRandom->Uniform(0,maxWeight)){
			i--;
			continue;
		}
		// pack the decay products into a Kinematics object
		Pe=*(generator.GetDecay(0));  //This is the e- in the lab frame
		Pp=*(generator.GetDecay(1));  //This is the p in the lab frame
		Ppsi=*(generator.GetDecay(2));  //This is the J/Psi in the lab frame


		if (1){ //implement the cut on the scattered electron
			Ee=Pe.E();
			The=Pe.Theta();
			if (!((Ee>Eemin)&&(Ee<Eemax)&&(The>thmin)&&(The<thmax))){
				i--;
				continue;
			}
		}
		/*Now let the Psi decay. It is easy, use the ultra-rel limit*/
		PPsi=MJPsi/2;
		PhiPsi=gRandom->Uniform(0,2*TMath::Pi());
		cThetaPsi=gRandom->Uniform(-1,1);
		sThetaPsi=sqrt(1-cThetaPsi*cThetaPsi);
		Peminus.SetXYZT(PPsi*sThetaPsi*cos(PhiPsi),PPsi*sThetaPsi*sin(PhiPsi),PPsi*cThetaPsi,PPsi);
		Peplus.SetXYZT(-PPsi*sThetaPsi*cos(PhiPsi),-PPsi*sThetaPsi*sin(PhiPsi),-PPsi*cThetaPsi,PPsi);
		/*Go back to the lab frame*/
		Peminus.Boost(Ppsi.BoostVector());
		Peplus.Boost(Ppsi.BoostVector());


		m_P.clear();
		m_P.push_back(Pe);
		m_P.push_back(Pp);
		m_P.push_back(Peplus);
		m_P.push_back(Peminus);


		Writer.writeEvent(m_P,1,pid,status);





		if (i%10000==0) cout<<i<<endl;
	}
	cout << "phase space done." << endl;





}
