#include <iostream>
#include <opencv2/opencv.hpp>
#include "FruitProcess.h"
#include <string.h>
#include <algorithm>

#define EASY 300
#define CLEAR 600

int main()
{
    int rotate = 0;
    int xpos_speed = 0;
    int ypos_speed = 0;

    int font = cv::FONT_HERSHEY_SIMPLEX;
    double fontscale = 1.3;

    std::string mystr_score;
    std::string mystr_life;

    FruitProcess* fruit_pro = new FruitProcess();

    cv::Mat img = fruit_pro->getBackground(); //3채널

    cv::Point location_text = cv::Point(img.rows - 180, 30);
    cv::Point location_life(img.rows - 180, 60); //찬스 텍스트 위치
    cv::Point location_level(0, 40); //레벨 위치    
   
    //4(BGRA) 채널 이미지 읽기
    cv::Mat img_watermelon = cv::imread("images\\watermelon.png", cv::IMREAD_UNCHANGED);
    cv::Mat img_banana = cv::imread("images\\banana.png", cv::IMREAD_UNCHANGED);

    cv::resize(img_watermelon, img_watermelon, cv::Size(Width, Height));
    cv::resize(img_banana, img_banana, cv::Size(Width, Height));

    std::vector<cv::Mat> channels_wm, channels_bm;
    cv::split(img_watermelon, channels_wm);
    cv::split(img_banana, channels_bm);

    //Alpha(투명도) 채널을 마스크로 사용
	cv::Mat mask_wm = channels_wm[3];
    cv::Mat mask_bm = channels_bm[3];

	cv::Mat img_wm, img_ba, img_wrotated, img_brotated;

    //4채널을 3채널로 변환
	cv::cvtColor(img_watermelon, img_wm, cv::COLOR_BGRA2BGR);
    cv::cvtColor(img_banana, img_ba, cv::COLOR_BGRA2BGR);

    fruit_pro->startScreen(fontscale);

    int xpos_result = 0;
    int ypos_result = 0;

    while (1)
    {        
        img = cv::Scalar::all(0); //배경 이미지 초기화(이걸 넣어줘야 잔상이 안 남음)

        cv::RNG rng(cv::getTickCount());

        cv::Mat rotatedMask_wm, rotatedMask_ba;

        cv::Mat wm = cv::getRotationMatrix2D(cv::Point(50, 50), -rotate, 1); //수박 이미지 시계방향으로 회전
        cv::Mat ba = cv::getRotationMatrix2D(cv::Point(50, 50), rotate, 1); //바나나 이미지 반시계방향으로 회전

		cv::warpAffine(img_wm, img_wrotated, wm, img_watermelon.size()); // 수박 이미지 회전
		cv::warpAffine(mask_wm, rotatedMask_wm, wm, mask_wm.size()); // 수박 마스크 회전

        cv::warpAffine(img_ba, img_brotated, ba, img_banana.size()); // 바나나 이미지 회전
		cv::warpAffine(mask_bm, rotatedMask_ba, ba, mask_bm.size()); // 바나나 마스크 회전
        
        if (fruit_pro->getScore() < EASY)
        {
			fruit_pro->gameProcess(img_wrotated, img_brotated, rotatedMask_wm, rotatedMask_ba, location_level, 20, 1);
        }

        if (fruit_pro->getScore() >= EASY)
        {
             fruit_pro->gameProcess(img_wrotated, img_brotated, rotatedMask_wm, rotatedMask_ba, location_level, 30, 2);
        }

        mystr_score = std::to_string(fruit_pro->getScore());
        mystr_life = std::to_string(fruit_pro->getLife());

        if (fruit_pro->getScore() >= CLEAR) // 600점 달성 시
        {
            fruit_pro->gameClear(font, fontscale, mystr_score, location_text);
            cv::setMouseCallback("image", FruitProcess::mouse_callback, fruit_pro);

            if (fruit_pro->getRetryFlag())
            {
                fruit_pro->initParameters();
                continue;
            }
            if (fruit_pro->getCancelFlag())
            {
                fruit_pro->initParameters();
                cv::destroyAllWindows();
                delete fruit_pro;
                break;
            }
            continue; // 마우스 선택 이전엔 이미지 출력 X
        }
        if (fruit_pro->getLife() < 1) // 기회가 1 밑으로 떨어질시
        {
            fruit_pro->gameLose(font, fontscale, mystr_score, location_text);
            cv::setMouseCallback("image", FruitProcess::mouse_callback, fruit_pro);

            if (fruit_pro->getRetryFlag())
            {
				fruit_pro->initParameters();
                continue;
            }
            if (fruit_pro->getCancelFlag())
            {
                fruit_pro->initParameters();
                cv::destroyAllWindows();
                delete fruit_pro;
                break;
            }
			continue; // 마우스 선택 이전엔 이미지 출력 X
        }

        cv::putText(img, "Score:"+ mystr_score, location_text, font, fontscale, cv::Scalar(0, 255, 0)); // text 점수가 몇인지 출력
        cv::putText(img, "Chance:"+ mystr_life, location_life, font, fontscale, cv::Scalar(0, 255, 255)); // 기회 점수가 몇인지 출력

        rotate += 30; // 30만큼 누적해서 회전
        imshow("image", img); // 회전된 이미지들을 다 출력

        cv::setMouseCallback("image", FruitProcess::mouse_callback, fruit_pro);
        cv::waitKey(100); //0.1초마다 시퀀스 동작
    }
}