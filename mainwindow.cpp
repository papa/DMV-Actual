#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dmvtestercommunicator.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "chrono"
#include "thread"
#include "ctime"
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
bool tru = true;
void MainWindow::on_startBtn_clicked()
{
    tru = true;
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


int redovi;
int kolone;
int centri[256][2];
pair<int,int> matrica[20][20];
int sens = 10;

void napraviMatricu()
{
    for(int i = 0;i<redovi;i++)
    {
        vector<pair<int,int> > v;
        for(int j = 0;j<kolone;j++)
        {
            int ind = i*kolone+j;
            v.push_back({centri[ind][1],centri[ind][0]});
        }
        sort(v.begin(),v.end());
        for(int j=0;j<kolone;j++)
        {
            matrica[i][j].first = v[j].second;
            matrica[i][j].second = v[j].first;
        }
    }
}

void odrediDimenzije(int nm)
{
    if(nm==64)
    {
        redovi = 8;
        kolone = 8;
    }
    else if(nm==256)
    {
        redovi = 16;
        kolone = 16;
    }
    else
    {
        //todo
        //proveri gde je y koordinata
        //razlika da zavisi od ukupne visine
        int y8 = centri[7][0];
        int y9 = centri[8][0];
        if(max(y9,y8)-min(y8,y9)<=20)
        {
            redovi = 8;
            kolone = 16;
        }
        else
        {
            redovi = 16;
            kolone = 8;
        }
    }
    napraviMatricu();
}

void MainWindow::on_obeleziBtn_clicked()
{
    /*mTester.nextPattern();
    clock_t wait = 2000 + clock();
    while(wait>clock());
    mTester.nextStep();
    wait = 2000 + clock();
    while(wait>clock());*/
    //todo
    inRange(m_mat, Scalar(255-sens, 255-sens, 255-sens), Scalar(255, 255, 255), m_mat);
    tru = false;
    int belo;
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
                qDebug() << centri[c][0] << " " << centri[c][1] << "\n";
                c++;
            }
        }
    }
    odrediDimenzije(c);
    for(int i=0;i<redovi;i++)
    {
        qDebug() << "novi red";
        for(int j=0;j<kolone;j++)
        {
            qDebug() << matrica[i][j].first << " " << matrica[i][j].second;
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
    if(tru){
    if (!m_videoCapture.isOpened() || !m_videoCapture.read(m_mat) || m_mat.empty())
	{
        on_stopBtn_clicked();
		return;
    }
    //Indijac se zove Sadekar
    imshow("Output", m_mat);
}
}

void MainWindow::on_brightBtn_clicked()
{
	const quint8 value = ui->brightLineEdit->text().toUInt();
    mTester.setBrightness(value);
}

void testiraj()
{

}

void MainWindow::preracunajPozicije()
{
    //zastava
    //sve
    //4
    //vertikalne
    //horizontalne
    //sporedna
    //glavna
    int trenutniRaspored[10][30][redovi][kolone];
    int pattern;
    int cnt;

    //pattern 0 zastava

    for(int i=0;i<redovi/4;i++)
        for(int j=0;j<kolone;j++)
           trenutniRaspored[0][0][i][j] = 1;

    for(int i=redovi/4;i<redovi/2;i++)
        for(int j=0;j<kolone;j++)
           trenutniRaspored[0][0][i][j] = 3;

    for(int i=redovi/2;i<redovi*3/4;i++)
        for(int j=0;j<kolone;j++)
           trenutniRaspored[0][0][i][j] = 4;

    //testiraj();
    //mTester.nextPattern();

    //pattern 1 svi
    for(int i=0;i<redovi;i++)
        for(int j=0;j<kolone;j++)
            for(int k = 1;k<=6;k++)
                trenutniRaspored[1][k-1][i][j] = k;

   //pattern 2

   int r1 = 0;
   int k1 = 0;
   int r2 = 0;
   int k2 = kolone/2 + 1;
   int r3 = redovi/2 + 1;
   int k3 = 0;
   int r4 = redovi/2+1;
   int k4 = kolone/2+1;

   cnt = 0;

   for(int k=1;k<=6;k++)
   {
       while(k1 < kolone/2 -1)
       {
           trenutniRaspored[2][cnt][r1][k1] =  trenutniRaspored[2][cnt][r2][k2] = trenutniRaspored[2][cnt][r3][k3] =
                   trenutniRaspored[2][cnt][r4][k4] =k;
           cnt++;
           k1++;
           k2++;
           k3++;
           k4++;
       }
       trenutniRaspored[2][cnt][r1][k1] = trenutniRaspored[2][cnt][r3][k1] = k;
       cnt++;
       k1++;
       trenutniRaspored[2][cnt][r1][k1] = trenutniRaspored[2][cnt][r3][k1] = k;
       //dovrsi
   }

   // mTester.nextPattern();

    //pattern 3 kolone


    cnt = 0;
    for(int k=1;k<=6;k++)
    {
        int k1 = 0;
        int k2 = kolone/2+1;
        while(k1<(kolone/2-1))
        {
            for(int i=0;i<redovi;i++)
            {
                trenutniRaspored[3][cnt][i][k1] = trenutniRaspored[3][cnt][i][k2] = k;
            }
            k1++;
            k2++;
            cnt++;
        }
        for(int i=0;i<redovi;i++) trenutniRaspored[3][cnt][i][k1]  = k;
        cnt++;
        k1++;
        for(int i=0;i<redovi;i++) trenutniRaspored[3][cnt][i][k1]  = k;
        cnt++;
    }


    //pattern 4 redovi

    cnt = 0;
    for(int k=1;k<=6;k++)
    {
        int r1 = 0;
        int r2 = redovi/2+1;
        while(r1 < (redovi/2-1))
        {
            for(int i = 0;i<kolone;i++)
            {
                trenutniRaspored[4][cnt][r1][i] = trenutniRaspored[4][cnt][r2][i] = k;
            }
            r1++;
            r2++;
            cnt++;
        }
        for(int i=0;i<kolone;i++) trenutniRaspored[4][cnt][r1][i] = k;
        cnt++;
        r1++;
        for(int i=0;i<kolone;i++) trenutniRaspored[4][cnt][r1][i] = k;
        cnt++;
    }

    //pattern 5

}
