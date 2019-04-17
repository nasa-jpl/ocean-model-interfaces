#ifndef MODEL_DATA_H
#define MODEL_DATA_H

struct ModelData
{
	//eastward current velocity
	double u;

	//northward current velocity
	double v;

	//upward current velocity
	double w;
	
	double temp;
	double salt;
	double dye;
    double depth;
};

#endif