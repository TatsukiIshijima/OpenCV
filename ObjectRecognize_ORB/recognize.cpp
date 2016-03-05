#include "stdafx.h"
#include "dirent.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
#include <opencv2\opencv.hpp>
#include "recognize.h"

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

/**
// ORB�����_�A�L�q�q�̒��o
// @parama[in]    fileName : �摜�t�@�C����
// @parame[out]   descriptor : keyPoint�̓�����
*/
int extractORB(const string fileName, cv::Mat* descriptor) {

	// �摜�̓ǂݍ���
	cv::Mat image = cv::imread(fileName);
	// �ǂݍ��߂Ȃ��Ƃ�
	if (!image.data)
	{
		cerr << "Can't read image!" << fileName << endl;
		return 1;
	}

	// �O���[�摜�pMat
	cv::Mat gray_img;
	// �O���[�X�P�[���ϊ�
	cv::cvtColor(image, gray_img, CV_BGR2GRAY);
	// ���K��
	cv::normalize(gray_img, gray_img, 0, 255, CV_MINMAX);

	// ORB�������o��
	cv::OrbFeatureDetector detector(80, 1.25f, 4, 7, 0, 2, 0, 31);
	// ORB�����ʒ��o��
	cv::OrbDescriptorExtractor extractor(80, 1.25f, 4, 7, 0, 2, 0, 31);
	// keyPoint�i�[�̂��߂�vector
	vector<cv::KeyPoint> keypoints;
	// keyPoint���o
	detector.detect(gray_img, keypoints);
	// �����ʒ��o(�����ʂ�*descriptor�Ɋi�[)
	extractor.compute(gray_img, keypoints, *descriptor);

	return 0;
}

/**
// ORB�����ʂ̕ۑ�
// @parama[in]    fileName : �ۑ�����CSV�t�@�C����
// @parama[in]    FileList : �P���摜�t�H���_
*/
void SaveCSV(const string fileName, vector<string> FileList) 
{
	cv::Mat t_descriptor;                                          // �����ʕۑ��pMat

	ofstream ofs(fileName);                                        // �ۑ��t�@�C�����J��

	cout << "Extracting ORB..." << endl;

	for (int i=0; i< FileList.size(); i++) {

		extractORB(FileList[i], &t_descriptor);                    // �����ʒ��o

		if (t_descriptor.type() != CV_32F) {                       // FLANN�p�Ɍ`���ϊ�(�ꉞ)
			t_descriptor.convertTo(t_descriptor, CV_32F);
		}

		ofs << cv::format(t_descriptor, "csv");                    // �����ʕۑ�
		
	}

	cout << "Extracting ORB...ok" << endl;
}

/**
// CSV�t�@�C���̓ǂݍ��݁�Mat�ɕϊ�
// @parama[in]    FileName : �ǂݍ���CSV�t�@�C����
// @parama[in]    ImageNum : �P���摜��
// @parame[out]   TrainData : �e�摜�̓����ʔz��
*/
void LoadFeatureData(char* FileName, int ImageNum, cv::Mat* TrainData)
{
	int pixel_value;                                                // �e�Z���̒l���i�[�p�ϐ�
	FILE* input_file;
	
	fopen_s(&input_file, FileName, "r");                            // �t�@�C�����J��

	cout << "Loading CSV file..." << endl;

	for (int n = 0; n < ImageNum; n++) {

		TrainData[n] = cv::Mat(80, 32, CV_32F);                      // �L�[�|�C���g��,�@�p�b�`�T�C�Y,�@CV_32F��ݒ�

		for (int row = 0; row < 80; row++) {
			for (int col = 0; col < 32; col++) {
				fscanf_s(input_file, "%f,", &pixel_value);
				TrainData[n].at<int>(row, col) = pixel_value;        // �摜(1������)�̓�����z��Ɋi�[
			}
		}
	}

	fclose(input_file);

	cout << "Loading CSV file...ok" << endl;
}

