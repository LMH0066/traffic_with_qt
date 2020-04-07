#ifndef TRACKER_H
#define TRACKER_H

#include "overall.h"

struct TrackerSettings {
    ///
    /// \brief m_useLocalTracking
    /// Use local tracking for regions between two frames
    /// It was coined for tracking small and slow objects: key points on objects
    /// tracking with LK optical flow The most applications don't need this
    /// parameter
    ///
    bool m_useLocalTracking = false;

    //    tracking::DistType m_distType = tracking::DistCenters;
    //    tracking::KalmanType m_kalmanType = tracking::KalmanLinear;
    //    tracking::FilterGoal m_filterGoal = tracking::FilterCenter;
    //    tracking::LostTrackType m_lostTrackType = tracking::TrackKCF;
    //    tracking::MatchType m_matchType = tracking::MatchHungrian;

    ///
    /// \brief m_dt
    /// Time step for Kalman
    ///
    double m_dt = 1;

    ///
    /// \brief m_accelNoiseMag
    /// Noise magnitude for Kalman
    ///
    double m_accelNoiseMag = 0.1;

    ///
    /// \brief m_distThres
    /// Distance threshold for Assignment problem for tracking::DistCenters or
    /// for tracking::DistRects (for tracking::DistJaccard it need from 0 to 1)
    ///
    double m_distThres = 50;

    ///
    /// \brief m_maximumAllowedSkippedFrames
    /// If the object don't assignment more than this frames then it will be
    /// removed
    ///
    size_t m_maximumAllowedSkippedFrames = 25;

    ///
    /// \brief m_maxTraceLength
    /// The maximum trajectory length
    ///
    size_t m_maxTraceLength = 50;

    ///
    /// \brief m_useAbandonedDetection
    /// Detection abandoned objects
    ///
    bool m_useAbandonedDetection = false;

    ///
    /// \brief m_minStaticTime
    /// After this time (in seconds) the object is considered abandoned
    ///
    int m_minStaticTime = 5;
    ///
    /// \brief m_maxStaticTime
    /// After this time (in seconds) the abandoned object will be removed
    ///
    int m_maxStaticTime = 25;
};

class Tracker {
  private:
    int frame_width;
    int frame_height;
    Mat current_frame;
    TrackerSettings track_setting;
    Mat prev_frame;
  public:
    Tracker();
    void setFrameWidth(int width);
    void setFrameHeight(int height);
    void setCurrentFrame(Mat frame);
    int startTrack();
    void update();
};

#endif  // TRACKER_H
