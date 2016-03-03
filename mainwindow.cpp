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
//    for(int i = 0; i < m1.rows; i++){
//        q.at<double>(i,0) = i-resto;
//    }
//    for (int i = 0; i < m1.rows;i++){
//        m1.at<double>(i,1)  = q.at<double>(i,0);
//        m1.at<double>(i,2) = qPow(q.at<double>(i,0),2);

//    }
//    for(int i = 0 ; i < m1.rows;i++){
//        qDebug( ) <<m1.at<double>(i,0) <<m1.at<double>(i,1) << m1.at<double>(i,2);
//    }
    //smoothBySavitzkyGolay(xAxisVector,corrRedVector,251,2);

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

QVector<double> MainWindow::smoothBySavitzkyGolay(QVector<double> x, QVector<double> y, int f, int grade)
{
    QElapsedTimer *mtimer = new QElapsedTimer();
    mtimer->start();
    std::vector<double> xVect =x.toStdVector();
    std::vector<double> yVect =y.toStdVector();
    cv::Mat cvX = cv::Mat(xVect);
    cv::Mat cvY = cv::Mat(yVect);
    cv::Mat solution;
    int n = x.length();
    f = std::min(f,n);
    f = f- ((f-1) % 2);
    //cv::Mat idx = cv::Mat(n,1,CV_8U);
    cv::Mat q = cv::Mat(f,1,CV_64F);
    int ncols = grade+1;
    cv::Mat v = cv::Mat::ones(f,ncols,CV_64F);

    QVector<double> c;
    int L,R,HF;
//    for(int i= 1; i <= n;i++){
//        idx.at< unsigned int>(i-1,0) = i;
//    }

    for(int i= 1; i <= n;i++){
        L=i;
        R=i;
        HF = std::ceil(std::max(0,(f - (R-L+1))/2));
        L = std::min(n-f+1, std::max(1,L-HF));
        R = std::min(n,std::max(R+HF,L+f-1));
        cv::subtract(cvX(cv::Rect(0,L-1,1,f) ), cvX.at<cv::Scalar>(i-1,0), q );
//        if(i >4999){
//            qDebug() << fromCVMatDoubleToQVector(q);
//            cv::Mat m;
//            cv::pow(q, 2, m);
//            qDebug() << fromCVMatDoubleToQVector(m);
//        }

        for (int j = 1; j < ncols;j++){
            cv::pow(q, j, v.col(j));

        }
        cv::solve(v,cvY( cv::Rect(0,L-1,1,f) ),solution,cv::DECOMP_QR);

        c.append( solution.at<double>(0,0) );

    }
    qDebug() << mtimer->elapsed();
    return c;

}

void MainWindow::on_pushButton_clicked()
{
    QTextStream in;
    QString lineReaded;
    QStringList splitList;
    QString fileName = "/Users/GIBICUDEA/Desktop/prueba_123.txt"; //QFileDialog::getOpenFileName(0,tr("Nombre de Archivo"),QString(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)),tr("Text files (*.txt)"));
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
//    stdCorrRedVector = corrRedVector.toStdVector();
//    m2 = cv::Mat(stdCorrRedVector);
//    qDebug() <<  m2.isContinuous();
//    //qDebug() << m2.at<double>(0,0);
//    partOfY = m2(cv::Rect(0,0,1,250));
//    m1.col(1) = cv::Range(1,250);
//    m1.col(2) = cv::Range(1,250);

//    cv::solve(m1,partOfY,qr,cv::DECOMP_QR);

//    for(int i = 0 ; i < qr.rows;i++){
//        qDebug( ) <<qr.at<double>(i,0); //<<m1.at<double>(i,1) <<m1.at<double>(i,2);
//    }
//    qDebug() << fromCVMatDoubleToQVector(partOfY);

//    //qDebug() << fromCVMatDoubleToQVector(m5);

    mVector = smoothBySavitzkyGolay(xAxisVector,corrRedVector,251,2);

}

void MainWindow::on_pushButton_2_clicked()
{
    QTextStream out;

       if( !mVector.isEmpty() ){
           QString fileName = QFileDialog::getSaveFileName(this,
                                                           tr("Guardar Archivo"),
                                                           QString(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)),tr("Text files (*.txt)"));
           if( ! fileName.isEmpty() ){
               //se dio una dirección: Código
               QFile file(fileName);
               file.open(QIODevice::WriteOnly | QIODevice::Text);
               if(!file.isOpen()){
                   qDebug()<< ":( vida gono";
               }else{
                   out.setDevice(&file);
                   out << "Frame" << "," << "Value RSmooth"  << endl;
                   for(int i =0; i < mVector.count();i++){
                       out << QString::number(xAxisVector.at(i))<<","<<QString::number(mVector.at(i))<< endl;
                   }
                   file.flush();
                   file.close();
               }
            }
       }else{
           //QMessageBox::warning(this,"Advertencia","No hay señales para exportar");
           qDebug() << "No hay señales para exportar";
       }

}
