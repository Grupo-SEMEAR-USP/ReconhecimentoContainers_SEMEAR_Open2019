#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
    char *image_name = argv[1];
    Mat original_image = imread(image_name, IMREAD_COLOR);
    namedWindow("Original Image", WINDOW_NORMAL);
    imshow("Original Image", original_image);

    Mat gauss_image, hsv_image, imgThresholded;
    GaussianBlur(original_image, gauss_image, Size(5, 5), 0, 0, BORDER_DEFAULT);
    namedWindow("Gaussian Filter", WINDOW_NORMAL);
    imshow("Gaussian Filter", gauss_image);

    cvtColor(gauss_image, hsv_image, CV_RGB2HSV);

    namedWindow("Control", WINDOW_NORMAL);
    resizeWindow("Control",Size(500,800));

    int iLowH = 0;
    int iHighH = 0;

    int iLowS = 0;
    int iHighS = 0;

    int iLowV = 0;
    int iHighV = 0;

    createTrackbar("LowH", "Control", &iLowH, 255);
    createTrackbar("HighH", "Control", &iHighH, 255);

    createTrackbar("LowS", "Control", &iLowS, 255);
    createTrackbar("HighS", "Control", &iHighS, 255);

    createTrackbar("LowV", "Control", &iLowV, 255);
    createTrackbar("HighV", "Control", &iHighV, 255);

    while (true)
    {
        inRange(hsv_image, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded);
        imshow("Control", imgThresholded);
        if(waitKey(30) == 27)
            break;
    }

    destroyAllWindows();

    return 0;
}
