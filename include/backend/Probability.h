#ifndef PROBABILITY_H
#define PROBABILITY_H

#include "AnalysisRobot.h"
#include "RescueRobot.h"
#include "ResearchRobot.h"

class Probability{
	public:
		int takeDamage(Vehicle* v, float danger);			// invoking damage on a analysis robot
		int takeDamage(RescueRobot& r, float danger);		// invoking damage on a rescue robot
		int takeDamage(ResearchRobot& s, float danger);		// invoking damage on a research robot
		float calc_probability(float ability, float danger);// calculating the probability
	private:
		int calc_damage(float probability);	// calculating the damage

};


#endif