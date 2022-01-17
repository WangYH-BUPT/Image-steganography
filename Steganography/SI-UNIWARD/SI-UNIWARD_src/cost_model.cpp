#include "base_cost_model.h"
#include "cost_model.h"
#include "mat2D.h"
#include "cost_model_config.h"
#include "base_cost_model_config.h"
#include <math.h>
#include "jstruct.h"
#include <sstream>

cost_model::cost_model(jstruct * coverStruct, cost_model_config* config) : base_cost_model(coverStruct, (base_cost_model_config *)config)
{
	this->config = config;
	double wetCost = (double)10000000000000;
	mat2D<int> * spatialCover = coverStruct->spatial_arrays[0];

	// compute rounding errors
	mat2D<double> * e = new mat2D<double>(coverStruct->image_height, coverStruct->image_width);
	for (int r=0; r<(int)coverStruct->image_height; r++)
		for (int c=0; c<(int)coverStruct->image_width; c++)
			e->Write(r, c, ((double)coverStruct->coef_arrays[0]->Read(r,c)) - coverStruct->unrounded_coef_arrays->Read(r,c));

	// adjust wavelet impact to the quality factor
	mat2D<mat2D<double> *> * LHwaveletImpact = new mat2D<mat2D<double> *>(8, 8);
	mat2D<mat2D<double> *> * HLwaveletImpact = new mat2D<mat2D<double> *>(8, 8);
	mat2D<mat2D<double> *> * HHwaveletImpact = new mat2D<mat2D<double> *>(8, 8);
	for (int row=0; row<8; row++)
		for (int col=0; col<8; col++)
		{
			int quant = coverStruct->quant_tables[0]->Read(row, col);
			LHwaveletImpact->Write(row, col, mat2D<double>::ChangeToAbsValue(mat2D<double>::MultiplyByNumber(config->LHwaveletImpact->Read(row, col), quant)));
			HLwaveletImpact->Write(row, col, mat2D<double>::ChangeToAbsValue(mat2D<double>::MultiplyByNumber(config->HLwaveletImpact->Read(row, col), quant)));
			HHwaveletImpact->Write(row, col, mat2D<double>::ChangeToAbsValue(mat2D<double>::MultiplyByNumber(config->HHwaveletImpact->Read(row, col), quant)));
		}

	// Create padded image
	mat2D<int> * spatialCover_padded_int = mat2D<int>::Padding_Mirror(spatialCover, config->padsize, config->padsize);
	mat2D<double>* spatialCover_padded_double = mat2D<double>::Retype_int2double(spatialCover_padded_int);
	delete spatialCover_padded_int;

	// Compute residuals - wavelet sub-bands
	mat2D<double> * R_LH = mat2D<double>::ChangeToAbsValue(mat2D<double>::Correlation_Same_basicFilters(spatialCover_padded_double, config->Tlpdf, config->hpdf));
	mat2D<double> * R_HL = mat2D<double>::ChangeToAbsValue(mat2D<double>::Correlation_Same_basicFilters(spatialCover_padded_double, config->Thpdf, config->lpdf));
	mat2D<double> * R_HH = mat2D<double>::ChangeToAbsValue(mat2D<double>::Correlation_Same_basicFilters(spatialCover_padded_double, config->Thpdf, config->hpdf));
	delete spatialCover_padded_double;

	for (int row=0; row < (int)coverStruct->image_height; row++)
		for (int col=0; col < (int)coverStruct->image_width; col++)
		{
			int modRow = row%8;
			int modCol = col%8;
			
			int subRowsFrom = row-modRow-6+config->padsize;
			int subColsFrom = col-modCol-6+config->padsize;

			double e_val = e->Read(row, col);
			double rho = 0;
			// 04 coeffs
			if ((modRow % 4 == 0) && (modCol % 4 == 0) && (fabs(e_val) > 0.4999)) rho = wetCost;
			else
			{
				for (int r_sub=0; r_sub<7+config->padsize; r_sub++)
					for (int c_sub=0; c_sub<7+config->padsize; c_sub++)
					{
						rho += LHwaveletImpact->Read(modRow, modCol)->Read(r_sub, c_sub) / (R_LH->Read(subRowsFrom+r_sub, subColsFrom+c_sub) + config->sigma);
						rho += HLwaveletImpact->Read(modRow, modCol)->Read(r_sub, c_sub) / (R_HL->Read(subRowsFrom+r_sub, subColsFrom+c_sub) + config->sigma);
						rho += HHwaveletImpact->Read(modRow, modCol)->Read(r_sub, c_sub) / (R_HH->Read(subRowsFrom+r_sub, subColsFrom+c_sub) + config->sigma);
					}
				rho *= 1-(2*fabs(e_val));
				rho += 0.0001;
				if (rho > wetCost) rho = wetCost;
			}
			// pixel_costs[0] is the cost of -1, pixel_costs[0] is the cost of no change, pixel_costs[0] is the cost of +1
			float* pixel_costs = costs + ((col+row*cover->cols)*3);

			int cover_val = cover->Read(row, col);

			if (cover_val <= -1023) pixel_costs[0] = (float)wetCost;
			else if (e_val>=0) pixel_costs[0] = (float)rho;
			else pixel_costs[0] = wetCost;

			pixel_costs[1] = 0;

			if (cover_val >= 1023) pixel_costs[2] = (float)wetCost;
			else if (e_val<=0) pixel_costs[2] = (float)rho;
			else pixel_costs[2] = wetCost;
		}
	delete R_LH; delete R_HL; delete R_HH;	delete e;
}

cost_model::~cost_model()
{
}