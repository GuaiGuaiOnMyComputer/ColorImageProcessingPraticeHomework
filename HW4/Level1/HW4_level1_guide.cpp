# include <opencv2/opencv.hpp>
# include <random>
# include <array>
# include <algorithm>

#define MOVING_VIDEO_PATH   "data/moving.mp4"  // The path to the video file "moving.mp4"
#define DROP_VIDEO_PATH     "data/drop.mp4"    // The path to the video file "drop.mp4"
#define COIN01_PATH         "data/c01.png"     // The path to the coin
#define COIN05_PATH         "data/c05.png"     // The path to the coin
#define COIN10_PATH         "data/c10.png"     // The path to the coin
#define COIN50_PATH         "data/c50.png"     // The path to the coin
#define COINS_COUNT         50

struct CoinSpawnInfo
{
	uint64_t SpawnFrame; // the frame at which an aribritary coin will spawn
	uint8_t  CoinType;   // the type of a coin
	uint16_t x;          // the horizontal location of the coin
};

void loadCoinImgs(std::vector<cv::Mat>&, std::vector<cv::Mat>&);
void determineCoinSpawn(std::array<CoinSpawnInfo, COINS_COUNT>&, const int, const int);
inline void spawnCoinsOnFrame(cv::Mat&, const std::array<CoinSpawnInfo, COINS_COUNT> &, const std::vector<cv::Mat> &, const std::vector<cv::Mat> &,  const size_t&, const size_t&);

int main(void)
{
	std::vector<cv::Mat> coins;        // vector storing all the coin images as cv::mat
	std::vector<cv::Mat> coinsMasks;   // vector storing all the coin masks as cv::mat

	loadCoinImgs(coins, coinsMasks);   // read the coin images into the previous two vectors

	cv::VideoCapture cap(MOVING_VIDEO_PATH);
	cv::Size frameSize = cv::Size(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT));
	cv::VideoWriter dropVid(DROP_VIDEO_PATH, cv::VideoWriter::fourcc('m', 'p', '4', 'v'), 60, frameSize); 
		
	// do not proceed if the video cannot be read
	if (!cap.isOpened()){
		std::cout << "Cannot open video, check path:" << MOVING_VIDEO_PATH << std::endl;
		return -1;
	}

	// do not proceed if the output video path is not valid
	if (!dropVid.isOpened()){
		std::cout << "File could not be created for writing." << std::endl;
		return -1;
	}

	const int FRAME_COUNT = cap.get(cv::CAP_PROP_FRAME_COUNT);

	// create a window displaying coins falling on professor's face
	cv::namedWindow("Video"); 
	std::array<CoinSpawnInfo, COINS_COUNT> coinSpawnInfo;  // array holding the spawning information of each coin as struct CoinSpawnInfo
	determineCoinSpawn(coinSpawnInfo, FRAME_COUNT, frameSize.width);  // get the random spawniing stats of each coin by calling this custom function

	// process each frame of the video
	for (int frameIdx = 0; frameIdx < FRAME_COUNT; frameIdx++)
	{
		cv::Mat im_bg;
		cap >> im_bg;
		if (im_bg.empty()) 
		{
			std::cout << " --(!) No captured im -- Break!\n";
			break;
		}

		spawnCoinsOnFrame(im_bg, coinSpawnInfo, coins, coinsMasks, frameIdx, frameSize.height);

		cv::imshow("Video", im_bg);
		cv::waitKey(10); 
		dropVid << im_bg;
	}
	std::cout << "please check: data/drop.mp4" << std::endl; 
	return 0;
}

/// @brief loads all the coin images into std::vector _coins_out and their masks into std::vector _coinsBin_out
/// @param _coins_out the vector storing the coin images as cv::Mat
/// @param _coinsBin_out the vector storing the mask of each coin
void loadCoinImgs(std::vector<cv::Mat>& _coins_out, std::vector<cv::Mat>& _coinsBin_out)
{
	_coins_out.reserve(4);
	_coinsBin_out.reserve(4);
	_coins_out.push_back(cv::imread(COIN01_PATH, 1)); 
	_coins_out.push_back(cv::imread(COIN05_PATH, 1)); 
	_coins_out.push_back(cv::imread(COIN10_PATH, 1)); 
	_coins_out.push_back(cv::imread(COIN50_PATH, 1)); 
	cv::Mat tmp;
	for (cv::Mat aCoinImg : _coins_out){
		cv::cvtColor(aCoinImg, tmp, cv::COLOR_BGR2GRAY);
		threshold(tmp, tmp, 10, 255, cv::THRESH_BINARY);
		_coinsBin_out.push_back(tmp.clone());
	}
}

/// @brief compares the spawining frame of two CoinSpawinInfo element in the array
/// @param coinInfo1 one of the two elements to be compared
/// @param coinInfo2 one of the two elements to be compared
/// @return true if coinInfo1 < coinInfo2
bool frameLarger(const CoinSpawnInfo &coinInfo1, const CoinSpawnInfo &coinInfo2)
{
	return coinInfo1.SpawnFrame < coinInfo2.SpawnFrame;
}

/// @brief assigns random spawining frame, spawing horizontal location and coin type of each coin. The array _coinSpawnInfo will be sorted in ascending order according to the spawning frame of each coin
/// @param _coinSpawnInfo the array holding all the spawning information of the coins
/// @param _FRAME_COUNT number of frames in the video, used to determine the last spawning frame
/// @param _FRAME_WIDTH the width of the video, used to determine the right-most spawning horizontal location
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


/// @brief paste the coins on to the displayed image according the spawning information previously determined
/// @param _imBg_out the result of the frame pasted with the coin images
/// @param _coinSpawnInfo previously determined spawning information of all the coins
/// @param _coins the loaded coin images saved into std::vector<Mat>
/// @param _coinsMasks the masks for each coin
/// @param _frameIdx index of the current frame. Used to determine the y location of each coin
/// @param _frameHeight height of the frame. Used to determine the minimum height of each coin before it de-spawns
inline void spawnCoinsOnFrame(
	cv::Mat& _imBg_out, 
	const std::array<CoinSpawnInfo, COINS_COUNT>& _coinSpawnInfo, 
	const std::vector<cv::Mat>& _coins,
	const std::vector<cv::Mat>& _coinsMasks,
	const size_t& _frameIdx, 
	const size_t& _frameHeight)
{
	for (const CoinSpawnInfo& aCoinInfo : _coinSpawnInfo)
	{
		int y = 5 * (_frameIdx - aCoinInfo.SpawnFrame);
		if (y > 0 && y < _frameHeight - 150)
		{
			cv::Mat coinRoi = cv::Mat(_imBg_out, cv::Rect(aCoinInfo.x, y, _coins[aCoinInfo.CoinType].cols, _coins[aCoinInfo.CoinType].rows));
			_coins[aCoinInfo.CoinType].copyTo(coinRoi, _coinsMasks[aCoinInfo.CoinType]);
		}
	}
}