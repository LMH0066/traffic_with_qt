#ifndef DETECTOR_H
#define DETECTOR_H

#include "overall.h"
#include "clipp.hpp"
#include "common.hpp"
#include "tcpconnect.h"

using namespace dnn;

class Detector {
  public:
    Detector();
    void Init_mxnet();
    int detect();
    void detect_mxnet(int* number);
    void frameMerge();          //图像拼接
    void goRestart();           //重新传入图片识别
    void pushFrame(Mat frame, Mat frame1);
    void initTcp();
    Mat now_frame[2][8];
    bool want_tcp = false;      //是否通信
    bool empty_tracking;        //是否清空跟踪所依据的信息
  private:
    String weights, protxt;
    Mat my_frame[2], frames[2][8];
    Net net;
    float confidenceThreshold;
    int frame_number;

    String model_mxnet;
    Executor *exec;
    Context ctx = Context::cpu();
    Symbol net_mxnet;
    std::map<std::string, NDArray> args, auxs;
    int min_size, max_size;
    int multiplier;
    int top_cols = 0, top_rows = 0;
    int bottom_cols = 0, bottom_rows = 0;

    TcpConnect *tcp_connect = nullptr;
};

extern Detector m_detector;

#endif // DETECTOR_H
