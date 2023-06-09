# include <opencv2/opencv.hpp>
# include <random>
# include <array>
# include <algorithm>
// #include "stdafx.h" //�p�G�M�׼��Y�ɸ̸̦�stdafx.h�A��l�{���ɦ�stdafx.cpp�A�ݥ[�J�o�@��C�p�G�O�űM�סA�h���Υ[�o�@��C

#define MOVING_VIDEO_PATH   "data/moving.mp4"  // The path to the video file "moving.mp4"
#define DROP_VIDEO_PATH     "data/drop.mp4"    // The path to the video file "drop.mp4"
#define COIN01_PATH         "data/c01.png"     // The path to the coin
#define COIN05_PATH         "data/c05.png"     // The path to the coin
#define COIN10_PATH         "data/c10.png"     // The path to the coin
#define COIN50_PATH         "data/c50.png"     // The path to the coin
#define COINS_COUNT         20                 


struct CoinSpawnInfo
{
	uint64_t SpawnFrame;
	uint8_t  CoinType;
	uint16_t x;
};

void loadCoinImgs(std::vector<cv::Mat>& _coins_out, std::vector<cv::Mat>& _coinsBin_out);
void determineCoinSpawn(std::array<CoinSpawnInfo, COINS_COUNT>& _coinSpawnInfo, const int _FRAME_COUNT, const int _FRAME_WIDTH);
inline void spawnCoinsOnFrame(std::array<CoinSpawnInfo, COINS_COUNT> &_coinSpawnInfo, std::vector<cv::Mat> &_coins, std::vector<cv::Mat> &_coinsMasks, cv::Mat& _imBg_out, size_t _frameIdx, size_t _frameHeight);

int main(void)
{
	std::vector<cv::Mat> coins;
	std::vector<cv::Mat> coinsMasks;

	loadCoinImgs(coins, coinsMasks);

	cv::VideoCapture cap(MOVING_VIDEO_PATH);
	cv::Size frameSize = cv::Size(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT));
	cv::VideoWriter dropVid(DROP_VIDEO_PATH, cv::VideoWriter::fourcc('X', '2', '6', '4'), 60, frameSize); 
		
	if (!cap.isOpened()){
		std::cout << "Cannot open video, check path:" << MOVING_VIDEO_PATH << std::endl;
		return -1;
	}

	if (!dropVid.isOpened()){
		std::cout << "File could not be created for writing." << std::endl;
		return -1;
	}

	const int FRAME_COUNT = cap.get(cv::CAP_PROP_FRAME_COUNT);

	cv::namedWindow("Video"); 
	std::array<CoinSpawnInfo, COINS_COUNT> coinSpawnInfo;
	determineCoinSpawn(coinSpawnInfo, FRAME_COUNT, frameSize.width);

#if _DEBUG
	cv::namedWindow("ROI with coin");
	cv::namedWindow("ROI bg");
#endif

	for (int frameIdx = 0; frameIdx < FRAME_COUNT; frameIdx++)
	{
		cv::Mat im_bg;
		cap >> im_bg;
		if (im_bg.empty()) 
		{
			std::cout << " --(!) No captured im -- Break!\n";
			break;
		}

		spawnCoinsOnFrame(coinSpawnInfo, coins, coinsMasks, im_bg, frameIdx, frameSize.height);

		cv::imshow("Video", im_bg);
		cv::waitKey(10); 
		dropVid << im_bg;
	}
	std::cout << "please check: data/drop.mp4" << std::endl; 
	return 0;
}


void loadCoinImgs(std::vector<cv::Mat>& _coins_out, std::vector<cv::Mat>& _coinsBin_out)
{
	_coins_out.reserve(4);
	_coinsBin_out.reserve(4);
	_coins_out.push_back(cv::imread(COIN01_PATH, 1)); 
	_coins_out.push_back(cv::imread(COIN05_PATH, 1)); 
	_coins_out.push_back(cv::imread(COIN10_PATH, 1)); 
	_coins_out.push_back(cv::imread(COIN50_PATH, 1)); 
	cv::Mat tmp;
	for (size_t i = 0; i < _coins_out.size(); i ++){
		cv::cvtColor(_coins_out[i], _coins_out[i], cv::COLOR_BGR2GRAY);
		threshold(_coins_out[i], tmp, 10, 255, cv::THRESH_BINARY);
		_coinsBin_out.push_back(tmp.clone());
	}
}

bool frameLarger(const CoinSpawnInfo &coinInfo1, const CoinSpawnInfo &coinInfo2)
{
	return coinInfo1.SpawnFrame < coinInfo2.SpawnFrame;
}

void determineCoinSpawn(std::array<CoinSpawnInfo, COINS_COUNT>& _coinSpawnInfo, const int _FRAME_COUNT, const int _FRAME_WIDTH)
{
	auto rng = std::mt19937(time(0));
	for (CoinSpawnInfo& coinInfo : _coinSpawnInfo)
	{
		coinInfo.SpawnFrame = rng() % (_FRAME_COUNT - 100);   
		coinInfo.CoinType   = rng() % 4;                       
		coinInfo.x          = rng() % (_FRAME_WIDTH - 150); 
	}

	std::sort(_coinSpawnInfo.begin(), _coinSpawnInfo.end(), frameLarger);
}

inline void spawnCoinsOnFrame(
	std::array<CoinSpawnInfo, COINS_COUNT>& _coinSpawnInfo, 
	std::vector<cv::Mat>& _coins,
	std::vector<cv::Mat>& _coinsMasks,
	cv::Mat& _imBg_out, 
	size_t _frameIdx, 
	size_t _frameHeight)
{
	for (CoinSpawnInfo& aCoinInfo : _coinSpawnInfo)
	{
		int y = 5 * (_frameIdx - aCoinInfo.SpawnFrame);
		if (y > 0 && y < _frameHeight - 150)
		{
			cv::Mat coinRoi = cv::Mat(_imBg_out, cv::Rect(aCoinInfo.x, y, _coins[aCoinInfo.CoinType].cols, _coins[aCoinInfo.CoinType].rows));
#if _DEBUG
			cv::imshow("ROI bg", coinRoi);
#endif
			_coins[aCoinInfo.CoinType].copyTo(coinRoi, _coinsMasks[aCoinInfo.CoinType]);
#if _DEBUG
			cv::imshow("ROI With coin", coinRoi);
#endif
		}
	}
}