/**
// �����}�b�`���O
// @parama[in]    DirList : �P���f�B���N�g�����X�g
// @parama[in]    FileNum : �P���摜��
// @parama[in]    queryImg : �N�G���摜
// @parama[in]    TrainData : �e�P���摜�̓����ʔz��
// @parame[out]   ���̖��Ɠ��[����2�����z���Ԃ�
*/
vector<vector<int>> MatchFeatures(vector<string> DirList, int FileNum, string queryImg, cv::Mat TrainData[], int DirImgNum)
{
	cv::Mat query_descriptor;                                        // �N�G���摜�̓����ʕۑ��pMat

	cout << "Extracting features from query image ..." << endl;

	extractORB(queryImg, &query_descriptor);                         // �N�G���摜�̓������o

	if (query_descriptor.type() != CV_32F) {
		query_descriptor.convertTo(query_descriptor, CV_32F);
	}

	cout << "Extracting features from query image...ok" << endl;

	cv::FlannBasedMatcher matcher;

	int vote_count = 0;                                              // ���[�����J�E���g���邽�߂̕ϐ�
	int ID = 0;                                                      // ���̂̃��x��

	//vector<int> object_vote(DirList.size());

	vector<vector<int>> obj_array;                                   // ���̖��Ɠ��[�����i�[���邽�߂̓��I�z��
	obj_array.resize(DirList.size());
	for (int i = 0; i < DirList.size(); i++) {
		obj_array[i].resize(2);                                      // ���2��ɐݒ�(����ID(���x��)�C���[��)
	}

	cout << "Matching..." << endl;

	for (int i = 0; i < FileNum; i++) {

		int good_point = 0;                                          // �ǂ��}�b�`���O�_���J�E���g���邽�߂̕ϐ�

		vector<cv::DMatch> matches;                                  // �}�b�`���O���ʊi�[�̂��߂�vector
		vector<cv::DMatch> match12, match21;                         // �N���X�`�F�b�N�̂��߂�match12, match21
		vector<cv::DMatch> good_matches;                             // �ǂ��}�b�`���O������vector
		
		// �N���X�`�F�b�N
		matcher.match(query_descriptor, TrainData[i], match12);
		matcher.match(TrainData[i], query_descriptor, match21);

		for (size_t j = 0; j < match12.size(); j++) {
			cv::DMatch forward = match12[j];
			cv::DMatch backward = match21[forward.trainIdx];
			if (backward.trainIdx == forward.queryIdx) {
				matches.push_back(forward);
			}
		}

		// matches�̒�����ŏ�������������H
		// min_dist��float�^�̍ő�l���i�[
		double min_dist = DBL_MAX;
		for (int k = 0; k < (int)matches.size(); k++){
			// dist�Ƀ}�b�`���O���ʂ�����i�[
			double dist = matches[k].distance;
			//cout << dist << endl;
			// dist��min_dist��菬�����ꍇ
			if (dist < min_dist) min_dist = dist;
		}

		// �ǂ��y�A�����c��
		for (int m = 0; m < (int)matches.size(); m++) {
			// �}�b�`���O���ʂ�S�������A�}�b�`���O�̋������ŏ�������3�{�ȉ��̏ꍇ
			// �ǂ��}�b�`���O���ʂƂ��Ēǉ�
			//cout << "���� : " << matches[i].distance << endl;
			if (matches[m].distance <= 3.0 * min_dist){
				good_matches.push_back(matches[m]);
				good_point++;
			}
		}

		vote_count += good_point;
		//cout << "�ŏ�����: " << min_dist << endl;
		//cout << "GOOD_PARE_COUNT : " << good_count << endl;
		//cout << "count:" << count << endl;

		// �t�H���_�ŋ�؂邽�߂̕���
		if ((i + 1) % DirImgNum == 0) {
			//cout << "--------------------------------------------------------" << endl;
			obj_array[ID][0] = ID;
			obj_array[ID][1] = vote_count;
			// ���̖����x���ɓ��[�����i�[
			//object_vote[ID] = vote_count;
			vote_count = 0;
			//cout << "Object_Name:" << obj_array[ID][0] <<  "\t" << "Vote_Score:" << obj_array[ID][1] << endl;
			//cout << "Object_Name:" << DirList[ID] <<  "\t" << "Vote_Score:" <<object_vote[ID] << endl;
			//cout << "--------------------------------------------------------" << endl;
			ID++;
		}
	}

	cout << "Matching...ok" << endl;

	return obj_array;  
}

/**
// ���[���Ń\�[�g
// @parama[in]    obj_array : ���̖��Ɠ��[����2�����z��(n�̕��̖��~2��)
// @parama[in]    DirList : �P���f�B���N�g�����X�g
*/
void SortVoteNum(vector<vector<int>> obj_array, vector<string> DirList)
{
	int tmp;
	string tmp_string;

	for (int i = 0; i < (int)(obj_array.size()-1); i++) {
		for (int j = i + 1; j < (int)obj_array.size(); j++){
			if(obj_array[i][1] < obj_array[j][1]) {
				tmp = obj_array[i][1];
				tmp_string = DirList[obj_array[i][0]];
				obj_array[i][1] = obj_array[j][1];
				DirList[obj_array[i][0]] = DirList[obj_array[j][0]];
				obj_array[j][1] = tmp;
				DirList[obj_array[j][0]] = tmp_string;
			}
		}
	}
	cout << "�N�G���摜:" << "test_image.jpg" << endl;
	// �\�[�g���ʕ\��
	for (int k = 0; k < (int)obj_array.size(); k++) {
		cout << "Name:" << DirList[obj_array[k][0]] << ":\t"  << "Vote_Score:" << obj_array[k][1] << endl;;
	}
}