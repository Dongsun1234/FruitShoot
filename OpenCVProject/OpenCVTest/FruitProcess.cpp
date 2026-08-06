#include <opencv2/opencv.hpp>
#include "FruitProcess.h"
#include <algorithm>
#include <string.h>


FruitProcess::FruitProcess()
{
    count_score = 0;
	count_life = 3;

    retryFlag = false;
    cancelFlag = false;
    resumeFlag = false;

    img = cv::Mat(570, 600, CV_8UC3, cv::Scalar::all(0));
   
	std::memset(random_xpos, 0, sizeof(random_xpos));
    std::memset(random_ypos, 0, sizeof(random_ypos));

    img_watermelon_cut = cv::imread("images\\watermelon_cut.png");
    img_banana_cut = cv::imread("images\\banana_cut.png");
    img_click = cv::imread("images\\bomb.png", cv::IMREAD_UNCHANGED);

    cv::resize(img_watermelon_cut, img_watermelon_cut, cv::Size(Width, Height));
    cv::resize(img_banana_cut, img_banana_cut, cv::Size(Width, Height));
    cv::resize(img_click, img_click, cv::Size(50, 50));

    std::vector<cv::Mat> channels_bomb;

    cv::split(img_click, channels_bomb);
    mask_bomb = channels_bomb[3];

    cv::cvtColor(img_click, img_click, cv::COLOR_BGRA2BGR);
}

void  FruitProcess::initParameters()
{
    count_score = 0;
    count_life = 3;
    retryFlag = false;
    cancelFlag = false;
    resumeFlag = false;

    std::memset(random_xpos, 0, sizeof(random_xpos));
    std::memset(random_ypos, 0, sizeof(random_ypos));
}

bool FruitProcess::getRetryFlag()
{
    return retryFlag;
}

bool FruitProcess::getCancelFlag()
{
    return cancelFlag;
}

cv::Mat FruitProcess::getBackground()
{
    return img;
}

void FruitProcess::miunsLife()
{
    count_life--;
}

int FruitProcess::getScore()
{
    return count_score;
}

int FruitProcess::getLife()
{
    return count_life;
}

void FruitProcess::getClick()
{
    count_score += 30;
    cv::imshow("image", img);
    cv::waitKey(30);
}

void FruitProcess::startScreen(double fontscale)
{
    cv::Mat img_start = cv::imread("images\\start.png");
    cv::resize(img_start, img_start, cv::Size(img.cols, img.rows));
    cv::cvtColor(img_start, img_start, cv::COLOR_BGR2GRAY);
    cv::threshold(img_start, img_start, 200, 255, cv::THRESH_BINARY_INV);
    cv::putText(img_start, "Press on your key", cv::Point(170, 260), 5, fontscale, cv::Scalar::all(255));

    while (1)
    {
        int key = cv::waitKey(0);
        imshow("image", img_start);

        if (key != -1)
            break;
    }
}
void FruitProcess::gameProcess(cv::Mat img_wrotated, cv::Mat img_brotated, cv::Mat rotatedMask_wm, cv::Mat rotatedMask_bm, cv::Point location_level, int speed, int level)
{
    cv::RNG rng(cv::getTickCount());

    for (int i = 0; i < 5; i++)
    {
        int xpos_speed = rng.uniform(2, speed); //랜덤으로 스피드 
        int ypos_speed = rng.uniform(2, speed);

        random_xpos[i] += xpos_speed; // 스피드를 랜덤 배열에 저장하면서 누적
        random_ypos[i] += ypos_speed;

        //cols는 열의 개수=width, rows는 행의 개수=height
        int xpos_result = std::clamp(random_xpos[i] + Width, 0, img.cols); //x좌표 + 이미지 너비가 최대 좌표를 넘어갈 시 0~img.cols 사이로 제한
        int ypos_result = std::clamp(random_ypos[i] + Height, 0, img.rows); //y좌표 + 이미지 높이가 최대 좌표를 넘어갈 시 0~img.rows 사이로 제한

        if (xpos_result == img.cols || ypos_result == img.rows) //x좌표 + 이미지 너비가 최대 좌표를 넘어갈 시
        {
            random_xpos[i] = 0; // 좌표 0으로 초기화
            random_ypos[i] = 0;
            miunsLife(); // 목숨 깍임
        }

        if (i % 2 == 0)
        {
            img_wrotated.copyTo(img(cv::Rect(random_xpos[i], Height * i, Width, Height)), rotatedMask_wm); //누적된 x좌표 및 y좌표에 수박 이미지 출력(마스크된 부분만 적용)
            img_wrotated.copyTo(img(cv::Rect(Width * i, random_ypos[i], Width, Height)), rotatedMask_wm);
        }
        else
        {
            img_brotated.copyTo(img(cv::Rect(random_xpos[i], Height * i, Width, Height)), rotatedMask_bm); //누적된 x좌표 및 y좌표에 바나나 이미지 출력(마스크된 부분만 적용)
            img_brotated.copyTo(img(cv::Rect(Width * i, random_ypos[i], Width, Height)), rotatedMask_bm);
        }
    }
    cv::putText(img, "Level:"+ std::to_string(level), location_level, 5, 2, cv::Scalar(255, 255, 0));

}

