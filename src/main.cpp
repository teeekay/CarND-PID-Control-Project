#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>
#include "debugging.h"
#include <sysexits.h>

// for convenience
using json = nlohmann::json;
using namespace std;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}


//fit steer to value between 1.0 and -1.0

double fix_steer(double steer, double max_steerval) {
  double new_steer = steer;
  if (fabs(steer) > max_steerval) 
  {
    //new_steer = 1.0 * steer / (fabs(steer)); 
    new_steer = max_steerval * steer / (fabs(steer));
    D(cout << "\rFix Steer from " << steer << " to  " << new_steer << ". \r" << flush;)
  }
  return(new_steer);
}

int main(int argc, char *argv[])
{
  uWS::Hub h;

  double init_steerval;
  double max_steerval;
  double max_throttle;
  PID steerpid;
  PID speedpid;
  double init_throttle = 0.3;

  if (argc == 2)
  {
    cout << "The initial throttle argument supplied is " << argv[1] << endl;
    double tmp_throttle = atof(argv[1]);
    if (tmp_throttle > 0 and tmp_throttle < 1.0) {
      init_throttle = tmp_throttle;
    }
  }
  else {
    cout << "To specify the throttle value to use at run time call " << endl << endl;
    cout << argv[0] << " throttle_value" << endl << endl;
    cout << "like this:" << endl << argv[0] << " 0.5" << endl;
    cout << "throttle_value should be a float between 0.1 and 0.999." << endl;
    cout << "Otherwise, " << init_throttle << " will be used by default." << endl;
  }

  cout << "Using " << init_throttle << " as throttle on this run." << endl << endl;


  // TODO: Initialize the pid variable - (in connection).

  h.onMessage([&steerpid, &speedpid, &init_steerval, &max_steerval, &init_throttle, &max_throttle, &h]
     (uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) 
  {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data).substr(0, length));
      if (s != "") {
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") 
        {
          /**
          * Put the car at the start of the track at the start of the run
          **/
          if (!steerpid.reset) {
            std::string reset_msg = "42[\"reset\", {}]";
            ws.send(reset_msg.data(), reset_msg.length(), uWS::OpCode::TEXT);
            steerpid.reset = true;
            return;
          }

           // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<std::string>());
          double speed = std::stod(j[1]["speed"].get<std::string>());
          double angle = std::stod(j[1]["steering_angle"].get<std::string>());
          double steer_value;

          // figure out if anything else useful coming in JSON message
          D1(cout << "J:" << j << endl;)

          //pass in cte to calculate cte error terms and steering response
          steerpid.UpdateError(cte);

          //deal with first msg coming in before pid initialized for run
          steer_value = steerpid.TotalError();
          
          if (steerpid.tuning == true) {
            // set up to run profiles of mean square error as values of max_steerval
            // and max_throttle changed.
            max_steerval = init_steerval;// +steerpid.runnumber * 0.05;
            max_throttle = init_throttle;// +steerpid.runnumber *0.025;
            //exit conditions when tuning steer
            if (max_steerval > 0.8) {
              exit(EX_OK);
            }
            if (max_throttle > 0.9) {
              exit(EX_OK);
            }
          }

          //keep steering values in the range of +/- max_steerval
          steer_value = fix_steer(steer_value, max_steerval);
          
          if (steerpid.tuning && steerpid.iteration == 12000) 
          {
            // Output MSE for cte on last 12000 iterations
            cout << "Iteration: " << steerpid.iteration << ", Error: " << steerpid.cum_cte_sq_err <<
              " for settings (Kp, Ki, Kd) = (" << steerpid.Kp << ", " << steerpid.Ki <<
              ", " << steerpid.Kd << ") max_steer = " << max_steerval << ", throttle = " << max_throttle << endl;

            // send a reset to put the car at the start
            std::string reset_msg = "42[\"reset\", {}]";
            ws.send(reset_msg.data(), reset_msg.length(), uWS::OpCode::TEXT);

            steerpid.reset = true;
            return;
          }

          // put in routine just to crank up throttle every couple of laps to see 
          // how fast it can go - 
          if (!steerpid.tuning) {
            if (steerpid.iteration % 12000 == 0) {
              max_throttle = max_throttle + 0.01;
              cout << endl << endl << "Boosting throttle to " << max_throttle << " after " << steerpid.iteration << " iterations - error = " << steerpid.cum_cte_sq_err << endl;
              steerpid.cum_cte_sq_err = 0; 
            }
          }
          
          D1(std::cout << "CTE: " << cte << " Steering Value: " << steer_value << std::endl;)

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = max_throttle;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          D1(std::cout << msg << std::endl;)
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }
      }
      else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&steerpid, &speedpid, &init_steerval, &max_steerval, &max_throttle, &init_throttle , &h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << flush;

    // optimized PID parameters
    double Taup = 0.244;
    double Taui = 0.0;
    double Taud = 8.91; // 8.91;

    steerpid.Init(Taup, Taui, Taud);
    
    //when tuning comment out next line
    steerpid.tuning = false;

    
    init_steerval = 0.75;//0.6175;
    max_steerval = init_steerval; //set once in case not tuning

    //init_throttle passed into program at start of main()
    max_throttle = init_throttle;
    cout << endl << "Limiting maximum steer to (-" << init_steerval << "," << init_steerval << ")." << endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    std::cerr << "Disconnected: code=" << code << ":" << std::string(message, length) << std::endl;
    exit(EX_OK);
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
