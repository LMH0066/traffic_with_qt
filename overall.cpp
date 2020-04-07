#include "overall.h"
double refresh_time = 10;
void Init(double time) { refresh_time = time; }

QImage Mat2QImage(Mat cvImg) {
    QImage qImg;
    if (cvImg.channels() == 3) {  // 3 channels color image

        //        cv::cvtColor(cvImg,cvImg,CV_BGR2RGB);
        qImg =
            QImage((const unsigned char*)(cvImg.data), cvImg.cols, cvImg.rows,
                   cvImg.cols * cvImg.channels(), QImage::Format_RGB888);
    } else if (cvImg.channels() == 1) {  // grayscale image
        qImg =
            QImage((const unsigned char*)(cvImg.data), cvImg.cols, cvImg.rows,
                   cvImg.cols * cvImg.channels(), QImage::Format_Indexed8);
    } else {
        qImg =
            QImage((const unsigned char*)(cvImg.data), cvImg.cols, cvImg.rows,
                   cvImg.cols * cvImg.channels(), QImage::Format_RGB888);
    }
    return qImg;
}

const size_t inWidth = 300;
const size_t inHeight = 300;

const float inScaleFactor = 0.007843f;
const float meanVal = 127.5;
