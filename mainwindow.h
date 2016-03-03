#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QVector>
#include <QFileDialog>
#include <QElapsedTimer>
#include <QDebug>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video.hpp>
#include <opencv2/videoio.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QVector<double> fromCVMatDoubleToQVector(cv::Mat m);
    QVector<double> smoothBySavitzkyGolay(QVector<double> x, QVector<double> y, int f, int grade );

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    cv::Mat m1;
    cv::Mat m2;
    cv::Mat m5;
    cv::Mat partOfY;
    cv::Mat qr;
    cv::Mat q;
    QVector<double> corrRedVector;
    QVector<double> corrGreenVector;
    QVector<double> corrBlueVector;
    QVector<double> xAxisVector;
    QVector<double> redSmooth;
    QVector<double> greenSmooth;
    QVector<double> blueSmooth;
    std::vector<double> stdCorrRedVector;
};

#endif // MAINWINDOW_H
