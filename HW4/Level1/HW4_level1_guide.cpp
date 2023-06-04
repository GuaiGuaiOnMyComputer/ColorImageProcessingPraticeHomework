/*HW4_Level1: �ɪO�H������*/
# include <opencv2/opencv.hpp>
# include <random>
// #include "stdafx.h" //�p�G�M�׼��Y�ɸ̸̦�stdafx.h�A��l�{���ɦ�stdafx.cpp�A�ݥ[�J�o�@��C�p�G�O�űM�סA�h���Υ[�o�@��C

#define MOVING_VIDEO_PATH   "data/moving.mp4"  // The path to the video file "moving.mp4"
#define DROP_VIDEO_PATH     "data/drop.mp4"    // The path to the video file "drop.mp4"
#define COIN01_PATH         "data/c01.png"     // The path to the coin
#define COIN05_PATH         "data/c05.png"     // The path to the coin
#define COIN10_PATH         "data/c10.png"     // The path to the coin
#define COIN50_PATH         "data/c50.png"     // The path to the coin
#define COINS               20                 //�ɪO�H���������`�q�A�����O��ơA�ƭȶV�C�V�}��


struct CoinSpawnInfo;
void loadCoinImgs(std::vector<cv::Mat>& coins_out, std::vector<cv::Mat>& coinsBin_out);

int main(void)
{
	/*�ܼ�(�v���e��)�ŧi*/
	auto rng = std::mt19937(time(0));
	std::vector<cv::Mat> coins;
	std::vector<cv::Mat> coinsMasks;
	loadCoinImgs(coins, coinsMasks);

	cv::VideoCapture cap(MOVING_VIDEO_PATH); //Ū���I���v��
	if (!cap.isOpened()) 
		return 0;

	cv::Size frameSize(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT)); //Ū�����T�e/��
	const int FRAME_COUNT = cap.get(cv::CAP_PROP_FRAME_COUNT); //Ū�����T�e���`��

	cv::VideoWriter dropVid(DROP_VIDEO_PATH, cv::VideoWriter::fourcc('X', '2', '6', '4'), 60, frameSize); //�إ߻ɪO�H�����������T����

	if (!dropVid.isOpened())
	{
		std::cout << "File could not be created for writing." << std::endl;
		return -1;
	}
	cv::namedWindow("Video"); //�إߵ��T����

	CoinSpawnInfo coinSpawnInfo[COINS]; //�C�ӻɪO�������_�l�e���s���B�����B������m
	for (int i = 0; i < COINS; i++)
	{
		coinSpawnInfo[i].SpawnFrame = rng() % (FRAME_COUNT - 100);     // �ɪOi���_�l�e���s��(�H��)
		coinSpawnInfo[i].CoinType   = rng() % 5;                       // �ɪOi������(�H��)
		coinSpawnInfo[i].x   = rng() % (frameSize.width - 150); // �ɪOi��������V��m(�H��)
	}

	//��for�j��Ū���C�@�e��
	for (int frameIdx = 0; frameIdx < FRAME_COUNT; frameIdx++)
	{
		cv::Mat im_bg;
		cap >> im_bg; //Ū���I���v��
		if (im_bg.empty()) //�p�G�v�����s�b
		{
			std::cout << " --(!) No captured im -- Break!\n";  //��ܿ��~�T��
			break;
		}

		//�N��i �ӻɪO�����v���[�J�I���v�����j��
		for (int i = 0; i < COINS; i++)
		{
			int y = 5 * (frameIdx - coinSpawnInfo[i].SpawnFrame); //�ɪOi�x�ήت�y�y��(�x�ήؤW�t��������V�y��)
			if (y > 0 && y < frameSize.height - 150) //�p�G�ɪOi�x�ή�(������V)�b�e���d�򤺡A����U�C�ʧ@
			{
				cv::Mat coinRoi = im_bg(cv::Rect(coinSpawnInfo[i].x, y, coins[i].cols, coins[i].rows));
				coins[coinSpawnInfo[i].CoinType].copyTo(coinRoi, coinsMasks[coinSpawnInfo[i].CoinType]);
			}
		}

		cv::imshow("Video", im_bg); //����|�W�ɪO�᪺�I���v��
		cv::waitKey(10); //����10�@��
		dropVid << im_bg; //�ץX�v����n��X�����T��
	}
	std::cout << "please check: data/drop.mp4" << std::endl; //��ܿ�X���T�����|/�ɦW
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
	coins_out.push_back(cv::imread(COIN01_PATH, 1)); //���J1���ɪO�v��
	coins_out.push_back(cv::imread(COIN05_PATH, 1)); //���J5���ɪO�v��
	coins_out.push_back(cv::imread(COIN10_PATH, 1)); //���J10���ɪO�v��
	coins_out.push_back(cv::imread(COIN50_PATH, 1)); //���J50���ɪO�v��
	cv::Mat tmp;
	for (size_t i = 0; i < coins_out.size(); i ++){
		cv::cvtColor(coins_out[i], coins_out[i], cv::COLOR_BGR2GRAY); //1���ɪO�ഫ�ܦǶ��榡
		threshold(coins_out[i], tmp, 10, 255, cv::THRESH_BINARY);  //1���ɪO�G�Ȥ�
		coinsBin_out.push_back(tmp.clone());
	}
}