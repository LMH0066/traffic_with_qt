#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "decide.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    identification = false;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(playVideo()));
    timer->start(50);  //开始计时，超时则发出timeout()信号,1000为1秒，50毫秒取一帧

    //信息框，用于显示各路口车辆数等信息
    messageDock = new QDockWidget("Messages", this);
    messageDock->setMinimumSize(QSize(100, 100));
    setDockNestingEnabled(true);    //开启dock嵌套功能
    messageDock->setFeatures(QDockWidget::DockWidgetMovable);   //可移动
    messageDock->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);   //设置可停靠区域
    messageLabel = new QLabel();
    messageLabel->setText("number_1 : 0\nnumber_2 : 0\nnumber_3 : 0\nnumber_4 : 0");
    messageDock->setWidget(messageLabel);
    addDockWidget(Qt::RightDockWidgetArea, messageDock);
    messageDock->hide();

    //折线图，用于显示各路口车辆数等信息
    splineSeries = new QSplineSeries();
    scatterSeries = new QScatterSeries();
    scatterSeries->setMarkerSize(8);
    chart = new QChart();
    chart->addSeries(splineSeries);
    chart->addSeries(scatterSeries);
    chart->legend()->hide();
    chart->setTitle("实时动态曲线");
    chart->createDefaultAxes();
    chart->axisX()->setRange(0, 30);
    chart->axisY()->setRange(0, 40);
    chartView = new QChartView(chart);

    QValueAxis *axisx = new QValueAxis;
    axisx->setGridLineVisible(false);
    axisx->setTitleText("time(s)");
    chartView->chart()->setAxisX(axisx, splineSeries);

    QValueAxis *axisy = new QValueAxis;
    axisy->setGridLineVisible(false);
    axisy->setTitleText("car_number");
    chartView->chart()->setAxisY(axisy,splineSeries);

    chartView->setRenderHint(QPainter::Antialiasing);

    diagramDock = new QDockWidget("diagram", this);
    diagramDock->setMinimumSize(QSize(400, 400));
    diagramDock->setFeatures(QDockWidget::DockWidgetMovable);   //可移动
    diagramDock->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);   //设置可停靠区域
    diagramDock->setWidget(chartView);
    splitDockWidget(messageDock, diagramDock, Qt::Vertical);
    diagramDock->hide();

    connect(ui->actionmessages, SIGNAL(triggered()), this, SLOT(showMessageDock()));
    connect(ui->actiondiagram, SIGNAL(triggered()), this, SLOT(showDiagramDock()));

    row_time = col_time = 50;
}

MainWindow::~MainWindow() {
    timer->stop();
    delete ui;
}

void MainWindow::playVideo() {
    m_detector.goRestart();
    ui->video_1->getFrame(identification);
    ui->video_2->getFrame(identification);
    ui->video_3->getFrame(identification);
    ui->video_4->getFrame(identification);

    int number[8] = {0};
    m_detector.detect_mxnet(number);
    int car_number[4] = {number[0], number[1], number[2], number[3]};
    begin_decide(car_number, row_time, col_time);
    QString num_out;
    for (int i = 0; i < 8; i++) {
        num_out += "number_" + QString::number(i) + " : " + QString::number(number[i]) + "\n";
    }
    num_out += "rowTime : " + QString::number(row_time) + "\n";
    num_out += "colTime : " + QString::number(col_time) + "\n";
    messageLabel->setText(num_out);
    drawDiagram(number);

    int i = 0;
    int separate[4] = {0};
    separate[0] = ui->video_1->getSeparating();
    separate[1] = ui->video_2->getSeparating();
    separate[2] = ui->video_3->getSeparating();
    separate[3] = ui->video_4->getSeparating();
    if(separate[0] != 3) {
        ui->video_1->drawFrame(identification, m_detector.now_frame[1][i], m_detector.now_frame[1][i]);
        i++;
    } else {
        ui->video_1->drawFrame(identification, m_detector.now_frame[1][i], m_detector.now_frame[1][i+1]);
        i+=2;
    }
    if(separate[1] != 3) {
        ui->video_2->drawFrame(identification, m_detector.now_frame[1][i], m_detector.now_frame[1][i]);
        i++;
    } else {
        ui->video_2->drawFrame(identification, m_detector.now_frame[1][i], m_detector.now_frame[1][i+1]);
        i+=2;
    }
    if(separate[2] != 3) {
        ui->video_3->drawFrame(identification, m_detector.now_frame[1][i], m_detector.now_frame[1][i]);
        i++;
    } else {
        ui->video_3->drawFrame(identification, m_detector.now_frame[1][i], m_detector.now_frame[1][i+1]);
        i+=2;
    }
    if(separate[3] != 3) {
        ui->video_4->drawFrame(identification, m_detector.now_frame[1][i], m_detector.now_frame[1][i]);
        i++;
    } else {
        ui->video_4->drawFrame(identification, m_detector.now_frame[1][i], m_detector.now_frame[1][i+1]);
        i+=2;
    }
}

void MainWindow::changeIdentity() {
    identification = !identification;
    m_detector.empty_tracking = true;
}

void MainWindow::showMessageDock() {
    if(messageDock->isHidden())
        messageDock->show();
    else
        messageDock->hide();
}

void MainWindow::showDiagramDock() {
    if(diagramDock->isHidden())
        diagramDock->show();
    else
        diagramDock->hide();
}

void MainWindow::drawDiagram(int* number) {
    for (int i = 0; i < 8; i++) {
        if (data[i].size() > 30) {
            for (int i = 0; i < 8; i++) {
                data[i].clear();
            }
            break;
        }
    }
    for (int i = 0; i < 8; i++) {
        data[i] << number[i];
    }
    if (!diagramDock->isHidden()) {
        splineSeries->clear();
        scatterSeries->clear();
        for (int i = 0; i < data[0].size(); ++i) {
            splineSeries->append(i, data[0].at(i));
            scatterSeries->append(i, data[0].at(i));
        }
    }
}
