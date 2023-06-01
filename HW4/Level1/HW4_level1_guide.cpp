/*HW4_Level1: 銅板隨機掉落*/
# include <opencv2/opencv.hpp>
# include <random>
# include <memory>
// #include "stdafx.h" //如果專案標頭檔裡裡有stdafx.h，原始程式檔有stdafx.cpp，需加入這一行。如果是空專案，則不用加這一行。

#define MOVING_VIDEO_PATH   "data/moving.mp4"  // The path to the video file "moving.mp4"
#define DROP_VIDEO_PATH     "data/drop.mp4"    // The path to the video file "drop.mp4"
#define COIN01_PATH         "data/c01.png"     // The path to the coin
#define COIN05_PATH         "data/c05.png"     // The path to the coin
#define COIN10_PATH         "data/c10.png"     // The path to the coin
#define COIN50_PATH         "data/c50.png"     // The path to the coin
#define COINS               20                 //銅板隨機掉落的總量，必須是整數，數值越低越稀疏

using Mat_ptr = std::unique_ptr<cv::Mat>;

void loadCoinImgs(std::vector<Mat_ptr>& coins_out, std::vector<Mat_ptr>& coinsBin_out);

int main(void)
{
	/*變數(影像容器)宣告*/
	auto rng = std::mt19937(time(0));
	std::vector<std::unique_ptr<cv::Mat>> coins;
	std::vector<std::unique_ptr<cv::Mat>> coinsBin;
	loadCoinImgs(coins, coinsBin);
	cv::Mat im1, im2, im3, im4;
	cv::Mat im1Bin, im2Bin, im3Bin, im4Bin;

	enum
	{
		COIN_SPAWN_FRAME,
		COIN_TYPE,
		COIN_POS
	};

	cv::VideoCapture cap(MOVING_VIDEO_PATH); //讀取背景影片
	if (!cap.isOpened()) 
		return 0;

	cv::Size frameSize(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT)); //讀取視訊寬/高
	const int FRAME_COUNT = cap.get(cv::CAP_PROP_FRAME_COUNT); //讀取視訊畫面總數

	cv::VideoWriter dropVid(DROP_VIDEO_PATH, cv::VideoWriter::fourcc('X', '2', '6', '4'), 60, frameSize); //建立銅板隨機掉落的視訊物件

	if (!dropVid.isOpened())
	{
		std::cout << "File could not be created for writing." << std::endl;
		return -1;
	}
	cv::namedWindow("Video"); //建立視訊視窗

	int start[COINS][3]; //每個銅板的掉落起始畫面編號、種類、水平位置
	for (int i = 0; i < COINS; i++)
	{
		start[i][COIN_SPAWN_FRAME] = rng() % (FRAME_COUNT - 100);     // 銅板i的起始畫面編號(隨機)
		start[i][COIN_TYPE]        = rng() % 5;                       // 銅板i的種類(隨機)
		start[i][COIN_POS]         = rng() % (frameSize.width - 150); // 銅板i的水平方向位置(隨機)
	}

	//用for迴圈讀取每一畫格
	for (int n = 0; n < FRAME_COUNT; n++)
	{
		cv::Mat im_bg;
		cap >> im_bg; //讀取背景影像
		if (im_bg.empty()) //如果影像不存在
		{
			printf(" --(!) No captured im -- Break!");  //顯示錯誤訊息
			break;
		}

		//將第i 個銅板掉落影像加入背景影像的迴圈
		for (int i = 0; i < COINS; i++)
		{
			int y = 5 * (n - start[i][0]); //銅板i矩形框的y座標(矩形框上緣的垂直方向座標)
			if (y > 0 && y < frameSize.height-150) //如果銅板i矩形框(垂直方向)在畫面範圍內，執行下列動作
			{
				// switch (start[i][1]) //根據銅板i的種類，選擇銅板影像
				// {
				// 	case 1: //1元硬幣
				// 	{
				// 		...  //根據start[i][2]與y的位置，建立 roi1 矩形框(Rect type)
				// 		... //將 im1 複製到 im_bg 的矩形框，以 im1_gray 為遮罩
				// 		break; //離開 switch
				// 	}
				// 	case 2: //5元硬幣
				// 	{
				// 		...  //根據start[i][2]與y的位置，建立 roi2 矩形框(Rect type)
				// 		... //將 im2 複製到 im_bg 的矩形框，以 im2_gray 為遮罩
				// 		break; //離開 switch
				// 	}
				// 	case 3: //10元硬幣
				// 	{
				// 		...  //根據start[i][2]與y的位置，建立 roi3 矩形框(Rect type)
				// 		... //將 im3 複製到 im_bg 的矩形框，以 im3_gray 為遮罩
				// 		break; //離開 switch
				// 	}
				// 	case 4: //50元硬幣
				// 	{
				// 		...  //根據start[i][2]與y的位置，建立 roi4 矩形框(Rect type)
				// 		... //將 im4 複製到 im_bg 的矩形框，以 im4_gray 為遮罩
				// 	};
				// }
			}
		}

		cv::imshow("Video", im_bg); //顯示疊上銅板後的背景影像
		cv::waitKey(10); //等待10毫秒
		dropVid << im_bg; //匯出影像到要輸出的視訊檔
	}
	std::cout << "please check: data/drop.mp4" << std::endl; //顯示輸出視訊的路徑/檔名

	return 0;
}

void loadCoinImgs(std::vector<Mat_ptr>& coins_out, std::vector<Mat_ptr>& coinsBin_out)
{
	coins_out.reserve(4);
	coinsBin_out.reserve(4);
	coins_out.push_back(std::make_unique<cv::Mat>(cv::imread(COIN01_PATH, 1))); //載入1元銅板影像
	coins_out.push_back(std::make_unique<cv::Mat>(cv::imread(COIN05_PATH, 1))); //載入5元銅板影像
	coins_out.push_back(std::make_unique<cv::Mat>(cv::imread(COIN10_PATH, 1))); //載入10元銅板影像
	coins_out.push_back(std::make_unique<cv::Mat>(cv::imread(COIN50_PATH, 1))); //載入50元銅板影像
	cv::Mat tmp;
	for (size_t i = 0; i < coins_out.size(); i ++){
		cv::cvtColor(*coins_out[i], *coins_out[i], cv::COLOR_BGR2GRAY); //1元銅板轉換至灰階格式
		threshold(*coins_out[i], tmp, 10, 255, cv::THRESH_BINARY);  //1元銅板二值化
		coinsBin_out.push_back(std::make_unique<cv::Mat>(tmp.clone()));
	}
}