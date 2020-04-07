#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QFileDialog>
#include <QLabel>
#include <QMouseEvent>
#include <QPoint>
#include <QTimer>
#include <QWidget>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <QDockWidget>
#include "detector.h"

using namespace dnn;

class VideoWidget : public QWidget {
    Q_OBJECT
  public:
    explicit VideoWidget(QWidget *parent = nullptr);
    QTimer *click_timer, *timer;
    int getSeparating();

  signals:
    void clicked();

  public slots:
    void openCameraClicked();
    void closeCameraClicked();
    void getFrame(bool identification);
    bool drawFrame(bool identification, Mat m_frame, Mat m_frame1);
    void slotClicked();         //槽函数
    void beginSeparate();

  protected:
    void mousePressEvent(QMouseEvent *event);       //单击
    void mouseDoubleClickEvent(QMouseEvent *event);

  private:
    VideoCapture video_capture;
    Mat frame;
    QLabel *label;
    Detector my_detector;
    vector<int> car_num;
    int separating;  //四个状态：0为没有分割，1为点击了按钮但是还没确立第一个点，2为后续的连线，3为分割完毕。
    MyRegion region_point[2];
    Rect rectL, rectR;

};

#endif  // VIDEOWIDGET_H
