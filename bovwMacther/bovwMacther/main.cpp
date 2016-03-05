// bovwMacther.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
// BoVW��p�����}�b�`���O

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>
#include <opencv2\opencv.hpp>

using namespace std;

void LoadFeatureData(char* FileName, int ImageNum, cv::Mat* TrainData)
{
	int pixel_value;                                                // �e�Z���̒l���i�[�p�ϐ�
	FILE* input_file;
	
	fopen_s(&input_file, FileName, "r");                            // �t�@�C�����J��

	cout << "Loading CSV file..." << endl;

	for (int n = 0; n < ImageNum; n++) {

		TrainData[n] = cv::Mat(1, 500, CV_32F);                      // 1�s,�@������(�N���X�^��),�@CV_32F��ݒ�
		for (int col = 0; col < 500; col++) {
			fscanf_s(input_file, "%f,", &pixel_value);
			TrainData[n].at<int>(0, col) = pixel_value;              // �摜(1������)�̓�����z��Ɋi�[
		}
	}

	fclose(input_file);
	cout << "Loading CSV file...ok" << endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	const string queryImg = "test.jpg";                                                                           // �N�G���摜
	const string vocab_file = "vocabulary.yml";                                                                   // �N���X�^�����O���ʃt�@�C��
	//char* q_csvfile = "BoVW_queryData.csv";                                                                     // �N�G���摜�����ۑ��t�@�C��
	char* t_csvfile = "BoVW_FeatureData.csv";                                                                     //  �P���摜�����t�@�C��

	cv::Ptr<cv::FeatureDetector> detector = cv::FeatureDetector::create("ORB");                                   // �����_���o��
	cv::Ptr<cv::DescriptorExtractor> descriptor = cv::DescriptorExtractor::create("ORB");                         // �����ʒ��o��
	cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create("BruteForce-Hamming");                 // �}�b�`���O��
	cv::Ptr<cv::DescriptorMatcher> f_matcher = cv::DescriptorMatcher::create("FlannBased");
	cv::BOWImgDescriptorExtractor bowDespExtracter(descriptor, matcher);                                          // BoW�쐬��H

	cv::Mat TrainData[24];                                                                                        // �P�������i�[�pMat
	LoadFeatureData(t_csvfile, 24, TrainData);                                                                    // �P�������t�@�C���ǂݍ���

	cv::Mat vocabulary;                                                                                           // �N���X�^�����O���ʊi�[�pMat
	cv::FileStorage fs(vocab_file, cv::FileStorage::READ);                                                        // �N���X�^�����O���ʓǂݍ���
	fs["vocabulary"] >> vocabulary;
	fs.release();

	if (vocabulary.type() != CV_8U) {
		vocabulary.convertTo(vocabulary, CV_8U);
	}

	bowDespExtracter.setVocabulary(vocabulary);                                                                   // �N���X�^�����O���ʂ�ݒ�
	cout << "Vocabulary setting...ok" << endl;
	
	cout << "QueryImage reading..." << endl;
	vector<cv::KeyPoint> queryKeypoint;
	cv::Mat query = cv::imread(queryImg, CV_LOAD_IMAGE_GRAYSCALE);                                                // �N�G���摜�ǂݍ���
	detector->detect(query, queryKeypoint);                                                                       // �N�G���摜��������_���o

	cv::Mat queryDescriptor;
	bowDespExtracter.compute(query, queryKeypoint, queryDescriptor);                                              // �N�G���摜��BoW�쐬
	cout << "QueryImage reading...ok" << endl;
	
	//ofstream ofs(q_csvfile);                                                                                    // �N�G���摜��BoW�ۑ�
	//ofs << cv::format(queryDescriptor, "csv") << endl;

	for (int i = 0; i < 24; i++) {

		//int good_point = 0;

		vector<cv::DMatch> matches;
		vector<cv::DMatch> match12, match21;
		vector<cv::DMatch> good_matches;

		f_matcher->match(queryDescriptor, TrainData[i], match12);                                                 // �N���X�`�F�b�N
		f_matcher->match(TrainData[i], queryDescriptor, match21);                                                 // �P���摜�ƃN�G���摜

		for (size_t j = 0; j < match12.size(); j++) {
			cv::DMatch forward = match12[j];
			cv::DMatch backward = match21[forward.trainIdx];
			if (backward.trainIdx == forward.queryIdx) {
				matches.push_back(forward);
			}
		}

		double min_dist = DBL_MAX;
		for (int k = 0; k < (int)matches.size(); k++){
			// dist�Ƀ}�b�`���O���ʂ�����i�[
			double dist = matches[k].distance;
			cout <<  (1- dist) << endl;
			// dist��min_dist��菬�����ꍇ
			if (dist < min_dist) {
				min_dist = dist;
			}
		}
	}

	return 0;
}

