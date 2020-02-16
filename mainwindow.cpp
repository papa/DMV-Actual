#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dmvtestercommunicator.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "chrono"
#include "thread"
#include "ctime"
#include "future"
#include "obelezavanjecentara.cpp"
#include "preracunavanjepozicija.cpp"
#include "prijava.h"
#include "QMessageBox"
//zastava
//sve
//4
//vertikalne
//horizontalne
//sporedna
//glavna

pair<int,int>** koordinateCentara;
Pattern** preracunatRaspored;
int ukupanBrojKoraka;
cv::Mat abe;
int sens = 70;
int trenutniPattern=1;
int trenutniKorak=0;
int trenutnaBoja=1;
int trenutniRaspored[10][80][20][20];
int centri[256][2];
pair<int,int> matrica[20][20];
int niz[7];
int progress=0;
int ukupanBrojBoja;
int boje[7];
int matricaBoja[3][12]={
    {230,255,100,190,80,170,200,255,170,255,220,255},
    {130,230,200,255,165,255,200,255,140,225,140,205},
    {120,210,170,255,254,255,200,255,100,170,110,165}};
//hsv odozdo na gore
/*int matricaBoja[3][12]={
    {50,255,50,255,50,255,0,255,50,255,50,255},
    {30,255,30,255,30,255,0,30,30,255,30,255},
    {150,180,60,93,94,130,0,180,10,25,0,20}
};*/

Prijava *podesavanja;



MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_frameTimer.setTimerType(Qt::TimerType::PreciseTimer);
    m_frameTimer.setSingleShot(false);
    QObject::connect(&m_frameTimer, &QTimer::timeout         ,
                     this         , &MainWindow::onFrameTimer);
    podaciSave=new podaci();

    //hide();
    //prijav = new Prijava(this);
    //prijav->show();
    //prijav->setujPodaci(podaciSave);

    this->setVisible(false);
    /*Prijava pr(this);
    pr.setujPodaci(podaciSave);
    pr.setujPodatkeGlobal();
    //pr.setModal(true);
    pr.exec();*/
    Prijava *prijava=new Prijava(this);
    prijava->setujPodaci(podaciSave);
    prijava->setujPodatkeGlobal();
    prijava->show();
    this->setVisible(false);

    //Prijava pr;
    //pr.setModal(true);
    //pr.exec();
}

int redovi,kolone;
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_podesavanjaBtn_clicked()
{
    this->setVisible(false);
    /*Prijava pr(this);
    pr.setujPodaci(podaciSave);
    pr.setujPodatkeGlobal();
    //pr.setModal(true);
    pr.exec();*/
    Prijava *prijava=new Prijava(this);
    prijava->setujPodaci(podaciSave);
    prijava->setujPodatkeGlobal();
    prijava->show();
}
void MainWindow::vratiBoje(QString bojee)
{
    qDebug()<<bojee;
    /*qDebug()<<QString::fromStdString(prijava->sourceString);
    if (m_videoCapture.isOpened()) m_videoCapture.release();
    if (!m_videoCapture.open(prijava->sourceString) || !m_videoCapture.isOpened())
    {
        this->setEnabled(false);
        QMessageBox msgBox;
        msgBox.setInformativeText("Source error");
        msgBox.setStandardButtons(QMessageBox::Save);
        msgBox.setDefaultButton(QMessageBox::Save);
        //QMessageBox msgBox(QMessageBox::Information,"Error","Source error",QMessageBox::Ok);
       // QObject::connect(msgBox.button(QMessageBox::Ok),&QAbstractButton::pressed,[](){msgBox.close();prijava->setVisible(true);qDebug() << "Set visible true";});
        int ret = msgBox.exec();
        switch(ret)
        {
            case QMessageBox::Save:
             prijava->setVisible(true);
             msgBox.close();
            break;
        }
        //msgBox.close();
        return;
    }
    else
    {

    }*/
}
void MainWindow::brojKorakaPoPaternu(int re,int ko)
{
    niz[0]=niz[1]=1;
    niz[2]=(re/2+1)*(ko/2+1);
    niz[3]=ko/2+1;
    niz[4]=re/2+1;
    niz[5]=(re+ko-1)/3;
    niz[6]=(re+ko-1)/3;
    ukupanBrojKoraka = niz[1] + niz[2] + niz[3] + niz[4] + niz[5] + niz[6];
}

QString imeOperatera;
QString radniNalog;
QString redniBrojPloce;
QString opisGreske;
QString path;

