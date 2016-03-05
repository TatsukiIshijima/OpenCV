// BoFSURF.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/nonfree/features2d.hpp>

using namespace cv;
using namespace std;

#define DICTIONARY_BUILD 0 // set DICTIONARY_BUILD 1 to do Step 1, otherwise it goes to step 2

int _tmain(int argc, _TCHAR* argv[])
{	
int minHessian = 400; //Hessian Threshold
#if DICTIONARY_BUILD == 1

	//Step 1 - Obtain the set of bags of features.

	//to store the input file names
	char * filename = new char[100];		
	//to store the current input image
	Mat input;	

	//To store the keypoints that will be extracted by SURF
	vector<KeyPoint> keypoints;
	//To store the SURF descriptor of current image
	Mat descriptor;
	//To store all the descriptors that are extracted from all the images.
	Mat featuresUnclustered(0,0,CV_32F);
	//The SURF feature extractor and descriptor		
	OrbDescriptorExtractor detector;
	
	//I select 20 (1000/50) images from 1000 images to extract feature descriptors and build the vocabulary
	for(int f=0;f<999;f+=50){		
		//create the file name of an image
		sprintf(filename,"G:\\testimages\\image\\%i.jpg",f);
		//open the file
		input = imread(filename, CV_LOAD_IMAGE_GRAYSCALE); //Load as grayscale				
		//detect feature points
		detector.detect(input, keypoints);
		//compute the descriptors for each keypoint
		detector.compute(input, keypoints,descriptor);		
		//put the all feature descriptors in a single Mat object 
		featuresUnclustered.push_back(descriptor);		
		//print the percentage
		printf("%i percent done\n",f/10);
	}	


	//Construct BOWKMeansTrainer
	//the number of bags
	int dictionarySize=200;
	//define Term Criteria
	TermCriteria tc(CV_TERMCRIT_ITER,100,0.001);
	//retries number
	int retries=1;
	//necessary flags
	int flags=KMEANS_RANDOM_CENTERS;
	//Create the BoW (or BoF) trainer
	BOWKMeansTrainer bowTrainer(dictionarySize,tc,retries,flags);
	//convert featuresUnclustered to type CV_32F
	Mat featuresUnclusteredF(featuresUnclustered.rows,featuresUnclustered.cols,CV_32F);
	featuresUnclustered.convertTo(featuresUnclusteredF,CV_32F);
	//cluster the feature vectors
	Mat dictionary=bowTrainer.cluster(featuresUnclusteredF);	
	//store the vocabulary
	FileStorage fs("dictionary.yml", FileStorage::WRITE);
	fs << "vocabulary" << dictionary;
	fs.release();
	
#else
	//Step 2 - Obtain the BoF descriptor for given image/video frame. 

    //prepare BOW descriptor extractor from the dictionary    
	Mat dictionaryF; 
	FileStorage fs("dictionary.yml", FileStorage::READ);
	fs["vocabulary"] >> dictionaryF;
	fs.release();	

	//convert to 8bit unsigned format
	Mat dictionary(dictionaryF.rows,dictionaryF.cols,CV_8U);
	dictionaryF.convertTo(dictionary,CV_8U);
    
	//create a matcher with BruteForce-Hamming distance
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");	
	
	//create SURF feature point extracter
	Ptr<FeatureDetector> detector(new OrbFeatureDetector());
	//create SURF descriptor extractor
	Ptr<DescriptorExtractor> extractor(new OrbDescriptorExtractor());	
	//create BoF (or BoW) descriptor extractor
	BOWImgDescriptorExtractor bowDE(extractor,matcher);
	//Set the dictionary with the vocabulary we created in the first step
	bowDE.setVocabulary(dictionary);

	//To store the image file name
	char * filename = new char[100];
	//To store the image tag name - only for save the descriptor in a file
	char * imageTag = new char[10];

	//open the file to write the resultant descriptor
	FileStorage fs1("descriptor.yml", FileStorage::WRITE);	
	
	//the image file with the location. change it according to your image file location
	sprintf(filename,"G:\\testimages\\image\\1.jpg");		
	//read the image
	Mat img=imread(filename,CV_LOAD_IMAGE_GRAYSCALE);		
	//To store the keypoints that will be extracted by SURF
	vector<KeyPoint> keypoints;		
	//Detect SURF keypoints (or feature points)
	detector->detect(img,keypoints);
	//To store the BoW (or BoF) representation of the image
	Mat bowDescriptor;		
	//extract BoW (or BoF) descriptor from given image
	bowDE.compute(img,keypoints,bowDescriptor);

	//prepare the yml (some what similar to xml) file
	sprintf(imageTag,"img1");			
	//write the new BoF descriptor to the file
	fs1 << imageTag << bowDescriptor;		

	//You may use this descriptor for classifying the image.
			
	//release the file storage
	fs1.release();
#endif
	printf("\ndone\n");	
    return 0;
}