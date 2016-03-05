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

// 関数のプロトタイプ宣言

vector<string> GetFilePath(string directory);

vector<int> GetFileNum(string directory);

vector<string> GetDirName(string directory);

int extractORB(const string fileName, cv::Mat* descriptor);

void SaveCSV(const string fileName, vector<string> FileList);

void LoadFeatureData(char* FileName, int ImageNum, cv::Mat* TrainData);

vector<vector<int>> MatchFeatures(vector<string> DirList, int FileNum, string queryImg, cv::Mat TrainData[], int DirImgNum);

void SortVoteNum(vector<vector<int>> obj_array, vector<string> DirList);