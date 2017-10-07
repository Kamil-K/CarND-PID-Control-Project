Kamil Kaczmarczyk
06-11-2017

# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

![alt tex](https://github.com/Kamil-K/CarND-PID-Control-Project/blob/master/pics/run60mph.JPG "Successful Run")

## Introduction
This code contains successful implementation of a PID controller for a self-driving vehicle on a race track in a Unity Simulator for a Udacity Self-Driving-Car NanoDegree.

## Goal
Goal of this project was to build and implement a PID controller, tune its hyperparameters and to verify the solution in a simulated environment with an autonomous vehicle going around a track.

## IO

Simulator takes in as input steering angle and throttle commands to drive the car around the track and its outputs are cross-track error (CTE) which is essentially the distance from the ideal path around the track, then there is speed and finally the steering angle.

## Discussion

In a PID controller stands for proportional-integral-derivative controller and as the name suggests it consists of three terms. 

<i>Proportional Term Kp</i><p>
This component has the highest impact on the behaviour of the controller. It is responsible for the proportional steering angle of the wheels of the car to the distance from the center lane or an ideal drive path. In this particular project this distance is called cross-track error or CTE. Example of this is if a car is slightly to the right of the center then it will turn left towards the center position proportionally strong with the distance to this center.

<i>Integral Term Ki</i><p>
This component is responsible for getting rid of the bias in the control. Example of a bias can be in the shape of a drift such as a steering drift.

<i>Derivative Term Kd</i><p>
This differential term is responsible for reducing the overshooting effect caused by the proportional term. Term takes into account the difference of cross-tract-error CTE from the current step and previous one so that the closer the car is to the center line the smaller this coefficient gets. This can be thought of as a damping coefficient. Example of that is when the car is to the right from the center and it starts turning left then in a traditional P or P-I controller it would overshoot it and start oscilating around the middle. in a P-D or P-I-D controller this term supports the damping and faster convergence towards the center line by decreasing the overshoot.

## Parameters Tunning

There are several approaches to PID gains tunning ranging from manual methodologies, Ziegler–Nichols going towards more effective and yet simple ones such as Twiddle where parameters are optimized based on the best error obtained.

In this work manual setting of PID gains is used. It follows few steps:

- Step 1: Select Kp so that the vehicle starts oscilating around the center line. At the same time keep Ki and Kd at 0s.
- Step 2: Keep selected Kp, keep Kd at 0 and start playing with Ki to minimize the drift caused by the steering angle (here the drift was around 0.44 deg)
- Step 3: Keep Kp and Ki preselected before and find Kd so that it dumps the overshoot and yet proposes a high enough reaction to having higher CTE values.
- Step 4: Final finetunning also including the speed target. In this work final speed target of 50 mph was used and the Kp, and Kd parameters needed to be slightly modified to account for better stability and smoother ride.

Final values chosen are:
- Kp = 0.09
- Ki = 0.0001
- Kd = 1.5

## Final Results

Final PID gain values and speed limit of 50 mph give car enough of stability for a smooth ride around the track and yet are fast enough so that if the lap was timed would result in a reasonable time around the track. Screenshot and a video from the ride can be seen below.

YouTube video of the final ride <b>(click on the image or [here](https://img.youtube.com/vi/b4SwUM74Jz8/0.jpg)](https://www.youtube.com/watch?v=b4SwUM74Jz8) to go to YouTube)</b>
[![Part of the track ride at 50 mph](https://img.youtube.com/vi/b4SwUM74Jz8/0.jpg)](https://www.youtube.com/watch?v=b4SwUM74Jz8)

---

# Original README from UDACITY

## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1(mac, linux), 3.81(Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `./install-mac.sh` or `./install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets 
    cd uWebSockets
    git checkout e94b6e1
    ```
    Some function signatures have changed in v0.14.x. See [this PR](https://github.com/udacity/CarND-MPC-Project/pull/3) for more details.
* Simulator. You can download these from the [project intro page](https://github.com/udacity/self-driving-car-sim/releases) in the classroom.

There's an experimental patch for windows in this [PR](https://github.com/udacity/CarND-PID-Control-Project/pull/3)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`. 

Tips for setting up your environment can be found [here](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/0949fca6-b379-42af-a919-ee50aa304e6a/lessons/f758c44c-5e40-4e01-93b5-1a82aa4e044f/concepts/23d376c7-0195-4276-bdf0-e02f1f3c665d)

## Editor Settings

We've purposefully kept editor configuration files out of this repo in order to
keep it as simple and environment agnostic as possible. However, we recommend
using the following settings:

* indent using spaces
* set tab width to 2 spaces (keeps the matrices in source code aligned)

## Code Style

Please (do your best to) stick to [Google's C++ style guide](https://google.github.io/styleguide/cppguide.html).

## Project Instructions and Rubric

Note: regardless of the changes you make, your project must be buildable using
cmake and make!

More information is only accessible by people who are already enrolled in Term 2
of CarND. If you are enrolled, see [the project page](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/f1820894-8322-4bb3-81aa-b26b3c6dcbaf/lessons/e8235395-22dd-4b87-88e0-d108c5e5bbf4/concepts/6a4d8d42-6a04-4aa6-b284-1697c0fd6562)
for instructions and the project rubric.

## Hints!

* You don't have to follow this directory structure, but if you do, your work
  will span all of the .cpp files here. Keep an eye out for TODOs.

## Call for IDE Profiles Pull Requests

Help your fellow students!

We decided to create Makefiles with cmake to keep this project as platform
agnostic as possible. Similarly, we omitted IDE profiles in order to we ensure
that students don't feel pressured to use one IDE or another.

However! I'd love to help people get up and running with their IDEs of choice.
If you've created a profile for an IDE that you think other students would
appreciate, we'd love to have you add the requisite profile files and
instructions to ide_profiles/. For example if you wanted to add a VS Code
profile, you'd add:

* /ide_profiles/vscode/.vscode
* /ide_profiles/vscode/README.md

The README should explain what the profile does, how to take advantage of it,
and how to install it.

Frankly, I've never been involved in a project with multiple IDE profiles
before. I believe the best way to handle this would be to keep them out of the
repo root to avoid clutter. My expectation is that most profiles will include
instructions to copy files to a new location to get picked up by the IDE, but
that's just a guess.

One last note here: regardless of the IDE used, every submitted project must
still be compilable with cmake and make./

## How to write a README
A well written README file can enhance your project and portfolio.  Develop your abilities to create professional README files by completing [this free course](https://www.udacity.com/course/writing-readmes--ud777).

