// Clustering.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
// BOVWを用いたクラスタリング

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
// 2階層ディレクトリのファイルパス名を取得
// @parama[in]    directory : ディレクトリ名
// @parame[return]   FileList : ファイルパス名のリスト
*/
vector<string> GetFilePath(string directory) {

	// ファイルパス名を格納する動的配列
	vector<string> FileList;

	DIR* dp1;
	DIR* dp2;
	// ディレクトリを開く
	if ((dp1 = opendir(directory.c_str())) == NULL) {
		cerr << "Can't open directory : " << directory.c_str() << endl;
		exit(1);
	}

	// IMAGE_DIRの画像フォルダを走査
	struct dirent* entry1;
	struct dirent* entry2;

	// ディレクトリの中身を読み込む
	while ((entry1 = readdir(dp1)) != NULL) {
		string DirName = string(entry1->d_name);
		if (DirName.compare(".") == 0 || DirName.compare("..") == 0){
			continue;
		}

		// ディレクトリをPath名に変換
		// Ex : Sample_Dir -> IMAGE_DIR/Sample_Dir
		string dirPath;
		// Path名を生成
		dirPath = directory + "/" + DirName;

		// Pathからディレクトリを開く
		if ((dp2 = opendir(dirPath.c_str())) == NULL) {
			cerr << "Can't open directory : " << dirPath << endl;
			exit(1);
		}

		// ディレクトリの中身を読み込む
		while ((entry2 = readdir(dp2)) != NULL) {
			string FileName = string(entry2->d_name);
			if (FileName.compare(".") == 0 || FileName.compare("..") == 0) {
				continue;
			}

			// Path名に変換
			// Ex : sample.jpg -> IMAGE_DIR/Sample_Dir/sample.jpg
			string filePath;
			//Path名を生成
			filePath = dirPath + "/" + FileName;

			//cout << filePath << endl;

			// ファイルパスをリストに追加
			FileList.push_back(filePath);

		}

		// ディレクトリを閉じる
		closedir(dp2);
	}
	// ディレクトリを閉じる
	closedir(dp1);

	return FileList;
}

/**
// 2階層ディレクトリの各ディレクトリのファイル数を取得
// @parama[in]    directory : ディレクトリ名
// @parame[return]   FileNum : ファイル数のリスト
*/
vector<int> GetFileNum(string directory) {

	// ファイル数を格納する動的配列
	vector<int> FileNum;

	int count = 0;

	DIR* dp1;
	DIR* dp2;
	// ディレクトリを開く
	if ((dp1 = opendir(directory.c_str())) == NULL) {
		cerr << "Can't open directory : " << directory.c_str() << endl;
		exit(1);
	}

	// IMAGE_DIRの画像フォルダを走査
	struct dirent* entry1;
	struct dirent* entry2;

	// ディレクトリの中身を読み込む
	while ((entry1 = readdir(dp1)) != NULL) {
		string DirName = string(entry1->d_name);
		if (DirName.compare(".") == 0 || DirName.compare("..") == 0){
			continue;
		}

		// ディレクトリをPath名に変換
		// Ex : Sample_Dir -> IMAGE_DIR/Sample_Dir
		string dirPath;
		// Path名を生成
		dirPath = directory + "/" + DirName;

		// Pathからディレクトリを開く
		if ((dp2 = opendir(dirPath.c_str())) == NULL) {
			cerr << "Can't open directory : " << dirPath << endl;
			exit(1);
		}

		// ディレクトリの中身を読み込む
		while ((entry2 = readdir(dp2)) != NULL) {
			string FileName = string(entry2->d_name);
			if (FileName.compare(".") == 0 || FileName.compare("..") == 0) {
				continue;
			}
			count++;
		}
		FileNum.push_back(count);
		// ディレクトリを閉じる
		closedir(dp2);
		count = 0;
	}
	// ディレクトリを閉じる
	closedir(dp1);

	return FileNum;
}

