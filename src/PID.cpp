#include "PID.h"
#include <iostream>
#include "debugging.h"

using namespace std;

void PID::Init(double K_p, double K_i, double K_d) {
  //Load in values of Tau
  Kp = K_p;
  Ki = K_i;
  Kd = K_d;

  // initialize all values to 0.0
  last_cte = 0.0;
  p_error = 0.0;
  i_error = 0.0;
  d_error = 0.0;
  iteration = 1;
}

void PID::UpdateError(double cte) {
  p_error = cte;
  i_error += cte;
  d_error = cte - last_cte;
  last_cte = cte;
}

double PID::TotalError() {
  iteration++;
  cum_cte_sq_err += (last_cte*last_cte);
  double regional_error = cum_cte_sq_err / double(iteration);
  D(cout << "Err: " << regional_error << " P: " << (-Kp * p_error) << ", D: " << (-Kd * d_error) << " I: " << (-Ki * i_error) << " " << endl;)
  return(-Kp * p_error - Kd * d_error - Ki * i_error);
}