void upisiRezultateUFajl(QString poruka)
{
    QTime vreme= QTime::currentTime();
    int sati=vreme.hour();
    int minuti=vreme.minute();
    int sekunde = vreme.second();

    QString tacnovreme = QString::number(sati) + ":" + QString::number(minuti)+":"+QString::number(sekunde);
    QString ufajl = tacnovreme+","+imeOperatera+","+radniNalog+","+redniBrojPloce;

    ufajl+=", "+poruka;

    QFile qfile(path);
    qfile.open(QIODevice::WriteOnly);
    qfile.write(ufajl.toUtf8());
    qfile.close();
}

QString napraviPorukuZaGresku(int pat,int kor,int red,int kol,int tip)
{
    QString poruka = "Javila se greska u toku sekvence " + QString::number(pat) + " i koraka " + QString::number(kor) +
            " na diodi u redu " + QString::number(red) + " i koloni " + QString::number(kol) + ".";
    if(tip==2)
        poruka+="Greska je nastala jer bi dioda trebalo da svetli a uopste ne svetli.";
    else if(tip==3)
        poruka+="Greska je nastala jer dioda ne bi trebalo da svetli a svetli.";
    else
        poruka+="Greska je nastala jer dioda svetli drugacijom bojom od ocekivane.";

    return poruka;
}

void MainWindow::proveraSlike(int pat,int kor,cv::Mat m,cv::Mat bela)
{
    //1-dobro (svetli i treba da svetli ili ne svetli i ne treba da svetli)
    //2-lose(treba da svetli a ne svetli)
    //3-lose(ne treba da svetli a svetli)
    //4-lose(svetli drugacijom bojom)

    //todo
    //odredi precizno granicu
    int granica = 5;

    uint8_t* pixptr;
    uint8_t* pixptr2;
    pixptr = (uint8_t*)m.data;
    pixptr2 = (uint8_t*)bela.data;
    int ocena[redovi][kolone];
    for(int i=0;i<redovi;i++)
        for(int j=0;j<kolone;j++)
            ocena[i][j] = 1;

    for(int i=0;i<redovi;i++)
    {
        for(int j=0;j<kolone;j++)
        {
            //qDebug() << i << " " << j;

            int yc = koordinateCentara[i][j].first;
            int xc = koordinateCentara[i][j].second;
            int ce = pixptr2[yc*bela.cols + xc];

            // qDebug() << yc << " " << xc;

            //if(ce) qDebug() << "upaljen";
            //else qDebug() << "nije upaljen";

            if(!ce && preracunatRaspored[pat]->getBoja(kor,i,j)==0)
            {
               //qDebug() << "ne svetli i ne treba da svetli";
                continue;
            }

            if(ce && preracunatRaspored[pat]->getBoja(kor,i,j)==0)
            {
                //qDebug() << "svetli a ne treba da svetli";
                upisiRezultateUFajl(napraviPorukuZaGresku(pat,kor,i,j,3));
                ocena[i][j] = 3;
                continue;
            }

            if(!ce && preracunatRaspored[pat]->getBoja(kor,i,j)!=0)
            {
                //qDebug() << "ne svetli a treba da svetli";
                upisiRezultateUFajl(napraviPorukuZaGresku(pat,kor,i,j,2));
                ocena[i][j] = 2;
                continue;
            }

            int ystart = yc - 25;
            int xstart = xc - 25;
            int yend = yc+25;
            int xend = xc+25;
            int cntoko = 0;

           // qDebug() << ystart << " " << yend << " " << xstart << " " << xend;

            for(int k = ystart;k<=yend;k++)
            {
                for(int l=xstart;l<=xend;l++)
                {
                    int boja = pixptr[k*m.cols + l];
                    if(boja) cntoko++;
                }
            }

            //qDebug() << cntoko << " ";

            if(cntoko < 1)
            {
               //qDebug() << "svetli drugom bojom";
                upisiRezultateUFajl(napraviPorukuZaGresku(pat,kor,i,j,4));
                ocena[i][j] = 4;
                continue;
            }

            //qDebug() << "sve ok";
        }
       // qDebug() << "\n";
    }
    //qDebug() << "\n";

    for(int i=0;i<redovi;i++)
    {
        QString qs = "";
        for(int j=0;j<kolone;j++)
        {
            if(ocena[i][j]!=1)
            {
               qDebug() << "Greska " << ocena[i][j];
               exit(0);
            }
            qs=qs + QString::number(ocena[i][j]) + " ";
        }
        qDebug() << qs;
    }
    qDebug() << "\n";

}

