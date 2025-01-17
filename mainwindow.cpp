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
#include <qstylefactory.h>
#include <qelapsedtimer.h>

//zastava
//sve
//4
//vertikalne
//horizontalne
//sporedna
//glavna


QElapsedTimer qelat;
bool aaaaa=false;
pair<int,int>** koordinateCentara;
Pattern** preracunatRaspored;
int ukupanBrojKoraka;
cv::Mat abe;
cv::Mat frameTestMat;
int greskaCounter = 0;
int sens = 70;
int trenutniPattern=-1;
int trenutniKorak=0;
int trenutnaBoja=0;
int trenutniRaspored[10][80][20][20];
int centri[256][2];
pair<int,int> matrica[20][20];
int niz[7];
int progress=0;
int ukupanBrojBoja;
int boje[6];
int matricaBoja[3][12]={
    {140,255,  0, 50,  0, 50,100,255, 50,200,100,220},
    {  0, 50, 80,255, 50,100,100,255, 50,150,  0, 50},
    {  0, 50,  0, 80,100,255,100,255,  0,100,  0, 50}};
cv::Mat belaMatrica;
cv::Mat belaRezerva;
Prijava *podesavanja;
int delay;
uint8_t* piksd;


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_frameTimer.setTimerType(Qt::TimerType::PreciseTimer);
    m_frameTimer.setSingleShot(false);
    plocaPatternTimer.setTimerType(Qt::TimerType::PreciseTimer);
    plocaPatternTimer.setSingleShot(false);
    testerTimer.setTimerType(Qt::TimerType::PreciseTimer);
    testerTimer.setSingleShot(false);
    shooterTimer.setTimerType(Qt::TimerType::PreciseTimer);
    shooterTimer.setSingleShot(true);


    QObject::connect(&plocaPatternTimer, &QTimer::timeout, this, &MainWindow::onPlocaPatternTimer);
    QObject::connect(&m_frameTimer,&QTimer::timeout,this,&MainWindow::onFrameTimer);
    QObject::connect(&testerTimer,&QTimer::timeout, this,&MainWindow::onTesterTimer);
    QObject::connect(&shooterTimer,&QTimer::timeout,this,&MainWindow::onShooterTimer);


    podaciSave=new podaci();
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    this->setVisible(false);
    Prijava *prijava=new Prijava(this);
    prijava->setujPodaci(podaciSave);
    prijava->setujPodatkeGlobal();
    prijava->show();
    this->setVisible(false);
}

int redovi,kolone;
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_podesavanjaBtn_clicked()
{
    this->setVisible(false);
    Prijava *prijava=new Prijava(this);
    prijava->setujPodaci(podaciSave);
    prijava->setujPodatkeGlobal();
    prijava->show();
}

