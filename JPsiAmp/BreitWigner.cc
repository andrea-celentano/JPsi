/***********************************************************************************
27/11/2012
This amplitude is a relativistic Breit-Wigner for a particle with mass MR and width W0 and spin J decaying in two particles "a" and "b", with branching ration BR.

The amplitude is written in covariant way: it is valid in ANY reference frame.


The formula is:

BW(s)=(MR * W0 *sqrt(BR))/[MR*MR - s - i M W(s)]

where 

W(s)=W0 * MR/M [ P(s) / P(M*M)] ^ (2J+1) *

P(s) is the module of the 3-momentum of (both) decay particles in their reference frame.

P(s)=sqrt{1/ (4s) * [(s-Ma^2-Mb^2)^2 - 4 * Ma^2 * Mb^2]}

 */







#include <cassert>
#include <iostream>
#include <string>
#include <complex>
#include <cstdlib>
#include "TLorentzVector.h"
#include "IUAmpTools/Kinematics.h"
#include "JPsiAmp/BreitWigner.h"

/*LIST OF THE PARAMETERS. 2 Parameters
0 -Resonance Mass
1 -Resonance NominalWidth
 */

BreitWigner::BreitWigner( const vector<string> &args):
UserAmplitude< BreitWigner >(args)
{


	assert( args.size() == 2 );

	m_mass0 = AmpParameter(args[0]);
	m_width0 = AmpParameter(args[1]);



	// need to register any free parameters so the framework knows about them
	registerParameter( m_mass0 );
	registerParameter( m_width0 );
}

complex< GDouble >
BreitWigner::calcAmplitude( GDouble** pKin ) const {

	TLorentzVector Pbeam(pKin[0][1], pKin[0][2],
			pKin[0][3], pKin[0][0]);

	TLorentzVector Ptarget(pKin[1][1], pKin[1][2],
			pKin[1][3], pKin[1][0]);

	TLorentzVector Peprime(pKin[2][1], pKin[2][2],
			pKin[2][3], pKin[2][0]);

	TLorentzVector Pproton(pKin[3][1], pKin[3][2],
			pKin[3][3], pKin[3][0]);

	TLorentzVector PePlus(pKin[4][1], pKin[4][2],
			pKin[4][3], pKin[4][0]);

	TLorentzVector PeMinus(pKin[5][1], pKin[5][2],
			pKin[5][3], pKin[5][0]);

	TLorentzVector PJ=PePlus+PeMinus;

	complex <GDouble> amp;

	//invariant mass of the two daughters, and their mass
	GDouble s = (PJ+Pproton).M2();
	GDouble W = sqrt(s);



	complex<GDouble> bwdenominator(s-m_mass0*m_mass0, 1*m_width0*W);


	amp=1./bwdenominator;
	return  amp;

}

#ifdef GPU_ACCELERATION
void
BreitWigner::launchGPUKernel( dim3 dimGrid, dim3 dimBlock, GPU_AMP_PROTO ) const {

	// use integers to endcode the string of daughters -- one index in each
	// decimal place

	GPUBreitWigner_exec( dimGrid,  dimBlock, GPU_AMP_ARGS,
			m_mass0, m_width0, m_daughter1, m_daughter2 );

}
#endif //GPU_ACCELERATION


