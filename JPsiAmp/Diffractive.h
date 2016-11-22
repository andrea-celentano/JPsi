#if !defined(DIFFRACTIVE)
#define DIFFRACTIVE

#include "IUAmpTools/Amplitude.h"
#include "IUAmpTools/AmpParameter.h"
#include "GPUManager/GPUCustomTypes.h"

#include "Clas12PhotonsAmplitude.h"

#include <utility>
#include <string>
#include <complex>
#include <vector>

#ifdef GPU_ACCELERATION

void GPUDiffractive_exec( dim3 dimGrid, dim3 dimBlock, GPU_AMP_PROTO,
                         int helicity_beam,int helicity_electron,int helicity_target,int helicity_proton);

#endif // GPU_ACCELERATION

using std::complex;
using namespace std;

class Kinematics;

class Diffractive : public Clas12PhotonsAmplitude{ //deriva da Clas12PhotonsAmplitude (che a sua volta deriva da Amplitude!)

public:
  
  Diffractive() : Clas12PhotonsAmplitude() { }
    
    Diffractive(const vector < string > &args);

    ~Diffractive(){}
    
    Diffractive* clone() const;
    Diffractive* newAmplitude(const vector < string > &args) const;

    string name() const { return "Diffractive"; }
    complex< GDouble > calcHelicityAmplitude(int helicity,GDouble** pKin ) const;
        


#ifdef GPU_ACCELERATION

    void launchGPUKernel( dim3 dimGrid, dim3 dimBlock, GPU_AMP_PROTO ) const;

    bool isGPUEnabled() const { return true; }
    
#endif // GPU_ACCELERATION
  
 private:
    int m_helicity_target,m_helicity_recoil,m_deltaHelicity_leptons;
    vector < string > m_params;
};

#endif