void MainWindow::brojKorakaPoPaternu(int re,int ko)
{
    niz[0]=1;
    niz[1]=(re/2+1)*(ko/2+1);
    niz[2]=ko/2+1;
    niz[3]=re/2+1;
    niz[4]=(re+ko-1)/3;
    niz[5]=(re+ko-1)/3;
    ukupanBrojKoraka = niz[0] + niz[1] + niz[2] + niz[3] + niz[5] + niz[4];
    ukupanBrojKoraka*=ukupanBrojBoja;
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

int MainWindow::proveraSlike(int pat,int kor,cv::Mat m,cv::Mat bela)
{
    //1-dobro (svetli i treba da svetli ili ne svetli i ne treba da svetli)
    //2-lose(treba da svetli a ne svetli)
    //3-lose(ne treba da svetli a svetli)
    //4-lose(svetli drugacijom bojom)

    //todo
    //odredi precizno granicu
    int granica_boja = 15;

    uint8_t* pixptr;
    uint8_t* pixptr2;
    uint8_t* piks;
    piks = (uint8_t*)belaRezerva.data;
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
            //int ce = pixptr2[yc*bela.cols + xc];
            int ce = 0;

            int ystart = yc-4;
            int xstart = xc-4;
            int yend = yc+4;
            int xend = xc+4;
            int brojc = 0;

            for(int k = ystart;k<=yend;k++)
            {
                for(int l=xstart;l<=xend;l++)
                {
                    int boja = pixptr2[k*bela.cols + l];
                    if(boja) brojc++;
                }
            }

           // qDebug() << "Zivot je kazino";
           // qDebug() << QString::number(brojc);
            if(brojc>=10) ce = 1;


            //piks[yc*bela.cols + xc] = 100;

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
                //return 3;
                //qDebug() << "svetli a ne treba da svetli";
                upisiRezultateUFajl(napraviPorukuZaGresku(pat,kor,i,j,3));
                ocena[i][j] = 3;
                continue;
            }

            if(!ce && preracunatRaspored[pat]->getBoja(kor,i,j)!=0)
            {
                //return 2;
                //qDebug() << "ne svetli a treba da svetli";
                upisiRezultateUFajl(napraviPorukuZaGresku(pat,kor,i,j,2));
                ocena[i][j] = 2;
                continue;
            }

            ystart = yc - 10;
            xstart = xc - 10;
            yend = yc+10;
            xend = xc+10;
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

            if(cntoko < granica_boja)
            {
                //return 4;
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

    bool valja = true;

    for(int i=0;i<redovi;i++)
    {
        //QString qs = "";
        for(int j=0;j<kolone;j++)
        {
            int yc = koordinateCentara[i][j].first;
            int xc = koordinateCentara[i][j].second;
            //int ce = pixptr2[yc*bela.cols + xc];

            if( preracunatRaspored[pat]->getBoja(kor,i,j)!=0) piks[yc*bela.cols + xc] = 100;

            if(ocena[i][j]!=1)
            {
                valja = false;
               //return ocena[i][j];
              // qDebug() << "Greska " << ocena[i][j];
               //exit(0);
            }
            //qs=qs + QString::number(ocena[i][j]) + " ";
        }
       // qDebug() << qs;
    }
    //qDebug() << "\n";

    if(valja)  return 1;

    return 2;
}

bool preso;
bool fail;
int ug;

void MainWindow::predjiNaSledeci()
{
    if(trenutniPattern==-1)
    {
        trenutniPattern = trenutniKorak = trenutnaBoja = 0;
        return;
    }

    trenutniKorak++;
    if(trenutniKorak==niz[trenutniPattern])
    {
        trenutniKorak=0;
        trenutnaBoja++;
        if(trenutnaBoja==ukupanBrojBoja)
        {
            preso = true;
            mTester.nextPattern();
            trenutniPattern++;
            trenutnaBoja=0;
            if(trenutniPattern==6)
            {
                mTester.nextPattern();
                long long y = 1000000000;
                while(y > 0) y--;
                mTester.nextPattern();
                trenutniPattern = 0;
                trenutnaBoja = 0;
                trenutniKorak = 0;
            }
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
}

extern int MainWindow::vrtiPaterne(int par)
{
    //qDebug() << progress;

    preso = false;

    cv::Mat mat;


    qDebug() << trenutniPattern << " " << trenutniKorak << " " << trenutnaBoja;


    //ui->progressBar->setValue(progress);
    //ui->label_4->setText("Loop "+QString::number(1)+", sekvenca "+QString::number(trenutniPattern)+", korak "+QString::number(trenutniKorak)+", boja "+QString::number(boje[trenutnaBoja]));
    //QLabel *label4 = new QLabel;
    //label4->setText("Loop "+QString::number(1)+", sekvenca "+QString::number(trenutniPattern)+", korak "+QString::number(trenutniKorak)+", boja "+QString::number(trenutnaBoja));
    inRange(abe, Scalar(255-sens,255-sens,255-sens), Scalar(255,255,255),belaMatrica);
    belaRezerva=belaMatrica;
    //cvtColor(abe, hsvsh, CV_BGR2HSV);
    inRange(abe, Scalar(matricaBoja[2][(boje[trenutnaBoja]-1)*2],matricaBoja[1][(boje[trenutnaBoja]-1)*2],matricaBoja[0][(boje[trenutnaBoja]-1)*2] ), Scalar(matricaBoja[2][(boje[trenutnaBoja]-1)*2+1],matricaBoja[1][(boje[trenutnaBoja]-1)*2+1],matricaBoja[0][(boje[trenutnaBoja]-1)*2+1]), mat);

    //imshow("maska",mat);
    int tp = trenutniPattern;
    int tk = trenutniKorak;
    int tb = trenutnaBoja;

    //if(par==3) predjiNaSledeci();

    return  proveraSlike(tp,tk,mat,belaMatrica);


    /*if(dobar!=1)
    {
        trenutniPattern = tp2;
        trenutniKorak = tk2;
        trenutnaBoja = tb2;
        if(fail)
        {
            qDebug() << "Naivna naivna";
            exit(0);
        }
        else
        {
            qDebug()<<"Svetlo crveno";
        }
        fail = true;
    }
    else
    {
        if(fail)
        {
            qDebug() << "Naivna Ena Popov";
        }
        else
        {
            qDebug() << "Tako mlada";
        }
        fail = false;
    }*/
}

void MainWindow::testiranjeAuto()
{
    /*
    fail = false;
    //qDebug() << QString::number(ukupanBrojKoraka);
    //trenutnaBoja=boje[0];
    qelat.restart();
    for(int i=0;i<=2*ukupanBrojKoraka;i++)
    {
        progress=(i*100)/ukupanBrojKoraka;

        long long x;
        //if(preso) x = 400000000*3/5;
        //else
        x = 90000000;
        while(x > 0) x--;
        int prviput = vrtiPaterne(1);
        if(prviput==1)
        {
            predjiNaSledeci();
            //qDebug()<<qelat.elapsed();
            continue;
        }
       // qDebug() << QString::number(prviput);
      //  qDebug() << "Govno 1";
        int xx = 60000000;
        while(xx > 0) xx--;
        int drugiput = vrtiPaterne(2);
        //predjiNaSledeci();
        if(drugiput==1)
        {
            predjiNaSledeci();
            //qDebug()<<qelat.elapsed();
            continue;
           // exit(0);
        }
       // qDebug() << QString::number(drugiput);
       // qDebug() << "Govno 2";
        xx = 300000000;
        while(xx > 0) xx--;
        int treciput = vrtiPaterne(3);
        //predjiNaSledeci();
        if(treciput!=1)
        {
           // qDebug() << QString::number(treciput);
            //qDebug() << "Govno 3";
            //exit(0);
            greskaCounter++;
            imshow("Greska " + to_string(greskaCounter), abe);
            imshow("Mreska " + to_string(greskaCounter), belaRezerva);
            break;
        }
        predjiNaSledeci();
       // qDebug() << "Govno oprano";
        //qDebug()<<qelat.elapsed();
    }
    qDebug()<<qelat.elapsed();
    upisiRezultateUFajl("sve je u redu");*/
}


void obeleziSve()
{
    obelezavanjecentara* ob = new obelezavanjecentara(redovi,kolone);
    ob->obeleziCentre(abe,sens);
    koordinateCentara = ob->matrica;
}
void preracunavanjePozicija()
{
    qDebug() << QString::number(redovi) + " " + QString::number(kolone);
    preracunavanjepozicija* ppoz = new preracunavanjepozicija(redovi,kolone,6,ukupanBrojBoja,boje);
    ppoz->preracunajSvePozicije();
    preracunatRaspored = ppoz->sviPatterni;
}
void MainWindow::on_praviMaskuBtn_clicked()
{
    citajBoje();
    brojKorakaPoPaternu(redovi,kolone);
    obeleziSve();
    preracunavanjePozicija();

    piksd = (uint8_t*)abe.data;
    //ce = piksd[360*abe.cols*3 + 335*3+2];
    aaaaa=true;
    qelat.restart();
    mTester.nextPattern();

    //ovde racuna delay
    // redovi = 8;
    // kolone = 8;
    // preracunajPozicije();
    //vrtiPaterne();
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
    ukupanBrojBoja = podaciSave->brojBoja;
    if(ukupanBrojBoja==3) ukupanBrojBoja = 6;
    QString bb =  podaciSave->bojePrenos;
    qDebug() << bb;
    qDebug() << QString::number(ukupanBrojBoja);
    for(int i=0;i<ukupanBrojBoja;i++)
    {
        boje[i] = bb[i].digitValue();
        qDebug() << QString::number(boje[i]);
    }
}

void MainWindow::on_startBtn_clicked()
{
    redovi = podaciSave->redovi;
    kolone = podaciSave->kolone;
    napraviFajl();
    qDebug()<<podaciSave->sourceString;
    for(int i=0;i<6;i++)
    {
       mTester.prevPattern();
       std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
    if (m_videoCapture.isOpened()) m_videoCapture.release();
    if (   !m_videoCapture.open(podaciSave->sourceString.toUtf8().constData())
        || !m_videoCapture.isOpened())
    {
        qDebug() << "SOURCE ERROR";
        return;
    }
    const quint16 fps = podaciSave->fps;
    m_frameTimer.setInterval(1000 / (fps ? fps : 1));
    m_frameTimer.start();
    //plocaPatternTimer.setInterval(300);
    //plocaPatternTimer.start();
}

void stampa(int p,int c)
{
    for(int i =0;i<redovi;i++)
    {
        QString s = "";
        for(int j=0;j<kolone;j++) s = s + QString::number(preracunatRaspored[p]->getBoja(c,i,j))+ " ";
        qDebug() << s;
    }
    qDebug() << "\n";
}

int ce;
void MainWindow::on_nadjiDelay_clicked()
{
    /*piksd = (uint8_t*)abe.data;
    //ce = piksd[360*abe.cols*3 + 335*3+2];
    aaaaa=true;
    qelat.restart();
    mTester.nextPattern();*/
}
void provera()
{
    //pattern 0
    qDebug() << 0;
    stampa(0,0);

    //pattern 1;
    qDebug() << 1;
    for(int c =0;c<10;c++)
       stampa(1,c);

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
}





void MainWindow::on_obeleziBtn_clicked()
{
    /*citajBoje();
    brojKorakaPoPaternu(redovi,kolone);
    obeleziSve();
    preracunavanjePozicija();*/
    //provera();
    //imshow("Output3",m_mat);



    //QFuture<void> future = QtConcurrent::run(this,MainWindow::testiranjeAuto);
}

void MainWindow::on_stopBtn_clicked()
{
    m_frameTimer.stop();
    m_videoCapture.release();
    cv::destroyAllWindows();
}
void MainWindow::onPlocaPatternTimer()
{
    predjiNaSledeci();
}
void MainWindow::onFrameTimer()
{
    if(aaaaa)
    {
        //335 360
        int yc = koordinateCentara[redovi/2-1][kolone/2-1].first;
        int xc = koordinateCentara[redovi/2-1][kolone/2-1].second;
        //ce = piksd[360*abe.cols*3 + 335*3+2];
        ce=piksd[(yc-10)*abe.cols*3 + (xc-10)*3];
        if(ce<100)
        {
            delay=qelat.elapsed();

            qDebug()<<"vreme:";
            qDebug()<<delay;

            plocaPatternTimer.setInterval(100);
            plocaPatternTimer.start();
            //shooterTimer.setInterval(delay+400);
            //shooterTimer.start();
            //mTester.nextPattern();
            shooterTimer.setInterval(delay);
            shooterTimer.start();

            aaaaa=false;
        }
        else
        {
            qDebug()<<ce;
        }
    }
    if (!m_videoCapture.isOpened() || !m_videoCapture.read(abe) || abe.empty())
    {
        on_stopBtn_clicked();
        return;
    }
    //Indijac se zove Sadekar
    imshow("Output", abe);
}

void MainWindow::onShooterTimer()
{
    testerTimer.setInterval(100);
    testerTimer.start();
    qDebug()<<"Shooter";
}

void MainWindow::onTesterTimer()
{
    if(vrtiPaterne(1) !=1 )
        qDebug()<<"Greska";



}


void MainWindow::on_brightBtn_clicked()
{
    const quint8 value = ui->brightLineEdit->text().toUInt();
    mTester.setBrightness(value);
}