extern void MainWindow::vrtiPaterne()
{
    qDebug() << progress;

    cv::Mat mat;
    cv::Mat belaMatrica;
    cv::Mat hsvsh;

    qDebug() << trenutniPattern << " " << trenutniKorak << " " << trenutnaBoja;

    ui->progressBar->setValue(progress);
    ui->label_4->setText("Loop "+QString::number(1)+", sekvenca "+QString::number(trenutniPattern)+", korak "+QString::number(trenutniKorak)+", boja "+QString::number(boje[trenutnaBoja]));
    //QLabel *label4 = new QLabel;
    //label4->setText("Loop "+QString::number(1)+", sekvenca "+QString::number(trenutniPattern)+", korak "+QString::number(trenutniKorak)+", boja "+QString::number(trenutnaBoja));
    inRange(abe, Scalar(255-sens,255-sens,255-sens), Scalar(255,255,255),belaMatrica);
    //cvtColor(abe, hsvsh, CV_BGR2HSV);
    inRange(abe, Scalar(matricaBoja[2][(boje[trenutnaBoja]-1)*2],matricaBoja[1][(boje[trenutnaBoja]-1)*2],matricaBoja[0][(boje[trenutnaBoja]-1)*2] ), Scalar(matricaBoja[2][(boje[trenutnaBoja]-1)*2+1],matricaBoja[1][(boje[trenutnaBoja]-1)*2+1],matricaBoja[0][(boje[trenutnaBoja]-1)*2+1]), mat);
    proveraSlike(trenutniPattern,trenutniKorak,mat,belaMatrica);
    trenutniKorak++;
    if(trenutniKorak==niz[trenutniPattern])
    {
        trenutniKorak=0;
        trenutnaBoja++;
        if(trenutnaBoja==7)
        {
            mTester.nextPattern();
            trenutniPattern++;
            trenutnaBoja=1;
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

   /*if(trenutnaBoja==7)
    {
        qDebug () << "radi";
        trenutnaBoja=1;
        trenutniPattern++;
        mTester.nextPattern();
    }
    if(trenutniPattern==7)
    {
        trenutniPattern=0;
    }*/
}

void MainWindow::testiranjeAuto()
{
    for(int i=0;i<ukupanBrojKoraka;i++)
    {
        progress=(i*100)/ukupanBrojKoraka;
                    //ui->progressBar->setValue((i*100)/276);
        vrtiPaterne();
        long long x = 23000000000;
        while(x > 0) x--;
                //QCoreApplication::postEvent(progresBar,QMouseEvent,10);

              //qDebug()<<"testiranje auto";
             // QFuture<void> future = QtConcurrent::run(this,MainWindow::vrtiPaterne);
            //future<void> result(async(vrtiPaterne));
           //result.get();
          //auto result=async()
         //long long x = (long long)1000000000;
        //while(x > 0) x--;
    }
    //QFuture<void> future = QtConcurrent::run(this,MainWindow::vrtiPaterne);
    upisiRezultateUFajl("sve je u redu");
}


void MainWindow::on_praviMaskuBtn_clicked()
{
   // redovi = 8;
   // kolone = 8;
   // preracunajPozicije();
   vrtiPaterne();
}

QString formatiraj(int x)
{
    if(x<10) return "0"+QString::number(x);
    else return QString::number(x);
}

void napraviFajl()
{
    QDate datum = QDate::currentDate();
    int dan=datum.day();
    int mesec=datum.month();
    int godina=datum.year();

    QTime vreme= QTime::currentTime();
    int sati=vreme.hour();
    int minuti=vreme.minute();

    QString datumVreme=QString::number(godina)+"_"+formatiraj(mesec)+"_"+formatiraj(dan)+"_"+formatiraj(sati)+"_"+formatiraj(minuti);
    QString imeFajla = "izvestaj_o_testiranju_"+datumVreme+".txt";
    path = "D:/Resursi_Konkurs2020/"+imeFajla;
    qDebug() << imeFajla;

    QFile qfile(path);
    qfile.open(QIODevice::WriteOnly);
    qfile.close();
}

void MainWindow::citajBoje()
{
    QString bb =  podaciSave->bojePrenos;
    qDebug() << bb;
    for(int i=1;i<=ukupanBrojBoja;i++) boje[i] = bb.mid(i-1,i-1).toInt();
}
void MainWindow::on_startBtn_clicked()
{
    //start
    for(int i=0;i<6;i++)
    {
       mTester.prevPattern();
       std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }


    brojKorakaPoPaternu(redovi,kolone);

   // m_frameTimer.setInterval(1000 / (main.prijava->fps ? main.prijava->fps : 1));
    m_frameTimer.start();

    napraviFajl();

}

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
                trenutniRaspored[1][k-1][i][j] = boje[k];

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
                   trenutniRaspored[2][cnt][r4][k4] =boje[k];
               cnt++;
               k1++;
               k2++;
               k3++;
               k4++;
          }
           trenutniRaspored[2][cnt][r1][k1] = trenutniRaspored[2][cnt][r3][k1] = boje[k];
           cnt++;
           k1++;
           trenutniRaspored[2][cnt][r1][k1] = trenutniRaspored[2][cnt][r3][k1] = boje[k];
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
                trenutniRaspored[2][cnt][r1][k1] =  trenutniRaspored[2][cnt][r2][k2] = boje[k];
                cnt++;
                k1++;
                k2++;
            }
            trenutniRaspored[2][cnt][r1][k1] = boje[k];
            cnt++;
            k1++;
            trenutniRaspored[2][cnt][r1][k1] =  boje[k];
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
                trenutniRaspored[3][cnt][i][k1] = trenutniRaspored[3][cnt][i][k2] =boje[k];
            }
            k1++;
            k2++;
            cnt++;
        }
        for(int i=0;i<redovi;i++) trenutniRaspored[3][cnt][i][k1]  = boje[k];
        cnt++;
        k1++;
        for(int i=0;i<redovi;i++) trenutniRaspored[3][cnt][i][k1]  = boje[k];
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
                trenutniRaspored[4][cnt][r1][i] = trenutniRaspored[4][cnt][r2][i] = boje[k];
            }
            r1++;
            r2++;
            cnt++;
        }
        for(int i=0;i<kolone;i++) trenutniRaspored[4][cnt][r1][i] = boje[k];
        cnt++;
        r1++;
        for(int i=0;i<kolone;i++) trenutniRaspored[4][cnt][r1][i] = boje[k];
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
                trenutniRaspored[5][cnt][t][ko] = boje[k];
                t--;
                ko++;
            }
            t = r2;
            ko = 0;
            while(t >= 0)
            {
                trenutniRaspored[5][cnt][t][ko] = boje[k];
                t--;
                ko++;
            }
            re = redovi-1;
            t = k1;
            while(t < kolone)
            {
                trenutniRaspored[5][cnt][re][t] = boje[k];
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
                trenutniRaspored[5][cnt][t][ko] = boje[k];
                t--;
                ko++;
            }
            re = redovi-1;
            t = k1;
            while(t < kolone)
            {
                trenutniRaspored[5][cnt][re][t] = boje[k];
                re--;
                t++;
            }
            re = redovi-1;
            t = k2;
            while(t < kolone)
            {
                trenutniRaspored[5][cnt][re][t] = boje[k];
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
                trenutniRaspored[6][cnt][re][temp] = boje[k];
                temp--;
                re--;
            }
            re = redovi-1;
            temp = k2;
            while(temp >= 0)
            {
                trenutniRaspored[6][cnt][re][temp] = boje[k];
                temp--;
                re--;
            }
            temp = r1;
            ko = kolone-1;
            while(temp >= 0)
            {
                trenutniRaspored[6][cnt][temp][ko] = boje[k];
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
                trenutniRaspored[6][cnt][re][temp] = boje[k];
                temp--;
                re--;
            }
            temp = r1;
            ko = kolone-1;
            while(temp >= 0)
            {
                trenutniRaspored[6][cnt][temp][ko] = boje[k];
                temp--;
                ko--;
            }
            temp = r2;
            ko = kolone-1;
            while(temp >= 0)
            {
                trenutniRaspored[6][cnt][temp][ko] = boje[k];
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
   // imshow("Output4",m_mat);
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
                if(brkomp <=5) continue;
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

    QFuture<void> future = QtConcurrent::run(this,MainWindow::testiranjeAuto);
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


void obeleziSve()
{
    obelezavanjecentara* ob = new obelezavanjecentara(redovi,kolone);
    ob->obeleziCentre(abe,sens);
    koordinateCentara = ob->matrica;
}


void preracunavanjePozicija()
{
    preracunavanjepozicija* ppoz = new preracunavanjepozicija(redovi,kolone,6);
    ppoz->preracunajSvePozicije();
    preracunatRaspored = ppoz->sviPatterni;
}
