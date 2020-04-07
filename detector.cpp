#include "detector.h"

Detector m_detector;

Detector::Detector() {
    model_mxnet = "yolo3_darknet53_voc";
    Init_mxnet();
    confidenceThreshold = 0.4f;
    frame_number = 0;
    initTcp();
    empty_tracking = true;
}

void Detector::Init_mxnet() {
    LoadCheckpoint(model_mxnet, 0, &net_mxnet, &args, &auxs, ctx);
    min_size = 512;
    max_size = 1024;
    multiplier = 32;
}


namespace synset {
// some commonly used datasets
static std::vector<std::string> VOC_CLASS_NAMES = {
    "aeroplane", "bicycle", "bird", "boat",
    "bottle", "bus", "car", "cat", "chair",
    "cow", "diningtable", "dog", "horse",
    "motorbike", "person", "pottedplant",
    "sheep", "sofa", "train", "tvmonitor"
};
};

//将得到的所有图片拼接在一起
void Detector::frameMerge() {
    if(!frame_number) return;
    if(frame_number == 1) {
        my_frame[0] = frames[0][0];
        my_frame[1] = frames[1][0];
        now_frame[0][0] = my_frame[0];
        now_frame[1][0] = my_frame[1];
        return;
    }
    Mat top, bottom;
    top_cols = top_rows = 0;
    bottom_cols = bottom_rows = 0;
    for (int i = 0; i < frame_number/2; i++) {
        top_cols += frames[0][i].cols;
        bottom_cols += frames[0][i + frame_number/2].cols;
        top_rows = MAX(frames[0][i].rows, top_rows);
        bottom_rows = MAX(frames[0][i + frame_number/2].rows, bottom_rows);
    }
    Size size(MAX(top_cols, bottom_cols), bottom_rows + top_rows);
    my_frame[0] = Mat(size, CV_MAKETYPE(frames[0][0].depth(), 3), Scalar(0, 0, 0));
    my_frame[1] = Mat(size, CV_MAKETYPE(frames[1][0].depth(), 3), Scalar(0, 0, 0));
    int now_cols = 0;
    for (int i = 0; i < frame_number/2; i++) {
        now_frame[0][i] = my_frame[0](Rect(now_cols, 0, frames[0][i].cols, frames[0][i].rows));
        now_frame[1][i] = my_frame[1](Rect(now_cols, 0, frames[1][i].cols, frames[1][i].rows));
        now_cols += frames[0][i].cols;
        frames[0][i].copyTo(now_frame[0][i]);
        frames[1][i].copyTo(now_frame[1][i]);
    }
    now_cols = 0;
    for (int i = frame_number/2; i < frame_number; i++) {
        now_frame[0][i] = my_frame[0](Rect(now_cols, top_rows, frames[0][i].cols, frames[0][i].rows));
        now_frame[1][i] = my_frame[1](Rect(now_cols, top_rows, frames[1][i].cols, frames[1][i].rows));
        now_cols += frames[0][i].cols;
        frames[0][i].copyTo(now_frame[0][i]);
        frames[1][i].copyTo(now_frame[1][i]);
    }
//    namedWindow("image", 0);
//    imshow("image", my_frame[0]);
//    waitKey(0);
}

void Detector::goRestart() {
    frame_number = 0;
}

void Detector::pushFrame(Mat frame, Mat frame1) {
    frames[0][frame_number] = frame;
    frames[1][frame_number] = frame1;
    frame_number += 1;
}

int Detector::detect() {
    if(!frame_number) return 0;
    frameMerge();
    int number[8] = {0};
    Mat inputBlob =
        blobFromImage(my_frame[0], inScaleFactor, Size(inWidth, inHeight),
                      Scalar(meanVal, meanVal, meanVal), false, false);
    net.setInput(inputBlob);
    Mat detection = net.forward();

    Mat detectionMat(detection.size[2], detection.size[3], CV_32F,
                     detection.ptr<float>());
    cout<<detectionMat.size()<<endl;
    for (int i = 0; i < detectionMat.rows; i++) {
        for(int j = 0; j < 7; j++) {
            cout<<detectionMat.at<float>(i, j)<<" ";
        }
        cout<<endl;
        float confidence = detectionMat.at<float>(i, 2);
        if (confidence > confidenceThreshold) {
            size_t objectClass = (size_t)(detectionMat.at<float>(i, 1));
            int tl_x =
                static_cast<int>(detectionMat.at<float>(i, 3) * my_frame[0].cols);
            int tl_y =
                static_cast<int>(detectionMat.at<float>(i, 4) * my_frame[0].rows);
            int br_x =
                static_cast<int>(detectionMat.at<float>(i, 5) * my_frame[0].cols);
            int br_y =
                static_cast<int>(detectionMat.at<float>(i, 6) * my_frame[0].rows);
            rectangle(my_frame[1], Point(tl_x, tl_y), Point(br_x, br_y), Scalar(0, 0, 255), 3);
            number[0] += 1;
        }
    }
    return number[0];
}

void Detector::detect_mxnet(int *number) {
    if(!frame_number) return;
    frameMerge();
    clock_t t_r1 = clock();
    if (want_tcp) {
        tcp_connect->sendMessage(Mat2QImage(my_frame[0]));
        tcp_connect->tcpSocketConnection->waitForReadyRead();
        return;
    }
//    int number[8] = {0};
    double scale;
    Mat image = ResizeShortWithin(my_frame[0], min_size, max_size, multiplier, scale);
//    LOG(INFO) << "Image shape: " << image.cols << " x " << image.rows;
    auto data = AsData(image, ctx);
    args["data"] = data;
    exec = net_mxnet.SimpleBind(
               ctx, args, std::map<std::string, NDArray>(),
               std::map<std::string, OpReqType>(), auxs);
//    NDArray::WaitAll();
    exec->Forward(false);
    auto ids = exec->outputs[0].Copy(Context(kCPU, 0));
    auto scores = exec->outputs[1].Copy(Context(kCPU, 0));
    auto bboxes = exec->outputs[2].Copy(Context(kCPU, 0));
    NDArray::WaitAll();
//    cout << "my_frame[0].cols " << my_frame[0].cols << " my_frame[0].rows " << my_frame[0].rows << endl;
//    cout << "image.cols " << image.cols << " image.rows " << image.rows << endl;
    auto plt = viz::PlotBbox(my_frame[1], bboxes, scores, ids, confidenceThreshold, synset::VOC_CLASS_NAMES,
                             std::map<int, cv::Scalar>(), true, scale, frames[0], frame_number, top_rows, number, empty_tracking);
    empty_tracking = false;
    delete exec;
    clock_t t_r2 = clock();
    std::cout<<"time_all:"<<(t_r2 - t_r1) * 1.0 / CLOCKS_PER_SEC<<std::endl;
}

void Detector::initTcp() {
    if (tcp_connect == nullptr) {
        tcp_connect = new TcpConnect();
    }
}
