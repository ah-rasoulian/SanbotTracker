#include "videoprocessor.h"
#include <QThread>

VideoProcessor::VideoProcessor()
{
    qRegisterMetaType<cv::Rect2d>();
    qRegisterMetaType<cv::Rect>();
    qRegisterMetaType<cv::Rect*>();
    qRegisterMetaType<cv::Mat>();
    isRobotTracked = false;
    isObstaclesDetected = false;
    isCapturingActivate = false;

    tracker = new RobotTracker();
    tracker->moveToThread(new QThread());
    connect(tracker->thread(), &QThread::started, tracker, &RobotTracker::startTracking);
    connect(tracker->thread(), &QThread::finished, tracker, &RobotTracker::deleteLater);
    connect(tracker, &RobotTracker::trackingBox, this, &VideoProcessor::getTrackingBox, Qt::DirectConnection);
    connect(this, &VideoProcessor::updateFrame , tracker, &RobotTracker::getFrame, Qt::DirectConnection);
    tracker->thread()->start();

    detector = new RobotDetector();
    detector->moveToThread(new QThread());
    connect(detector->thread(), &QThread::started, detector, &RobotDetector::startDetecting);
    connect(detector->thread(), &QThread::finished, detector, &RobotDetector::deleteLater);
    connect(detector, &RobotDetector::detectingBox, tracker, &RobotTracker::getDetectingBox, Qt::DirectConnection);
    connect(this, &VideoProcessor::updateHSVFrame , detector, &RobotDetector::getHSVFrame, Qt::DirectConnection);
    detector->thread()->start();

    obsDetector = new ObstaclesDetector();
    obsDetector->moveToThread(new QThread());
    connect(obsDetector->thread(), &QThread::started, obsDetector, &ObstaclesDetector::startDetecting);
    connect(detector->thread(), &QThread::finished, obsDetector, &ObstaclesDetector::deleteLater);
    connect(obsDetector, &ObstaclesDetector::detectingObstacles, this, &VideoProcessor::getObstaclesBoxes, Qt::DirectConnection);
    connect(this, &VideoProcessor::updateHSVFrame, obsDetector, &ObstaclesDetector::getHSVFrame, Qt::DirectConnection);
    obsDetector->thread()->start();
}

VideoProcessor::~VideoProcessor(){
    tracker->stopTracking();
    tracker->thread()->quit();
    tracker->thread()->wait();

    detector->stopDetecting();
    detector->thread()->quit();
    detector->thread()->wait();

    obsDetector->stopDetecting();
    obsDetector->thread()->quit();
    obsDetector->thread()->wait();
}

void VideoProcessor::startVideo(){
    using namespace cv;
    camera.open("/home/amirhossein/Data/Sanbot Files/IMG_2388.MOV");
    // camera.open(0);

    VideoCapture camera2;
    Mat frame1;
    Mat frame2;
    camera2.open("/home/amirhossein/Data/Sanbot Files/norobot.wmv");

    stoped = false;
    while(camera.isOpened() && camera2.isOpened() && !stoped){
        camera >> frame1;
        camera2 >> frame2;

        if(frame1.empty() || frame2.empty())
            continue;

        resize(frame2, frame2, frame1.size(), 0, 0, INTER_CUBIC);
        hconcat(frame1, frame2, frame1);
        hconcat(frame2, frame2, frame2);
        vconcat(frame1, frame2, frame1);

        if(frame1.size().width < frame1.size().height)
            resize(frame1, frame, Size(frame1.size().width * 800 / frame1.size().height, 800), 0, 0, INTER_CUBIC);
        else
            resize(frame1, frame, Size(1600, frame1.size().height * 1600 / frame1.size().width), 0, 0, INTER_CUBIC);


        cv::cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV);
        emit updateFrame(frame);
        emit updateHSVFrame(hsvFrame);

        if (isCapturingActivate) {
            emit sendBox(robotBox);
        }

        // proccessing
        if(isObstaclesDetected)
            for (int i = 0; i < numberOfObstacles; i++)
                cv::rectangle(frame, obstacles[i], cv::Scalar(0, 255, 0), 2);
        if(isRobotTracked)
            cv::rectangle(frame, robotBox , cv::Scalar(255, 0, 0), 2);

        // displaying
        emit primaryDisplay(
                    QPixmap::fromImage(
                              QImage(static_cast<uchar*>(frame.data), frame.cols, frame.rows, static_cast<int>(frame.step), QImage::Format_RGB888)
                              .rgbSwapped()
                        )
                    );
    }
}

void VideoProcessor::stopVideo(){
    stoped = true;
}

void VideoProcessor::getTrackingBox(cv::Rect2d newRobotBox){
    robotBox = newRobotBox;
    isRobotTracked = true;
}

void VideoProcessor::getObstaclesBoxes(cv::Rect* newObstacleBoxes, int newNumberOfObstacles){
    obstacles = newObstacleBoxes;
    numberOfObstacles = newNumberOfObstacles;
    isObstaclesDetected = true;
}


void VideoProcessor::startCapturing(){
    isCapturingActivate = true;
}

void VideoProcessor::stopCapturing(){
    isCapturingActivate = false;
    emit showResult(frame);
}
