#include "backend/Probability.h"
#include "backend/Random.h"

#include <cmath>

float Probability::calc_probability(float ability, float danger){
	// calculating the probability of the robot getting damaged
	float p = danger*(1-ability);
	p = truncf(p*10.0)/10.0;

	return p;
}

/*- -------------------------------------------------------------- -*/
int Probability::calc_damage(float probability){
	bool prob[10];		// boolean array representing whether the robot will take damage
						// depending on the calculated probability

	float count = 0.0;	// counter for the probability frequency

	probability *= 10;

	// making an array representing the probability
	for (int i=0; i<9; i++){
		if (count<=probability)
			prob[i] = true;
		else
			prob[i] = false;

		count++;	// increasing the counter
	}

	Random r; // making an instance for the class random

	int random_num = r.random_int(9);

	if (prob[random_num])
		return 1;		// returning the damage tha should be taken by the robot
	else
		return 0;		// returning zero damaga to be taken by the robot

}

/*- -------------------------------------------------------------- -*/
int Probability::takeDamage(Vehicle* v, float danger){
	float p = calc_probability(v->get_ability(), danger);	// calculating the probability
	int damage = calc_damage(p);			// calculating the damage

	v->set_status(v->get_status()-damage);	// invoking the damage on the robot

	v->total_damages += damage;				// increasing the robot's damage counter

	return damage;
}

/*- -------------------------------------------------------------- -*/
