#if !defined(ACONSTANT)
#define ACONSTANT

#include "IUAmpTools/Amplitude.h"
#include "IUAmpTools/UserAmplitude.h"
#include "IUAmpTools/AmpParameter.h"
#include "GPUManager/GPUCustomTypes.h"

#include <utility>
#include <string>
#include <complex>
#include <vector>

#ifdef GPU_ACCELERATION

void GPUConstant_exec( dim3 dimGrid, dim3 dimBlock, GPU_AMP_PROTO,
                          GDouble module, GDouble phase);

#endif // GPU_ACCELERATION

using std::complex;
using namespace std;

class Kinematics;

class Constant : public UserAmplitude< Constant >{ //deriva da Amplitude.

public:

  //il costruttore: costruisce Constant ma prima di poter fare qualsiasi cosa costruisce Amplitude con il suo costruttore
  //Una stupida ampiezza costante (compless), con modulo e fase
  Constant() : UserAmplitude< Constant >(){ }

    Constant( const vector< string > &args ); 

  ~Constant(){}

string name() const { return "Constant"; }

complex< GDouble > calcAmplitude( GDouble** pKin ) const;


#ifdef GPU_ACCELERATION

  void launchGPUKernel( dim3 dimGrid, dim3 dimBlock, GPU_AMP_PROTO ) const;

  bool isGPUEnabled() const { return true; }

#endif // GPU_ACCELERATION
  
private:
	
  AmpParameter m_module;
  AmpParameter m_phase;
};

#endif
