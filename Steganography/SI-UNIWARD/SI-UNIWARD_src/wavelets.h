#ifndef WAVELETS_H_
#define WAVELETS_H_

#include "mat2D.h"
#include <stdlib.h>

enum waveletEnum {daubechies8};

class Wavelets
{
public:
	static std::pair<mat2D<double> *, mat2D<double> *> GetWavelets(waveletEnum waveletType)
	{
		mat2D<double> * lpdf;
		mat2D<double> * hpdf;
		if (waveletType==daubechies8)
		{
			lpdf = new mat2D<double>(1, 16);
			lpdf->Write(0, 0, -0.00011747678400228192);
			lpdf->Write(0, 1,  0.00067544940599855677);
			lpdf->Write(0, 2, -0.00039174037299597711);
			lpdf->Write(0, 3, -0.0048703529930106603);
			lpdf->Write(0, 4,  0.0087460940470156547);
			lpdf->Write(0, 5,  0.013981027917015516);
			lpdf->Write(0, 6, -0.044088253931064719);
			lpdf->Write(0, 7, -0.017369301002022108);
			lpdf->Write(0, 8,  0.12874742662018601);
			lpdf->Write(0, 9,  0.00047248457399797254);
			lpdf->Write(0,10, -0.28401554296242809);
			lpdf->Write(0,11, -0.015829105256023893);
			lpdf->Write(0,12,  0.58535468365486909);
			lpdf->Write(0,13,  0.67563073629801285);
			lpdf->Write(0,14,  0.31287159091446592);
			lpdf->Write(0,15,  0.054415842243081609);

			hpdf = new mat2D<double>(1, 16);
			hpdf->Write(0, 0, -0.054415842243081609);
			hpdf->Write(0, 1,  0.31287159091446592);
			hpdf->Write(0, 2, -0.67563073629801285);
			hpdf->Write(0, 3,  0.58535468365486909);
			hpdf->Write(0, 4,  0.015829105256023893);
			hpdf->Write(0, 5, -0.28401554296242809);
			hpdf->Write(0, 6, -0.00047248457399797254);
			hpdf->Write(0, 7,  0.12874742662018601);
			hpdf->Write(0, 8,  0.017369301002022108);
			hpdf->Write(0, 9, -0.044088253931064719);
			hpdf->Write(0,10, -0.013981027917015516);
			hpdf->Write(0,11,  0.0087460940470156547);
			hpdf->Write(0,12,  0.0048703529930106603);
			hpdf->Write(0,13, -0.00039174037299597711);
			hpdf->Write(0,14, -0.00067544940599855677);
			hpdf->Write(0,15, -0.00011747678400228192);
		}
		return std::make_pair(lpdf, hpdf);
	}
};

#endif