#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QIntValidator>
#include <QDebug>
#include <QFuture>
#include <QtConcurrent/QtConcurrentRun>
#include "dmvtestercommunicator.h"
#include <QDateTime>
#include <QFile>
#include <prijava.h>
#include <podaci.h>
//#include <prijava.cpp>

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();


private slots:
	void onFrameTimer();
    void onPlocaPatternTimer();
    void onTesterTimer();
    void onShooterTimer();

	void on_startBtn_clicked();
	void on_stopBtn_clicked();

    void on_podesavanjaBtn_clicked();

    void on_enableVKBtn_clicked()  { mTester.enableVK(); }
    void on_disableVKBtn_clicked() { mTester.enableVK(); }

    void on_enableTestBtn_clicked()  { mTester.enableTest();  }
    void on_disableTestBtn_clicked() { mTester.disableTest(); }

    void on_speed0Btn_clicked() { mTester.speed0(); }
    void on_speed1Btn_clicked() { mTester.speed1(); }
    void on_speed2Btn_clicked() { mTester.speed2(); }

    void on_prevStepBtn_clicked() { mTester.prevStep(); }
    void on_nextStepBtn_clicked() { mTester.nextStep(); }

    void on_nadjiDelay_clicked();

    void citajBoje();

    void predjiNaSledeci();

    void on_prevPatternBtn_clicked() { mTester.prevPattern(); }
    void on_nextPatternBtn_clicked() { mTester.nextPattern(); }

    void on_praviMaskuBtn_clicked();



	void on_brightBtn_clicked();
    int vrtiPaterne(int par);
    void brojKorakaPoPaternu(int re,int ko);

    int proveraSlike(int patt,int kor,cv::Mat mat,cv::Mat bela);

    void on_obeleziBtn_clicked();
    void testiranjeAuto();
    //void called_from_async();
private:
    DMVTesterCommunicator mTester;

	Ui::MainWindow* ui;
    Prijava *prijav;


	QTimer           m_frameTimer;
    QTimer           plocaPatternTimer;
    QTimer           testerTimer;
    QTimer           shooterTimer;
    cv::VideoCapture m_videoCapture;
	cv::Mat          m_mat;

public:
    void vratiBoje(QString bojee);
    Prijava *prijavaaaaaa;
    podaci *podaciSave;
    //void podesavanjeRef(Prijava *prijava);
};

#endif // MAINWINDOW_H
