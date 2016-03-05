// Clustering.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
// BOVW��p�����N���X�^�����O

#include "stdafx.h"
#include "dirent.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
#include <opencv2\opencv.hpp>

using namespace std;

/**
// 2�K�w�f�B���N�g���̃t�@�C���p�X�����擾
// @parama[in]    directory : �f�B���N�g����
// @parame[return]   FileList : �t�@�C���p�X���̃��X�g
*/
vector<string> GetFilePath(string directory) {

	// �t�@�C���p�X�����i�[���铮�I�z��
	vector<string> FileList;

	DIR* dp1;
	DIR* dp2;
	// �f�B���N�g�����J��
	if ((dp1 = opendir(directory.c_str())) == NULL) {
		cerr << "Can't open directory : " << directory.c_str() << endl;
		exit(1);
	}

	// IMAGE_DIR�̉摜�t�H���_�𑖍�
	struct dirent* entry1;
	struct dirent* entry2;

	// �f�B���N�g���̒��g��ǂݍ���
	while ((entry1 = readdir(dp1)) != NULL) {
		string DirName = string(entry1->d_name);
		if (DirName.compare(".") == 0 || DirName.compare("..") == 0){
			continue;
		}

		// �f�B���N�g����Path���ɕϊ�
		// Ex : Sample_Dir -> IMAGE_DIR/Sample_Dir
		string dirPath;
		// Path���𐶐�
		dirPath = directory + "/" + DirName;

		// Path����f�B���N�g�����J��
		if ((dp2 = opendir(dirPath.c_str())) == NULL) {
			cerr << "Can't open directory : " << dirPath << endl;
			exit(1);
		}

		// �f�B���N�g���̒��g��ǂݍ���
		while ((entry2 = readdir(dp2)) != NULL) {
			string FileName = string(entry2->d_name);
			if (FileName.compare(".") == 0 || FileName.compare("..") == 0) {
				continue;
			}

			// Path���ɕϊ�
			// Ex : sample.jpg -> IMAGE_DIR/Sample_Dir/sample.jpg
			string filePath;
			//Path���𐶐�
			filePath = dirPath + "/" + FileName;

			//cout << filePath << endl;

			// �t�@�C���p�X�����X�g�ɒǉ�
			FileList.push_back(filePath);

		}

		// �f�B���N�g�������
		closedir(dp2);
	}
	// �f�B���N�g�������
	closedir(dp1);

	return FileList;
}

/**
// 2�K�w�f�B���N�g���̊e�f�B���N�g���̃t�@�C�������擾
// @parama[in]    directory : �f�B���N�g����
// @parame[return]   FileNum : �t�@�C�����̃��X�g
*/
vector<int> GetFileNum(string directory) {

	// �t�@�C�������i�[���铮�I�z��
	vector<int> FileNum;

	int count = 0;

	DIR* dp1;
	DIR* dp2;
	// �f�B���N�g�����J��
	if ((dp1 = opendir(directory.c_str())) == NULL) {
		cerr << "Can't open directory : " << directory.c_str() << endl;
		exit(1);
	}

	// IMAGE_DIR�̉摜�t�H���_�𑖍�
	struct dirent* entry1;
	struct dirent* entry2;

	// �f�B���N�g���̒��g��ǂݍ���
	while ((entry1 = readdir(dp1)) != NULL) {
		string DirName = string(entry1->d_name);
		if (DirName.compare(".") == 0 || DirName.compare("..") == 0){
			continue;
		}

		// �f�B���N�g����Path���ɕϊ�
		// Ex : Sample_Dir -> IMAGE_DIR/Sample_Dir
		string dirPath;
		// Path���𐶐�
		dirPath = directory + "/" + DirName;

		// Path����f�B���N�g�����J��
		if ((dp2 = opendir(dirPath.c_str())) == NULL) {
			cerr << "Can't open directory : " << dirPath << endl;
			exit(1);
		}

		// �f�B���N�g���̒��g��ǂݍ���
		while ((entry2 = readdir(dp2)) != NULL) {
			string FileName = string(entry2->d_name);
			if (FileName.compare(".") == 0 || FileName.compare("..") == 0) {
				continue;
			}
			count++;
		}
		FileNum.push_back(count);
		// �f�B���N�g�������
		closedir(dp2);
		count = 0;
	}
	// �f�B���N�g�������
	closedir(dp1);

	return FileNum;
}

/**
// 2�K�w�f�B���N�g���̃t�H���_�����擾
// @parama[in]    directory : �f�B���N�g����
// @parame[return]   DirList : �t�H���_���̃��X�g
*/
vector<string> GetDirName(string directory) {

	vector<string> DirList;

	DIR* dp1;
	// �f�B���N�g�����J��
	if ((dp1 = opendir(directory.c_str())) == NULL) {
		cerr << "Can't open directory : " << directory.c_str() << endl;
		exit(1);
	}

	// IMAGE_DIR�̉摜�t�H���_�𑖍�
	struct dirent* entry1;

	// �f�B���N�g���̒��g��ǂݍ���
	while ((entry1 = readdir(dp1)) != NULL) {
		string DirName = string(entry1->d_name);
		if (DirName.compare(".") == 0 || DirName.compare("..") == 0){
			continue;
		}

		DirList.push_back(DirName);
	}

	return DirList;
}

