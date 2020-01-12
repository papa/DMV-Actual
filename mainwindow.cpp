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
int redovi,kolone;
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
    QString kolones=ui->koloneTxt->text();
    QString redovis=ui->redoviTxt->text();
    redovi=redovis.toInt();
    kolone=kolones.toInt();
    const quint16 fps = ui->fpsLineEdit->text().toUInt();
    m_frameTimer.setInterval(1000 / (fps ? fps : 1));
    m_frameTimer.start();
}
cv::Mat abe;
uint8_t* pixelPtr;
int iMax,iMin,jMax,jMin;
int brkomp;
void MainWindow::obelezavanje(int i,int j)
{
    brkomp++;
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


int centri[256][2];
pair<int,int> matrica[20][20];
int sens = 20;
int niz[7];
int matricaBoja[3][12]={
    {254,255,100,190,80,170,200,255,170,255,220,255},
    {150,220,200,255,165,255,200,255,140,225,140,205},
    {140,200,170,255,254,255,200,255,100,170,110,165}
};
int Vri=1,Vrj=0,Vrk=1;

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

    /*qDebug() << "Matrica koordinate";
    for(int i=0;i<redovi;i++)
    {
        for(int j=0;j<kolone;j++)
        {
            qDebug() << matrica[i][j].first << " " << matrica[i][j].second;
        }
        qDebug() << "\n";
    }
    qDebug() << "\n";*/

}

void odrediDimenzije(int nm)
{
    if(nm<kolone*redovi)
    {
        //todo izbaci error kako se vec izbacuju errori
        qDebug()<<kolone*redovi-nm;
    }
    /*else if(nm==256)
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
    }*/
    napraviMatricu();
}

int trenutniRaspored[10][80][20][20];

void stampa(int p,int c)
{
    for(int i =0;i<redovi;i++)
    {
        QString s = "";
        for(int j=0;j<kolone;j++) s = s + QString::number(trenutniRaspored[p][c][i][j]) + " ";
        qDebug() << s;
    }
    qDebug() << "\n";
}

