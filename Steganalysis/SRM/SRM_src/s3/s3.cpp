/*
 This function outputs co-occurrences of ALL 3rd-order residuals
 listed in Figure 1 in our journal HUGO paper (version from June 14), 
 including the naming convention.
 List of outputted features:

 1a) spam14h
 1b) spam14v (orthogonal-spam)
 1c) minmax22v
 1d) minmax24
 1e) minmax34v
 1f) minmax41
 1g) minmax34
 1h) minmax48h
 1i) minmax54

 Naming convention:

 name = {type}{f}{sigma}{scan}
 type \in {spam, minmax}
 f \in {1,2,3,4,5} number of filters that are "minmaxed"
 sigma \in {1,2,3,4,8} symmetry index
 scan \in {h,v,\emptyset} scan of the cooc matrix (empty = sum of both 
 h and v scans).
 */

#include "../mat2D.h"
#include "../submodel.h"
#include "../config.cpp"
#include "../s.h"

#include "s3_spam14h.cpp"
#include "s3_spam14v.cpp"
#include "s3_minmax22h.cpp"
#include "s3_minmax22v.cpp"
#include "s3_minmax24.cpp"
#include "s3_minmax34.cpp"
#include "s3_minmax34h.cpp"
#include "s3_minmax34v.cpp"
#include "s3_minmax41.cpp"
#include "s3_minmax48h.cpp"
#include "s3_minmax48v.cpp"
#include "s3_minmax54.cpp"

