#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

void select_Channel(Mat &in, Mat &out, Scalar low, Scalar high);
void calculate_pixels(Mat &in, vector<Point2i> &points_rigth, vector<Point2i> &points_left);
void calculate_containers(Mat &in, vector<Point2i> points, vector<Point2i> &key_points, int &n_containers, Scalar color);
void identify_containers(vector<Point2i> blue_pointskey, vector<Point2i> green_pointskey, vector<Point2i> red_pointskey, int n_blue, int n_green, int n_red, int pos[]);
int comparation(int elementA1, int elementA2, int elementB1, int elementB2, int elementC1, int elementC2);

/*Valores do Range para selecionamento das cores*/
Scalar blue_low = Scalar(0, 81, 63);
Scalar blue_high = Scalar(18, 243, 255);

Scalar green_low = Scalar(30, 142, 91);
Scalar green_high = Scalar(144, 255, 188);

Scalar red_low = Scalar(61, 198, 167);
Scalar red_high = Scalar(120, 255, 249);

int main(int argc, char **argv)
{
    /*Lendo e mostrando na tela imagem original*/
    char *image_name = argv[1];
    Mat original_image = imread(image_name, IMREAD_COLOR);
    namedWindow("Original Image", WINDOW_NORMAL);
    imshow("Original Image", original_image);

    /*Declarando as matrizes que guardaram as imagens binárias geradas na select_Channel
    contendo pixels brancos aonde recenheceu as cores*/
    Mat binary_image_blue, binary_image_green, binary_image_red;
    select_Channel(original_image, binary_image_green, green_low, green_high);
    select_Channel(original_image, binary_image_blue, blue_low, blue_high);
    select_Channel(original_image, binary_image_red, red_low, red_high);

    /*Mostrando na tela as imagens binárias de cada cor reconhecida*/
    namedWindow("Binary Blue Image", WINDOW_NORMAL);
    namedWindow("Binary Green Image", WINDOW_NORMAL);
    namedWindow("Binary Red Image", WINDOW_NORMAL);
    imshow("Binary Blue Image", binary_image_blue);
    imshow("Binary Green Image", binary_image_green);
    imshow("Binary Red Image", binary_image_red);

    /*Declarando os vetores que guardarão os pontos a esquerda e a direita da imagem dos pixels que foram reconhecidos 
    como containers através da calculate_pixels*/
    vector<Point2i> blue_points_rigth;
    vector<Point2i> blue_points_left;
    vector<Point2i> green_points_rigth;
    vector<Point2i> green_points_left;
    vector<Point2i> red_points_rigth;
    vector<Point2i> red_points_left;

    //AZUL
    calculate_pixels(binary_image_blue, blue_points_rigth, blue_points_left);

    int n_blue_left = 0;
    vector<Point2i> blue_pointskey_left;
    calculate_containers(original_image, blue_points_left, blue_pointskey_left, n_blue_left, Scalar(222, 246, 101));

    //cout << n_blue_left << endl << blue_pointskey_left << endl;

    int n_blue_rigth = 0;
    vector<Point2i> blue_pointskey_rigth;
    calculate_containers(original_image, blue_points_rigth, blue_pointskey_rigth, n_blue_rigth, Scalar(222, 246, 101));

    //cout << n_blue_rigth << endl << blue_pointskey_rigth << endl;

    //VERDE
    calculate_pixels(binary_image_green, green_points_rigth, green_points_left);

    int n_green_left = 0;
    vector<Point2i> green_pointskey_left;
    calculate_containers(original_image, green_points_left, green_pointskey_left, n_green_left, Scalar(78, 255, 24));

    //cout << n_green_left << endl << green_pointskey_left << endl;

    int n_green_rigth = 0;
    vector<Point2i> green_pointskey_rigth;
    calculate_containers(original_image, green_points_rigth, green_pointskey_rigth, n_green_rigth, Scalar(78, 255, 24));

    //cout << n_green_rigth << endl << green_pointskey_rigth << endl;

    //VERMELHO
    calculate_pixels(binary_image_red, red_points_rigth, red_points_left);

    int n_red_left = 0;
    vector<Point2i> red_pointskey_left;
    calculate_containers(original_image, red_points_left, red_pointskey_left, n_red_left, Scalar(255, 20, 147));

    //cout << n_red_left << endl << red_pointskey_left << endl;

    int n_red_rigth = 0;
    vector<Point2i> red_pointskey_rigth;
    calculate_containers(original_image, red_points_rigth, red_pointskey_rigth, n_red_rigth, Scalar(255, 20, 147));

    //cout << n_red_rigth << endl << red_pointskey_rigth << endl;

    namedWindow("Found Containers", WINDOW_NORMAL);
    imshow("Found Containers", original_image);

    cout << "\n\n";

    int pos_left[4];
    identify_containers(blue_pointskey_left, green_pointskey_left, red_pointskey_left, n_blue_left, n_green_left, n_red_left, pos_left);
    cout << "Containers à Esquerda: " << endl;
    for (int i = 0; i < 4; i++)
    {
        if (pos_left[i] == 0)
            cout << "pos_left[" << i << "] = " << pos_left[i] <<" -- Azul," << endl;
        else if (pos_left[i] == 1)
            cout << "pos_left[" << i << "] = " << pos_left[i] <<" -- Verde," << endl;
        else
            cout << "pos_left[" << i << "] = " << pos_left[i] <<" -- Vermelho," << endl;
    }

    cout << "\n\n";

    int pos_rigth[4];
    identify_containers(blue_pointskey_rigth, green_pointskey_rigth, red_pointskey_rigth, n_blue_rigth, n_green_rigth, n_red_rigth, pos_rigth);
    cout << "Containers à Direita: " << endl;
    for (int i = 0; i < 4; i++)
    {
        if (pos_rigth[i] == 0)
            cout << "pos_rigth[" << i << "] = " << pos_rigth[i] <<" -- Azul," << endl;
        else if (pos_rigth[i] == 1)
            cout << "pos_rigth[" << i << "] = " << pos_rigth[i] <<" -- Verde," << endl;
        else
            cout << "pos_rigth[" << i << "] = " << pos_rigth[i] <<" -- Vermelho," << endl;
    }

    int k = waitKey(0);
    destroyAllWindows();
    original_image.release();
    binary_image_blue.release();
    binary_image_green.release();
    binary_image_red.release();

    return 0;
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
        if (pixels >= (in.cols / 4) - 5)
        {
            points_left.push_back(Point(0, row));
        }

        pixels = 0;
        for (col = 3 * in.cols / 4; col < in.cols; col++)
        {
            if (in.at<uchar>(row, col) != 0)
            {
                pixels++;
            }
        }
        if (pixels >= (in.cols / 4) - 5)
        {
            points_rigth.push_back(Point(in.cols, row));
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
        int size_teste = 35; //Considerando que cada container tem 35 pixels em média de altura
        if (n_containers == 1)
        {
            int teste = size_points / size_teste;
            if (teste == 1)
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