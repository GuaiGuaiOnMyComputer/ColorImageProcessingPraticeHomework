/*HW4_Level1: 銅板隨機掉落*/
# include <opencv2/opencv.hpp>
# include <random>
// #include "stdafx.h" //如果專案標頭檔裡裡有stdafx.h，原始程式檔有stdafx.cpp，需加入這一行。如果是空專案，則不用加這一行。

#define MOVING_VIDEO_PATH   "data/moving.mp4"  // The path to the video file "moving.mp4"
#define DROP_VIDEO_PATH     "data/drop.mp4"    // The path to the video file "drop.mp4"
#define COIN01_PATH         "data/c01.png"     // The path to the coin
#define COIN05_PATH         "data/c05.png"     // The path to the coin
#define COIN10_PATH         "data/c10.png"     // The path to the coin
#define COIN50_PATH         "data/c50.png"     // The path to the coin
#define COINS               20                 //銅板隨機掉落的總量，必須是整數，數值越低越稀疏


struct CoinSpawnInfo;
void loadCoinImgs(std::vector<cv::Mat>& coins_out, std::vector<cv::Mat>& coinsBin_out);

int main(void)
{
	/*變數(影像容器)宣告*/
	auto rng = std::mt19937(time(0));
	std::vector<cv::Mat> coins;
	std::vector<cv::Mat> coinsMasks;
	loadCoinImgs(coins, coinsMasks);

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

	CoinSpawnInfo coinSpawnInfo[COINS]; //每個銅板的掉落起始畫面編號、種類、水平位置
	for (int i = 0; i < COINS; i++)
	{
		coinSpawnInfo[i].SpawnFrame = rng() % (FRAME_COUNT - 100);     // 銅板i的起始畫面編號(隨機)
		coinSpawnInfo[i].CoinType   = rng() % 5;                       // 銅板i的種類(隨機)
		coinSpawnInfo[i].x   = rng() % (frameSize.width - 150); // 銅板i的水平方向位置(隨機)
	}

	//用for迴圈讀取每一畫格
	for (int frameIdx = 0; frameIdx < FRAME_COUNT; frameIdx++)
	{
		cv::Mat im_bg;
		cap >> im_bg; //讀取背景影像
		if (im_bg.empty()) //如果影像不存在
		{
			std::cout << " --(!) No captured im -- Break!\n";  //顯示錯誤訊息
			break;
		}

		//將第i 個銅板掉落影像加入背景影像的迴圈
		for (int i = 0; i < COINS; i++)
		{
			int y = 5 * (frameIdx - coinSpawnInfo[i].SpawnFrame); //銅板i矩形框的y座標(矩形框上緣的垂直方向座標)
			if (y > 0 && y < frameSize.height - 150) //如果銅板i矩形框(垂直方向)在畫面範圍內，執行下列動作
			{
				cv::Mat coinRoi = im_bg(cv::Rect(coinSpawnInfo[i].x, y, coins[i].cols, coins[i].rows));
				coins[coinSpawnInfo[i].CoinType].copyTo(coinRoi, coinsMasks[coinSpawnInfo[i].CoinType]);
			}
		}

		cv::imshow("Video", im_bg); //顯示疊上銅板後的背景影像
		cv::waitKey(10); //等待10毫秒
		dropVid << im_bg; //匯出影像到要輸出的視訊檔
	}
	std::cout << "please check: data/drop.mp4" << std::endl; //顯示輸出視訊的路徑/檔名
	return 0;
}

struct CoinSpawnInfo
{
	uint64_t SpawnFrame;
	uint8_t  CoinType;
	uint16_t x;
};

void loadCoinImgs(std::vector<cv::Mat>& coins_out, std::vector<cv::Mat>& coinsBin_out)
{
	coins_out.reserve(4);
	coinsBin_out.reserve(4);
	coins_out.push_back(cv::imread(COIN01_PATH, 1)); //載入1元銅板影像
	coins_out.push_back(cv::imread(COIN05_PATH, 1)); //載入5元銅板影像
	coins_out.push_back(cv::imread(COIN10_PATH, 1)); //載入10元銅板影像
	coins_out.push_back(cv::imread(COIN50_PATH, 1)); //載入50元銅板影像
	cv::Mat tmp;
	for (size_t i = 0; i < coins_out.size(); i ++){
		cv::cvtColor(coins_out[i], coins_out[i], cv::COLOR_BGR2GRAY); //1元銅板轉換至灰階格式
		threshold(coins_out[i], tmp, 10, 255, cv::THRESH_BINARY);  //1元銅板二值化
		coinsBin_out.push_back(tmp.clone());
	}
}