// ObjectRecognize_ORB.cpp : コンソール アプリケーションのエントリ ポイントを定義します。

/****************************************  使い方  *******************************************/
// 特徴量を保存するSaveCSVは単独で使って下さい．(使用するときはSaveCSV以下をコメントアウト)
// CSVファイルの保存名はSaveCSV(".csv", FileList)の".csv"の中に設定して下さい．
// 反対にSaveCSVを使用しない場合はコメントアウトし，SaveCSV以下のコメントを外してください.
// SaveCSVで保存したファイルをLoadFeatureDataに入れて使用して下さい．
// 訓練画像フォルダは2階層フォルダを指定して下さい．
// 各フォルダの画像数は一定にしてください．
// TrainDataとLoadFeatureDataの要素数は訓練画像数(CSVファイルの行数)を指定してください．
// インクルードヘッダーの"dirent.h"がない場合はネット等で探して下さい．
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
	const string trainDir = "./caltech_test2";                                                                       // 訓練画像フォルダ
	const string queryImg = "test_image.jpg";                                                                        // クエリ画像
	char* csvfile = "ORB_feature_data4.csv";                                                                         // 読み込むCSVファイル

	vector<string> DirList = GetDirName(trainDir);                                                                   // ディレクトリ名のリスト(訓練画像フォルダ名の取得)
	vector<string> FileList = GetFilePath(trainDir);                                                                 // ファイルパス名のリスト(訓練画像パスの取得)
	vector<int> FileNumList = GetFileNum(trainDir);                                                                  // 各ディレクトリのファイル数リスト(訓練画像フォルダ内のファイル数取得)

	int DirImgNum = (int)FileList.size() / (int)DirList.size();                                                      // フォルダ毎の画像枚数

	cout << fixed << right;
	for (int i=0; i<FileNumList.size(); i++) { 
		cout << DirList[i] << " : " << FileNumList[i] << endl; 
	}
	cout << "-------------------------------------" << endl;
	cout << " ディレクトリ数 : " << DirList.size() << endl;
	cout << "     合計画像数 : " << FileList.size() << endl;
	cout << "-------------------------------------" << endl;
	
	//SaveCSV("xxxxx.csv", FileList);                                                                                 // 訓練用画像の特徴抽出＆保存

	cv::Mat TrainData[24];                                                                                            // 各画像の特徴量を格納するMat(要素数は画像数を指定)
	LoadFeatureData(csvfile, 24, TrainData);                                                                          // CSVファイル読み込み(第二引数は画像数を指定)
	
	vector<vector<int>> obj_array;                                                                                    // 物体名と投票数を格納するための動的配列
	obj_array = MatchFeatures(DirList, (int)FileList.size(), queryImg, TrainData, DirImgNum);                         // マッチング

	SortVoteNum(obj_array, DirList);                                                                                  // ソート
	
	return 0;
}