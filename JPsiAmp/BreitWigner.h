#if !defined(BREITWIGNER)
#define BREITWIGNER

#include "IUAmpTools/Amplitude.h"
#include "IUAmpTools/UserAmplitude.h"
#include "IUAmpTools/AmpParameter.h"
#include "GPUManager/GPUCustomTypes.h"

#include <utility>
#include <string>
#include <complex>
#include <vector>

#ifdef GPU_ACCELERATION

void GPUBreitWigner_exec( dim3 dimGrid, dim3 dimBlock, GPU_AMP_PROTO,
                          GDouble mass0, GDouble width0,
                          int daughter1, int daughter2 );

#endif // GPU_ACCELERATION

using std::complex;
using namespace std;

class Kinematics;

class BreitWigner : public UserAmplitude< BreitWigner > { //deriva da Amplitude.

public:

  //il costruttore: costruisce BreitWigner ma prima di poter fare qualsiasi cosa costruisce Amplitude con il suo costruttore
  BreitWigner() : UserAmplitude< BreitWigner > () { }

  BreitWigner( const vector < string > &args);

  ~BreitWigner(){}

  string name() const { return "BreitWigner"; }

  complex< GDouble > calcAmplitude( GDouble** pKin ) const;

  double BarrierFactor(int L,double q,double q0) const;

#ifdef GPU_ACCELERATION

  void launchGPUKernel( dim3 dimGrid, dim3 dimBlock, GPU_AMP_PROTO ) const;

  bool isGPUEnabled() const { return true; }

#endif // GPU_ACCELERATION
  
private:
	
  AmpParameter m_mass0;
  AmpParameter m_width0;



};

#endif
