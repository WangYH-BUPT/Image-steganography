#ifndef CONFIG_H_
#define CONFIG_H_

#include "base_cost_model_config.h"
#include <vector>
#include "mat2D.h"

class cost_model_config : public base_cost_model_config
{
public:
	mat2D<double> * lpdf;
	mat2D<double> * hpdf;
	mat2D<double> * Tlpdf;
	mat2D<double> * Thpdf;
	mat2D<mat2D<double> *> * LHwaveletImpact;
	mat2D<mat2D<double> *> * HLwaveletImpact;
	mat2D<mat2D<double> *> * HHwaveletImpact;
	int padsize;
	double sigma;

	cost_model_config(float payload, bool verbose, int wavelet, unsigned int stc_constr_height, int randSeed);
	~cost_model_config();

private:
	void set_filters(int wavelet);
	void GetWaveletImpacts();
	double alpha(int coord);
};
#endif