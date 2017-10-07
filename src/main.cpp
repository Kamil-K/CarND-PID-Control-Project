#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>

using namespace std;

// for convenience
using json = nlohmann::json;

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

int main()
{
  uWS::Hub h;

  PID pid;
  // TODO: Initialize the pid variable.
  
  //manual method for selecting gains, see article below:
  //http://en.wikipedia.org/wiki/PID_controller#Manual_tuning
  
  //Step 1:
  //Kp set to a value so that the car starts oscilating
  //Ki set to 0
  //Kd set to 0
  //pid.Init(0.05, 0, 0);
  
  //Step 2:
  //Kp set to a value so that the car starts oscilating - from previous step
  //Ki set to a value so that it can reduce the offset of 0.44 deg in the steering angle
  //Kd set to 0
  //pid.Init(0.05, 0.0001, 0);	//Ki = 0.001 is set to a small value as the offset seemed negligible
  
  //Step 3:
  //Kp set to a value so that the car starts oscilating - from previous step
  //Ki set to a value so that it can reduce the offset of 0.44 deg in the steering angle - from previous step
  //Kd set to increase until the loop is acceptably quick to reach its reference after a load disturbance
  //pid.Init(0.05, 0.0001, 0.1);
  //pid.Init(0.05, 0.0001, 0.5);
  //pid.Init(0.05, 0.0001, 1);
  //pid.Init(0.05, 0.0001, 1.5);	//gives reasonable behaviour but the proportional response Kp seems too small!
  
  //Step4: fine tunning of parameters
  //pid.Init(0.1, 0.0001, 1.5);		//track completed for the first time at 20 mph
  //pid.Init(0.15, 0.0001, 1.5);	//track completed at 30 mph
  //pid.Init(0.15, 0.0001, 3);		//track completed at 30 mph and a bit faster
  //pid.Init(0.15, 0.0001, 2);		//track completed at 50 mph, Kd slightly lowered due to overshoot at high speeds
  //pid.Init(0.15, 0.0001, 2);		//track NOT completed at 100 mph - car out of the track!
  //pid.Init(0.085, 0.0001, 1.3);	//track completed at 70 mph, but feels unstable - parameters to be tuned further or speed to be reduced
  pid.Init(0.09, 0.0001, 1.5);		//reduced speed to 50 mph
  
  h.onMessage([&pid](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
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
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<std::string>());
          double speed = std::stod(j[1]["speed"].get<std::string>());
          //double angle = std::stod(j[1]["steering_angle"].get<std::string>());
          double steer_value;
		  double thr;
		  double target_speed = 50;	//70, 60, 50 and 30 set previously
          /*
          * TODO: Calcuate steering value here, remember the steering value is
          * [-1, 1].
          * NOTE: Feel free to play around with the throttle and speed. Maybe use
          * another PID controller to control the speed!
          */
		  
		  pid.UpdateError(cte);		  
		  steer_value = pid.TotalError();
		  
		  if(steer_value > 1.0) {
        	  steer_value = 1;
          }
		  else if(steer_value < -1.0) {
        	  steer_value = -1;
          }

		  if (speed > target_speed) {
			  thr = 0;
		  }
		  else {
			  thr = 1.0;
		  }

          // DEBUG
		  std::cout << "Speed is: " << speed << endl;
          std::cout << "CTE: " << cte << " Steering Value: " << steer_value << std::endl;

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          //msgJson["throttle"] = 0.3;
		  msgJson["throttle"] = thr;
		  auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }
      } else {
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

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
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
