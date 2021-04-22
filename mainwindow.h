#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <videoprocessor.h>
#include <pathcapture.h>
#include <QThread>
#include <QTimer>
#include <QtPlugin>

class VideoProcessor;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pathButton_clicked();

    void on_goBackButton_clicked();

    void on_resultPage_clicked();

private:
    VideoProcessor *processor;
    PathCapture *capture;
    Ui::MainWindow *ui;


signals:
    void startCapturing();
    void stopCapturing();
};

#endif // MAINWINDOW_H