void FruitProcess::btnDraw(cv::Mat img)
{
    cv::rectangle(img, cv::Rect(100, 400, 150, 100), cv::Scalar::all(128), cv::FILLED);
    cv::putText(img, "Retry", cv::Point(120, 460), cv::FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 0), 3);
    cv::rectangle(img, cv::Rect(350, 400, 160, 100), cv::Scalar::all(128), cv::FILLED);
    cv::putText(img, "Cancel", cv::Point(350, 460), cv::FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 0), 3);

	resumeFlag = true;
}

void FruitProcess::gameClear(int font, double fontscale, std::string mystr_score, cv::Point location_text)
{
    cv::Mat img_clear = cv::imread("images\\win.png");
    cv::resize(img_clear, img_clear, cv::Size(img.cols, img.rows));
    cv::putText(img_clear, "Score:" + mystr_score, location_text, font, fontscale, cv::Scalar(0, 255, 0));

    btnDraw(img_clear);

    while (1)
    {        
        cv::imshow("image", img_clear);
        cv::waitKey(1);

        if (!retryFlag || !cancelFlag)
            break;
    }
}

void FruitProcess::gameLose(int font, double fontscale, std::string mystr_score, cv::Point location_text)
{

    cv::Mat img_end = cv::imread("images\\lose.png");
    cv::resize(img_end, img_end, cv::Size(img.cols, img.rows));
    cv::putText(img_end, "Score:" + mystr_score, location_text, font, fontscale, cv::Scalar(0, 255, 0));

    btnDraw(img_end);	

    while (1)
    {
        cv::imshow("image", img_end);
        cv::waitKey(1);
        if (!retryFlag || !cancelFlag)
            break;
    }
}

void FruitProcess::mouse_callback(int event, int x, int y, int flag, void* userdata)
{

    FruitProcess* self = static_cast<FruitProcess*>(userdata);

    bool isCut = false;

    switch (event) {

    case cv::EVENT_LBUTTONDOWN:
        if (self->resumeFlag)
        {
            if (x >= 100 && x <= 250 && y >= 400 && y <= 500) // Retry 버튼 영역 안을 클릭하면                
                self->retryFlag = true;
            if (x >= 350 && x <= 510 && y >= 400 && y <= 500) // Cancel 버튼 영역 안을 클릭하면
                self->cancelFlag = true;
        }
        else
        {
            for (int i = 0; i < 5; i++)
            {          
                if (x >= self->random_xpos[i] && x <= self->random_xpos[i] + Width && y >= Height * i && y <= Height * i + Height)
                {
                    if (i % 2 == 0)
                        self->img_watermelon_cut.copyTo(self->img(cv::Rect(self->random_xpos[i], (Height * i), Width, Height))); // x축에 의해 카피
                    else
                        self->img_banana_cut.copyTo(self->img(cv::Rect(self->random_xpos[i], (Height * i), Width, Height))); // x축에 의해 카피

                    self->random_xpos[i] = 0; // 컷이 되면 x축 0으로 초기화

                    isCut = true;
                }
                if (x >= Width * i && x <= Width * i + Width && y >= self->random_ypos[i] && y <= self->random_ypos[i] + Height)
                {
                    if (i % 2 == 0)
                        self->img_watermelon_cut.copyTo(self->img(cv::Rect(Width * i, self->random_ypos[i], Width, Height))); // y축에 의해 카피
                    else
                        self->img_banana_cut.copyTo(self->img(cv::Rect(Width * i, self->random_ypos[i], Width, Height))); // y축에 의해 카피

                    self->random_ypos[i] = 0; // 컷이 되면 y축 0으로 초기화

                    isCut = true;
                }
            }

            self->img_click.copyTo(self->img(cv::Rect(x - 50/2, y- 50/2, 50, 50)),self->mask_bomb);
        }

        if(isCut)
            self->getClick(); // 사물을 클릭하면 +30 및 이미지 출력
        else
			self->miunsLife(); // 사물을 클릭하지 않으면 목숨 -1


        std::cout << "EVENT_LBUTTONDOWN: " << x << ", " << y << std::endl;
        std::cout << self->count_score << std::endl;
        break;

    case cv::EVENT_LBUTTONUP:

        std::cout << "EVENT_LBUTTONUP: " << x << ", " << y << std::endl;
        break;
    }
}
