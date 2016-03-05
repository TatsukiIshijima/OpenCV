// bovwMacther.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
// BoVWを用いたマッチング

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
	int pixel_value;                                                // 各セルの値を格納用変数
	FILE* input_file;
	
	fopen_s(&input_file, FileName, "r");                            // ファイルを開く

	cout << "Loading CSV file..." << endl;

	for (int n = 0; n < ImageNum; n++) {

		TrainData[n] = cv::Mat(1, 500, CV_32F);                      // 1行,　次元数(クラスタ数),　CV_32Fを設定
		for (int col = 0; col < 500; col++) {
			fscanf_s(input_file, "%f,", &pixel_value);
			TrainData[n].at<int>(0, col) = pixel_value;              // 画像(1枚ずつ)の特徴を配列に格納
		}
	}

	fclose(input_file);
	cout << "Loading CSV file...ok" << endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	const string queryImg = "test.jpg";                                                                           // クエリ画像
	const string vocab_file = "vocabulary.yml";                                                                   // クラスタリング結果ファイル
	//char* q_csvfile = "BoVW_queryData.csv";                                                                     // クエリ画像特徴保存ファイル
	char* t_csvfile = "BoVW_FeatureData.csv";                                                                     //  訓練画像特徴ファイル

	cv::Ptr<cv::FeatureDetector> detector = cv::FeatureDetector::create("ORB");                                   // 特徴点抽出器
	cv::Ptr<cv::DescriptorExtractor> descriptor = cv::DescriptorExtractor::create("ORB");                         // 特徴量抽出器
	cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create("BruteForce-Hamming");                 // マッチング器
	cv::Ptr<cv::DescriptorMatcher> f_matcher = cv::DescriptorMatcher::create("FlannBased");
	cv::BOWImgDescriptorExtractor bowDespExtracter(descriptor, matcher);                                          // BoW作成器？

	cv::Mat TrainData[24];                                                                                        // 訓練特徴格納用Mat
	LoadFeatureData(t_csvfile, 24, TrainData);                                                                    // 訓練特徴ファイル読み込み

	cv::Mat vocabulary;                                                                                           // クラスタリング結果格納用Mat
	cv::FileStorage fs(vocab_file, cv::FileStorage::READ);                                                        // クラスタリング結果読み込み
	fs["vocabulary"] >> vocabulary;
	fs.release();

	if (vocabulary.type() != CV_8U) {
		vocabulary.convertTo(vocabulary, CV_8U);
	}

	bowDespExtracter.setVocabulary(vocabulary);                                                                   // クラスタリング結果を設定
	cout << "Vocabulary setting...ok" << endl;
	
	cout << "QueryImage reading..." << endl;
	vector<cv::KeyPoint> queryKeypoint;
	cv::Mat query = cv::imread(queryImg, CV_LOAD_IMAGE_GRAYSCALE);                                                // クエリ画像読み込み
	detector->detect(query, queryKeypoint);                                                                       // クエリ画像から特徴点検出

	cv::Mat queryDescriptor;
	bowDespExtracter.compute(query, queryKeypoint, queryDescriptor);                                              // クエリ画像のBoW作成
	cout << "QueryImage reading...ok" << endl;
	
	//ofstream ofs(q_csvfile);                                                                                    // クエリ画像のBoW保存
	//ofs << cv::format(queryDescriptor, "csv") << endl;

	for (int i = 0; i < 24; i++) {

		//int good_point = 0;

		vector<cv::DMatch> matches;
		vector<cv::DMatch> match12, match21;
		vector<cv::DMatch> good_matches;

		f_matcher->match(queryDescriptor, TrainData[i], match12);                                                 // クロスチェック
		f_matcher->match(TrainData[i], queryDescriptor, match21);                                                 // 訓練画像とクエリ画像

		for (size_t j = 0; j < match12.size(); j++) {
			cv::DMatch forward = match12[j];
			cv::DMatch backward = match21[forward.trainIdx];
			if (backward.trainIdx == forward.queryIdx) {
				matches.push_back(forward);
			}
		}

		double min_dist = DBL_MAX;
		for (int k = 0; k < (int)matches.size(); k++){
			// distにマッチング結果を一つずつ格納
			double dist = matches[k].distance;
			cout <<  (1- dist) << endl;
			// distがmin_distより小さい場合
			if (dist < min_dist) {
				min_dist = dist;
			}
		}
	}

	return 0;
}