int _tmain(int argc, _TCHAR* argv[])
{
	const string trainDir = "./caltech_test2";                                                        // �P���摜�t�H���_
	char* csvfile = "BoVW_FeatureData.csv";                                                           // BoVW�ۑ��p��CSV�t�@�C��
	ofstream ofs(csvfile);                                                                            // csv�t�@�C�����J��

	vector<string> DirList = GetDirName(trainDir);                                                    // �f�B���N�g�����̃��X�g(�P���摜�t�H���_���̎擾)
	vector<string> FileList = GetFilePath(trainDir);                                                  // �t�@�C���p�X���̃��X�g(�P���摜�p�X�̎擾)
	vector<int> FileNumList = GetFileNum(trainDir);                                                   // �e�f�B���N�g���̃t�@�C�������X�g(�P���摜�t�H���_���̃t�@�C�����擾)

	cout << fixed << right;
	for (int i = 0; i<FileNumList.size(); i++) {
		cout << DirList[i] << " : " << FileNumList[i] << endl;
	}
	cout << "-------------------------------------" << endl;
	cout << " �f�B���N�g���� : " << DirList.size() << endl;
	cout << "     ���v�摜�� : " << FileList.size() << endl;
	cout << "-------------------------------------" << endl;

	cv::Ptr<cv::FeatureDetector> detector = cv::FeatureDetector::create("ORB");                       // �����_���o��
	cv::Ptr<cv::DescriptorExtractor> descriptor = cv::DescriptorExtractor::create("ORB");             // �����ʒ��o��
	cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create("BruteForce-Hamming");     // �}�b�`���O��
	//cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create("FlannBased");

	/********************** BOWKmeansTrainer �p�����[�^�ݒ� ***************************/
	int ClusterCount = 500;                                                                           // �N���X�^��
	cv::TermCriteria termcrit(CV_TERMCRIT_ITER, 10, 0.001);
	int attemps = 1;
	int flags = cv::KMEANS_RANDOM_CENTERS;

	cv::BOWKMeansTrainer bowTrainer(ClusterCount, termcrit, attemps, flags);                          // BOW�P����
	cv::BOWImgDescriptorExtractor bowDespExtracter(descriptor, matcher);                              // BOW���o��H

	/************************** �P���摜��������ʒ��o *******************************/
	cout << "Feature extract..." << endl;
	for (int i = 0; i < FileList.size(); i++) {
		vector<cv::KeyPoint> trainKeypoint;                                                           // �����_�i�[�pVector
		cv::Mat trainFeature;                                                                         // �����ʊi�[�pMat

		cv::Mat trainImg = cv::imread(FileList[i], CV_LOAD_IMAGE_GRAYSCALE);                          // �P���摜�ǂݍ���

		detector->detect(trainImg, trainKeypoint);                                                    // �����_���o
		descriptor->compute(trainImg, trainKeypoint, trainFeature);                                   // �����ʒ��o

		if (trainFeature.type() != CV_32F) {
			trainFeature.convertTo(trainFeature, CV_32F);
		}

		bowTrainer.add(trainFeature);                                                                 // �e�����ʂ�BOW�P����ɉ�����
	}
	cout << "Feature extract...ok" << endl;

	/************************** �����ʂ̃N���X�^�����O *******************************/
	cout << "Clustering..." << endl;
	cv::Mat vocabulary = bowTrainer.cluster();                                                        // �����������ʂ��N���X�^�����O

	if (vocabulary.type() != CV_8U) {
		vocabulary.convertTo(vocabulary, CV_8U);
	}

	bowDespExtracter.setVocabulary(vocabulary);                                                       // �N���X�^�����O���ʂ𒊏o��ɐݒ�

	cv::FileStorage fs("vocabulary.yml", cv::FileStorage::WRITE);                                     // �N���X�^�����O���ʂ̕ۑ�
	fs << "vocabulary" << vocabulary;
	fs.release();

	cout << "Clustering...ok" << endl;

	/************************** BOW�q�X�g�O�����̍쐬 ********************************/
	cout << "Create Histgram..." << endl;
	for (int i = 0; i < FileList.size(); i++){
		vector<cv::KeyPoint> Keypoint;                                                                // �����_�i�[�pVector

		cv::Mat Img = cv::imread(FileList[i], CV_LOAD_IMAGE_GRAYSCALE);                               // �P���摜�ǂݍ���

		detector->detect(Img, Keypoint);                                                              // �����_���o

		cv::Mat bowDescriptor;
		bowDespExtracter.compute(Img, Keypoint, bowDescriptor);                                       // �q�X�g�O�����쐬(bowDescriptor)

		ofs << cv::format(bowDescriptor, "csv") << endl;                                              // CSV�t�@�C���ɕۑ�
	}
	cout << "Create Histgram...ok" << endl;

	return 0;
}

