#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dmvtestercommunicator.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "chrono"
#include "thread"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->fpsLineEdit   ->setValidator(m_fpsIntV);
	ui->brightLineEdit->setValidator(m_brtIntV);

	m_frameTimer.setTimerType(Qt::TimerType::PreciseTimer);
	m_frameTimer.setSingleShot(false);
	QObject::connect(&m_frameTimer, &QTimer::timeout         ,
                     this         , &MainWindow::onFrameTimer);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_startBtn_clicked()
{
    for(int i=0;i<6;i++)
    {
       mTester.prevPattern();
       std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }

	if (m_videoCapture.isOpened()) m_videoCapture.release();

	if (   !m_videoCapture.open(ui->sourceLineEdit->text().toUtf8().constData())
	    || !m_videoCapture.isOpened())
	{
		qDebug() << "SOURCE ERROR";
		return;
	}

	const quint16 fps = ui->fpsLineEdit->text().toUInt();
	m_frameTimer.setInterval(1000 / (fps ? fps : 1));
	m_frameTimer.start();
}
uint8_t* pixelPtr;
int iMax,iMin,jMax,jMin;
void MainWindow::obelezavanje(int i,int j)
{
    if(i<iMin)iMin=i;
    else if(i>iMax)iMax=i;
    if(j<jMin)jMin=j;
    else if(j>jMax)jMax=j;

    pixelPtr[i*m_mat.cols+j]=0;

    if(i>0 && pixelPtr[(i-1)*m_mat.cols+j]!=0)
        obelezavanje(i-1,j);
    if(i<m_mat.rows-1 && pixelPtr[(i+1)*m_mat.cols+j]!=0)
        obelezavanje(i+1,j);
    if(j>0 && pixelPtr[i*m_mat.cols+j-1]!=0)
        obelezavanje(i,j-1);
    if(j<m_mat.cols-1 && pixelPtr[i*m_mat.cols+j+1]!=0)
        obelezavanje(i,j+1);
}

void MainWindow::on_obeleziBtn_clicked()
{
    mTester.nextPattern();
    this_thread::sleep_for(chrono::milliseconds(100));
    mTester.nextStep();
    this_thread::sleep_for(chrono::milliseconds(5000));
    mTester.prevStep();
    this_thread::sleep_for(chrono::milliseconds(5000));
    m_frameTimer.stop();
    m_videoCapture.release();
    int belo;
    int centri[256][2];
    int c=0;
    pixelPtr = (uint8_t*)m_mat.data;
    for(int i=0;i<m_mat.rows;i++)
    {
        for(int j=0;j<m_mat.cols;j++)
        {
            belo=pixelPtr[i*m_mat.cols+j];
            if(belo)
            {
                iMax=i;
                iMin=i;
                jMax=j;
                jMin=j;
                obelezavanje(i,j);
                centri[c][1]=(jMin+jMax)/2;
                centri[c][0]=(iMin+iMax)/2;
                c++;
            }
        }
    }
    qDebug() << "show";
    for(int i=0;i<c;i++)
    {
        pixelPtr[centri[i][0]*m_mat.cols+centri[i][1]]=255;
    }
    imshow("Output",m_mat);
}

void MainWindow::on_stopBtn_clicked()
{
	m_frameTimer.stop();
	m_videoCapture.release();
	cv::destroyAllWindows();
}

void MainWindow::onFrameTimer()
{
    if (!m_videoCapture.isOpened() || !m_videoCapture.read(m_mat) || m_mat.empty())
	{
        on_stopBtn_clicked();
		return;
    }
    //Indijac se zove Sadekar
    int sens = 10;
    inRange(m_mat, Scalar(255-sens, 255-sens, 255-sens), Scalar(255, 255, 255), m_mat);
    imshow("Output", m_mat);
}

void MainWindow::on_brightBtn_clicked()
{
	const quint8 value = ui->brightLineEdit->text().toUInt();
    mTester.setBrightness(value);
}


