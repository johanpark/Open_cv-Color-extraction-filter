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
        Vec3b color_pixel = img_color.at<Vec3b>(y, x); // Ŭ���� ���� �����ȼ��� ����
        Mat bgr_color = Mat(1, 1, CV_8UC3, color_pixel); // 8 unsinged, 3Chnnel
        Mat hsv_color;
        cvtColor(bgr_color, hsv_color, COLOR_BGR2HSV); // HSV�������� ����Ʈ
        int hue = hsv_color.at<Vec3b>(0, 0)[0]; // ������ Hue�� �־�

        //�ȼ����� ������ �ȼ��� ���� ����
        if (hue < 10) //���� Hue���� 10�̸��� ���
        {
            lower_blue1 = Vec3b(hue - 10 + 180, threshold1, threshold1);
            upper_blue1 = Vec3b(180, 255, 255);
            lower_blue2 = Vec3b(0, threshold1, threshold1);
            upper_blue2 = Vec3b(hue, 255, 255);
            lower_blue3 = Vec3b(hue, threshold1, threshold1);
            upper_blue3 = Vec3b(hue + 10, 255, 255);
        }
        else if (hue > 170) // 170�̻��� ���
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

    VideoCapture cap(0); //��ķ ����
    if (!cap.isOpened()) {
        cout << "ī�޶� �� �� �����ϴ�." << endl;
        return -1;
    }

    while (1)
    {
        cap.read(img_color);
        cvtColor(img_color, img_hsv, COLOR_BGR2HSV); //���� ���� HSV�������� ��ȯ
        cvtColor(img_color, gray, COLOR_BGR2GRAY); //GRAY�������� ��ȯ
        cvtColor(gray, gray, COLOR_GRAY2BGR); //������ ��ġ�� ���� �ٽ� RGB�������� ��ȯ

        //���� ������ HSV�̹��� ����ũ ����
        Mat img_mask1, img_mask2, img_mask3, img_mask;
        inRange(img_hsv, lower_blue1, upper_blue1, img_mask1); // ���� ���̸� 0, ���� ���̸� 1
        inRange(img_hsv, lower_blue2, upper_blue2, img_mask2);
        inRange(img_hsv, lower_blue3, upper_blue3, img_mask3);
        img_mask = img_mask1 | img_mask2 | img_mask3; // Mask ����, ���� ���� ��������

        Mat img_result, gray_area;
        bitwise_and(img_color, img_color, img_result, img_mask); // Mask���� ������ �ΰ��� ������ ��Ʈ����AND�����
        gray.copyTo(gray_area, img_mask); //��⸦ ���߱� ���� �������� �κи�ŭ�� �׷��̿��� ���� 
        gray = gray - gray_area; // ���� �׷��� ����� ������ �κи�ŭ�� �׷��� ������ �� �������� ����.
        dst = (img_result + gray); //�׷���-�÷� ����� ������� �÷� ���� ��ħ

        imshow("img_color", img_color); //���� ����
        //imshow("img_mask", img_mask); //���� ����ũ
        //imshow("img_result", img_result); //����� �� ����ũ
        //imshow("gray_area", gray_area); //����� ����ŭ�� �׷��� ����
        imshow("img_dst", dst);

        if (waitKey(1) > 0) // Ű������ ����
            break;
    }
    return 0;
}