/* Add boost libraries - Depend on Windows (32/64 bit) and run-time (debug/release) */
#if _WIN32 || _WIN64
	#if _WIN64
		#ifdef NDEBUG
	  		#pragma comment( lib, "lib/x64/Release/libboost_filesystem-vc100-mt-1_47.lib" )
			#pragma comment( lib, "lib/x64/Release/libboost_program_options-vc100-mt-1_47.lib" )
			#pragma comment( lib, "lib/x64/Release/libboost_system-vc100-mt-1_47.lib" )
		#else
	 		#pragma comment( lib, "lib/x64/Debug/libboost_filesystem-vc100-mt-gd-1_47.lib" )
			#pragma comment( lib, "lib/x64/Debug/libboost_program_options-vc100-mt-gd-1_47.lib" )
			#pragma comment( lib, "lib/x64/Debug/libboost_system-vc100-mt-gd-1_47.lib" )
		#endif
	#else
		#ifdef NDEBUG
	  		#pragma comment( lib, "lib/win32/Release/libboost_filesystem-vc100-mt-1_47.lib" )
			#pragma comment( lib, "lib/win32/Release/libboost_program_options-vc100-mt-1_47.lib" )
			#pragma comment( lib, "lib/win32/Release/libboost_system-vc100-mt-1_47.lib" )
		#else
	 		#pragma comment( lib, "lib/win32/Debug/libboost_filesystem-vc100-mt-gd-1_47.lib" )
			#pragma comment( lib, "lib/win32/Debug/libboost_program_options-vc100-mt-gd-1_47.lib" )
			#pragma comment( lib, "lib/win32/Debug/libboost_system-vc100-mt-gd-1_47.lib" )
		#endif
	#endif
#endif 

#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include "exception.cpp"
#include <time.h>
#include <vector>
#include "SRMclass.h"
#include "submodel.h"
#include "config.cpp"

typedef unsigned int uint;
namespace fs = boost::filesystem;
namespace po = boost::program_options;

void printInfo(){
    std::cout << "Extracts SRM features from all greyscale pgm images in the directory input-dir.\n";
	std::cout << "Author: Vojtech Holub.\n";
	std::cout << "For further details read:\n";
	std::cout << "   Rich Models for Steganalysis of Digital Images, J. Fridrich and J. Kodovsky,\n   IEEE Transactions on Information Forensics and Security, 2012\n";
	std::cout << "   http://dde.binghamton.edu/kodovsky/pdf/TIFS2012-SRM.pdf \n\n";
	std::cout << "usage: SRM -I input-dir -o output-file [-v] [--T threshold] [--order markov-order] [--order markov-order] [--merge-spams] [-ss] [-sr] [-sm] [--eraseLSB] [--parity] \n\n";
}

void WriteFeaToFiles(SRMclass *SRMobj, std::string oDir, bool verbose)
{
	if (verbose) std::cout << std::endl << "---------------------" << std::endl << "Writing features to the output directory" << std::endl;
	std::vector<Submodel *> submodels = SRMobj->GetSubmodels();
	for (int submodelIndex=0; submodelIndex < (int)submodels.size(); submodelIndex++)
	{
		Submodel *currentSubmodel = submodels[submodelIndex];
		std::string submodelName = currentSubmodel->GetName();
		if (verbose) std::cout << "   " << submodelName << std::endl;

		fs::path dir (oDir);
		fs::path file (submodelName);
		std::string full_path = ((dir / file).string()+ ".fea");

		if (fs::exists(full_path)) fs::remove(full_path);

		std::ofstream outputFile;
		outputFile.open(full_path.c_str());
		for (int imageIndex=0; imageIndex < (int)currentSubmodel->ReturnFea().size(); imageIndex++)
		{
			float *currentLine = (currentSubmodel->ReturnFea())[imageIndex];
			for (int feaIndex=0; feaIndex < currentSubmodel->symmDim; feaIndex++)
			{
				outputFile << currentLine[feaIndex] << " ";
			}
			outputFile << SRMobj->imageNames[imageIndex] << std::endl;
		}
		outputFile.close();
	}
}

int main(int argc, char** argv)
{
	try { 
		int T, order;
		std::string iDir, oDir;
		std::vector< std::string > images;
		bool verbose = false;
		bool symmSign = false, symmReverse = false, symmMinMax = false; bool mergeSpams = false;
		bool eraseLSB = false, parity = false;
		po::variables_map vm;

        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
			("verbose,V",       po::bool_switch(&verbose),                     "print out verbose messages")
            ("input-dir,I",     po::value<std::string>(&iDir),                 "directory with images to calculate the features from")
            ("images,i",        po::value<std::vector<std::string> >(&images),  "list of images to calculate the features from")
			("output-dir,O",    po::value<std::string>(&oDir),                 "dir to output features from all submodels")
            ("T",               po::value<int>(&T)->default_value(2),          "upper bound on absolute value of the residuals")
            ("order",           po::value<int>(&order)->default_value(4),      "order of the co-occurrence matrix")
			("merge-spams",		po::bool_switch(&mergeSpams),                 "switch off merging SPAM features")
			("ss",				po::bool_switch(&symmSign),                    "switch off sign symmetry")
			("sr",				po::bool_switch(&symmReverse),                 "switch off reverse symmetry")
			("sm",				po::bool_switch(&symmMinMax),                  "switch off minmax symmetry")
			("eraseLSB",		po::bool_switch(&eraseLSB),                  "switch on erasing image LSBs before processing")
			("parity",			po::bool_switch(&parity),                  "switch on perity residual")
            ;

        po::positional_options_description p;
        p.add("cover-images", -1);

        po::store(po::command_line_parser(argc,argv).options(desc).positional(p).run(), vm);
        po::notify(vm);

		symmSign = !symmSign;
		symmReverse = !symmReverse;
		symmMinMax = !symmMinMax;
		mergeSpams = !mergeSpams;

        if (vm.count("help"))  { printInfo(); std::cout << desc << "\n"; return 1; }
        if (!vm.count("output-dir")){ std::cout << "'output-dir' is required.\n" << desc << "\n"; return 1; }
		else if (!fs::is_directory(fs::path(oDir))) { std::cout << "'output-dir' must be an existing directory.\n" << desc << "\n"; return 1; }

		// add all pgm and files from the input directory to the vector
		fs::directory_iterator end_itr; // default construction yields past-the-end
		if (vm.count("input-dir"))
			for ( fs::directory_iterator itr(iDir); itr!=end_itr; ++itr ) 
			{
				if ( (!fs::is_directory(itr->status())) && (itr->path().extension()==".pgm") )
					images.push_back(itr->path().string());
            }

		// create config object
		Config *config = new Config(verbose, T, order, symmSign, symmReverse, symmMinMax, mergeSpams, eraseLSB, parity);
		// create object with all the submodels and compute the features
		SRMclass *SRMobj = new SRMclass(config);

		// Run the feature computation
		SRMobj->ComputeFeatures(images);

		// writes features from all the submodels to the separate files
		WriteFeaToFiles(SRMobj, oDir, verbose);

		// Remove SRMobj from the memory
		delete SRMobj;
    } 
	catch(std::exception& e) 
	{ 
		std::cerr << "error: " << e.what() << "\n"; return 1; 
	} 
	catch(...) 
	{ 
		std::cerr << "Exception of unknown type!\n"; 
	}		
}
