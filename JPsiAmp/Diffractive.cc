/*
This amplitude is for the eta-pi photoproduction, in the case of a double-regge exchange 
The formula is given in the document V. Mathieud gave me.

It is NOT in covariant form: 4-vectors have to be reported in the GJ frame of eta - pi. But this is handled in CLAS12PhotonsDataReader automatically!

*/

#include <cassert>
#include <iostream>
#include <string>
#include <complex>
#include <cstdlib>
#include <cmath>
#include "IUAmpTools/Kinematics.h"
#include "JPsiAmp/Diffractive.h"


/*parameters are: 
0  helicity_electron_beam    -> not used here, but in parent class
1  helicity_electron_recoil  -> not used here, but in parent class
2  helicity_target
3  helicity_recoil_proton
4  ID of the electron counting from 0 -> not used here, but in parent class 
*/
Diffractive::Diffractive(const vector<string> &args) ://beam electron target recoil
  Clas12PhotonsAmplitude(args),
  m_params(args)
{
  m_helicity_target=atoi(args[2].c_str());
  m_helicity_recoil=atoi(args[3].c_str());
  m_deltaHelicity_leptons=atoi(args[5].c_str());
}

/*Order of the particles in pKin:
0  beam (THE ELECTRON!)
1  target
2  eprime
3  proton
4  ePlus
5  eMinus
*/


complex< GDouble > Diffractive::calcHelicityAmplitude(int helicity,GDouble** pKin ) const{

if (helicity==0) return complex<GDouble> (0.,0.); //put this here because for a longitudinal quasi-real photon the amplitude is 0, don't go further!
if ((m_helicity_target*m_helicity_recoil)==-1) return complex<GDouble>(0.,0.); //for this specific case, +- or -+ in target, recoil. Amplitude is 0

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

  
  
  TLorentzVector Pg,Ppsi;


  GDouble s,t;
  GDouble norm;
  GDouble alpha;
  GDouble beta;
  GDouble s0=1.;
  GDouble b=0.7;
  GDouble theta,phi;

  //The amplitude
  complex<double> amp(1.,0.);
  complex<double> i(0.,1.);
  complex<double> one(1.,0.);


  //1:define the photon beam and the JPsi
  Pg=Pbeam-Peprime;
  Ppsi=PePlus+PeMinus;

  //2: define the invariants
  s=(Pg+Ptarget).M2();
  t=(Ptarget-Pproton).M2(); //negative here!!!

  
  //3: define the trajectories, the couplings, and the signature factors.
  alpha=2.26+0.25*t;

  //4: for the Psi decay, there is a term D*_(lambdaGamma,deltaLambdaLeptons) (phi2,theta2,0)
  //Where phi2 and theta2 are measured in the psi rest frame, with z axis equal to the momentum direction of the psi in the gamma-proton frame.
  //so boost!
  PePlus.Boost(-(Ppsi.BoostVector()));
  PeMinus.Boost(-(Ppsi.BoostVector()));
  theta=PeMinus.Theta();
  phi=PeMinus.Phi();


  amp=one*pow(s/s0,alpha)*exp(b*t);

  /*
   * Add the D* term=exp(phi2*lambdaGamma)*d(theta)*exp(-phi2*deltaLambda)
   */
  amp=amp*(one*cos(phi)+i*(helicity*sin(phi)));
  amp=amp*(one*cos(phi)-i*(m_deltaHelicity_leptons*sin(phi)));
  if (helicity==m_deltaHelicity_leptons) amp=amp*((1+cos(theta))/2);
  else amp=amp*((1-cos(theta))/2);

  
  

  amp=amp*(1./sqrt(TMath::TwoPi())); //The factor 2Pi is for the integration in dPh	
  return amp;
	
}





#ifdef GPU_ACCELERATION 
void
Constant::launchGPUKernel( dim3 dimGrid, dim3 dimBlock, GPU_AMP_PROTO ) const {
  
  // use integers to endcode the string of daughters -- one index in each
  // decimal place
  
  GPUDiffractive_exec( dimGrid,  dimBlock, GPU_AMP_ARGS,int helicity_beam,int helicity_electron,int helicity_target,int helicity_recoil);

}
#endif //GPU_ACCELERATION

Diffractive*
Diffractive::newAmplitude(const vector < string > & args) const {
  return new Diffractive(args);
}

Diffractive*
Diffractive::clone() const {
  return ( isDefault() ? new Diffractive() :
	   new Diffractive(m_params) );

}