class s3 : s
{
public:
	void CreateKernels()
	{
		mat2D<int> *temp;
		cutEdgesForParityBy = 2;
		
		// Right Kernel
		temp = new mat2D<int>(5, 5);
		temp->Write(0, 0, 0); temp->Write(0, 1, 0); temp->Write(0, 2, 0); temp->Write(0, 3, 0); temp->Write(0, 4, 0);
		temp->Write(1, 0, 0); temp->Write(1, 1, 0); temp->Write(1, 2, 0); temp->Write(1, 3, 0); temp->Write(1, 4, 0);
		temp->Write(2, 0, 0); temp->Write(2, 1, 1); temp->Write(2, 2,-3); temp->Write(2, 3, 3); temp->Write(2, 4,-1);
		temp->Write(3, 0, 0); temp->Write(3, 1, 0); temp->Write(3, 2, 0); temp->Write(3, 3, 0); temp->Write(3, 4, 0);
		temp->Write(4, 0, 0); temp->Write(4, 1, 0); temp->Write(4, 2, 0); temp->Write(4, 3, 0); temp->Write(4, 4, 0);
		kerR = temp;

		// Left Kernel
		temp = new mat2D<int>(5, 5);
		temp->Write(0, 0, 0); temp->Write(0, 1, 0); temp->Write(0, 2, 0); temp->Write(0, 3, 0); temp->Write(0, 4, 0);
		temp->Write(1, 0, 0); temp->Write(1, 1, 0); temp->Write(1, 2, 0); temp->Write(1, 3, 0); temp->Write(1, 4, 0);
		temp->Write(2, 0,-1); temp->Write(2, 1, 3); temp->Write(2, 2,-3); temp->Write(2, 3, 1); temp->Write(2, 4, 0);
		temp->Write(3, 0, 0); temp->Write(3, 1, 0); temp->Write(3, 2, 0); temp->Write(3, 3, 0); temp->Write(3, 4, 0);
		temp->Write(4, 0, 0); temp->Write(4, 1, 0); temp->Write(4, 2, 0); temp->Write(4, 3, 0); temp->Write(4, 4, 0);
		kerL = temp;

		// Up Kernel
		temp = new mat2D<int>(5, 5);
		temp->Write(0, 0, 0); temp->Write(0, 1, 0); temp->Write(0, 2,-1); temp->Write(0, 3, 0); temp->Write(0, 4, 0);
		temp->Write(1, 0, 0); temp->Write(1, 1, 0); temp->Write(1, 2, 3); temp->Write(1, 3, 0); temp->Write(1, 4, 0);
		temp->Write(2, 0, 0); temp->Write(2, 1, 0); temp->Write(2, 2,-3); temp->Write(2, 3, 0); temp->Write(2, 4, 0);
		temp->Write(3, 0, 0); temp->Write(3, 1, 0); temp->Write(3, 2, 1); temp->Write(3, 3, 0); temp->Write(3, 4, 0);
		temp->Write(4, 0, 0); temp->Write(4, 1, 0); temp->Write(4, 2, 0); temp->Write(4, 3, 0); temp->Write(4, 4, 0);
		kerU = temp;

		// Down Kernel
		temp = new mat2D<int>(5, 5);
		temp->Write(0, 0, 0); temp->Write(0, 1, 0); temp->Write(0, 2, 0); temp->Write(0, 3, 0); temp->Write(0, 4, 0);
		temp->Write(1, 0, 0); temp->Write(1, 1, 0); temp->Write(1, 2, 1); temp->Write(1, 3, 0); temp->Write(1, 4, 0);
		temp->Write(2, 0, 0); temp->Write(2, 1, 0); temp->Write(2, 2,-3); temp->Write(2, 3, 0); temp->Write(2, 4, 0);
		temp->Write(3, 0, 0); temp->Write(3, 1, 0); temp->Write(3, 2, 3); temp->Write(3, 3, 0); temp->Write(3, 4, 0);
		temp->Write(4, 0, 0); temp->Write(4, 1, 0); temp->Write(4, 2,-1); temp->Write(4, 3, 0); temp->Write(4, 4, 0);
		kerD = temp;

		// Right Up Kernel
		temp = new mat2D<int>(5, 5);
		temp->Write(0, 0, 0); temp->Write(0, 1, 0); temp->Write(0, 2, 0); temp->Write(0, 3, 0); temp->Write(0, 4,-1);
		temp->Write(1, 0, 0); temp->Write(1, 1, 0); temp->Write(1, 2, 0); temp->Write(1, 3, 3); temp->Write(1, 4, 0);
		temp->Write(2, 0, 0); temp->Write(2, 1, 0); temp->Write(2, 2,-3); temp->Write(2, 3, 0); temp->Write(2, 4, 0);
		temp->Write(3, 0, 0); temp->Write(3, 1, 1); temp->Write(3, 2, 0); temp->Write(3, 3, 0); temp->Write(3, 4, 0);
		temp->Write(4, 0, 0); temp->Write(4, 1, 0); temp->Write(4, 2, 0); temp->Write(4, 3, 0); temp->Write(4, 4, 0);
		kerRU = temp;

		// Right Down Kernel
		temp = new mat2D<int>(5, 5);
		temp->Write(0, 0, 0); temp->Write(0, 1, 0); temp->Write(0, 2, 0); temp->Write(0, 3, 0); temp->Write(0, 4, 0);
		temp->Write(1, 0, 0); temp->Write(1, 1, 1); temp->Write(1, 2, 0); temp->Write(1, 3, 0); temp->Write(1, 4, 0);
		temp->Write(2, 0, 0); temp->Write(2, 1, 0); temp->Write(2, 2,-3); temp->Write(2, 3, 0); temp->Write(2, 4, 0);
		temp->Write(3, 0, 0); temp->Write(3, 1, 0); temp->Write(3, 2, 0); temp->Write(3, 3, 3); temp->Write(3, 4, 0);
		temp->Write(4, 0, 0); temp->Write(4, 1, 0); temp->Write(4, 2, 0); temp->Write(4, 3, 0); temp->Write(4, 4,-1);
		kerRD = temp;

		// Left Up Kernel
		temp = new mat2D<int>(5, 5);
		temp->Write(0, 0,-1); temp->Write(0, 1, 0); temp->Write(0, 2, 0); temp->Write(0, 3, 0); temp->Write(0, 4, 0);
		temp->Write(1, 0, 0); temp->Write(1, 1, 3); temp->Write(1, 2, 0); temp->Write(1, 3, 0); temp->Write(1, 4, 0);
		temp->Write(2, 0, 0); temp->Write(2, 1, 0); temp->Write(2, 2,-3); temp->Write(2, 3, 0); temp->Write(2, 4, 0);
		temp->Write(3, 0, 0); temp->Write(3, 1, 0); temp->Write(3, 2, 0); temp->Write(3, 3, 1); temp->Write(3, 4, 0);
		temp->Write(4, 0, 0); temp->Write(4, 1, 0); temp->Write(4, 2, 0); temp->Write(4, 3, 0); temp->Write(4, 4, 0);
		kerLU = temp;

		// Left Down Kernel
		temp = new mat2D<int>(5, 5);
		temp->Write(0, 0, 0); temp->Write(0, 1, 0); temp->Write(0, 2, 0); temp->Write(0, 3, 0); temp->Write(0, 4, 0);
		temp->Write(1, 0, 0); temp->Write(1, 1, 0); temp->Write(1, 2, 0); temp->Write(1, 3, 1); temp->Write(1, 4, 0);
		temp->Write(2, 0, 0); temp->Write(2, 1, 0); temp->Write(2, 2,-3); temp->Write(2, 3, 0); temp->Write(2, 4, 0);
		temp->Write(3, 0, 0); temp->Write(3, 1, 3); temp->Write(3, 2, 0); temp->Write(3, 3, 0); temp->Write(3, 4, 0);
		temp->Write(4, 0,-1); temp->Write(4, 1, 0); temp->Write(4, 2, 0); temp->Write(4, 3, 0); temp->Write(4, 4, 0);
		kerLD = temp;
	}

