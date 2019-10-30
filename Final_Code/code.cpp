#include <iostream>
#include <math.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

#define PI 3.1415926


void cropp_ROI(Mat& src, Mat& dst, int x, int y, int width, int heigth);
void InversePerspectiveMapping(const Mat& src, Mat& tr1, int alpha_, int beta_, int gamma_, int f_, int dist_);
void select_Channel(Mat &in, Mat &out, Scalar low, Scalar high);
void calculate_pixels(Mat &in, vector<Point2i> &points_rigth, vector<Point2i> &points_left);
void calculate_containers(Mat &in, vector<Point2i> points, vector<Point2i> &key_points, int &n_containers, Scalar color);
void identify_containers(vector<Point2i> blue_pointskey, vector<Point2i> green_pointskey, vector<Point2i> red_pointskey, int n_blue, int n_green, int n_red, int pos[]);
int comparation(int elementA1, int elementA2, int elementB1, int elementB2, int elementC1, int elementC2);

/*Valores do Range para selecionamento das cores*/
Scalar blue_low = Scalar(0, 96, 23);
Scalar blue_high = Scalar(14, 236, 123);

Scalar green_low = Scalar(31, 42, 9);
Scalar green_high = Scalar(76, 214, 101);

Scalar red_low = Scalar(109, 118, 0);
Scalar red_high = Scalar(127, 255, 255);

 int main( int argc, char** argv )
 {
    /*Lendo e mostrando na tela imagem original*/
    char *image_name = argv[1];
    Mat original_image = imread(image_name, IMREAD_COLOR);
    namedWindow("Original Image", WINDOW_NORMAL);
    imshow("Original Image", original_image);

    /*Adquirindo ROI pilha a esquerda*/
    Mat cropped_left;
    int x_left = 0;
    int y_left = original_image.rows*0.5 - 100;
    int width_left = original_image.cols*0.5;
    int heigth_left = original_image.rows*0.5 + 100;

    cropp_ROI(original_image,cropped_left,x_left,y_left,width_left,heigth_left);
    //namedWindow("Cropped Left", WINDOW_NORMAL);
    //imshow("Cropped Left", cropped_left);

    /*Adquirindo ROI containers pilha a direita*/
    Mat cropped_rigth;
    int x_rigth = original_image.cols*0.5;
    int y_rigth = original_image.rows*0.5 - 100;
    int width_rigth = original_image.cols*0.5;
    int heigth_rigth = original_image.rows*0.5 + 100;

    cropp_ROI(original_image,cropped_rigth,x_rigth,y_rigth,width_rigth,heigth_rigth);
    //namedWindow("Cropped Rigth", WINDOW_NORMAL);
    //imshow("Cropped Rigth", cropped_rigth);

    /* Mudando perspectiva a esquerda*/
    Mat persp_left;
    int alpha_l = 101, beta_l = 148, gamma_l = 90;
    int f_l = 500, dist_l = 500;

    InversePerspectiveMapping(cropped_left, persp_left,alpha_l,beta_l,gamma_l, f_l,dist_l);

    //namedWindow("Perspective Left", WINDOW_NORMAL);
    //imshow("Perspective Left", persp_left);

    /* Mudando perspectiva a direita*/
    Mat persp_rigth;
    int alpha_r = 99, beta_r = 30, gamma_r = 90;
    int f_r = 500, dist_r = 500;

    InversePerspectiveMapping(cropped_rigth, persp_rigth,alpha_r,beta_r,gamma_r, f_r,dist_r);

    //namedWindow("Perspective Rigth", WINDOW_NORMAL);
    //imshow("Perspective Rigth", persp_rigth);

    /* Corte final a esquerda*/

    Mat crop_fin_l;
    int x_fin_l = 95;
    int y_fin_l = 100;
    int width_fin_l = persp_left.cols - x_fin_l;
    int heigth_fin_l = persp_left.rows - y_fin_l;

    cropp_ROI(persp_left,crop_fin_l,x_fin_l,y_fin_l,width_fin_l,heigth_fin_l);
    namedWindow("Cropped Final Left", WINDOW_NORMAL);

    /* Corte final a direita*/

    Mat crop_fin_r;
    int x_fin_r = 0;
    int y_fin_r = 100;
    int width_fin_r = persp_rigth.cols - x_fin_r;
    int heigth_fin_r = persp_rigth.rows - y_fin_r;

    cropp_ROI(persp_rigth,crop_fin_r,x_fin_r,y_fin_r,width_fin_r,heigth_fin_r);
    namedWindow("Cropped Final Rigth", WINDOW_NORMAL);

    /*Selecao de cores Pilha esquerda*/

    /*Declarando as matrizes que guardaram as imagens binárias geradas na select_Channel
    contendo pixels brancos aonde recenheceu as cores*/
    Mat binary_image_blue_l, binary_image_green_l, binary_image_red_l;
    select_Channel(crop_fin_l, binary_image_green_l, green_low, green_high);
    select_Channel(crop_fin_l, binary_image_blue_l, blue_low, blue_high);
    select_Channel(crop_fin_l, binary_image_red_l, red_low, red_high);

    /*Mostrando na tela as imagens binárias de cada cor reconhecida*/
    /*namedWindow("Binary Blue Image Left", WINDOW_NORMAL);
    namedWindow("Binary Green Image Left", WINDOW_NORMAL);
    namedWindow("Binary Red Image Left", WINDOW_NORMAL);
    imshow("Binary Blue Image Left", binary_image_blue_l);
    imshow("Binary Green Image Left", binary_image_green_l);
    imshow("Binary Red Image Left", binary_image_red_l);*/

    /*Selecao de cores Pilha a direita*/

    Mat binary_image_blue_r, binary_image_green_r, binary_image_red_r;
    select_Channel(crop_fin_r, binary_image_green_r, green_low, green_high);
    select_Channel(crop_fin_r, binary_image_blue_r, blue_low, blue_high);
    select_Channel(crop_fin_r, binary_image_red_r, red_low, red_high);

    /*Mostrando na tela as imagens binárias de cada cor reconhecida*/
    /*namedWindow("Binary Blue Image Rigth", WINDOW_NORMAL);
    namedWindow("Binary Green Image Rigth", WINDOW_NORMAL);
    namedWindow("Binary Red Image Rigth", WINDOW_NORMAL);
    imshow("Binary Blue Image Rigth", binary_image_blue_r);
    imshow("Binary Green Image Rigth", binary_image_green_r);
    imshow("Binary Red Image Rigth", binary_image_red_r);*/    

    /* ------------ Identificando pilha a esquerda ------------- */

    /*Declarando os vetores que guardarão os pontos a esquerda e a direita da imagem dos pixels que foram reconhecidos 
    como containers da pilha esquerda através da calculate_pixels*/
    vector<Point2i> blue_points_rigth_l;
    vector<Point2i> blue_points_left_l;
    vector<Point2i> green_points_rigth_l;
    vector<Point2i> green_points_left_l;
    vector<Point2i> red_points_rigth_l;
    vector<Point2i> red_points_left_l;

    //AZUL PILHA ESQUERDA
    calculate_pixels(binary_image_blue_l,blue_points_left_l, blue_points_rigth_l);

    int n_blue_left_l = 0;
    vector<Point2i> blue_pointskey_left_l;
    calculate_containers(crop_fin_l, blue_points_left_l, blue_pointskey_left_l, n_blue_left_l, Scalar(222, 246, 101));

    //cout << n_blue_left_l << endl << blue_pointskey_left_l << endl;

    int n_blue_rigth_l = 0;
    vector<Point2i> blue_pointskey_rigth_l;
    calculate_containers(crop_fin_l, blue_points_rigth_l, blue_pointskey_rigth_l, n_blue_rigth_l, Scalar(222, 246, 101));

    //cout << n_blue_rigth_l << endl << blue_pointskey_rigth_l << endl;

     //VERDE PILHA ESQUERDA
    calculate_pixels(binary_image_green_l, green_points_left_l, green_points_rigth_l);

    int n_green_left_l = 0;
    vector<Point2i> green_pointskey_left_l;
    calculate_containers(crop_fin_l, green_points_left_l, green_pointskey_left_l, n_green_left_l, Scalar(78, 255, 24));

    //cout << n_green_left_l << endl << green_pointskey_left_l << endl;

    int n_green_rigth_l = 0;
    vector<Point2i> green_pointskey_rigth_l;
    calculate_containers(crop_fin_l, green_points_rigth_l, green_pointskey_rigth_l, n_green_rigth_l, Scalar(78, 255, 24));

    //cout << n_green_rigth_l << endl << green_pointskey_rigth_l << endl;

    //VERMELHO
    calculate_pixels(binary_image_red_l,red_points_left_l, red_points_rigth_l);

    int n_red_left_l = 0;
    vector<Point2i> red_pointskey_left_l;
    calculate_containers(crop_fin_l, red_points_left_l, red_pointskey_left_l, n_red_left_l, Scalar(255, 20, 147));

    //cout << n_red_left << endl << red_pointskey_left << endl;

    int n_red_rigth_l = 0;
    vector<Point2i> red_pointskey_rigth_l;
    calculate_containers(crop_fin_l, red_points_rigth_l, red_pointskey_rigth_l, n_red_rigth_l, Scalar(255, 20, 147));

    //cout << n_red_rigth << endl << red_pointskey_rigth << endl;

    cout << "-------------- Pilha Esquerda: ------------" << endl;
    int pos_left_l[4];
    identify_containers(blue_pointskey_left_l, green_pointskey_left_l, red_pointskey_left_l, n_blue_left_l, n_green_left_l, n_red_left_l, pos_left_l);
    cout << "Containers à Esquerda: " << endl;
    for (int i = 0; i < 4; i++)
    {
        if (pos_left_l[i] == 0)
            cout << "pos_left_l[" << i << "] = " << pos_left_l[i] <<" -- Azul," << endl;
        else if (pos_left_l[i] == 1)
            cout << "pos_left_l[" << i << "] = " << pos_left_l[i] <<" -- Verde," << endl;
        else
            cout << "pos_left_l[" << i << "] = " << pos_left_l[i] <<" -- Vermelho," << endl;
    }

    cout << "\n\n";

    int pos_rigth_l[4];
    identify_containers(blue_pointskey_rigth_l, green_pointskey_rigth_l, red_pointskey_rigth_l, n_blue_rigth_l, n_green_rigth_l, n_red_rigth_l, pos_rigth_l);
    cout << "Containers à Direita: " << endl;
    for (int i = 0; i < 4; i++)
    {
        if (pos_rigth_l[i] == 0)
            cout << "pos_rigth_l[" << i << "] = " << pos_rigth_l[i] <<" -- Azul," << endl;
        else if (pos_rigth_l[i] == 1)
            cout << "pos_rigth_l[" << i << "] = " << pos_rigth_l[i] <<" -- Verde," << endl;
        else
            cout << "pos_rigth_l[" << i << "] = " << pos_rigth_l[i] <<" -- Vermelho," << endl;
    }

    /*******************************************/

/* ------------ Identificando pilha a direita ------------- */

    /*Declarando os vetores que guardarão os pontos a esquerda e a direita da imagem dos pixels que foram reconhecidos 
    como containers da pilha esquerda através da calculate_pixels*/
    vector<Point2i> blue_points_rigth_r;
    vector<Point2i> blue_points_left_r;
    vector<Point2i> green_points_rigth_r;
    vector<Point2i> green_points_left_r;
    vector<Point2i> red_points_rigth_r;
    vector<Point2i> red_points_left_r;

    //AZUL PILHA DIREITA
    calculate_pixels(binary_image_blue_r, blue_points_left_r, blue_points_rigth_r);

    int n_blue_left_r = 0;
    vector<Point2i> blue_pointskey_left_r;
    calculate_containers(crop_fin_r, blue_points_left_r, blue_pointskey_left_r, n_blue_left_r, Scalar(222, 246, 101));

    //cout << n_blue_left_r << endl << blue_pointskey_left_r << endl;

    int n_blue_rigth_r = 0;
    vector<Point2i> blue_pointskey_rigth_r;
    calculate_containers(crop_fin_r, blue_points_rigth_r, blue_pointskey_rigth_r, n_blue_rigth_r, Scalar(222, 246, 101));

    //cout << n_blue_rigth_r << endl << blue_pointskey_rigth_r << endl;
    
    //VERDE PILHA DIREITA
    calculate_pixels(binary_image_green_r, green_points_left_r, green_points_rigth_r);

    int n_green_left_r = 0;
    vector<Point2i> green_pointskey_left_r;
    calculate_containers(crop_fin_r, green_points_left_r, green_pointskey_left_r, n_green_left_r, Scalar(78, 255, 24));

    //cout << n_green_left_r << endl << green_pointskey_left_r << endl;

    int n_green_rigth_r = 0;
    vector<Point2i> green_pointskey_rigth_r;
    calculate_containers(crop_fin_r, green_points_rigth_r, green_pointskey_rigth_r, n_green_rigth_r, Scalar(78, 255, 24));

    //cout << n_green_rigth_r << endl << green_pointskey_rigth_r << endl;

    //VERMELHO
    calculate_pixels(binary_image_red_r,red_points_left_r, red_points_rigth_r);

    int n_red_left_r = 0;
    vector<Point2i> red_pointskey_left_r;
    calculate_containers(crop_fin_r, red_points_left_r, red_pointskey_left_r, n_red_left_r, Scalar(255, 20, 147));

    //cout << n_red_left_r << endl << red_pointskey_left_r << endl;

    int n_red_rigth_r = 0;
    vector<Point2i> red_pointskey_rigth_r;
    calculate_containers(crop_fin_r, red_points_rigth_r, red_pointskey_rigth_r, n_red_rigth_r, Scalar(255, 20, 147));

    //cout << n_red_rigth_r << endl << red_pointskey_rigth_r << endl;

    cout << "\n\n";

    cout << "-------------- Pilha Direita: ------------" << endl;
    int pos_left_r[4];
    identify_containers(blue_pointskey_left_r, green_pointskey_left_r, red_pointskey_left_r, n_blue_left_r, n_green_left_r, n_red_left_r, pos_left_r);
    cout << "Containers à Esquerda: " << endl;
    for (int i = 0; i < 4; i++)
    {
        if (pos_left_r[i] == 0)
            cout << "pos_left_r[" << i << "] = " << pos_left_r[i] <<" -- Azul," << endl;
        else if (pos_left_r[i] == 1)
            cout << "pos_left_r[" << i << "] = " << pos_left_r[i] <<" -- Verde," << endl;
        else
            cout << "pos_left_r[" << i << "] = " << pos_left_r[i] <<" -- Vermelho," << endl;
    }

    cout << "\n\n";

    cout << "Pilha a Direita" << endl;
    int pos_rigth_r[4];
    identify_containers(blue_pointskey_rigth_r, green_pointskey_rigth_r, red_pointskey_rigth_r, n_blue_rigth_r, n_green_rigth_r, n_red_rigth_r, pos_rigth_r);
    cout << "Containers à Direita: " << endl;
    for (int i = 0; i < 4; i++)
    {
        if (pos_rigth_r[i] == 0)
            cout << "pos_rigth_r[" << i << "] = " << pos_rigth_r[i] <<" -- Azul," << endl;
        else if (pos_rigth_r[i] == 1)
            cout << "pos_rigth_r[" << i << "] = " << pos_rigth_r[i] <<" -- Verde," << endl;
        else
            cout << "pos_rigth_r[" << i << "] = " << pos_rigth_r[i] <<" -- Vermelho," << endl;
    }
    

    imshow("Cropped Final Left", crop_fin_l);
    imshow("Cropped Final Rigth", crop_fin_r);
    int k;
    while(true){
        k = waitKey(0);
        if(k == 27)
            break;
    }
    destroyAllWindows();

   

    return 0;
}

