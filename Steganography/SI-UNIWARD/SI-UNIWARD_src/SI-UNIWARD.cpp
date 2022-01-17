#include <stdio.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>
#include <iomanip>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/mersenne_twister.hpp>

#include "image.h"
#include "mi_embedder.h"
#include "cost_model_config.h"
#include "exception.hpp"
#include "cost_model.h"
#include "mat2D.h"
#include "jstruct.h"

typedef unsigned int uint;
namespace fs = boost::filesystem;
namespace po = boost::program_options;

void Save_Image(std::string imagePath, mat2D<int>* I);
mat2D<int> * Load_Image(std::string imagePath, cost_model_config *config);
mat2D<int> * Embed(mat2D<int> *cover, cost_model_config * config, float &alpha_out, float &coding_loss_out, unsigned int &stc_trials_used, float &distortion);

void printInfo(){
	std::cout << "This program embeds a payload using while minimizing 'J-UNIWARD' steganographic distortion to all greyscale 'JPG' images in the directory input-dir and saves the stego images into the output-dir." << std::endl << std::endl;
	std::cout << "Author: Vojtech Holub, e-mail: vojtech_holub@yahoo.com" << std::endl << std::endl;
	std::cout << "usage: SI_UNIWARD -I input-dir -C output-cover-dir -S output-cover-dir -a payload_nzAC -q quality_factor [-v] [-h STC-height] \n\n";
}

