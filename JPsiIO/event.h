#ifndef EVENT_H_GUARD
#define EVENT_H_GUARD


typedef struct{	
	unsigned int run_number;
	float weight;
	float v,W,t,Q2,Ee,The;

	float m2ePeM; //e+e- invariant mass squared
	float mm2ep;   //missing mass proton-electron

	float m2pEpEm,mm2e,m2eEpEm,mm2p,m2ep,mm2EpEm;
}event;


#endif
