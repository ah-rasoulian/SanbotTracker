#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore/QObject>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    processor = new VideoProcessor();
    processor->moveToThread(new QThread());
    ui->setupUi(this);
    connect(processor->thread(), &QThread::started, processor, &VideoProcessor::startVideo);
    connect(processor->thread(), &QThread::finished, processor, &VideoProcessor::deleteLater);
    connect(processor, SIGNAL(primaryDisplay(QPixmap)), ui->primaryLabel, SLOT(setPixmap(QPixmap)));
    connect(this, &MainWindow::startCapturing, processor, &VideoProcessor::startCapturing, Qt::DirectConnection);
    connect(this, &MainWindow::stopCapturing, processor, &VideoProcessor::stopCapturing, Qt::DirectConnection);
    processor->thread()->start();

    capture = new PathCapture();
    capture->moveToThread(new QThread());
    connect(capture, SIGNAL(resultDisplay(QPixmap)), ui->resultLabel, SLOT(setPixmap(QPixmap)));
    connect(processor, &VideoProcessor::showResult, capture, &PathCapture::showResult);
    connect(processor, &VideoProcessor::sendBox, capture, &PathCapture::recieveBox);
    capture->thread()->start();
}

MainWindow::~MainWindow()
{
    processor->stopVideo();
    processor->thread()->quit();
    processor->thread()->wait();

    capture->thread()->quit();
    capture->thread()->wait();
    delete ui;
}

void MainWindow::on_pathButton_clicked()
{
    if(ui->pathButton->text() == "start"){
        emit startCapturing();
        ui->pathButton->setText("stop");
    }
    else{
        emit stopCapturing();
        ui->pathButton->setText("start");
    }
}

void MainWindow::on_goBackButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_resultPage_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
