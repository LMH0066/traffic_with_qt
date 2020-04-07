#include "tracker.h"

Tracker::Tracker() {}

void Tracker::setFrameWidth(int width) {
    frame_width = width;
}

void Tracker::setFrameHeight(int height) {
    frame_height = height;
}

void Tracker::setCurrentFrame(Mat frame) {
    current_frame = frame;
}

int Tracker::startTrack() {
    if (true) {
        cout << "1" << endl;
    }
}

void Tracker::update() {

}
