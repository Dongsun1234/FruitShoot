#include <iostream>
#include <opencv2/opencv.hpp>

#define height 570
#define width 500

int rotate = 0;

cv::Mat img = cv::Mat(height, width, CV_8UC3, cv::Scalar(0, 0, 0));
cv::Mat img_set = cv::Mat(height, width, CV_8UC3, cv::Scalar(0, 0, 0));

cv::Mat img_watermelon_cut = cv::imread("C:\\Users\\rkeld\\LDS\\OPENCV\\workspace\\OpenCVProject\\data\\wcut.png");
cv::Mat img_banana_cut = cv::imread("C:\\Users\\rkeld\\LDS\\OPENCV\\workspace\\OpenCVProject\\data\\bcut.png");

cv::Point location_text(width - 170, 30);
cv::Point location_text_score(width - 60, 30);

int font = cv::FONT_HERSHEY_SIMPLEX;
int fontscale = 1.5;

int count_score = 0;
int count_life = 3;

char mystr_score[50] = { '0' };
char mystr_life[5] = { '0' };

int random[5] = { 0 };

void Clickget()
{   
    count_score += 30;
    imshow("image", img);
    cv::waitKey(50);
}

void start_screen()
{
    cv::Mat img_start = cv::imread("C:\\Users\\rkeld\\LDS\\OPENCV\\workspace\\OpenCVProject\\data\\start.png");
    cv::resize(img_start, img_start, cv::Size(height, width));
    cv::cvtColor(img_start, img_start, cv::COLOR_BGR2GRAY);
    cv::threshold(img_start, img_start, 200, 255, cv::THRESH_BINARY_INV);
    cv::putText(img_start, "Press on your key", cv::Point(170, 260), 5, fontscale, cv::Scalar(255, 255, 255));

    while (1)
    {
        int key = cv::waitKey(0);
        imshow("image", img_start);

        if (key != -1)
            break;
    }
}

void game_clear()
{
    cv::Mat clear = cv::imread("C:\\Users\\rkeld\\LDS\\OPENCV\\workspace\\OpenCVProject\\data\\clear.png");
    cv::resize(clear, clear, cv::Size(height, width));
    clear.copyTo(img);
    cv::putText(img, "Score: ", location_text, font, fontscale, cv::Scalar(0, 255, 0));
    cv::putText(img, mystr_score, location_text_score, font, fontscale, cv::Scalar(0, 255, 0));

    imshow("image", img);
    cv::waitKey(0);
}

void game_lose()
{
    
    cv::Mat end = cv::imread("C:\\Users\\rkeld\\LDS\\OPENCV\\workspace\\OpenCVProject\\data\\end.png");
    cv::resize(end, end, cv::Size(height, width));
    end.copyTo(img_set);
    cv::putText(img_set, "Score: ", location_text, font, fontscale, cv::Scalar(0,255,0));
    cv::putText(img_set, mystr_score, location_text_score, font, fontscale, cv::Scalar(0, 255, 0));

    imshow("image", img_set);
    cv::waitKey(0);
}

void mouse_callback(int event, int x, int y, int flag, void* userdata)
{          
    int i = 0;
   
    switch (event) {
       
    case cv::EVENT_LBUTTONDOWN:                         
        cv::resize(img_watermelon_cut, img_watermelon_cut, cv::Size(120, 100));
        cv::resize(img_banana_cut, img_banana_cut, cv::Size(120, 100));      
        if ((x >= random[0] && x <= random[0] + 100 && y >= 70 && y < 170))
        {
            img_watermelon_cut.copyTo(img(cv::Rect(random[0] , 70, 120, 100))); // x축에 의해 카피
            random[0] = 0; // 컷이 되면 x축 0으로 초기화
            Clickget(); // 사물을 클릭하면 +30 및 이미지 출력
        }
        else if ((x >= random[1] && x <= random[1] + 100 && y >= 170 && y < 270))
        {
            img_banana_cut.copyTo(img(cv::Rect(random[1] , 170, 120, 100)));
            random[1] = 0;
            Clickget();
        }
        else if ((x >= random[2] && x <= random[2] + 100 && y >= 270 && y < 370))
        {
            img_watermelon_cut.copyTo(img(cv::Rect(random[2] , 270, 120, 100)));
            random[2] = 0;
            Clickget();
        }
        else if ((x >= random[3] && x <= random[3] + 100 && y >= 370 && y < 470))
        {
            img_banana_cut.copyTo(img(cv::Rect(random[3], 370, 120, 100)));
            random[3] = 0;
            Clickget();
        }
        else if ((x >= random[4] && x <= random[4] + 100 && y >= 470 && y < 570))
        {
            img_watermelon_cut.copyTo(img(cv::Rect(random[4], 470, 120, 100)));
            random[4] = 0;
            Clickget();
        }
        else
        {
            count_score -=30;
            if (count_score < 0)
            {
                game_lose();
                break;
            }
        }
        std::cout << "EVENT_LBUTTONDOWN: " << x << ", " << y << std::endl;
        std::cout << count_score << std::endl;
        break;
        
    case cv::EVENT_LBUTTONUP:
        
        std::cout << "EVENT_LBUTTONUP: " << x << ", " << y << std::endl;       
            break;    
    }    
}

