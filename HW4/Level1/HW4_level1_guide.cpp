/*HW4_Level1: �ɪO�H������*/
# include <opencv2/opencv.hpp>
# include <random>
# include <memory>
// #include "stdafx.h" //�p�G�M�׼��Y�ɸ̸̦�stdafx.h�A��l�{���ɦ�stdafx.cpp�A�ݥ[�J�o�@��C�p�G�O�űM�סA�h���Υ[�o�@��C

#define MOVING_VIDEO_PATH   "data/moving.mp4"  // The path to the video file "moving.mp4"
#define DROP_VIDEO_PATH     "data/drop.mp4"    // The path to the video file "drop.mp4"
#define COIN01_PATH         "data/c01.png"     // The path to the coin
#define COIN05_PATH         "data/c05.png"     // The path to the coin
#define COIN10_PATH         "data/c10.png"     // The path to the coin
#define COIN50_PATH         "data/c50.png"     // The path to the coin
#define COINS               20                 //�ɪO�H���������`�q�A�����O��ơA�ƭȶV�C�V�}��

using Mat_ptr = std::unique_ptr<cv::Mat>;

void loadCoinImgs(std::vector<Mat_ptr>& coins_out, std::vector<Mat_ptr>& coinsBin_out);

int main(void)
{
	/*�ܼ�(�v���e��)�ŧi*/
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

	int start[COINS][3]; //�C�ӻɪO�������_�l�e���s���B�����B������m
	for (int i = 0; i < COINS; i++)
	{
		start[i][COIN_SPAWN_FRAME] = rng() % (FRAME_COUNT - 100);     // �ɪOi���_�l�e���s��(�H��)
		start[i][COIN_TYPE]        = rng() % 5;                       // �ɪOi������(�H��)
		start[i][COIN_POS]         = rng() % (frameSize.width - 150); // �ɪOi��������V��m(�H��)
	}

	//��for�j��Ū���C�@�e��
	for (int n = 0; n < FRAME_COUNT; n++)
	{
		cv::Mat im_bg;
		cap >> im_bg; //Ū���I���v��
		if (im_bg.empty()) //�p�G�v�����s�b
		{
			printf(" --(!) No captured im -- Break!");  //��ܿ��~�T��
			break;
		}

		//�N��i �ӻɪO�����v���[�J�I���v�����j��
		for (int i = 0; i < COINS; i++)
		{
			int y = 5 * (n - start[i][0]); //�ɪOi�x�ήت�y�y��(�x�ήؤW�t��������V�y��)
			if (y > 0 && y < frameSize.height-150) //�p�G�ɪOi�x�ή�(������V)�b�e���d�򤺡A����U�C�ʧ@
			{
				// switch (start[i][1]) //�ھڻɪOi�������A��ܻɪO�v��
				// {
				// 	case 1: //1���w��
				// 	{
				// 		...  //�ھ�start[i][2]�Py����m�A�إ� roi1 �x�ή�(Rect type)
				// 		... //�N im1 �ƻs�� im_bg ���x�ήءA�H im1_gray ���B�n
				// 		break; //���} switch
				// 	}
				// 	case 2: //5���w��
				// 	{
				// 		...  //�ھ�start[i][2]�Py����m�A�إ� roi2 �x�ή�(Rect type)
				// 		... //�N im2 �ƻs�� im_bg ���x�ήءA�H im2_gray ���B�n
				// 		break; //���} switch
				// 	}
				// 	case 3: //10���w��
				// 	{
				// 		...  //�ھ�start[i][2]�Py����m�A�إ� roi3 �x�ή�(Rect type)
				// 		... //�N im3 �ƻs�� im_bg ���x�ήءA�H im3_gray ���B�n
				// 		break; //���} switch
				// 	}
				// 	case 4: //50���w��
				// 	{
				// 		...  //�ھ�start[i][2]�Py����m�A�إ� roi4 �x�ή�(Rect type)
				// 		... //�N im4 �ƻs�� im_bg ���x�ήءA�H im4_gray ���B�n
				// 	};
				// }
			}
		}

		cv::imshow("Video", im_bg); //����|�W�ɪO�᪺�I���v��
		cv::waitKey(10); //����10�@��
		dropVid << im_bg; //�ץX�v����n��X�����T��
	}
	std::cout << "please check: data/drop.mp4" << std::endl; //��ܿ�X���T�����|/�ɦW

	return 0;
}

void loadCoinImgs(std::vector<Mat_ptr>& coins_out, std::vector<Mat_ptr>& coinsBin_out)
{
	coins_out.reserve(4);
	coinsBin_out.reserve(4);
	coins_out.push_back(std::make_unique<cv::Mat>(cv::imread(COIN01_PATH, 1))); //���J1���ɪO�v��
	coins_out.push_back(std::make_unique<cv::Mat>(cv::imread(COIN05_PATH, 1))); //���J5���ɪO�v��
	coins_out.push_back(std::make_unique<cv::Mat>(cv::imread(COIN10_PATH, 1))); //���J10���ɪO�v��
	coins_out.push_back(std::make_unique<cv::Mat>(cv::imread(COIN50_PATH, 1))); //���J50���ɪO�v��
	cv::Mat tmp;
	for (size_t i = 0; i < coins_out.size(); i ++){
		cv::cvtColor(*coins_out[i], *coins_out[i], cv::COLOR_BGR2GRAY); //1���ɪO�ഫ�ܦǶ��榡
		threshold(*coins_out[i], tmp, 10, 255, cv::THRESH_BINARY);  //1���ɪO�G�Ȥ�
		coinsBin_out.push_back(std::make_unique<cv::Mat>(tmp.clone()));
	}
}