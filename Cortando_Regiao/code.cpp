#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
    char *image_name = argv[1];
    char *img_write = argv[2];
    Mat src = imread(image_name, IMREAD_COLOR);
    namedWindow("Original Image", WINDOW_NORMAL);
    imshow("Original Image", src);

    Rect roi;

    roi.x = 0;
    roi.y = src.rows*0.5 - 100;
    roi.width = src.cols*0.5;
    roi.height = src.rows*0.5 + 100;

    Mat crop_img = src(roi);
    cout << img_write << endl;
    imwrite(img_write,crop_img);

    namedWindow("Cropped Img",WINDOW_NORMAL);  
    imshow("Cropped Img", crop_img);

    waitKey(0);
    destroyAllWindows();

    return 0;
}