int main()
{          
    cv::Point location_life(width - 170, 60); //찬스 텍스트 위치
    cv::Point location_life_score(width - 40, 60); //찬스 숫자 위치
    cv::Point location_level(0, 40); //레벨 위치    
   
    cv::Mat img_watermelon = cv::imread("C:\\Users\\rkeld\\LDS\\OPENCV\\workspace\\OpenCVProject\\data\\watermelon.png");
    cv::Mat img_banana = cv::imread("C:\\Users\\rkeld\\LDS\\OPENCV\\workspace\\OpenCVProject\\data\\banana.png");
       
    cv::resize(img_watermelon, img_watermelon, cv::Size(100, 100));
    cv::resize(img_banana, img_banana, cv::Size(100, 100));
             
    cv::Mat wm, img_wrotated, img_wrotated1;
    cv::Mat bm, img_brotated, img_brotated1;

    start_screen();

    while (1)
    {
        
        int j = 0;        
        img = cv::Scalar(0, 0, 0);

        int random_speed;

        wm = cv::getRotationMatrix2D(cv::Point(50, 50), -rotate, 1); //수박 이미지 시계방향으로 회전
        bm = cv::getRotationMatrix2D(cv::Point(50, 50), -rotate, 1);//바나ㅏ나 이미지 시계방향으로 회전

        cv::warpAffine(img_watermelon, img_wrotated, wm, cv::Size(100, 100)); // 이미지 회전
        cv::warpAffine(img_banana, img_brotated, bm, cv::Size(100, 100)); // 이미지 회전

        if (count_score < 300) //점수가 300점 미만이면
        {
            for (j = 0; j < 5; j++)
            {
                random_speed = rand() % 30; //랜덤으로 스피드 

                random[j] += random_speed; // 스피드를 랜덤 배열에 저장하면서 누적

                if ((random[j] + 100 > 500)) // 최대 좌표가 넘어갈 시 
                {
                    random[j] = 0; // 좌표 0으로 초기화
                    count_life--; // 목숨 깍임
                }
                if (j % 2 == 0)
                    img_wrotated.copyTo(img(cv::Rect(random[j], (j * 100) + 70, 100, 100))); //누적된 x좌표 및 y좌표에 수박 이미지 출력
                else if (j % 2 == 1)
                    img_brotated.copyTo(img(cv::Rect(random[j], (j * 100) + 70, 100, 100))); //누적된 x좌표 및 y좌표에 바나나 이미지 출력
            }
            cv::putText(img, "Level:1", location_level, 5, 2, cv::Scalar(255, 255, 0)); // 레벨1 텍스트 삽입

        }
        if (count_score >= 300) //점수가 300점 이상이면
        {
            for (j = 0; j < 5; j++)
            {
                random_speed = rand() % 45; //랜덤으로 스피드 
                random[j] += random_speed;

                if ((random[j] + 100 > 500))
                {
                    random[j] = 0;
                    count_life--;
                }
                if (j % 2 == 0)
                    img_wrotated.copyTo(img(cv::Rect(random[j], (j * 100) + 70, 100, 100)));
                else if (j % 2 == 1)
                    img_brotated.copyTo(img(cv::Rect(random[j], (j * 100) + 70, 100, 100)));
            }
            cv::putText(img, "Level:2", location_level, 5, 2, cv::Scalar(255, 255, 0)); // 레벨2 텍스트 삽입
        }

        _itoa_s(count_score, mystr_score, 10); // int형 점수-> 정수를 문자로 저장
        _itoa_s(count_life, mystr_life, 10); // int 목숨-> 정수를 문자로 저장

        if (count_score >= 600)
        {
            game_clear();
        }       


        cv::line(img, cv::Point(470, 62), cv::Point(470, 560), cv::Scalar(0, 0, 255), 2); //한계선 출력

        cv::putText(img, "Score: ", location_text, font, fontscale, cv::Scalar(0, 255, 0)); 
        cv::putText(img, mystr_score, location_text_score, font, fontscale, cv::Scalar(0, 255, 0)); // text 점수가 몇인지 출력

        cv::putText(img, "Chance: ", location_life, font, fontscale, cv::Scalar(0, 255, 255));
        cv::putText(img, mystr_life, location_life_score, font, fontscale, cv::Scalar(0, 255, 255)); // 기회 점수가 몇인지 출력

        rotate += 2; // 2만큼 누적해서 회전
        imshow("image", img); // 그 이미지들을 다 출력

        cv::setMouseCallback("image", mouse_callback);

        if (count_life < 1)
        {
            game_lose();
            break;
        }
        cv::waitKey(100);
    }
}