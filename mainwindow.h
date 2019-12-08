#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QIntValidator>
#include <QDebug>
#include "dmvtestercommunicator.h"

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

	void on_startBtn_clicked();
	void on_stopBtn_clicked();

    void on_enableVKBtn_clicked()  { mTester.enableVK(); }
    void on_disableVKBtn_clicked() { mTester.enableVK(); }

    void on_enableTestBtn_clicked()  { mTester.enableTest();  }
    void on_disableTestBtn_clicked() { mTester.disableTest(); }

    void on_speed0Btn_clicked() { mTester.speed0(); }
    void on_speed1Btn_clicked() { mTester.speed1(); }
    void on_speed2Btn_clicked() { mTester.speed2(); }

    void on_prevStepBtn_clicked() { mTester.prevStep(); }
    void on_nextStepBtn_clicked() { mTester.nextStep(); }

    void on_prevPatternBtn_clicked() { mTester.prevPattern(); }
    void on_nextPatternBtn_clicked() { mTester.nextPattern(); }

	void on_brightBtn_clicked();
    void preracunajPozicije();

    void on_obeleziBtn_clicked();
    void obelezavanje(int i,int j);

private:
    DMVTesterCommunicator mTester;

	Ui::MainWindow* ui;
	QIntValidator*  m_fpsIntV = new QIntValidator(1, 1000, this);
	QIntValidator*  m_brtIntV = new QIntValidator(1, 100 , this);

	QTimer           m_frameTimer;
	cv::VideoCapture m_videoCapture;
	cv::Mat          m_mat;

};

#endif // MAINWINDOW_H
