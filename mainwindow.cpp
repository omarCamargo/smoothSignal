#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m1 = cv::Mat::ones(250,3,CV_64F);
    q = cv::Mat::ones(250,1,CV_64F);
    int resto = 2;
    for(int i = 0; i < m1.rows; i++){
        q.at<double>(i,0) = i-resto;
    }
    for (int i = 0; i < m1.rows;i++){
        m1.at<double>(i,1)  = q.at<double>(i,0);
        m1.at<double>(i,2) = qPow(q.at<double>(i,0),2);

    }
    for(int i = 0 ; i < m1.rows;i++){
        qDebug( ) <<m1.at<double>(i,0) <<m1.at<double>(i,1) << m1.at<double>(i,2);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

QVector<double> MainWindow::fromCVMatDoubleToQVector(cv::Mat m)
{
    std::vector<double> vect(m);
    return QVector<double>::fromStdVector(vect);
}

void MainWindow::on_pushButton_clicked()
{
    QTextStream in;
    QString lineReaded;
    QStringList splitList;
    QString fileName = QFileDialog::getOpenFileName(0,tr("Nombre de Archivo"),QString(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)),tr("Text files (*.txt)"));
    qDebug() << "Nombre archivo: " << fileName;
    if( ! fileName.isEmpty() ){
        QFile file(fileName);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        if(!file.isOpen()){
            qDebug()<< ":( vida gono";
        }else{
            in.setDevice(&file);
            lineReaded = in.readLine();
            lineReaded = in.readLine();
            while( !lineReaded.isNull() ){

                splitList = lineReaded.split(",",QString::SkipEmptyParts);
                xAxisVector.append(splitList.at(0).toDouble());
                corrRedVector.append(splitList.at(1).toDouble());
                //corrGrennVector.append(splitList.at(2).toDouble());
                //corrBlueVector.append(splitList.at(3).toDouble());
                lineReaded = in.readLine();
            }


            file.close();

        }

    }else{
        qDebug() <<"Problema";
    }
    stdCorrRedVector = corrRedVector.toStdVector();
    m2 = cv::Mat(stdCorrRedVector);
    qDebug() <<  m2.isContinuous();
    //qDebug() << m2.at<double>(0,0);
    partOfY = m2(cv::Rect(0,0,1,250));
//    m1.col(1) = cv::Range(1,250);
//    m1.col(2) = cv::Range(1,250);

    cv::solve(m1,partOfY,qr,cv::DECOMP_QR);

    for(int i = 0 ; i < qr.rows;i++){
        qDebug( ) <<qr.at<double>(i,0); //<<m1.at<double>(i,1) <<m1.at<double>(i,2);
    }
    qDebug() << fromCVMatDoubleToQVector(partOfY);

    //qDebug() << fromCVMatDoubleToQVector(m5);



}
