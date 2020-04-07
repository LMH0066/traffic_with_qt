#pragma once
#include <QImage>
#include <iostream>
#include <opencv.hpp>
#include <string>
#include <QPen>
#include <QPainter>

using namespace std;
using namespace cv;

extern double refresh_time;

extern void Init(double time);
extern QImage Mat2QImage(Mat cvImg);

extern const size_t inWidth;
extern const size_t inHeight;
extern const float inScaleFactor;
extern const float meanVal;

typedef struct MyRegion {
    QPoint points[4];
    int point_number = 0;
    double x_max, x_min, y_max, y_min;
    double scale[2][4];
    void push(QPoint p, double xscale, double yscale) {
        points[point_number] = p;
        scale[0][point_number] = xscale;
        scale[1][point_number] = yscale;
        if (!point_number) {
            x_max = x_min = xscale;
            y_max = y_min = yscale;
        } else {
            x_max = x_max > xscale ? x_max : xscale;
            x_min = x_min < xscale ? x_min : xscale;
            y_max = y_max > yscale ? y_max : yscale;
            y_min = y_min < yscale ? y_min : yscale;
        }
        point_number++;
    }
    QImage draw(QImage image) {
        QPainter p;
        p.begin(&image);
        p.setPen(QPen(Qt::red, 5));
        for (int i = 0; i < point_number; i++) {
            p.drawLine(scale[0][i] * image.width(),
                       scale[1][i] * image.height(),
                       scale[0][(i + 1) % point_number] * image.width(),
                       scale[1][(i + 1) % point_number] * image.height());
        }
        p.end();
        return image;
    }
    bool Contains(double x, double y, double sizex, double sizey) {
        double Ax = (scale[0][0] - x_min) * sizex,
               Ay = (scale[1][0] - y_min) * sizey;
        double Bx = (scale[0][1] - x_min) * sizex,
               By = (scale[1][1] - y_min) * sizey;
        double Cx = (scale[0][2] - x_min) * sizex,
               Cy = (scale[1][2] - y_min) * sizey;
        double Dx = (scale[0][3] - x_min) * sizex,
               Dy = (scale[1][3] - y_min) * sizey;
        double a = (Bx - Ax) * (y - Ay) - (By - Ay) * (x - Ax);
        double b = (Cx - Bx) * (y - By) - (Cy - By) * (x - Bx);
        double c = (Dx - Cx) * (y - Cy) - (Dy - Cy) * (x - Cx);
        double d = (Ax - Dx) * (y - Dy) - (Ay - Dy) * (x - Dx);
        if ((a > 0 && b > 0 && c > 0 && d > 0) ||
                (a < 0 && b < 0 && c < 0 && d < 0)) {
            return true;
        }
        return false;
    }
} MyRegion;
