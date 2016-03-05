// ObjectRecognize_ORB.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B

/****************************************  �g����  *******************************************/
// �����ʂ�ۑ�����SaveCSV�͒P�ƂŎg���ĉ������D(�g�p����Ƃ���SaveCSV�ȉ����R�����g�A�E�g)
// CSV�t�@�C���̕ۑ�����SaveCSV(".csv", FileList)��".csv"�̒��ɐݒ肵�ĉ������D
// ���΂�SaveCSV���g�p���Ȃ��ꍇ�̓R�����g�A�E�g���CSaveCSV�ȉ��̃R�����g���O���Ă�������.
// SaveCSV�ŕۑ������t�@�C����LoadFeatureData�ɓ���Ďg�p���ĉ������D
// �P���摜�t�H���_��2�K�w�t�H���_���w�肵�ĉ������D
// �e�t�H���_�̉摜���͈��ɂ��Ă��������D
// TrainData��LoadFeatureData�̗v�f���͌P���摜��(CSV�t�@�C���̍s��)���w�肵�Ă��������D
// �C���N���[�h�w�b�_�[��"dirent.h"���Ȃ��ꍇ�̓l�b�g���ŒT���ĉ������D
/*********************************************************************************************/

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

int _tmain(int argc, _TCHAR* argv[])
{
	const string trainDir = "./caltech_test2";                                                                       // �P���摜�t�H���_
	const string queryImg = "test_image.jpg";                                                                        // �N�G���摜
	char* csvfile = "ORB_feature_data4.csv";                                                                         // �ǂݍ���CSV�t�@�C��

	vector<string> DirList = GetDirName(trainDir);                                                                   // �f�B���N�g�����̃��X�g(�P���摜�t�H���_���̎擾)
	vector<string> FileList = GetFilePath(trainDir);                                                                 // �t�@�C���p�X���̃��X�g(�P���摜�p�X�̎擾)
	vector<int> FileNumList = GetFileNum(trainDir);                                                                  // �e�f�B���N�g���̃t�@�C�������X�g(�P���摜�t�H���_���̃t�@�C�����擾)

	int DirImgNum = (int)FileList.size() / (int)DirList.size();                                                      // �t�H���_���̉摜����

	cout << fixed << right;
	for (int i=0; i<FileNumList.size(); i++) { 
		cout << DirList[i] << " : " << FileNumList[i] << endl; 
	}
	cout << "-------------------------------------" << endl;
	cout << " �f�B���N�g���� : " << DirList.size() << endl;
	cout << "     ���v�摜�� : " << FileList.size() << endl;
	cout << "-------------------------------------" << endl;
	
	//SaveCSV("xxxxx.csv", FileList);                                                                                 // �P���p�摜�̓������o���ۑ�

	cv::Mat TrainData[24];                                                                                            // �e�摜�̓����ʂ��i�[����Mat(�v�f���͉摜�����w��)
	LoadFeatureData(csvfile, 24, TrainData);                                                                          // CSV�t�@�C���ǂݍ���(�������͉摜�����w��)
	
	vector<vector<int>> obj_array;                                                                                    // ���̖��Ɠ��[�����i�[���邽�߂̓��I�z��
	obj_array = MatchFeatures(DirList, (int)FileList.size(), queryImg, TrainData, DirImgNum);                         // �}�b�`���O

	SortVoteNum(obj_array, DirList);                                                                                  // �\�[�g
	
	return 0;
}