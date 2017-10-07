#include "PID.h"
#include <iostream>

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd) {
	
	this->Kp = Kp;
	this->Ki = Ki;
	this->Kd = Kd;
	prev_cte = 0;
	int_cte = 0;
	p_error = 0;
	i_error = 0;
	d_error = 0;

}

void PID::UpdateError(double cte) {
	
	p_error = - Kp * cte;
	/*
	std::cout << "Kp value: " << Kp << endl;
	std::cout << "cte value: " << cte << endl;
	std::cout << "p_error value: " << p_error << endl;
	*/
	
	d_error = - Kd * (cte - prev_cte);
	prev_cte = cte;
	
	/*
	std::cout << "Kd value: " << Kd << endl;
	std::cout << "prev_cte value: " << prev_cte << endl;
	std::cout << "d_error value: " << d_error << endl;	
	*/
	
	int_cte += cte;	
	i_error = - Ki * int_cte;
	
	/*
	std::cout << "Ki value: " << Ki << endl;
	std::cout << "int_cte value: " << int_cte << endl;
	std::cout << "i_error value: " << i_error << endl;	
	//steer_value = Kp * cte - Kd * diff_cte - Ki * int_cte;
	*/
}

double PID::TotalError() {
	return p_error + i_error + d_error;
}

