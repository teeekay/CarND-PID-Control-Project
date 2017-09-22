#ifndef PID_H
#define PID_H

class PID {
public:
  /*
  * Errors
  */
  double p_error;
  double i_error;
  double d_error;

  /*
  * Coefficients
  */ 
  double Kp;
  double Ki;
  double Kd;

  double last_cte; //cte in last measurement - use to calc d_error
  double cum_cte_sq_err;

  bool tuning;
  bool reset;
  int runnumber;
  int iteration;



  /*
  * Constructor
  */
  PID() :reset(false), tuning(true), runnumber(0) {}

  /*
  * Destructor.
  */
  ~PID(){}

  /*
  * Initialize PID.
  */
  void Init(double Kp, double Ki, double Kd);

  /*
  * Update the PID error variables given cross track error.
  */
  void UpdateError(double cte);

  /*
  * Calculate the total PID error.
  * use cumulative square of cross track errors
  */
  double TotalError();

};
#endif /* PID_H */
