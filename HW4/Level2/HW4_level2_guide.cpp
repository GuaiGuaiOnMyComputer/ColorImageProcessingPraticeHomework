//HW4_Level2: 偵測 drop.mp4 視訊中，掉落的銅板，並計算其總金額。(註：沒偵測到眼睛時不算)
//1.設定參數
//2.偵測視訊中的人臉與人眼
//3.有偵測到人眼時，對掉落的銅板做 HoughCircles 偵測
//4.繪製人臉/人眼框
//5.繪製銅板圓形框，以及人眼到圓心的雷射光
//6.用直方圖統計每個水平區間，曾經偵測到圓心的最高金額(根據其半徑)
//7.輸出含有偵測框﹑雷射光、偵測金額的視訊
//8.根據直方圖，統計銅板的總金額

#include "opencv2/opencv.hpp" //載入opencv函式庫
#include <iostream> //載入c++函式庫
//#include "stdafx.h" //如果專案標頭檔裡裡有stdafx.h，原始程式檔有stdafx.cpp，需加入這一行。如果是空專案，則不用加這一行。

using namespace cv; //宣告 opencv 函式庫的命名空間
using namespace std; //宣告 C++函式庫的命名空間

/* 參數設定(全域變數) */
Mat frame; //視訊影像容器(Mat class)
int show_rect = 1; //是否顯示人臉/人眼框的參數(int type)
int frame_count = 0; //視訊畫面編號計數器(int type)
 //錢幣隨機掉落視訊物件(VideoCapture class), 可參考 cv/sleepDetection.cpp
 //視訊影像寬,高(Size type)
//輸出偵測結果的視訊物件(VideoWriter class)
//人臉級聯偵測物件(CascadeClassifier class)
//人眼級聯偵測物件(CascadeClassifier class)
vector<Rect> faces; //人臉矩形框向量(Rect type)
vector<Rect> eyes; //人眼矩形框向量(Rect type)
vector<Vec3f> circles; //銅板圓形向量(Vec3f type)
float binW = 10; //直方圖每個水平區間對映影像寬度的長度(單位:像素)

/* 子程式原型宣告 */
vector<Rect> detectFace(Mat); //人臉偵測
vector<Rect> detectEye(Mat);  //人眼偵測
vector<Vec3f> detectCircle(Mat); //銅板圓形偵測
Mat boxFace(Mat, vector<Rect>); //人臉框繪製
Mat drawCircle(Mat, vector<Vec3f>); //人眼框繪製
Mat drawEye(Mat, Rect, vector<Rect>); //銅板圓框繪製

