#include "videowidget.h"
#include "mainwindow.h"
#include <opencv2/imgproc/types_c.h>

VideoWidget::VideoWidget(QWidget* parent) : QWidget(parent) {
    click_timer = new QTimer(this);
    timer = new QTimer(this);
    label = new QLabel(this);
    label->setScaledContents(true);
    connect(click_timer, SIGNAL(timeout()), this, SLOT(slotClicked()));

    separating = 0;
}

void VideoWidget::slotClicked() {
    click_timer->stop();
}

void VideoWidget::mousePressEvent(QMouseEvent* event) {
    click_timer->start(1000);
    clicked();
    if (separating <= 0) return;
    if (event->button() == Qt::LeftButton) {
        double x_scale =
            static_cast<double>(event->pos().x()) / static_cast<double>(this->size().width());
        double y_scale =
            static_cast<double>(event->pos().y()) / static_cast<double>(this->size().height());
        if (region_point[0].point_number < 4) {
            region_point[0].push(event->pos(), x_scale, y_scale);
        } else if (region_point[1].point_number < 4) {
            region_point[1].push(event->pos(), x_scale, y_scale);
        } else
            return;
        if (separating == 1) {
            separating++;
        }
    }
}

void VideoWidget::mouseDoubleClickEvent(QMouseEvent* event) {
    click_timer->stop();
    openCameraClicked();
}

void VideoWidget::openCameraClicked() {
    QString file_name = QFileDialog::getOpenFileName(this, "video", "", "*.*");
    if(file_name.isNull()) return;
    video_capture.open(file_name.toStdString());
    if (!video_capture.isOpened()) {
        cout << "error!";
    }
    timer->start(50);  //开始计时，超时则发出timeout()信号,1000为1秒，50毫秒取一帧
}

void VideoWidget::closeCameraClicked() {
    timer->stop();
}

void VideoWidget::beginSeparate() {
    if(separating == 0) {
        separating = 1;
    } else
        separating = 0;
    region_point[0].point_number = region_point[1].point_number = 0;
}

void VideoWidget::getFrame(bool identification) {
    label->resize(this->size());
    if (!video_capture.isOpened()) {
        return;
    }
    if (region_point[0].point_number == 4 && region_point[1].point_number == 4)
        separating = 3;
    video_capture >> frame;
    Mat L, R, ROIL, ROIR;
    if(identification) {
        if (separating != 3) {
            m_detector.pushFrame(frame, frame);
        } else {
            rectL = Rect(
                        static_cast<int>(frame.cols * region_point[0].x_min),
                        static_cast<int>(frame.rows * region_point[0].y_min),
                        static_cast<int>(frame.cols * (region_point[0].x_max - region_point[0].x_min)),
                        static_cast<int>(frame.rows * (region_point[0].y_max - region_point[0].y_min)));
            rectR = Rect(
                        static_cast<int>(frame.cols * region_point[1].x_min),
                        static_cast<int>(frame.rows * region_point[1].y_min),
                        static_cast<int>(frame.cols * (region_point[1].x_max - region_point[1].x_min)),
                        static_cast<int>(frame.rows * (region_point[1].y_max - region_point[1].y_min)));
            ROIL = frame(rectL);
            ROIR = frame(rectR);
            frame(rectL).copyTo(L);
            frame(rectR).copyTo(R);
            for (int i = 0; i < L.rows; i++) {
                for (int j = 0; j < L.cols; j++) {
                    if (!region_point[0].Contains(j, i, frame.cols, frame.rows)) {
                        L.at<Vec3b>(i, j) = 0;
                    }
                }
            }
            for (int i = 0; i < R.rows; i++) {
                for (int j = 0; j < R.cols; j++) {
                    if (!region_point[1].Contains(j, i, frame.cols, frame.rows)) {
                        R.at<Vec3b>(i, j) = 0;
                    }
                }
            }
            m_detector.pushFrame(L, ROIL);
            m_detector.pushFrame(R, ROIR);
        }
    }
}

bool VideoWidget::drawFrame(bool identification, Mat m_frame, Mat m_frame1) {
    if (!video_capture.isOpened()) {
        return false;
    }
    if (identification) {
        if(separating != 3) {
            m_frame.copyTo(frame);
        } else {
            Mat ROIL = frame(rectL), ROIR = frame(rectR);
            m_frame.copyTo(ROIL);
            m_frame1.copyTo(ROIR);
        }
    }
    cv::cvtColor(frame, frame, CV_BGR2RGB);
    QImage image = Mat2QImage(frame);
    if (separating >= 1) {
        image = region_point[0].draw(image);
        image = region_point[1].draw(image);
    }
    label->setPixmap(QPixmap::fromImage(image));
    return true;
}

int VideoWidget::getSeparating() {
    return separating;
}
