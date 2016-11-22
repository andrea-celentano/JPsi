/*
 * JPsiDataWriteLUND.cc
 *
 *  Created on: Nov 22, 2016
 *      Author: celentan
 */

#include "JPsiDataWriterLUND.h"

JPsiDataWriterLUND::JPsiDataWriterLUND(const string& outFile) {
	// TODO Auto-generated constructor stub
	m_outFile.open(outFile.c_str());
	m_eventCounter = 0;
	m_P = new vector<TLorentzVector>;
	Ptarget.SetXYZT(0,0,0,MP);
}

JPsiDataWriterLUND::~JPsiDataWriterLUND() {
	// TODO Auto-generated destructor stub
	m_outFile.close();
}

void JPsiDataWriterLUND::writeEvent( const Kinematics& kin,int *pid,int *status){
	vector< TLorentzVector > particleList = kin.particleList();
	m_nP=particleList.size();

	double Ebeam=11.;
	Ptarget.SetXYZT(0,0,0,MP);
	Pbeam.SetXYZT(0,0,Ebeam,Ebeam);

	Peprime=particleList.at(0);
	Pproton=particleList.at(1);
	Peplus=particleList.at(2);
	Peminus=particleList.at(3);

	//start writing output
	//x is a don't care
	//header line: NumberParticles X X X X X X X X X
	m_outFile<<"4 0 0 0 0 0 0 0 0 0"<<endl;
	//each particle line: index charge type(1 is active) PID parentID dautherID px(GeV) py(GeV) pz(GeV) E mass vx(cm) vy(cm) vz(cm)
	m_outFile<<"1 -1 1 11 0 0 "<<Peprime.Px()<<" "<<Peprime.Py()<<" "<<Peprime.Pz()<<" "<<Peprime.E()<<" "<<Peprime.M()<<" 0 0 0 "<<endl;
	m_outFile<<"2 1 1 2212 0 0 "<<Pproton.Px()<<" "<<Pproton.Py()<<" "<<Pproton.Pz()<<" "<<Pproton.E()<<" "<<Pproton.M()<<" 0 0 0 "<<endl;
	m_outFile<<"3 1 1 -11 0 0 "<<Peplus.Px()<<" "<<Peplus.Py()<<" "<<Peplus.Pz()<<" "<<Peplus.E()<<" "<<Peplus.M()<<" 0 0 0 "<<endl;
	m_outFile<<"4 -1 1 11 0 0 "<<Peminus.Px()<<" "<<Peminus.Py()<<" "<<Peminus.Pz()<<" "<<Peminus.E()<<" "<<Peminus.M()<<" 0 0 0 "<<endl;


}

void JPsiDataWriterLUND::writeEvent( const vector<TLorentzVector>& P,double weight,int *pid,int *status){
	Kinematics kin(P,weight);
	this->writeEvent(kin,pid,status);
}