void provera()
{
    //pattern 0
    qDebug() << 0;
    stampa(0,0);

    //pattern 1;
    qDebug() << 1;
    stampa(1,0);

    //pattern 2
    qDebug() << 2;
    for(int c =0;c<10;c++)
        stampa(2,c);

    //pattern 3
    qDebug() << 3;
    for(int c =0;c<10;c++)
        stampa(3,c);

    //pattern 4
    qDebug() << 4;
    for(int c =0;c<10;c++)
        stampa(4,c);

    //pattern 5
    qDebug() << 5;
    for(int c =0;c<10;c++)
        stampa(5,c);

    //pattern 6
    qDebug() << 6;
    for(int c =0;c<10;c++)
        stampa(6,c);
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
    int pattern;
    int cnt;

    qDebug() << "pozvao";

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

    qDebug() << "proso";

    //pattern 1 svi
    for(int i=0;i<redovi;i++)
        for(int j=0;j<kolone;j++)
            for(int k = 1;k<=6;k++)
                trenutniRaspored[1][k-1][i][j] = k;

    qDebug() << "proso";

   //pattern 2

   cnt = 0;

   for(int k=1;k<=6;k++)
   {
       int r1,r2,r3,r4;
       int k1,k2,k3,k4;

       for(int i=0;i<redovi/2-1;i++)
       {
          r1 = i;
          r3 = i+redovi/2+1;
          r2 = i;
          r4 = r3;
          k1 = 0;
          k2 = kolone/2+1;
          k3 = 0;
          k4 = kolone/2+1;
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
           cnt++;
       }

       for(int i=redovi/2-1;i<=redovi/2;i++)
       {
            r1 = i;
            k1 = 0;
            r2 = r1;
            k2 = kolone/2+1;
            while(k1 < kolone/2-1)
            {
                trenutniRaspored[2][cnt][r1][k1] =  trenutniRaspored[2][cnt][r2][k2] = k;
                cnt++;
                k1++;
                k2++;
            }
            trenutniRaspored[2][cnt][r1][k1] = k;
            cnt++;
            k1++;
            trenutniRaspored[2][cnt][r1][k1] =  k;
            cnt++;
       }
   }

   qDebug() << "proso";

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


    qDebug() << "proso";

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

    qDebug() << "proso";

    //pattern 5

    cnt = 0;
    for(int k=1;k<=6;k++)
    {

        int r1 = 0;
        int r2 = 5;
        int k1 = 3;
        while(r2 < redovi)
        {
            int ko = 0;
            int t = r1;
            int re;
            while(t >= 0)
            {
                trenutniRaspored[5][cnt][t][ko] = k;
                t--;
                ko++;
            }
            t = r2;
            ko = 0;
            while(t >= 0)
            {
                trenutniRaspored[5][cnt][t][ko] = k;
                t--;
                ko++;
            }
            re = redovi-1;
            t = k1;
            while(t < kolone)
            {
                trenutniRaspored[5][cnt][re][t] = k;
                re--;
                t++;
            }
            cnt++;
            r1++;
            r2++;
            k1++;
        }

        int k2 = 1;
        while(k1 < kolone)
        {
            int ko = 0;
            int t = r1;
            int re;
            while(t >= 0)
            {
                trenutniRaspored[5][cnt][t][ko] = k;
                t--;
                ko++;
            }
            re = redovi-1;
            t = k1;
            while(t < kolone)
            {
                trenutniRaspored[5][cnt][re][t] = k;
                re--;
                t++;
            }
            re = redovi-1;
            t = k2;
            while(t < kolone)
            {
                trenutniRaspored[5][cnt][re][t] = k;
                re--;
                t++;
            }
            r1++;
            k1++;
            k2++;
            cnt++;
        }
    }

    qDebug() << "proso";

    //pattern 6

    cnt = 0;
    for(int k=1;k<=6;k++)
    {
        int k1 = 0;
        int k2 = 5;
        int r1 = 4;
        int temp;
        int ko;
        int re;
        while(k2<kolone)
        {
            re = redovi-1;
            temp = k1;
            while(temp >= 0)
            {
                trenutniRaspored[6][cnt][re][temp] = k;
                temp--;
                re--;
            }
            re = redovi-1;
            temp = k2;
            while(temp >= 0)
            {
                trenutniRaspored[6][cnt][re][temp] = k;
                temp--;
                re--;
            }
            temp = r1;
            ko = kolone-1;
            while(temp >= 0)
            {
                trenutniRaspored[6][cnt][temp][ko] = k;
                temp--;
                ko--;
            }
            r1--;
            k1++;
            k2++;
            cnt++;
        }

        int r2 = redovi-2;

        while(r1 >= 0)
        {
            re = redovi-1;
            temp = k1;
            while(temp >= 0)
            {
                trenutniRaspored[6][cnt][re][temp] = k;
                temp--;
                re--;
            }
            temp = r1;
            ko = kolone-1;
            while(temp >= 0)
            {
                trenutniRaspored[6][cnt][temp][ko] = k;
                temp--;
                ko--;
            }
            temp = r2;
            ko = kolone-1;
            while(temp >= 0)
            {
                trenutniRaspored[6][cnt][temp][ko] = k;
                temp--;
                ko--;
            }
            r1--;
            r2--;
            k1++;
            cnt++;
        }
    }

    qDebug() << "proso";

   // provera();
}


void MainWindow::on_obeleziBtn_clicked()
{
    //todo
    inRange(abe, Scalar(255-sens, 255-sens, 255-sens), Scalar(255, 255, 255), m_mat);
    //imshow("Output4",m_mat);
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
                brkomp=0;
                iMax=i;
                iMin=i;
                jMax=j;
                jMin=j;
                obelezavanje(i,j);
                if(brkomp <=20) continue;
                centri[c][1]=(jMin+jMax)/2;
                centri[c][0]=(iMin+iMax)/2;
                qDebug() << centri[c][0] << " " << centri[c][1] << "\n";
                c++;
            }
        }
    }
    odrediDimenzije(c);
    for(int i=0;i<c;i++)
    {
        pixelPtr[centri[i][0]*m_mat.cols+centri[i][1]]=255;
    }
    preracunajPozicije();
    imshow("Output3",m_mat);
}

void MainWindow::on_stopBtn_clicked()
{
    m_frameTimer.stop();
    m_videoCapture.release();
    cv::destroyAllWindows();
}

void MainWindow::onFrameTimer()
{
    if (!m_videoCapture.isOpened() || !m_videoCapture.read(abe) || abe.empty())
    {
        on_stopBtn_clicked();
        return;
    }
    //Indijac se zove Sadekar
    imshow("Output", abe);
}

void MainWindow::on_brightBtn_clicked()
{
    const quint8 value = ui->brightLineEdit->text().toUInt();
    mTester.setBrightness(value);
}

//zastava
//sve
//4
//vertikalne
//horizontalne
//sporedna
//glavna

