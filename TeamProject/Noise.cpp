#include "noise.h"
#include "perlin.h"

////////PERLIN NOISE
/*	AUTHORS
*	Group: 4
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*	Antoine Pontallier - 98316
*	Andr? Santos - 91000
*/
double PerlinNoise::noise2D(double x, double y, double alpha, double beta, int n)
{
	int i;
	double sum = 0;
	double p[2], scale = 1;

	p[0] = x;
	p[1] = y;
	for (i = 0; i < n; i++) {
		double val = Perlin::noise2(p);
		sum += val / scale;
		scale *= alpha;
		p[0] *= beta;
		p[1] *= beta;
	}
	return(sum);
}

double PerlinNoise::noise3D(double x, double y, double z, double alpha, double beta, int n)
{
	int i;
	double sum = 0;
	double p[3], scale = 1;

	p[0] = x;
	p[1] = y;
	p[2] = z;
	for (i = 0; i < n; i++) {
		double val = Perlin::noise3(p);
		sum += val / scale;
		scale *= alpha;
		p[0] *= beta;
		p[1] *= beta;
		p[2] *= beta;
	}
	return(sum);
}