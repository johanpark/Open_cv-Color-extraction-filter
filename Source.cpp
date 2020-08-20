#include"opencv2/opencv.hpp"
using namespace cv;
using namespace std;

int threshold1 = 30;
Vec3b lower_blue1, upper_blue1, lower_blue2, upper_blue2, lower_blue3, upper_blue3;
Mat img_color;

void mouse_callback(int event, int x, int y, int flags, void* param)
{
    if (event == EVENT_LBUTTONDOWN)
    {
        Vec3b color_pixel = img_color.at<Vec3b>(y, x); // 클릭한 곳의 영상픽셀값 추출
        Mat bgr_color = Mat(1, 1, CV_8UC3, color_pixel); // 8 unsinged, 3Chnnel
        Mat hsv_color;
        cvtColor(bgr_color, hsv_color, COLOR_BGR2HSV); // HSV영상으로 컨버트
        int hue = hsv_color.at<Vec3b>(0, 0)[0]; // 영상의 Hue값 넣어

        //픽셀값과 유사한 픽셀값 범위 지정
        if (hue < 10) //만약 Hue값이 10미만일 경우
        {
            lower_blue1 = Vec3b(hue - 10 + 180, threshold1, threshold1);
            upper_blue1 = Vec3b(180, 255, 255);
            lower_blue2 = Vec3b(0, threshold1, threshold1);
            upper_blue2 = Vec3b(hue, 255, 255);
            lower_blue3 = Vec3b(hue, threshold1, threshold1);
            upper_blue3 = Vec3b(hue + 10, 255, 255);
        }
        else if (hue > 170) // 170이상인 경우
        {
            lower_blue1 = Vec3b(hue, threshold1, threshold1);
            upper_blue1 = Vec3b(180, 255, 255);
            lower_blue2 = Vec3b(0, threshold1, threshold1);
            upper_blue2 = Vec3b(hue + 10 - 180, 255, 255);
            lower_blue3 = Vec3b(hue - 10, threshold1, threshold1);
            upper_blue3 = Vec3b(hue, 255, 255);
        }
        else
        {          
            lower_blue1 = Vec3b(hue, threshold1, threshold1);
            upper_blue1 = Vec3b(hue + 10, 255, 255);
            lower_blue2 = Vec3b(hue - 10, threshold1, threshold1);
            upper_blue2 = Vec3b(hue, 255, 255);
            lower_blue3 = Vec3b(hue - 10, threshold1, threshold1);
            upper_blue3 = Vec3b(hue, 255, 255);
        }
    }
}



int main()
{
    namedWindow("img_color");
    setMouseCallback("img_color", mouse_callback);

    Mat img_hsv, gray, dst;

    VideoCapture cap(0); //웹캠 감지
    if (!cap.isOpened()) {
        cout << "카메라를 열 수 없습니다." << endl;
        return -1;
    }

    while (1)
    {
        cap.read(img_color);
        cvtColor(img_color, img_hsv, COLOR_BGR2HSV); //원본 영상 HSV영상으로 변환
        cvtColor(img_color, gray, COLOR_BGR2GRAY); //GRAY영상으로 변환
        cvtColor(gray, gray, COLOR_GRAY2BGR); //영상을 합치기 위해 다시 RGB영상으로 변환

        //범위 값으로 HSV이미지 마스크 생성
        Mat img_mask1, img_mask2, img_mask3, img_mask;
        inRange(img_hsv, lower_blue1, upper_blue1, img_mask1); // 범위 안이면 0, 범위 밖이면 1
        inRange(img_hsv, lower_blue2, upper_blue2, img_mask2);
        inRange(img_hsv, lower_blue3, upper_blue3, img_mask3);
        img_mask = img_mask1 | img_mask2 | img_mask3; // Mask 병합, 값에 따른 영역추출

        Mat img_result, gray_area;
        bitwise_and(img_color, img_color, img_result, img_mask); // Mask범위 내에서 두개의 영상의 비트연산AND결과값
        gray.copyTo(gray_area, img_mask); //밝기를 맞추기 위한 색추출한 부분만큼의 그레이영역 추출 
        gray = gray - gray_area; // 원본 그레이 영상과 색추출 부분만큼의 그레이 영역을 뺀 영상으로 만듬.
        dst = (img_result + gray); //그레이-컬러 영상과 색추출된 컬러 영상 합침

        imshow("img_color", img_color); //원본 영상
        //imshow("img_mask", img_mask); //추출 마스크
        //imshow("img_result", img_result); //추출된 색 마스크
        //imshow("gray_area", gray_area); //추출된 색만큼의 그레이 영역
        imshow("img_dst", dst);

        if (waitKey(1) > 0) // 키눌러서 종료
            break;
    }
    return 0;
}