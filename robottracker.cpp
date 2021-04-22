#include "robottracker.h"

RobotTracker::RobotTracker()
{
    isFrameAvailable = false;
    isInitialBoxReceived = false;
    isRobotTracked = false;
    trackerFunction = cv::TrackerKCF::create();
}

void RobotTracker::startTracking(){
    using namespace cv;
    stoped = false;
    while (!stoped) {
        if(isRobotTracked){
            cv::Rect2d box;
            bool trackSuccess = trackerFunction->update(frame, box);
                if (trackSuccess){
                    isRobotTracked = true;
                    emit trackingBox(box);
                }
                else
                    isRobotTracked = false;
        }
        else {
            if(isInitialBoxReceived){
                trackerFunction->init(frame, initialBox);
                isInitialBoxReceived = false;
                isRobotTracked = true;
                emit trackingBox(initialBox);
            }
        }
        emit trackingBox(initialBox);
    }
}

void RobotTracker::stopTracking(){
    stoped = true;
}

void RobotTracker::getDetectingBox(cv::Rect newInitialBox){
    initialBox = newInitialBox;
}

void RobotTracker::getFrame(cv::Mat newFrame){
    frame = newFrame;
    isFrameAvailable = true;
}
