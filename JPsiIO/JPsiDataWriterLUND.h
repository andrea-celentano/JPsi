/*
 * JPsiDataWriteLUND.hh
 *
 *  Created on: Nov 22, 2016
 *      Author: celentan
 */

#ifndef JPSIIO_JPSIDATAWRITERLUND_H_
#define JPSIIO_JPSIDATAWRITERLUND_H_


#include "IUAmpTools/Kinematics.h"

#include "TLorentzVector.h"
#include "TVector3.h"
#include "TTree.h"
#include "TFile.h"
#include "event.h"
#include <fstream>
#define MP 0.9383
#define MJ 3.09691

class JPsiDataWriterLUND {
public:
	JPsiDataWriterLUND(const string& outFile);
	virtual ~JPsiDataWriterLUND();

	void writeEvent( const Kinematics& kin,int *pid=0,int *status=0);
	void writeEvent( const vector<TLorentzVector>& P,double weight=1,int *pid=0,int *status=0);
	int eventCounter() const { return m_eventCounter; }

private:
	  static const int kMaxParticles=20;
	  std::ofstream m_outFile;
	  int m_eventCounter;

	  vector < TLorentzVector > *m_P;
	  int m_nP;
	  int m_pid[kMaxParticles];
	  int m_status[kMaxParticles];
	  event m_e;


	  TLorentzVector Pbeam,Ptarget,Peprime,Pproton,Ppsi,Peplus,Peminus;


};

#endif /* JPSIIO_JPSIDATAWRITERLUND_H_ */