/**
// 2階層ディレクトリのフォルダ名を取得
// @parama[in]    directory : ディレクトリ名
// @parame[return]   DirList : フォルダ名のリスト
*/
vector<string> GetDirName(string directory) {

	vector<string> DirList;

	DIR* dp1;
	// ディレクトリを開く
	if ((dp1 = opendir(directory.c_str())) == NULL) {
		cerr << "Can't open directory : " << directory.c_str() << endl;
		exit(1);
	}

	// IMAGE_DIRの画像フォルダを走査
	struct dirent* entry1;

	// ディレクトリの中身を読み込む
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
	const string trainDir = "./caltech_test2";                                                        // 訓練画像フォルダ
	char* csvfile = "BoVW_FeatureData.csv";                                                           // BoVW保存用書CSVファイル
	ofstream ofs(csvfile);                                                                            // csvファイルを開く

	vector<string> DirList = GetDirName(trainDir);                                                    // ディレクトリ名のリスト(訓練画像フォルダ名の取得)
	vector<string> FileList = GetFilePath(trainDir);                                                  // ファイルパス名のリスト(訓練画像パスの取得)
	vector<int> FileNumList = GetFileNum(trainDir);                                                   // 各ディレクトリのファイル数リスト(訓練画像フォルダ内のファイル数取得)

	cout << fixed << right;
	for (int i = 0; i<FileNumList.size(); i++) {
		cout << DirList[i] << " : " << FileNumList[i] << endl;
	}
	cout << "-------------------------------------" << endl;
	cout << " ディレクトリ数 : " << DirList.size() << endl;
	cout << "     合計画像数 : " << FileList.size() << endl;
	cout << "-------------------------------------" << endl;

	cv::Ptr<cv::FeatureDetector> detector = cv::FeatureDetector::create("ORB");                       // 特徴点抽出器
	cv::Ptr<cv::DescriptorExtractor> descriptor = cv::DescriptorExtractor::create("ORB");             // 特徴量抽出器
	cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create("BruteForce-Hamming");     // マッチング器
	//cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create("FlannBased");

	/********************** BOWKmeansTrainer パラメータ設定 ***************************/
	int ClusterCount = 500;                                                                           // クラスタ数
	cv::TermCriteria termcrit(CV_TERMCRIT_ITER, 10, 0.001);
	int attemps = 1;
	int flags = cv::KMEANS_RANDOM_CENTERS;

	cv::BOWKMeansTrainer bowTrainer(ClusterCount, termcrit, attemps, flags);                          // BOW訓練器
	cv::BOWImgDescriptorExtractor bowDespExtracter(descriptor, matcher);                              // BOW抽出器？

	/************************** 訓練画像から特徴量抽出 *******************************/
	cout << "Feature extract..." << endl;
	for (int i = 0; i < FileList.size(); i++) {
		vector<cv::KeyPoint> trainKeypoint;                                                           // 特徴点格納用Vector
		cv::Mat trainFeature;                                                                         // 特徴量格納用Mat

		cv::Mat trainImg = cv::imread(FileList[i], CV_LOAD_IMAGE_GRAYSCALE);                          // 訓練画像読み込み

		detector->detect(trainImg, trainKeypoint);                                                    // 特徴点抽出
		descriptor->compute(trainImg, trainKeypoint, trainFeature);                                   // 特徴量抽出

		if (trainFeature.type() != CV_32F) {
			trainFeature.convertTo(trainFeature, CV_32F);
		}

		bowTrainer.add(trainFeature);                                                                 // 各特徴量をBOW訓練器に加える
	}
	cout << "Feature extract...ok" << endl;

	/************************** 特徴量のクラスタリング *******************************/
	cout << "Clustering..." << endl;
	cv::Mat vocabulary = bowTrainer.cluster();                                                        // 加えた特徴量をクラスタリング

	if (vocabulary.type() != CV_8U) {
		vocabulary.convertTo(vocabulary, CV_8U);
	}

	bowDespExtracter.setVocabulary(vocabulary);                                                       // クラスタリング結果を抽出器に設定

	cv::FileStorage fs("vocabulary.yml", cv::FileStorage::WRITE);                                     // クラスタリング結果の保存
	fs << "vocabulary" << vocabulary;
	fs.release();

	cout << "Clustering...ok" << endl;

	/************************** BOWヒストグラムの作成 ********************************/
	cout << "Create Histgram..." << endl;
	for (int i = 0; i < FileList.size(); i++){
		vector<cv::KeyPoint> Keypoint;                                                                // 特徴点格納用Vector

		cv::Mat Img = cv::imread(FileList[i], CV_LOAD_IMAGE_GRAYSCALE);                               // 訓練画像読み込み

		detector->detect(Img, Keypoint);                                                              // 特徴点抽出

		cv::Mat bowDescriptor;
		bowDespExtracter.compute(Img, Keypoint, bowDescriptor);                                       // ヒストグラム作成(bowDescriptor)

		ofs << cv::format(bowDescriptor, "csv") << endl;                                              // CSVファイルに保存
	}
	cout << "Create Histgram...ok" << endl;

	return 0;
}

