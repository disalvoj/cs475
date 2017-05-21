/*
 * Author: Joshua Kluthe
 * Date: 2017.05.16
 * 
 * Description: 
 * 
 */

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <omp.h>

int	NowYear;		// 2017 - 2022
int	NowMonth;		// 0 - 11

float NowPrecip;		// inches of rain per month
float NowTemp;		// temperature this month
float NowHeight;		// grain height in inches
int	NowNumDeer;		// number of deer in the current population

unsigned int seed;

const float GRAIN_GROWS_PER_MONTH =	8.0;
const float ONE_DEER_EATS_PER_MONTH = 0.5;

const float AVG_PRECIP_PER_MONTH = 6.0;	// average
const float AMP_PRECIP_PER_MONTH = 6.0;	// plus or minus
const float RANDOM_PRECIP =	2.0;	// plus or minus noise

const float AVG_TEMP = 50.0;	// average
const float AMP_TEMP = 20.0;	// plus or minus
const float RANDOM_TEMP = 10.0;	// plus or minus noise

const float MIDTEMP = 40.0;
const float MIDPRECIP =	10.0;

const float PI = 3.14159;

const int INIT_YEAR = 2017;
const int END_YEAR = 2023;

float Ranf(unsigned int *seedp, float low, float high);

int Ranf(unsigned int *seedp, int ilow, int ihigh);

void set_temp(unsigned int *seed);

void set_precip(unsigned int *seed);

void GrainDeer();

void GrainGrowth();

void Watcher();

/*
my agent simulates a river flooding the farmland
much like the Nile river floods, this initially decreases the grain height
but afterwards causes increased growth due to depositing nutrient rich soil.
This effect will be random instead of dependant on the temp or precip, so it is
kind of like the Nile floods in that it depends on conditions far upstream.
*/
void Flood();
float NowFloodEffect;
const float AVG_FLOOD = 50.0;
const float RANDOM_FLOOD = 25.0;

int main(int argc, char *argv[]) {
	
#ifndef _OPENMP
    std::fprintf(stderr, "OpenMP not supported, exiting program.\n");
    return 1;
#endif

	NowMonth = 0;
	NowYear = INIT_YEAR;
	NowNumDeer = 1;
	NowHeight = 1.;
	NowFloodEffect = 0.;
	unsigned int seed = 0;
	set_temp(&seed);
	set_precip(&seed);

	omp_set_num_threads(4);

	#pragma omp parallel sections
	{
		#pragma omp section
		{
			GrainDeer();
		}

		#pragma omp section
		{
			GrainGrowth();
		}

		#pragma omp section
		{
			Flood();
		}

		#pragma omp section
		{
			Watcher();
		}
	}

	return 0;
}

void GrainDeer() {
	
	int localDeer;

	while(NowYear < END_YEAR) {

		localDeer = NowNumDeer;
		if((float) localDeer > NowHeight)
			localDeer--;
		else if((float) localDeer < NowHeight)
			localDeer++;
		//done computing
		#pragma omp barrier

		NowNumDeer = localDeer;
		//done assigning
		#pragma omp barrier

		;
		//done printing
		#pragma omp barrier

	}
	
}

void GrainGrowth() {

	float localHeight;
	float tempFactor;
	float precipFactor;

	while(NowYear < END_YEAR) {

		localHeight = NowHeight;
		tempFactor = std::exp( - (NowTemp - MIDTEMP) * (NowTemp - MIDTEMP) / 10.);
		precipFactor = std::exp( - (NowPrecip - MIDPRECIP) * (NowPrecip - MIDPRECIP) / 10.);
		localHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
		localHeight -= (float) NowNumDeer * ONE_DEER_EATS_PER_MONTH;

		//printf("temp: %f, precip: %f, flood: %f\n", tempFactor, precipFactor, NowFloodEffect);

		//add effects from the Flood() function
		localHeight += (NowFloodEffect / 100.) * GRAIN_GROWS_PER_MONTH;

		if(localHeight < 0)
			localHeight = 0;
		//done computing
		#pragma omp barrier

		NowHeight = localHeight;
		//done assigning
		#pragma omp barrier

		;
		//done printing
		#pragma omp barrier

	}

}

void Flood() {

	float localFloodEffect;
	unsigned int seed = 0;

	while(NowYear < END_YEAR) {

		//flood every year in April
		if(NowMonth == 3) {

			localFloodEffect = - (AVG_FLOOD + Ranf(&seed, - RANDOM_FLOOD, RANDOM_FLOOD));
		
		//Nutrient boost set to - .5 * (April flood effect)
		} else if(NowMonth == 4) {
		
			localFloodEffect = - 0.5 * NowFloodEffect;

		//every other month of the year we decrease the nutrient effect by 10%
		} else {
		
			localFloodEffect = 0.9 * NowFloodEffect;

		}
		
		//done computing
		#pragma omp barrier

		NowFloodEffect = localFloodEffect;
		//done assigning
		#pragma omp barrier

		;
		//done printing
		#pragma omp barrier

	}

}

void Watcher() {

	FILE *data = std::fopen("data.csv", "w+");

	fprintf(data, "Month,Temperature (C),Precipitation (cm),Height (cm), Flood Effect, Deer\n");

	float tempC;
	float heightCM;
	float precipCM;
	int month;
	unsigned int seed = 0;

	while(NowYear < END_YEAR) {

		;
		//done computing
		#pragma omp barrier

		;
		//done assigning
		#pragma omp barrier
		
		//printf("Month: %d, Year: %d, Graindeer: %d, Grain Height: %.2f\n",
		//		NowMonth, NowYear, NowNumDeer, NowHeight);

		month = NowMonth + 12 * (NowYear - INIT_YEAR);
		tempC = (5. / 9.) * (NowTemp - 32);
		heightCM = 2.54 * NowHeight;
		precipCM = 2.54 * NowPrecip;
		
		fprintf(data, "%d,%.2f,%.2f,%.2f,%.2f,%d\n", month, tempC, precipCM, heightCM, NowFloodEffect, NowNumDeer);

		if(++NowMonth > 11)	{
			NowMonth = 0;
			NowYear++;
		}		
		
		set_temp(&seed);
		set_precip(&seed);
		
		//done printing
		#pragma omp barrier

	}

	std::fclose(data);

}

float _ang() {
	return (30. * (float) NowMonth + 15.) * (PI / 180.);
}

void set_temp(unsigned int *seed) {
	float temp = AVG_TEMP - AMP_TEMP * std::cos(_ang());
	NowTemp = temp + Ranf(seed, - RANDOM_TEMP, RANDOM_TEMP);
}

void set_precip(unsigned int *seed) {
	float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * std::sin(_ang());
	NowPrecip = precip + Ranf(seed, - RANDOM_PRECIP, RANDOM_PRECIP);
	if(NowPrecip < 0)
		NowPrecip = 0;
}

float Ranf(unsigned int *seedp, float low, float high) {

	float r = (float) rand_r(seedp);

	return (low + r * (high - low) / (float) RAND_MAX);
}

int Ranf(unsigned int *seedp, int ilow, int ihigh) {

	float low = (float) ilow;
	float high = (float) ihigh + 0.9999f;

	return (int) (Ranf(seedp, low, high));
}

