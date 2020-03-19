#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

using namespace std;

void thresholdIntegral(cv::Mat &inputMat, cv::Mat &outputMat, float T, int part)
{
    int nRows, nCols, S, s2, i, j;
    int x1, y1, x2, y2, count, sum, ip, it;
    cv::Mat sumMat;
    int *p_y1, *p_y2;
    uchar *p_inputMat, *p_outputMat;

    // accept only char type matrices
    CV_Assert(!inputMat.empty());
    CV_Assert(inputMat.depth() == CV_8U);
    CV_Assert(inputMat.channels() == 1);
    CV_Assert(!outputMat.empty());
    CV_Assert(outputMat.depth() == CV_8U);
    CV_Assert(outputMat.channels() == 1);

    // rows -> height -> y
    nRows = inputMat.rows;
    // cols -> width -> x
    nCols = inputMat.cols;

    // create the integral image
    cv::integral(inputMat, sumMat);

    CV_Assert(sumMat.depth() == CV_32S);
    CV_Assert(sizeof(int) == 4);

    S = MAX(nRows, nCols);
    S = (S > (part + part)) ? (S / part) : 2;

    // perform thresholding
    s2 = S/2;

    for(i = 0; i < nRows; ++i)
    {
        y1 = (i < s2) ? 0 : (i - s2);
        y2 = ((i + s2) < nRows) ? (i + s2) : (nRows - 1);

        p_y1 = sumMat.ptr<int>(y1);
        p_y2 = sumMat.ptr<int>(y2);
        p_inputMat = inputMat.ptr<uchar>(i);
        p_outputMat = outputMat.ptr<uchar>(i);

        for (j = 0; j < nCols; ++j)
        {
            x1 = (j < s2) ? 0 : (j - s2);
            x2 = ((j + s2) < nCols) ? (j + s2) : (nCols - 1);

            count = (x2 - x1) * (y2 - y1);

            // I(x,y)=s(x2,y2)-s(x1,y2)-s(x2,y1)+s(x1,x1)
            sum = p_y2[x2] - p_y1[x2] - p_y2[x1] + p_y1[x1];

            ip = (int)(p_inputMat[j] * count);
            it = (int)(sum * (1.0f - T));
            p_outputMat[j] = (ip < it) ? 0 : 255;
        }
    }
}

int main(int argc, char *argv[])
{
    float T = 0.15f;
    int part = 8, blur = 0;
    printf("Adaptive Integral Threshold.\n");
    if (argc < 3)
    {
        printf("Usage: aithreshold image_in image_out [T=0.15] [part=8] [blur=0]\n");
        return 0;
    }
    // Load the image
    cv::Mat src = cv::imread(argv[1]);
    if (argc > 3) T = atof(argv[3]);
    if (argc > 4) part = atoi(argv[4]);
    if (argc > 5) blur = atoi(argv[5]);

    // Check if image is loaded fine
    if(src.empty()) {
        cerr << "Problem loading image!!!" << endl;
        return -1;
    }

    // Transform source image to gray if it is not
    cv::Mat gray;

    if (src.channels() == 3)
    {
        cv::cvtColor(src, gray, CV_BGR2GRAY);
    }
    else
    {
        gray = src;
    }

    printf("T: %f, part: %d, blur: %d\n", T, part, blur);
    blur++;
    if (blur > 1) cv::blur(gray, gray, cv::Size(blur, blur));
    cv::Mat bw2 = cv::Mat::zeros(gray.size(), CV_8UC1);
    thresholdIntegral(gray, bw2, T, part);
    printf("%s -> %s\n", argv[1], argv[2]);

    // Write threshold image
    cv::imwrite(argv[2], bw2);
    return 0;
}
