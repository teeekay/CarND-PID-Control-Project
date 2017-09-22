#include "PID.h"
#include <iostream>
#include <sysexits.h>
#include "debugging.h"

using namespace std;

void PID::Init(double K_p, double K_i, double K_d) {
  
  //Load in values of Tau
  if (tuning == true)
  {
    Kp = K_p +runnumber * .001; //when adjusting Kp
    Ki = K_i;
    Kd = K_d;// +runnumber * .05;  //when adjusting Kd

    //if (Kd > 15) {
    if (Kp > 0.25) {
      exit(EX_OK);
    }
  }
  else 
  {
    Kp = K_p;
    Ki = K_i;
    Kd = K_d;
  }
  cout << endl << "(Kp,Ki,Kd) set to (" << Kp << ", " << Ki << ", " << Kd << ")." << endl;
  runnumber += 1;
  // initialize all values to 0.0
  last_cte = 0.0;
  p_error = 0.0;
  i_error = 0.0;
  d_error = 0.0;
  cum_cte_sq_err = 0;
  iteration = 0; 
}

void PID::UpdateError(double cte) {
  p_error = cte;
  i_error += cte;
  d_error = cte - last_cte;
  last_cte = cte;
}

double PID::TotalError() {
  iteration++;
  //deal with junk on first iteration  
  if (iteration > 1){
    cum_cte_sq_err += (last_cte*last_cte);
    D(cout << "Iteration: " << iteration << ", Err: " << cum_cte_sq_err << " P_err: " << (-Kp * p_error) << ", D_err: " << (-Kd * d_error) << " I_err: " << (-Ki * i_error) << "\r " << flush;)
    return(-Kp * p_error - Kd * d_error - Ki * i_error);
  }
  else
  {
    return(0.0);
  }
}