	s3(std::vector<float> qs, Config *config) : s(qs, config)
	{
		this->CreateKernels();
		quantMultiplier = 3;

		for (int qIndex=0; qIndex < (int)qs.size(); qIndex++)
		{
			float q = qs[qIndex];
			std::vector<Submodel *> submodelsForQ;

			submodelsForQ.push_back(new s3_spam14h(q, config));
			submodelsForQ.push_back(new s3_spam14v(q, config));
			submodelsForQ.push_back(new s3_minmax22h(q, config));
			submodelsForQ.push_back(new s3_minmax22v(q, config));
			submodelsForQ.push_back(new s3_minmax24(q, config));
			submodelsForQ.push_back(new s3_minmax34(q, config));
			submodelsForQ.push_back(new s3_minmax34h(q, config));
			submodelsForQ.push_back(new s3_minmax34v(q, config));
			submodelsForQ.push_back(new s3_minmax41(q, config));
			submodelsForQ.push_back(new s3_minmax48h(q, config));
			submodelsForQ.push_back(new s3_minmax48v(q, config));
			submodelsForQ.push_back(new s3_minmax54(q, config));

			this->submodels.push_back(submodelsForQ);
		}
	}

	~s3()
	{
		delete kerR; delete kerL; delete kerU; delete kerD;
		delete kerRU; delete kerRD; delete kerLU; delete kerLD;
	}

	void ComputeImage(mat2D<int> *img, mat2D<int> *parity)
	{
		mat2D<int> *R = GetResidual(img, kerR);
		mat2D<int> *L = GetResidual(img, kerL);
		mat2D<int> *U = GetResidual(img, kerU);
		mat2D<int> *D = GetResidual(img, kerD);
		mat2D<int> *RU = GetResidual(img, kerRU);
		mat2D<int> *RD = GetResidual(img, kerRD);
		mat2D<int> *LU = GetResidual(img, kerLU);
		mat2D<int> *LD = GetResidual(img, kerLD);

		for (int qIndex=0; qIndex < (int)submodels.size(); qIndex++)
		{
			float q = qs[qIndex] * quantMultiplier;
			std::vector<mat2D<int> *> QResVect;
			QResVect.push_back(Quantize(R, q));
			QResVect.push_back(Quantize(L, q));
			QResVect.push_back(Quantize(U, q));
			QResVect.push_back(Quantize(D, q));
			QResVect.push_back(Quantize(RU, q));
			QResVect.push_back(Quantize(RD, q));
			QResVect.push_back(Quantize(LU, q));
			QResVect.push_back(Quantize(LD, q));

			// If parity is turned on
			if (config->parity) MultiplyByParity(QResVect, parity);

			for (int submodelIndex=0; submodelIndex < (int)submodels[qIndex].size(); submodelIndex++)
			{
				// Compute the features for current submodel
				submodels[qIndex][submodelIndex]->ComputeFea(QResVect);
			}

			for (int i=0; i<(int)QResVect.size(); i++) delete QResVect[i];
		}
		delete R; delete L;delete U; delete D; 
		delete RU; delete RD; delete LU; delete LD;
	}

private:
	mat2D<int> *kerR;
	mat2D<int> *kerL;
	mat2D<int> *kerU;
	mat2D<int> *kerD;
	mat2D<int> *kerRU;
	mat2D<int> *kerRD;
	mat2D<int> *kerLU;
	mat2D<int> *kerLD;
};