int main(int argc, char** argv)
{
	try { 
		std::string iDir, cDir, sDir;
		float payload;
		int qf;
		bool verbose = false;
		unsigned int stc_constr_height = 0;
		int randSeed;

		po::variables_map vm;
		std::vector< std::string > images;

        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("input-dir,I",				po::value<std::string>(&iDir),									"directory with the 'pgm' cover images")
            ("images,i",				po::value<std::vector<std::string> >(&images),					"list of 'pgm' cover images")
			("output-cover-dir,C",		po::value<std::string>(&cDir),									"directory to output JPEG cover images")
			("output-stego-dir,S",		po::value<std::string>(&sDir),									"directory to output JPEG stego images")
            ("payload_nzAC,a",			po::value<float>(&payload),										"payload to embed in bits per non-zero AC DCT coefficient")
			("quality_factor,q",        po::value<int>(&qf),											"quality factor of compressed JPEG images")
			("verbose,v",				po::bool_switch(&verbose),										"print out verbose messages")
			("STC-height,h",			po::value<unsigned int>(&stc_constr_height)->default_value(0),	"0=simulate emb. on bound, >0 constraint height of STC, try 7-12")
			("random-seed,r",			po::value<int>(&randSeed)->default_value(0),					"default=0 (every time different)")
            ;

        po::positional_options_description p;

        po::store(po::command_line_parser(argc,argv).options(desc).positional(p).run(), vm);
        po::notify(vm);

        if (vm.count("help"))  { printInfo(); std::cout << desc << "\n"; return 1; }
        if (!vm.count("output-cover-dir")){ std::cout << "'output-cover-dir' is required.\n" << desc << "\n"; return 1; }
		if (!vm.count("output-stego-dir")){ std::cout << "'output-stego-dir' is required.\n" << desc << "\n"; return 1; }
		else if (!fs::is_directory(fs::path(cDir))) { std::cout << "'output-cover-dir' must be an existing directory.\n" << desc << "\n"; return 1; }
		else if (!fs::is_directory(fs::path(sDir))) { std::cout << "'output-stego-dir' must be an existing directory.\n" << desc << "\n"; return 1; }
		if ((payload<=0) || (payload>1)) { std::cout << "'payload' must be larger than 0 and smaller than 1.\n" << desc << "\n"; return 1; }
		if ((qf<=0) || (qf>100)) { std::cout << "'quality_factor' must be larger than 0 and smaller or equal than 100.\n" << desc << "\n"; return 1; }

		// add all jpg files from the input directory to the vector
		fs::directory_iterator end_itr; // default construction yields past-the-end
		if (vm.count("input-dir"))
		{
			for ( fs::directory_iterator itr(iDir); itr!=end_itr; ++itr ) 
			{
				if ( (!fs::is_directory(itr->status())) && (itr->path().extension()==".pgm") )
					images.push_back(itr->path().string());
            }
		}

        if (verbose) {
            std::cout << "# J-UNIWARD DISTORTION EMBEDDING SIMULATOR" << std::endl;
			if (vm.count("input-dir")) std::cout << "# input directory = " << iDir << std::endl;
            std::cout << "# output cover directory = " << cDir << std::endl;
			std::cout << "# output stego directory = " << sDir << std::endl;
            std::cout << "# running payload-limited sender with alpha = " << payload << " bits per nzAC" << std::endl;
            if (stc_constr_height==0)
                std::cout << "# simulating embedding as if the best coding scheme is available" << std::endl;
            else
                std::cout << "# using STCs with constraint height h=" << stc_constr_height << std::endl;
            std::cout << ")" << std::endl;
            std::cout << std::endl;
            std::cout << "#file name     seed            size          rel. payload     rel. distortion  ";
			if (stc_constr_height>0) std::cout << "coding loss      # stc emb trials";
			std::cout << std::endl;
        }

		clock_t begin=clock();
		cost_model_config *config = new cost_model_config(payload, verbose, 1, stc_constr_height, randSeed);
		
		for (int imageIndex=0; imageIndex<(int)images.size(); imageIndex++)
		{
            fs::path precoverPath(images[imageIndex]);
			fs::path coverPath(fs::path(cDir) / fs::path(images[imageIndex]).filename().replace_extension(".jpg"));
            fs::path stegoPath(fs::path(sDir) / fs::path(images[imageIndex]).filename().replace_extension(".jpg"));

			if (verbose) std::cout << std::left << std::setw( 15 ) << precoverPath.filename().string() << std::left << std::setw( 15 ) << randSeed;

			// Load cover
			mat2D<int> *precover = Load_Image(precoverPath.string(), config);
            if ( verbose ) std::cout << std::right << std::setw( 4 ) << precover->cols << "x" << std::left << std::setw( 10 )
                    << precover->rows << std::flush;

			// Load PGM image, compute unquantize DCT coefficients and round it
			jstruct * coverStruct = new jstruct(precover, qf);

			// Save cover image
			coverStruct->jpeg_write(coverPath.string(), true);
			
			// Create cost model
			base_cost_model * model = (base_cost_model *)new cost_model(coverStruct, config);
			
			// Embedding
			float alpha_out, coding_loss_out = 0, distortion = 0;
			unsigned int stc_trials_used = 0;
			mat2D<int> * cover = coverStruct->coef_arrays[0];
			coverStruct->coef_arrays[0] = model->Embed(alpha_out, coding_loss_out, stc_trials_used, distortion);
			delete cover;
			delete model;
			// Save stego
			coverStruct->jpeg_write(stegoPath.string(), true);
			if (verbose)
			{
				std::cout	<< std::left << std::setw( 17 ) << alpha_out
							<< std::left << std::setw( 17 ) << distortion / (coverStruct->image_height * coverStruct->image_width);
				if (stc_constr_height>0)
					std::cout	<< std::left << std::setw( 17 ) << coding_loss_out
								<< std::left << std::setw( 17 ) << stc_trials_used
								<< std::endl << std::flush;
				std::cout << std::endl;
			}
			delete precover;
			delete coverStruct;
		}
		
		delete config;
		images.clear();
		
		clock_t end=clock();
		if(config->verbose) std::cout << std::endl << "Time elapsed: " << double(((double)end-begin)/CLOCKS_PER_SEC) << " s"<< std::endl;
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

mat2D<int> * Load_Image(std::string imagePath, cost_model_config *config)
{
	// read image using the script in 'image.cpp'
	image *img = new image();
	if (imagePath.substr(imagePath.find_last_of(".")) == ".pgm")
		img->load_from_pgm(imagePath);
    else
        throw exception("File '" + imagePath + "' is in unknown format, we support grayscale 8bit pgm.");
		
	// move the image into a mat2D class
	mat2D<int> *I = new mat2D<int>(img->height, img->width);
	for (int r=0; r<I->rows; r++)
		for (int c=0; c<I->cols; c++)
			I->Write(r, c, (int)img->pixels[c*I->rows+r]);
	delete img;

	return I;
}