void cropp_ROI(Mat& src, Mat& dst, int x, int y, int width, int heigth){
    Rect roi;
    roi.x = x;
    roi.y = y;
    roi.width = width;
    roi.height = heigth;
    dst = src(roi);
}

void InversePerspectiveMapping(const Mat& src, Mat& dst, int alpha_, int beta_, int gamma_, int f_, int dist_)
{
    int offsetx = 0;
    int offsety = 0;

    Mat trans_mat = (Mat_<double>(2,3) << 1, 0, offsetx, 0, 1, offsety);
    warpAffine(src,dst,trans_mat,src.size());

    double focalLength, dist, alpha, beta, gamma; 

    alpha =((double)alpha_ -90) * PI/180;
    beta =((double)beta_ -90) * PI/180;
    gamma =((double)gamma_ -90) * PI/180;
    focalLength = (double)f_;
    dist = (double)dist_;

    Size image_size = src.size();
    double w = (double)image_size.width, h = (double)image_size.height;

    // Projecion matrix 2D -> 3D
    Mat A1 = (Mat_<float>(4, 3)<< 
        1, 0, -w/2,
        0, 1, -h/2,
        0, 0, 0,
        0, 0, 1 );

    // Rotation matrices Rx, Ry, Rz
    Mat RX = (Mat_<float>(4, 4) << 
        1, 0, 0, 0,
        0, cos(alpha), -sin(alpha), 0,
        0, sin(alpha), cos(alpha), 0,
        0, 0, 0, 1 );

    Mat RY = (Mat_<float>(4, 4) << 
        cos(beta), 0, -sin(beta), 0,
        0, 1, 0, 0,
        sin(beta), 0, cos(beta), 0,
        0, 0, 0, 1  );

    Mat RZ = (Mat_<float>(4, 4) << 
        cos(gamma), -sin(gamma), 0, 0,
        sin(gamma), cos(gamma), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1  );

    // R - rotation matrix
    Mat R = RX * RY * RZ;

    // T - translation matrix
    Mat T = (Mat_<float>(4, 4) << 
        1, 0, 0, 0,  
        0, 1, 0, 0,  
        0, 0, 1, dist,  
        0, 0, 0, 1); 

    // K - intrinsic matrix 
    Mat K = (Mat_<float>(3, 4) << 
        focalLength, 0, w/2, 0,
        0, focalLength, h/2, 0,
        0, 0, 1, 0
        ); 

    Mat transformationMat = K * (T * (R * A1));

    warpPerspective(dst, dst, transformationMat, image_size, INTER_CUBIC | WARP_INVERSE_MAP);
}

