#include <vector>
#include "submodel.h"
#include "SRMclass.h"

namespace mat {
	#include <mex.h>
}

/*
	prhs[0] - cell array of image paths
	prhs[1] - struct config
				config.T			- int32		- default 2		- residual threshold
				config.order		- int32		- default 4		- co-occurrence order
				config.merge_spams	- logical	- default true	- if true then spam features are merged
				config.symm_sign	- logical	- default true	- if true then spam symmetry is used
				config.symm_reverse	- logical	- default true	- if true then reverse symmetry is used
				config.symm_minmax	- logical	- default true	- if true then minmax symmetry is used
				config.eraseLSB		- logical	- default false	- if true then all LSB are erased from the image
				config.parity		- logical	- default false	- if true then parity residual is applied
*/
void mexFunction(int nlhs, mat::mxArray *plhs[], int nrhs, const mat::mxArray *prhs[]) 
{
	const mat::mxArray *imageSet = prhs[0];
	const mat::mxArray *configStruct = prhs[1];

	// Default config
	int T = 2;
	int order = 4;
	bool mergeSpams = true;
	bool ss = true, sr = true, sm = true;
	bool eraseLSB = false, parity = false;

	if ((nrhs != 1) && (nrhs != 2))
	{
		mat::mexErrMsgTxt ("One or two inputs are required.\n1 input - [cell_array Paths]\n2 inputs - [cell_array Paths] [struct config]");
	}
	if (!mxIsCell(imageSet))
		mat::mexErrMsgTxt ("The first input must be a cell array with image paths.");
	if (nrhs == 2)
	{
		int nfields = mat::mxGetNumberOfFields(configStruct);
		if (nfields==0) mat::mexErrMsgTxt ("The config structure is empty.");
		for(int fieldIndex=0; fieldIndex<nfields; fieldIndex++)
		{
			const char *fieldName = mat::mxGetFieldNameByNumber(configStruct, fieldIndex);
			const mat::mxArray *fieldContent = mat::mxGetFieldByNumber(configStruct, 0, fieldIndex);
			// if a field is not scalar
			if ((mat::mxGetM(fieldContent)!= 1) || (mat::mxGetN(fieldContent)!= 1))
				mat::mexErrMsgTxt ("All config fields must be scalars.");
			// if every field is scalar
			if (strcmp(fieldName, "T") == 0)
				if (mat::mxIsClass(fieldContent, "int32")) T = (int)mat::mxGetScalar(fieldContent);
				else mat::mexErrMsgTxt ("'config.T' must be of type 'int32'");
			if (strcmp(fieldName, "order") == 0)
				if (mat::mxIsClass(fieldContent, "int32")) order = (int)mat::mxGetScalar(fieldContent);
				else mat::mexErrMsgTxt ("'config.order' must be of type 'int32'");
			if (strcmp(fieldName, "merge_spams") == 0)
				if (mat::mxIsLogical(fieldContent)) mergeSpams = mxIsLogicalScalarTrue(fieldContent);
				else mat::mexErrMsgTxt ("'config.mergeSpams' must be of type 'logical'");
			if (strcmp(fieldName, "symm_sign") == 0)
				if (mat::mxIsLogical(fieldContent)) ss = mxIsLogicalScalarTrue(fieldContent);
				else mat::mexErrMsgTxt ("'config.symm_sign' must be of type 'logical'");
			if (strcmp(fieldName, "symm_reverse") == 0)
				if (mat::mxIsLogical(fieldContent)) sr = mxIsLogicalScalarTrue(fieldContent);
				else mat::mexErrMsgTxt ("'config.symm_reverse' must be of type 'logical'");
			if (strcmp(fieldName, "symm_minmax") == 0)
				if (mat::mxIsLogical(fieldContent)) sm = mxIsLogicalScalarTrue(fieldContent);
				else mat::mexErrMsgTxt ("'config.symm_minmax' must be of type 'logical'");
			if (strcmp(fieldName, "eraseLSB") == 0)
				if (mat::mxIsLogical(fieldContent)) eraseLSB = mxIsLogicalScalarTrue(fieldContent);
				else mat::mexErrMsgTxt ("'config.eraseLSB' must be of type 'logical'");
			if (strcmp(fieldName, "parity") == 0)
				if (mat::mxIsLogical(fieldContent)) parity = mxIsLogicalScalarTrue(fieldContent);
				else mat::mexErrMsgTxt ("'config.parity' must be of type 'logical'");
		}
	}

	int imageCount = (int)mat::mxGetNumberOfElements(imageSet);
	std::vector<std::string> imageNameVector;
	for (int imageIndex = 0; imageIndex < imageCount; imageIndex ++)
	{
		char *buf;
		int   buflen;
		int   status;
		mat::mxArray *mxImagePath = mat::mxGetCell(imageSet, imageIndex);
		buflen = (int)(mat::mxGetM(mxImagePath) * mat::mxGetN(mxImagePath)) + 1;
		buf = (char*)mat::mxCalloc(buflen, sizeof(char));
		status = mat::mxGetString(mxImagePath, buf, buflen);
		if (status != 0)
   			mat::mexErrMsgTxt("Could not convert input to a string.");
		imageNameVector.push_back(buf);
	}

	// create config object
	Config *config = new Config(false, T, order, ss, sr, sm, mergeSpams, eraseLSB, parity);

	// create object with all the submodels and compute the features
	SRMclass *SRMobj = new SRMclass(config);

	// Run the feature computation
	SRMobj->ComputeFeatures(imageNameVector);

	std::vector<Submodel *> submodels = SRMobj->GetSubmodels();
	const char **submodelNames = new const char*[submodels.size()];
	for (int submodelIndex=0; submodelIndex < (int)submodels.size(); submodelIndex++) 
	{
		submodelNames[submodelIndex] = (new std::string(submodels[submodelIndex]->GetName()))->c_str();
	}
	mat::mwSize structSize[2];
	structSize[0] = 1;
	structSize[1] = 1;
	plhs[0] = mat::mxCreateStructArray(1, structSize, submodels.size(), submodelNames);
	for (int submodelIndex=0; submodelIndex < submodels.size(); submodelIndex++)
	{
		Submodel *currentSubmodel = submodels[submodelIndex];
		mat::mwSize feaSize[2];
		feaSize[0] = (int)currentSubmodel->ReturnFea().size();
		feaSize[1] = currentSubmodel->symmDim;
		mat::mxArray *fea = mat::mxCreateNumericArray(2, feaSize, mat::mxSINGLE_CLASS, mat::mxREAL);
		for (int r=0; r<(int)currentSubmodel->ReturnFea().size(); r++)
		{
			for (int c=0; c<currentSubmodel->symmDim; c++)
			{
				((float*)mat::mxGetPr(fea))[(c*(int)currentSubmodel->ReturnFea().size())+r]=(currentSubmodel->ReturnFea())[r][c];
			}
		}
		mat::mxSetFieldByNumber(plhs[0],0,submodelIndex,fea);
	}

	delete SRMobj;
} 