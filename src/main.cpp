#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>
#include "debugging.h"
#include <sysexits.h>

//// from https://github.com/jdleesmiller/CarND-PID-Control-Project/
//// Use this code when closing the socket after we detect that the car has
//// crashed; this lets the server know that it was closed intentionally, rather
//// than due to a network / simulator crashing problem.
////const int CAR_CRASHED_CODE = 2000;
////const int MAX_RUNTIME_CODE = 2001;


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

double fix_steer(double steer) {
  double new_steer = steer;
  double max_steerval = 0.475;
  if (fabs(steer) > max_steerval) 
  {
    //new_steer = 1.0 * steer / (fabs(steer)); 
    new_steer = max_steerval * steer / (fabs(steer));
    D(cout << "Fix Steer from " << steer << " to  " << new_steer << "." << endl;)
  }
  return(new_steer);
}


int main()
{
  uWS::Hub h;

  PID steerpid;
  PID speedpid;
  // TODO: Initialize the pid variable - (in connection).

  h.onMessage([&steerpid, &speedpid, &h](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data).substr(0, length));
      if (s != "") {
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") {
          if (steerpid.tuning && !steerpid.reset) {
            std::string reset_msg = "42[\"reset\", {}]";
            ws.send(reset_msg.data(), reset_msg.length(), uWS::OpCode::TEXT);
            steerpid.reset = true;
            return;
          }

          ////// taken from https://github.com/jdleesmiller/CarND-PID-Control-Project/
          ////// Make sure we've reset the simulator once on this run.
          ////if (pid.tuning && !reset) {
          ////  std::string reset_msg = "42[\"reset\", {}]";
          ////  ws.send(reset_msg.data(), reset_msg.length(), uWS::OpCode::TEXT);
          ////  reset = true;
          ////  return;
          ////}

          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<std::string>());
          double speed = std::stod(j[1]["speed"].get<std::string>());
          double angle = std::stod(j[1]["steering_angle"].get<std::string>());
          double steer_value;

          steerpid.UpdateError(cte);
          steer_value = steerpid.TotalError();
          steer_value = fix_steer(steer_value);
 
          /*
          * TODO: Calcuate steering value here, remember the steering value is
          * [-1, 1].
          * NOTE: Feel free to play around with the throttle and speed. Maybe use
          * another PID controller to control the speed!
          */

          ////// If we've run all the way to the deadline, stop.
          ////if (pid.tuning && pid.runtime > max_runtime) {
          ////  std::cout << pid << std::endl;
          ////  ws.close(MAX_RUNTIME_CODE);
          ////  return;
          ////}
          // DEBUG


          D(std::cout << "CTE: " << cte << " Steering Value: " << steer_value << std::endl;)

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = 0.5;//originally 0.3
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

  h.onConnection([&steerpid, &speedpid, &h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
    ////if (!pid.tuning) {
    ////  std::cout << "Connected!!!" << std::endl;
    ////}
    ////pid.Init();
    //.08,0,0.8 got halfway round the lake, but wobbling was excessive from bridge onwards - increase D
    //.08,0,1.0 this is a good balance, but does not steer sharp enough around 2nd and 3rd corner - increase p and D
    //.1, 0, 1.2 - made it around track, but was a bit wobbly - increase both p and d slightly (at max 35 mph)
    // .13, 0, 1.4 - made around track, too wobbly
    // .13, 0, 1.8 - pretty good, not quite enough steer response on sharper corners.  try boosting both p and d
    //.15,0,2.5 - better, but needs a tiny bit more
    // .17, 0, 2.8 - better, still too wobbly, and could be damped more
    // .19, 0, 3.5 - better, could stil be damped more, and tweak reaction
    // 0.2, 0, 4.0 - very good - try boosting damping
    // 0.2, 0, 6.0 - turns a bit sharp, but enables fast reaction - try boosting velocity now to 0.5
    // 0.2, 0, 6.0, 0.5 takes car to 55 now, and car does not react quite quickly enough on last set of curves, and is wobbly - try
    // 0.25 0.0 8.0 0.5 good, still toches edge of road on last curves and sways
    // 0.27 0.0 10.0 0.5 too wobbly
    // 0.23 0.1 10.0 0.5 goes in circles!
    // 0.23 0.001 10.0 0.5 wobbly touches edges
    // 0.23 0.005 10.0 0.5 too much, causes sharp wobbles after 4th curve
    // 0.23 0.003 10.0 0.5 too much, causes sharp wobbles after 4th curve
    // 0.26 0.003 10.0 0.5 too much, causes sharp wobbles after 6th curve
    // 0.26 0.002 10.0 0.5 crashes
    // 0.26 0.002 6.0 0.5 crashes
    // 0.26 0.0015 16.0 0.5 around track pretty well on very edge of paint at corner 5/6
    // 0.26 0.0015 20.0 0.5 hits paint at corner 4
    // 0.27 0.0015 20.0 0.5 hits paint at corner 4
    // 0.27 0.00125 20.0 0.5 hits paint at 4 and 5
    // 0.27 0.0011 20.0 0.5 > error 0.9-0.85
    // 0.27 0.0011 20.0 0.6 
    // 0.27 0.008 20.0 0.6 - crash on first corner - swerving violently
    // 0.27 0.0 20.0 0.6 - makes it round, touching paint at 5/6 a bit too much swerve - top speed 59 at corner 1
    // 0.27 0.0 20.0 0.7 - makes it roundto corner 6 nicely, but too much swerving therecrashing  - top speed 65 at corner 2
    // 0.27 0.0 25.0 0.7 - better, generally makes it around, but can crash when weaving pattern started
    // 0.26 0.0 25.0 0.7 - also adjusted max_steerval to 0.9 - nice - but wobbles get out of control
    // 0.26 0.0 25.0 0.7 - also adjusted max_steerval to 0.8 - pretty good - still can crash
    // 0.26 0.0 25.0 0.7 - also adjusted max_steerval to 0.7 - wobbliness accentuated
    // 0.25 0.0 25.0 0.7 - also adjusted max_steerval to 0.7 - crashes on 4- 5 wobbles
    // 0.25 0.0 15.0 0.8 - also adjusted max_steerval to 0.5 - crashes on wobbles
    // 0.25 0.0 10.0 0.8 - also adjusted max_steerval to 0.5 - made it round 1 lap top speed 80mph crashed on wobbles
    // 0.25 0.0 8.0 0.8 - also adjusted max_steerval to 0.5  - super wobble
    // 0.25 0.0 12.0 0.8 - also adjusted max_steerval to 0.5  - super wobble
    // 0.25 0.0 9.0 0.8 - also adjusted max_steerval to 0.5   - super wobble
    // 0.25 0.0 11.0 0.8 - pretty good - big smash on curve 5
    // 0.24 0.0 11.0 0.8 - too wobbly crash curve 1
    // 0.24 0.0 11.0 0.8 adjusted max_steerval to 0.4 - wobbled between 4-5 crash at 5
    // 0.24 0.0 11.0 0.8 adjusted max_steerval to 0.35
    // 0.245 0.0 12.0 0.8 adjusted max_steerval to 0.35
    // 0.245 0.0 10.5 0.8 adjusted max_steerval to 0.35 - smash at curve 2
    // 0.245 0.0 10.5 0.8 adjusted max_steerval to 0.45
    // 0.2475 0.0 10.5 0.8 adjusted max_steerval to 0.45 - wobbled on bridge - crash
    // 0.2425 0.0 10.5 0.8 adjusted max_steerval to 0.45 - almost around track
    // 0.24125 0.0 10.5 0.8 adjusted max_steerval to 0.45 - crash on curve 5
    // 0.24375 0.0 10.5 0.8 adjusted max_steerval to 0.45 - crash on curve 5
    // 0.243 0.0 10.5 0.8 adjusted max_steerval to 0.45 - crash on curve 5
    // 0.242 0.0 10.5 0.8 adjusted max_steerval to 0.45 - crash on curve 5
    // 0.2428 0.0 10.5 0.8 adjusted max_steerval to 0.45 - too wobbly crash on bridge
    // 0.242 0.0 10.5 0.8 adjusted max_steerval to 0.45 
    // 0.242 0.0 10.7 0.8 adjusted max_steerval to 0.45 - slightly better
    // 0.242 0.0 10.8 0.8 adjusted max_steerval to 0.45 - better moderation, but bad result
    // 0.242 0.0 10.8 0.75 adjusted max_steerval to 0.45 - better moderation, but bad result
    // 0.242 0.0 10.75 0.75 adjusted max_steerval to 0.45 - better moderation, but bad result
    // 0.242 0.0 10.9 0.75 adjusted max_steerval to 0.45
    double Taup = 0.242;//was .07
    double Taui = 0.0;
    double Taud = 10.9;//was .7
    steerpid.Init(Taup, Taui, Taud);
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ////  h.onDisconnection([](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
      ////switch (code) {
      ////case CAR_CRASHED_CODE:
      ////  // The car crashed; let the caller know.
      ////  exit(1);
      ////case MAX_RUNTIME_CODE:
      ////   // The simulator ran until our deadline; that's a success.
      ////  exit(EX_OK);
      ////default:
      ////  // If the simulator exits, we seem to get code 1006 or 0.
      ////  std::cerr << "Disconnected: code=" << code << ":" <<
      ////    std::string(message, length) << std::endl;
      ////  exit(EX_UNAVAILABLE);
      ////}
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}

////void reset_simulator(uWS::WebSocket<uWS::SERVER>& ws)
////{
////  // reset
////  std::string msg("42[\"reset\", {}]");
////  ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
////}
