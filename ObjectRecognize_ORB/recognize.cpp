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

/**
// ORB特徴点、記述子の抽出
// @parama[in]    fileName : 画像ファイル名
// @parame[out]   descriptor : keyPointの特徴量
*/
int extractORB(const string fileName, cv::Mat* descriptor) {

	// 画像の読み込み
	cv::Mat image = cv::imread(fileName);
	// 読み込めないとき
	if (!image.data)
	{
		cerr << "Can't read image!" << fileName << endl;
		return 1;
	}

	// グレー画像用Mat
	cv::Mat gray_img;
	// グレースケール変換
	cv::cvtColor(image, gray_img, CV_BGR2GRAY);
	// 正規化
	cv::normalize(gray_img, gray_img, 0, 255, CV_MINMAX);

	// ORB特徴検出器
	cv::OrbFeatureDetector detector(80, 1.25f, 4, 7, 0, 2, 0, 31);
	// ORB特徴量抽出器
	cv::OrbDescriptorExtractor extractor(80, 1.25f, 4, 7, 0, 2, 0, 31);
	// keyPoint格納のためのvector
	vector<cv::KeyPoint> keypoints;
	// keyPoint検出
	detector.detect(gray_img, keypoints);
	// 特徴量抽出(特徴量を*descriptorに格納)
	extractor.compute(gray_img, keypoints, *descriptor);

	return 0;
}

/**
// ORB特徴量の保存
// @parama[in]    fileName : 保存するCSVファイル名
// @parama[in]    FileList : 訓練画像フォルダ
*/
void SaveCSV(const string fileName, vector<string> FileList) 
{
	cv::Mat t_descriptor;                                          // 特徴量保存用Mat

	ofstream ofs(fileName);                                        // 保存ファイルを開く

	cout << "Extracting ORB..." << endl;

	for (int i=0; i< FileList.size(); i++) {

		extractORB(FileList[i], &t_descriptor);                    // 特徴量抽出

		if (t_descriptor.type() != CV_32F) {                       // FLANN用に形式変換(一応)
			t_descriptor.convertTo(t_descriptor, CV_32F);
		}

		ofs << cv::format(t_descriptor, "csv");                    // 特徴量保存
		
	}

	cout << "Extracting ORB...ok" << endl;
}

/**
// CSVファイルの読み込み＆Matに変換
// @parama[in]    FileName : 読み込むCSVファイル名
// @parama[in]    ImageNum : 訓練画像数
// @parame[out]   TrainData : 各画像の特徴量配列
*/
void LoadFeatureData(char* FileName, int ImageNum, cv::Mat* TrainData)
{
	int pixel_value;                                                // 各セルの値を格納用変数
	FILE* input_file;
	
	fopen_s(&input_file, FileName, "r");                            // ファイルを開く

	cout << "Loading CSV file..." << endl;

	for (int n = 0; n < ImageNum; n++) {

		TrainData[n] = cv::Mat(80, 32, CV_32F);                      // キーポイント数,　パッチサイズ,　CV_32Fを設定

		for (int row = 0; row < 80; row++) {
			for (int col = 0; col < 32; col++) {
				fscanf_s(input_file, "%f,", &pixel_value);
				TrainData[n].at<int>(row, col) = pixel_value;        // 画像(1枚ずつ)の特徴を配列に格納
			}
		}
	}

	fclose(input_file);

	cout << "Loading CSV file...ok" << endl;
}

/**
// 特徴マッチング
// @parama[in]    DirList : 訓練ディレクトリリスト
// @parama[in]    FileNum : 訓練画像数
// @parama[in]    queryImg : クエリ画像
// @parama[in]    TrainData : 各訓練画像の特徴量配列
// @parame[out]   物体名と投票数の2次元配列を返す
*/
vector<vector<int>> MatchFeatures(vector<string> DirList, int FileNum, string queryImg, cv::Mat TrainData[], int DirImgNum)
{
	cv::Mat query_descriptor;                                        // クエリ画像の特徴量保存用Mat

	cout << "Extracting features from query image ..." << endl;

	extractORB(queryImg, &query_descriptor);                         // クエリ画像の特徴抽出

	if (query_descriptor.type() != CV_32F) {
		query_descriptor.convertTo(query_descriptor, CV_32F);
	}

	cout << "Extracting features from query image...ok" << endl;

	cv::FlannBasedMatcher matcher;

	int vote_count = 0;                                              // 投票数をカウントするための変数
	int ID = 0;                                                      // 物体のラベル

	//vector<int> object_vote(DirList.size());

	vector<vector<int>> obj_array;                                   // 物体名と投票数を格納するための動的配列
	obj_array.resize(DirList.size());
	for (int i = 0; i < DirList.size(); i++) {
		obj_array[i].resize(2);                                      // 列は2列に設定(物体ID(ラベル)，投票数)
	}

	cout << "Matching..." << endl;

	for (int i = 0; i < FileNum; i++) {

		int good_point = 0;                                          // 良いマッチング点をカウントするための変数

		vector<cv::DMatch> matches;                                  // マッチング結果格納のためのvector
		vector<cv::DMatch> match12, match21;                         // クロスチェックのためのmatch12, match21
		vector<cv::DMatch> good_matches;                             // 良いマッチングを入れるvector
		
		// クロスチェック
		matcher.match(query_descriptor, TrainData[i], match12);
		matcher.match(TrainData[i], query_descriptor, match21);

		for (size_t j = 0; j < match12.size(); j++) {
			cv::DMatch forward = match12[j];
			cv::DMatch backward = match21[forward.trainIdx];
			if (backward.trainIdx == forward.queryIdx) {
				matches.push_back(forward);
			}
		}

		// matchesの中から最小距離を見つける？
		// min_distにfloat型の最大値を格納
		double min_dist = DBL_MAX;
		for (int k = 0; k < (int)matches.size(); k++){
			// distにマッチング結果を一つずつ格納
			double dist = matches[k].distance;
			//cout << dist << endl;
			// distがmin_distより小さい場合
			if (dist < min_dist) min_dist = dist;
		}

		// 良いペアだけ残す
		for (int m = 0; m < (int)matches.size(); m++) {
			// マッチング結果を全検索し、マッチングの距離が最小距離の3倍以下の場合
			// 良いマッチング結果として追加
			//cout << "距離 : " << matches[i].distance << endl;
			if (matches[m].distance <= 3.0 * min_dist){
				good_matches.push_back(matches[m]);
				good_point++;
			}
		}

		vote_count += good_point;
		//cout << "最小距離: " << min_dist << endl;
		//cout << "GOOD_PARE_COUNT : " << good_count << endl;
		//cout << "count:" << count << endl;

		// フォルダで区切るための分岐
		if ((i + 1) % DirImgNum == 0) {
			//cout << "--------------------------------------------------------" << endl;
			obj_array[ID][0] = ID;
			obj_array[ID][1] = vote_count;
			// 物体名ラベルに投票数を格納
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
// 投票数でソート
// @parama[in]    obj_array : 物体名と投票数の2次元配列(n個の物体名×2列)
// @parama[in]    DirList : 訓練ディレクトリリスト
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
	cout << "クエリ画像:" << "test_image.jpg" << endl;
	// ソート結果表示
	for (int k = 0; k < (int)obj_array.size(); k++) {
		cout << "Name:" << DirList[obj_array[k][0]] << ":\t"  << "Vote_Score:" << obj_array[k][1] << endl;;
	}
}