void MainWindow::brojKorakaPoPaternu(int re,int ko)
{
    niz[0]=niz[1]=1;
    niz[2]=(re/2+1)*(ko/2+1);
    niz[3]=ko/2+1;
    niz[4]=re/2+1;
    niz[5]=(re+ko-1)/3;
    niz[6]=(re+ko-1)/3;
}


void MainWindow::proveraSlike(int pat,int kor,cv::Mat m,cv::Mat bela)
{
    //1-dobro (svetli i treba da svetli ili ne svetli i ne treba da svetli)
    //2-lose(treba da svetli a ne svetli)
    //3-lose(ne treba da svetli a svetli)
    //4-lose(svetli drugacijom bojom)

    //todo
    //odredi precizno granicu
    int granica = 20;

    uint8_t* pixptr;
    uint8_t* pixptr2;
    pixptr = (uint8_t*)m.data;
    pixptr2 = (uint8_t*)bela.data;
    int ocena[20][20];
    for(int i=0;i<redovi;i++)
        for(int j=0;j<kolone;j++)
            ocena[i][j] = 1;

    for(int i=0;i<redovi;i++)
    {
        for(int j=0;j<kolone;j++)
        {
            //qDebug() << i << " " << j;

            int yc = matrica[i][j].first;
            int xc = matrica[i][j].second;
            int ce = pixptr2[yc*bela.cols + xc];

           // qDebug() << yc << " " << xc;

            if(ce) qDebug() << "upaljen";
            else qDebug() << "nije upaljen";

            if(!ce && trenutniRaspored[pat][kor][i][j]==0)
            {
                qDebug() << "ne svetli i ne treba da svetli";
                continue;
            }

            if(ce && trenutniRaspored[pat][kor][i][j]==0)
            {
                qDebug() << "svetli a ne treba da svetli";
                ocena[i][j] = 3;
                continue;
            }

            if(!ce && trenutniRaspored[pat][kor][i][j]!=0)
            {
                qDebug() << "ne svetli a treba da svetli";
                ocena[i][j] = 2;
                continue;
            }

            int ystart = yc - 25;
            int xstart = xc - 25;
            int yend = yc+25;
            int xend = xc+25;
            int cntoko = 0;

            qDebug() << ystart << " " << yend << " " << xstart << " " << xend;

            for(int k = ystart;k<=yend;k++)
            {
                for(int l=xstart;l<=xend;l++)
                {
                    int boja = pixptr[k*m.cols + l];
                    if(boja) cntoko++;
                }
            }

            qDebug() << cntoko << " ";

            if(cntoko < granica)
            {
                qDebug() << "svetli drugom bojom";
                ocena[i][j] = 4;
                continue;
            }

            qDebug() << "sve ok";
        }
        qDebug() << "\n";
    }
    qDebug() << "\n";

    for(int i=0;i<redovi;i++)
    {
        QString qs = "";
        for(int j=0;j<kolone;j++)
        {
            qs=qs + QString::number(ocena[i][j]) + " ";
        }
        qDebug() << qs;
    }
    qDebug() << "\n";

}

void MainWindow::vrtiPaterne()
{
    cv::Mat mat;
    cv::Mat belaMatrica;
    brojKorakaPoPaternu(8,8);

    qDebug() << Vri << " " << Vrj << " " << Vrk;
    inRange(abe, Scalar(255-sens,255-sens,255-sens), Scalar(255,255,255),belaMatrica);
    inRange(abe, Scalar(matricaBoja[2][(Vrk-1)*2],matricaBoja[1][(Vrk-1)*2],matricaBoja[0][(Vrk-1)*2] ), Scalar(matricaBoja[2][(Vrk-1)*2+1],matricaBoja[1][(Vrk-1)*2+1],matricaBoja[0][(Vrk-1)*2+1]), mat);
    imshow("Output2",mat);
    imshow("Output3",belaMatrica);
    proveraSlike(Vri,Vrj,mat,belaMatrica);
    Vrj++;
    if(Vrj==niz[Vri])
    {
        Vrj=0;
        Vrk++;
        if(Vrk==7)
        {
            mTester.nextPattern();
            Vri++;
            Vrk=1;
        }
        else
        {
            mTester.nextStep();
        }
    }
    else
    {
        mTester.nextStep();
    }


   /*if(Vrk==7)
    {
        qDebug () << "radi";
        Vrk=1;
        Vri++;
        mTester.nextPattern();
    }
    if(Vri==7)
    {
        Vri=0;
    }*/
}


void MainWindow::on_praviMaskuBtn_clicked()
{
   // redovi = 8;
   // kolone = 8;
   // preracunajPozicije();
    vrtiPaterne();
}




