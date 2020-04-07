#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include "detector.h"
#include <QLabel>
#include <QtCharts>

using namespace cv;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
  public slots:
    void changeIdentity();
    void playVideo();
    void showMessageDock();
    void showDiagramDock();
    void drawDiagram(int* number);
  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
  private:
    Ui::MainWindow *ui;
    bool identification;
    QTimer *timer;
    QDockWidget *messageDock = NULL;
    QDockWidget *diagramDock = NULL;
    QLabel *messageLabel;

    QList<double> data[8]; // 存储业务数据的 list
    QChart *chart;
    QChartView *chartView;
    QSplineSeries *splineSeries;
    QScatterSeries *scatterSeries;

    double row_time, col_time;
};

#endif  // MAINWINDOW_H
