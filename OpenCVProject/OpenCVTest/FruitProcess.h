#pragma once
#include <opencv2/opencv.hpp>

#define Width 100
#define Height 100

class FruitProcess
{
private:
    int count_score;
    int count_life;
    cv::Mat img_watermelon_cut, img_banana_cut;
    cv::Mat img_click, mask_bomb;
    bool retryFlag;
    bool cancelFlag;
    bool resumeFlag;

    cv::Mat img;

public:
    int random_xpos[5];
    int random_ypos[5];

public:
    FruitProcess();
    cv::Mat getBackground();
    int getScore();
    void getClick();
    int getLife();
    void miunsLife();
    bool getRetryFlag();
    bool getCancelFlag();
	void initParameters();
    void startScreen(double fontscale);
    void gameProcess(cv::Mat img_wrotated, cv::Mat img_brotated, cv::Mat rotatedMask_wm, cv::Mat rotatedMask_bm, cv::Point location_level, int speed, int level);
    void btnDraw(cv::Mat img);
    void gameClear(int font, double fontscale, std::string mystr_score, cv::Point location_text);
    void gameLose(int font, double fontscale, std::string mystr_score, cv::Point location_text);
    static void mouse_callback(int event, int x, int y, int flag, void* userdata);
};