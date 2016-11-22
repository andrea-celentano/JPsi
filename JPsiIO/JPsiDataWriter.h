#if !defined(JPSIDATAWRITER)
#define JPSIDATAWRITER

#include "IUAmpTools/Kinematics.h"

#include "TLorentzVector.h"
#include "TVector3.h"
#include "TTree.h"
#include "TFile.h"
#include "event.h"

#define MP 0.9383
#define MJ 3.09691

class JPsiDataWriter{
 
public:
		
  JPsiDataWriter( const string& outFile,const string& treeName="GEN");
  ~JPsiDataWriter();

  void writeEvent( const Kinematics& kin,int *pid=0,int *status=0);
  void writeEvent( const vector<TLorentzVector>& P,double weight=1,int *pid=0,int *status=0);
  int eventCounter() const { return m_eventCounter; }

private:
  static const int kMaxParticles=20;
  TFile* m_outFile;
  TTree* m_outTree;
  int m_eventCounter;

  vector < TLorentzVector > *m_P;
  int m_nP;
  int m_pid[kMaxParticles];
  int m_status[kMaxParticles];
  event m_e;


  TLorentzVector Pbeam,Ptarget,Peprime,Pproton,Ppsi,Peplus,Peminus;


};

#endif