/* 主程式 */
int main(int, char** argv) {
	/* 級聯偵測物件初始化(載入xml參數) */
	if (!faceCascade.load("data/haarcascade_frontalface_alt.xml")) { printf("--(!)Error loading face cascade\n"); return -1; };
	if (!eyes_cascade.load("data/haarcascade_eye_tree_eyeglasses.xml")) { printf("--(!)Error loading eyes cascade\n"); return -1; };
	if (!put.isOpened()) { return -1; };

	/* 統計金額的直方圖 hist 初始化*/
	//Mat class，零矩陣，寬=視訊寬/binW，高=1，CV_32FC1格式

	//如果視訊物件可以讀取時，執行while迴圈中的內容
	while (cap.isOpened()) {
		cap.read(frame); //讀取畫格
		if (frame.empty()) break; //如果畫格是空的，跳出while迴圈

		/* 偵測人臉 */
		//子程式偵測人臉，輸出人臉框

		//如果有人臉框，執行下列動作
		{
			 //如果顯示人臉/人眼框的參數非0，執行下列繪圖子程式
			{
				//子程式繪製人臉框
				//子程式偵測人眼(注意：僅偵測faces[0]區塊)
				//子程式繪製人眼框
			}

			/* 當有偵測到眼睛 且 當下畫格編號除以5餘0時(可寫成not(frame_count %5))... */
			{ 
				//子程式偵測畫面中的圓形

				/* 當有偵測到圓(硬幣)時... */
				if (circles.size() > 0) {
					 //子程式繪製圓形框
					
					// for 迴圈，對偵測到的圓形，一一繪製來自人眼的雷射，顯示錢幣金額，並存入hist直方圖
					{						
						//第i個圓形圓心的x座標(int type)
						//第i個圓形圓心的y座標(int type)
						//第i個圓形的半徑(int type)

						//計算眼睛的中心座標(Point type)
						//繪製眼睛到圓心的紅色雷射(line())

						/* 利用硬幣半徑判斷金額 */
						{ //如果圓心<=47，金額=1
						}
						{ //如果圓心>47 && <=60，金額=5
						}
						{ //如果圓心>60 && <=67，金額=10
						}
						{ //否則，金額=50
						}
						//在金幣上方，用putText()顯示金額
						//根據圓心x座標選擇hist的區間，如果硬幣金額高於該區間的最高金額，則更新此金額。可能會用到at<float>, cvRound, MAX()
					}
				}
			}
		}

		/* 顯示影像 */
		//在畫面右下角，用putText()顯示銅板總金額。註：金額的字串可用 to_string(int(sum(hist).val[0]))
		imshow("frame", frame); //顯示銅板偵測畫面
		put << frame; //將畫面存入待輸出的視訊物件

		/* 顯示直方圖 */
		//建立直方圖影像容器(Mat class)
		//將hist，resize放大。水平方向放大8倍，垂直方向放大40倍，用第三種內差法(鄰近點取代法)，可參考cv_19
		//hist 影像中的數值是[0,1,5,10,50]，不容易看出明暗差別，因此將數值開根號 sqrt(src,dst)。
		//先將hist影像數值除以5，再顯示該影像。不同金額的銅板，將呈現不同的明暗，金額越高越亮。

		/* 鍵盤處理 */
		int c=waitKey(10);
		//按 Esc (ascii 27號) 離開 while 迴圈
		//按 'r' 切換人/眼框的顯示狀態, 可參考Tutorial/camshiftDemo.cpp
		//可按其它按鍵，增添功能(例如人臉改成橢圓形框、人臉二值化等)，但需要用cout在 command window說明用法

		//畫格編號加1
	}
	
	//在 command window 顯示總金額(用int(sum(hist).val[0]))
	destroyAllWindows(); //關閉所有視窗
	return 0;
}



// 偵測人臉的位置 (x, y, w, h)
vector<Rect> detectFace(Mat frame) {
	vector<Rect> faces;
	Mat frame_gray;
	//轉成灰階影像
	 //人臉多尺度級聯偵測，參數1.5, 4, 0, Size(80, 80)，請參考Tutorial/faceEyeDetection.cpp
	return faces;
}


// 偵測眼睛的位置 (x, y, w, h)
vector<Rect> detectEye(Mat faceROI) {
	Mat frame_gray;
	vector<Rect> eyes;
	//轉成灰階影像
	 //人眼多尺度級聯偵測，參數1.1, 2, 0, Size(20, 20)，請參考Tutorial/faceEyeDetection.cpp
	return eyes;
}	


// 偵測圓的位置 (x, y, r)
vector<Vec3f> detectCircle(Mat frame) {
	Mat gray;
	vector<Vec3f> circles;
	cvtColor(frame, gray, COLOR_BGR2GRAY); //轉成灰階影像
	//霍夫圓偵測，參數1.3, 120, 150, 100, 40, 90。請參閱Tutorial/HoughCircle_Demo.cpp
	return circles;
}


// 畫出人臉的位置(請參考Tutorial / faceEyeDetection.cpp)
Mat boxFace(Mat frame, vector<Rect> faces) {
	if (faces.size() > 0) {
		for (size_t i = 0; i < faces.size(); i++) {
			int x = faces[i].x;
			int y = faces[i].y;
			int w = faces[i].width;
			int h = faces[i].height;
			rectangle(frame, Point(x, y), Point(x + w, y + h), Scalar(255, 255, 0), 1, LINE_AA); 
		}
	}
	return frame;
}


// 畫出圓的位置(請參考cv_32)
Mat drawCircle(Mat frame, vector<Vec3f> circles) {
	{ //用for迴圈繪製 circles 的每一個圓形
		//可用需要cvRound()四捨五入成整數
		//用circle，紅色，線粗=2
	}
	return frame;
}


// 畫出眼睛的位置
Mat drawEye(Mat frame, Rect face, vector<Rect> eyes) {
	{ //用for迴圈繪製 eyes 的框
		//需要將face的x,y座標加上eyes的x,y座標
		//用retangle 或 circle，青色，線粗=1
	}
	return frame;
}


