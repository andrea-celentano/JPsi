#include <iostream>
#include <string>
#include <cmath>

#include "TF2.h"
#include "TMath.h"
#include "TAxis.h"

#include "TCanvas.h"
#include "TApplication.h"
#include "TStyle.h"
#include "TROOT.h"
using namespace std;

double flux_E_CTH(double *x,double *par);
double flux_E_TH(double *x,double *par);
double flux_w_Q2(double *x,double *par);

TApplication gui("gui",0,NULL);

int main(){
  
 gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  gStyle->SetNumberContours(100);

  gStyle->SetPadGridX(kTRUE);
  gStyle->SetPadGridY(kTRUE);


  double E0=11; //primary electron beam energy;
  double theta_min=2.5;
  double theta_max=4.5;
  double E_min=0.5;
  double E_max=4.5;

  //flux as a function of ENERGY (x) and ANGLE (Y)
  TF2 *f1=new TF2("f1",flux_E_TH,E_min,E_max,theta_min,theta_max,1);
  TF2 *f2=new TF2("f2",flux_E_CTH,E_min,E_max,cos(theta_max*TMath::DegToRad()),cos(theta_min*TMath::DegToRad()),1);
  TCanvas *c1=new TCanvas("c1","c1");
  f1->SetParameter(0,E0);
  f1->SetNpx(200);
  f1->SetNpy(200);
  f1->GetXaxis()->SetTitle("Scattered electron energy (GeV)");
  f1->GetYaxis()->SetTitle("Scattered electron angle (deg)");
  f1->Draw("colz");
  double flux1,flux2;
  flux1=f1->Integral(E_min,E_max,theta_min,theta_max);
  cout<<"Integral 1: "<<flux1<<endl;
 

  TCanvas *c2=new TCanvas("c2","c2");
  f2->SetParameter(0,E0);
  f2->SetNpx(1000);
 f2->SetNpy(1000);
  f2->GetXaxis()->SetTitle("Scattered electron energy (GeV)");
  f2->GetYaxis()->SetTitle("Scattered electron angle cosine");
  f2->Draw("colz");
  flux2=f2->Integral(E_min,E_max,cos(theta_max),cos(theta_min));
  cout<<"Integral 2: "<<flux2<<endl;
  

  gui.Run(1);
}


/*the foton flux, expressed as a function of
-scattered electron energy
-scattered electron angle
*/
double flux_E_TH(double *x,double *par){
  double E=x[0];
  double theta=x[1]*TMath::DegToRad(); //in x[1] in deg!!!
  double E0=par[0]; //primary beam energy 

  double val=0;
  //convert to Q2 and w.
  double new_x[2];
  new_x[0]=E0-E; //w
 
  new_x[1]=4*E*E0*sin(theta/2)*sin(theta/2); //q2

  val=flux_w_Q2(new_x,par);
  //add the Jacobian for the transformation (E,theta)->(W,Q2)
  val*=2*E0*E*sin(theta);
  return val;

}

/*the foton flux, expressed as a function of
-scattered photon energy w
-Q2
*/
double flux_w_Q2(double *x,double *par){
  double w=x[0];
  double Q2=x[1];
  double E0=par[0];
  const double alpha=1./137;
  const double me=511E-6;
  double val=0;
  val=alpha/(4*TMath::Pi()*E0*E0);
  val*=(1-4*me*me/Q2+(2*E0-w)*(2*E0-w)/(w*w+Q2));
  val=val*w;
  val=val/Q2;

   return val;


}

double flux_E_CTH(double *x,double *par){
  double E=x[0];
  double ctheta=x[1];
  double stheta=sqrt(1-ctheta*ctheta);
  double E0=par[0]; //primary beam energy 
  double w,Q2;
  const double alpha=1./137;
  const double me=511E-6;
  double val=0;
  //convert to Q2 and w.
  w=E0-E; //w
  Q2=2*E*E0*(1-ctheta);
  
  val=alpha/(2*TMath::Pi());
  val*=(E/E0);
  val*=(w/Q2);
  val*=(1+(2*E0-w)*(2*E0-w)/(w*w)+4*me*me/Q2);    
  return val;

}
