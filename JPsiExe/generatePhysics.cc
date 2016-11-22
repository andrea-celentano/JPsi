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




#include "JPsiIO/JPsiDataReader.h"
#include "JPsiIO/JPsiDataWriter.h"
#include "JPsiIO/JPsiDataWriterLUND.h"

#include "JPsiIO/dict.h"




//here goes our amplitudes
#include "JPsiAmp/BreitWigner.h"
#include "JPsiAmp/Constant.h"
#include "JPsiAmp/Diffractive.h"
#include "JPsiAmp/Resonance.h"

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
	cout << "\tgeneratePhysics -c <config file name> -i <infilename> -o <Out file> -s seed" << endl << endl;
	cout<<" Config file name: contains the Amplitude data to be used"<<endl;
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


	string cfgname;
	string infilename="in.root";
	string outfilename="out.root";
	int N;
	double seed=0;
	for (int ii=0;ii<argc;ii++){
		if (strcmp(argv[ii],"-c")==0) cfgname=argv[ii+1];
		if (strcmp(argv[ii],"-o")==0) outfilename=argv[ii+1];
		if (strcmp(argv[ii],"-i")==0) infilename=argv[ii+1];
		if (strcmp(argv[ii],"-s")==0) seed=atof(argv[ii+1]);
	}


	// string outfilename(argv[2]);


	cout << "Config file name = " << cfgname << endl << endl;
	cout << "Output file = "<<outfilename<<endl;
	cout << "Number of events = " <<N<<endl;
	gRandom->SetSeed(seed);

	// ************************
	// parse the config file
	// ************************

	ConfigFileParser parser(cfgname);
	ConfigurationInfo* cfgInfo = parser.getConfigurationInfo();
	cfgInfo->display();

	ReactionInfo* reaction = cfgInfo->reactionList()[0];


	// ************************
	// create an AmpToolsInterface
	// ************************

	cout << endl << endl;
	cout << "Creating AmpToolsInterface " << endl;

	AmpToolsInterface::registerAmplitude( BreitWigner() );
	AmpToolsInterface::registerAmplitude( Constant() );
	AmpToolsInterface::registerAmplitude( Diffractive() );
	AmpToolsInterface::registerAmplitude( Resonance() );

	vector < TLorentzVector > *m_P = new vector<TLorentzVector>;
	AmpToolsInterface ATI(cfgInfo);
	cout << "... Finished creating AmpToolsInterface" << endl;

	/*Output*/
//	JPsiDataWriter Writer(outfilename);
	JPsiDataWriterLUND Writer(outfilename);

	/*Input: for amplitudes*/
	vector<string> arguments;
	arguments.push_back(infilename);
	arguments.push_back("GEN");
	JPsiDataReader *Reader=new JPsiDataReader(arguments);
	Kinematics *kin;
	/*To write*/
	TFile *f=new TFile(infilename.c_str(),"READ");
	TTree *t=(TTree*)f->Get("GEN");
	t->SetBranchAddress("P",&m_P);



	N=Reader->numEvents();
	cout<<"Created input and output, OK"<<endl;
	cout<<"There are: "<<N<<" events"<<endl;
	cout<<"Read events"<<endl;
	ATI.loadEvents(Reader);
	cout<<"... Read done"<<endl;

	cout << "Doing physics" << endl;
	cout << "calculating intensities..." << endl;
	double maxIntensity;
	maxIntensity = ATI.processEvents(reaction->reactionName());//calcola le intensita' di tutti gli eventi, e ritorna il valore massimo.
	cout << "... finished calculating intensities. Max Intensity is: " <<maxIntensity<<endl;




	cout << "doing accept/reject..." << endl;
	double intensity=1;
	int saved=0;
	int pid[4]={11,2212,-11,11};
	int status[4]={1,1,1,1};
	for (int i = 0; i < N; i++){
		if ((i%10000)==0) cout<<"Event "<<i<<" intensity: "<< ATI.intensity(i)<<endl;
		intensity = ATI.intensity(i);
		t->GetEntry(i);
		if (intensity > gRandom->Uniform(0,maxIntensity)){  //se la intensita' attuale e' maggiore di quella casuale, SALVA EVENTO.
			Writer.writeEvent(*m_P,1,pid,status);
			saved++;
		}
	}



	cout << "... finished doing accept/reject" << endl;
	cout<<" Kept "<<saved<<" events "<<endl;


}
