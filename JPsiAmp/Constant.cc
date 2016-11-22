#include <cassert>
#include <iostream>
#include <string>
#include <complex>
#include <cstdlib>
#include "IUAmpTools/Kinematics.h"
#include "JPsiAmp/Constant.h"

Constant::Constant(const vector <string> & args) : //module, phase
  UserAmplitude< Constant > (args)
  
{
  assert (args.size()==2);
  m_module=AmpParameter(args[0]);
  m_phase=AmpParameter(args[1]);
  
  
  
  // need to register any free parameters so the framework knows about them
  registerParameter( m_module );
  registerParameter( m_phase );
  
}

//QUESTA e' LA FUNZIONE DI CALCOLO DELLA AMPIEZZA VERA E PROPRIA.
//RITORNA UN NUMBERO COMPLESSO
//IN INGRESSO, UN VETTORE DI VETTORI DI DOUBLE, corrispondenti alle 4 componenti delle N particelle.
//In Uscita, un numero complesso: l'ampiezza.

//QUESTO SEMPLICE ESEMPIO E' UNA AMPIEZZA COSTANTE (complessa): i 2 parametri sono MODULO e FASE.
complex< GDouble >
Constant::calcAmplitude( GDouble** pKin ) const {
  
  double phase=m_phase*M_PI/180;

  double real=m_module*cos(phase);
  double imaginary=m_module*sin(phase);

  return  complex<GDouble>(real,imaginary);

}
 


#ifdef GPU_ACCELERATION
void
Constant::launchGPUKernel( dim3 dimGrid, dim3 dimBlock, GPU_AMP_PROTO ) const {
  
  // use integers to endcode the string of daughters -- one index in each
  // decimal place
  
  GPUBreitWigner_exec( dimGrid,  dimBlock, GPU_AMP_ARGS, 
                       m_module,m_phase );

}
#endif //GPU_ACCELERATION
