#ifndef SRMCLASS_H_
#define SRMCLASS_H_

#include "submodel.h"
#include "image.h"
#include "mat2D.h"
#include <vector>
#include "s.h"
#include "config.cpp"

class SRMclass
{
public:
	std::vector<s *> submodelClasses;
	std::vector<std::string> imageNames;

	SRMclass(Config *config);
	~SRMclass();
	void ComputeFeatures(void);

	void ComputeFeatures(std::vector<std::string> imagePaths);
	std::vector<Submodel *> GetSubmodels();

private:
	bool verbose;
	Config *config;
	std::vector<Submodel *> AddedMergesSpams;

	mat2D<int> *LoadImage(std::string path);
	mat2D<int> *Image2mat2D(image *img);
	std::vector<Submodel *> PostProcessing(std::vector<Submodel *> submodels);
	mat2D<int> *GetParity(mat2D<int> *I);
};

#endif