void select_Channel(Mat &in, Mat &out, Scalar low, Scalar high)
{
    Mat gauss_image;
    GaussianBlur(in, gauss_image, Size(5, 5), 0, 0, BORDER_DEFAULT);
    cvtColor(gauss_image, gauss_image, CV_RGB2HSV);
    inRange(gauss_image, low, high, out);
    erode(out, out, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
    dilate(out, out, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
    dilate(out, out, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
    erode(out, out, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
}

void calculate_pixels(Mat &in, vector<Point2i> &points_rigth, vector<Point2i> &points_left)
{
    int col, row, pixels;

    for (row = 0; row < in.rows; row++)
    {
        pixels = 0;
        for (col = 0; col < in.cols / 4; col++)
        {
            if (in.at<uchar>(row, col) != 0)
            {
                pixels++;
            }
        }
        if (pixels >= (in.cols / 4) - 80)
        {
            points_rigth.push_back(Point(0, row));
        }

        pixels = 0;
        for (col = 3 * in.cols / 4; col < in.cols; col++)
        {
            if (in.at<uchar>(row, col) != 0)
            {
                pixels++;
            }
        }
        if (pixels >= (in.cols / 4) - 80)
        {
            points_left.push_back(Point(in.cols, row));
        }
    }
}

void calculate_containers(Mat &in, vector<Point2i> points, vector<Point2i> &key_points, int &n_containers, Scalar color)
{
    int size_points = points.size();
    if (size_points != 0)
    {
        n_containers = 0;
        key_points.push_back(points[0]);
        vector<Point2i> aux;
        for (int i = 0; i < size_points - 1; i++)
        {
            if ((points[i].y + 1) == points[i + 1].y)
            {
                aux.push_back(points[i]);
            }
            else
            {
                polylines(in, aux, 0, color, 3, 8, 0);
                aux.clear();
                n_containers++;
                key_points.push_back(points[i]);
                key_points.push_back(points[i + 1]);
            }
        }
        polylines(in, aux, 0, color, 3, 8, 0);
        aux.clear();
        n_containers++;
        /*if (n_containers == 1)
        {
            key_points.push_back(points[size_points / 2]);
            key_points.push_back(points[size_points / 2 + 1]);
        }*/
        int size_teste = 70; //Considerando que cada container tem 70 pixels em média de altura
        if (n_containers == 1)
        {
            int teste = size_points / size_teste;
            if (teste == 1 || teste == 0)
            {
                key_points.push_back(points[size_points - 1]);
                return;
            }
            else if (teste == 2)
            {
                n_containers++;
                key_points.push_back(points[size_points / 2]);
                key_points.push_back(points[size_points / 2 + 1]);
                key_points.push_back(points[size_points - 1]);
                return;
            }
            else if (teste == 3)
            {
                n_containers += 2;
                key_points.push_back(points[size_points / 3]);
                key_points.push_back(points[size_points / 3 + 1]);
                key_points.push_back(points[(2 * size_points) / 3]);
                key_points.push_back(points[(2 * size_points / 3) + 1]);
                key_points.push_back(points[size_points - 1]);
                return;
            }
            else
            {
                n_containers = 4;
                return;
            }
        }
        if (n_containers == 2)
        {
            int teste = size_points / size_teste;
            if (teste == 2)
            {
                //Caso de possuir 2 containers não em sequência
                key_points.push_back(points[size_points - 1]);
                return;
            }
            else if (teste == 3)
            {
                //Caso de possui um containers não em sequência e outros dois em sequencia
                int high_cont = key_points[1].y - key_points[0].y; //Calculo o tamanho do primeiro "container"(pode ser dois)
                if ((high_cont / size_teste) == 1)
                { //Se for realmente um
                    n_containers++;
                    int size_position = size_points - high_cont; // calculo o tamanho somente da parte que quero dividir
                    key_points.push_back(points[high_cont + (size_position / 2)]);
                    key_points.push_back(points[high_cont + (size_position / 2 + 1)]);
                }
                else
                {
                    //Caso que o de cima possui dois containers em sequencia e um outro nao em sequencia
                    n_containers++;
                    Point aux1 = key_points[1];
                    Point aux2 = key_points[2];
                    key_points[1] = points[high_cont / 2];
                    key_points[2] = points[high_cont / 2 + 1];
                    key_points.push_back(aux1);
                    key_points.push_back(aux2);
                    key_points.push_back(points[size_points - 1]);
                    return;
                }
            }
        }
        key_points.push_back(points[size_points - 1]);
    }
    else
    {
        n_containers = 0;
    }
}

void identify_containers(vector<Point2i> blue_pointskey, vector<Point2i> green_pointskey, vector<Point2i> red_pointskey, int n_blue, int n_green, int n_red, int pos[])
{
    // VERDE = 1 AZUL = 0 VERMELHO = -1
    Point temp;
    if (n_blue == 0 && n_green == 0)
    { //Caso em que os 4 containers sao vermelhos
        for (int i = 0; i < 4; i++)
            pos[i] = -1;
        return;
    }
    if (n_blue == 4)
    {
        for (int i = 0; i < 4; i++)
            pos[i] = 0;
        return;
    }
    if (n_red == 4)
    {
        for (int i = 0; i < 4; i++)
            pos[i] = -1;
        return;
    }
    if (n_green == 4)
    {
        for (int i = 0; i < 4; i++)
            pos[i] = 1;
        return;
    }
    //Point2i high_point, low_point;
    int cont = 0, cont_blue = 0, cont_green = 0, cont_red = 0;
    while (cont < 4)
    {
        /*if (n_red == 0)
        {
            if ((blue_pointskey[cont_blue].y < green_pointskey[cont_green].y) && (blue_pointskey[cont_blue + 1].y < green_pointskey[cont_green + 1].y))
            {
                pos[cont] = 0;
                cont++;
                cont_blue += 2;
            }
            if ((blue_pointskey[cont_blue].y > green_pointskey[cont_green].y) && (blue_pointskey[cont_blue + 1].y > green_pointskey[cont_green + 1].y))
            {
                pos[cont] = 1;
                cont++;
                cont_green += 2;
            }
            if (cont_blue == blue_pointskey.size())
            {
                while (cont < 4)
                {
                    pos[cont] = 1;
                    cont++;
                    cont_green++;
                }
            }
            if (cont_green == green_pointskey.size())
            {
                while (cont < 4)
                {
                    pos[cont] = 0;
                    cont++;
                    cont_blue++;
                }
            }
        }
        else
        {*/
            if (cont_red == red_pointskey.size())
            {

                red_pointskey.push_back(Point(1000, 1000));
                red_pointskey.push_back(Point(2000, 2000));
            }
            if (cont_green == green_pointskey.size())
            {

                green_pointskey.push_back(Point(1000, 1000));
                green_pointskey.push_back(Point(2000, 2000));
            }
            if (cont_blue == blue_pointskey.size())
            {

                blue_pointskey.push_back(Point(1000, 1000));
                blue_pointskey.push_back(Point(2000, 2000));
            }
            int teste = comparation(green_pointskey[cont_green].y, green_pointskey[cont_green + 1].y, blue_pointskey[cont_blue].y, blue_pointskey[cont_blue + 1].y, red_pointskey[cont_red].y, red_pointskey[cont_red + 1].y);

            if (teste == 0)
            {
                pos[cont] = 0;
                cont++;
                cont_blue += 2;
            }
            if (teste == 1)
            {
                pos[cont] = 1;
                cont++;
                cont_green += 2;
            }
            if (teste == -1)
            {
                pos[cont] = -1;
                cont++;
                cont_red += 2;
            }
            if (blue_pointskey[cont_blue].y == 1000 && red_pointskey[cont_red].y == 1000)
            {
                while (cont < 4)
                {
                    pos[cont] = 1;
                    cont++;
                    cont_green++;
                }
            }
            if (green_pointskey[cont_green].y == 1000 && red_pointskey[cont_red].y == 1000)
            {
                while (cont < 4)
                {
                    pos[cont] = 0;
                    cont++;
                    cont_blue++;
                }
            }
            if (blue_pointskey[cont_blue].y == 1000 && green_pointskey[cont_green].y == 1000)
            {
                while (cont < 4)
                {
                    pos[cont] = -1;
                    cont++;
                    cont_red++;
                }
            }
        //}
    }
}

//Função que acha o menor valor entre A, B e C
int comparation(int A1, int A2, int B1, int B2, int C1, int C2)
{
    // A menor == 1      / B menor = 0  / C menor = -1
    if (A1 < B1 && A2 < B2)
    {
        if (A1 < C1 && A2 < C2)
        {
            return 1;
        }
        else
        {
            if (C1 < B1 && C2 < B2)
                return -1;
        }
    }
    else
    {
        if (B1 < C1 && B2 < C2)
        {
            return 0;
        }
        else
        {
            if (C1 < A1 && C2 < A2)
                return -1;
        }
    }
}
