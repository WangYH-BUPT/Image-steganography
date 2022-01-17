#ifndef COST_MODEL_H_
#define COST_MODEL_H_

#include "mat2D.h"
#include "cost_model_config.h"
#include "base_cost_model.h"
#include <vector>
#include "jstruct.h"

class cost_model : public base_cost_model 
{
public:
	cost_model(jstruct * cover, cost_model_config* config);
	~cost_model();

private:
	cost_model_config* config;

	void calc_costs(int r, int c, mat2D<int>* cover_padded, float* pixel_costs);
	void eval_direction(int i, int j, int dir_i, int dir_j, mat2D<int>* cover_padded, float *pixel_costs);
	float eval_cost(int k, int l, int m);
};
#endif