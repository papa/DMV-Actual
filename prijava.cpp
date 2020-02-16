#include "prijava.h"
#include "ui_prijava.h"
#include "QDebug"
#include "QMessageBox"
#include "boja.h"
#include "QPushButton"
#include "mainwindow.h"

//MainWindow *mw;
QPushButton *trenutni;
int ind;
/*void Prijava::postaviMW(MainWindow *m)
{
    //mw=m;
   // m->close();
}*/

int bojee[7];
void Prijava::vrati(int x)
{
    if(x==1)
    {
        trenutni->setStyleSheet("background-color:red");
    }
    else if(x==2)
    {
        trenutni->setStyleSheet("background-color:green");
    }
    else if(x==3)
    {
        trenutni->setStyleSheet("background-color:blue");
    }
    else if(x==4)
    {
        trenutni->setStyleSheet("background-color:white");
    }
    else if(x==5)
    {
        trenutni->setStyleSheet("background-color:yellow");
    }
    else
    {
        trenutni->setStyleSheet("background-color:orange");
    }
    bojee[ind-1] = x;
}
void Prijava::setujPodatkeGlobal()
{
    if(pod->imaPodaci)
    {
        setujPodatke(pod->imeOperatera,pod->radniNalog,pod->sourceString,pod->fps,pod->redniBrojPloce,pod->kolone,pod->redovi,pod->bojePrenos,pod->brojBoja);
    }
    else
    {
        qDebug()<<"motherfucker";
        bojee[0]=1;
        bojee[1]=2;
        bojee[2]=3;
        bojee[3]=4;
        bojee[4]=5;
        bojee[5]=6;
        ui->boja1->setStyleSheet("background-color:red");
        ui->boja2->setStyleSheet("background-color:green");
        ui->boja3->setStyleSheet("background-color:blue");
        ui->boja4->setStyleSheet("background-color:white");
        ui->boja5->setStyleSheet("background-color:yellow");
        ui->boja6->setStyleSheet("background-color:orange");
    }
}
void Prijava::setujPodatke(QString ime, QString radniNalog,QString sors,quint16 fps, QString rbploce,int kolones,int redovis, QString boje, int brojboja)
{
    this->imeOperatera=ime;
    this->radniNalog=radniNalog;
    this->sourceString=sors;
    this->fps=fps;
    this->redniBrojPloce=rbploce;
    this->kolone=kolones;
    this->redovi=redovis;
    this->bojePrenos=boje;
    ui->imeTestera->setText(ime);
    ui->radniNalog->setText(radniNalog);
    ui->sourceLabel->setText(sors);
    ui->fpsLineEdit->setText(QString::number(fps));
    ui->redniBrojPloce->setText(rbploce);
    ui->koloneTxt->setText(QString::number(kolones));
    ui->redoviTxt->setText(QString::number(redovis));
    qDebug() << boje;
    for(int i=0;i<6;i++)
    {
        QString bojaTr="";
        if(boje[i]=='1')
        {
            bojaTr="background-color:red";
        }
        else if(boje[i]=='2')
        {
            bojaTr="background-color:green";
        }
        else if(boje[i]=='3')
        {
            bojaTr="background-color:blue";
        }
        else if(boje[i]=='4')
        {
            bojaTr="background-color:white";
        }
        else if(boje[i]=='5')
        {
            bojaTr="background-color:yellow";
        }
        else
        {
            bojaTr="background-color:orange";
        }
        if(i==0)
        {
            ui->boja1->setStyleSheet(bojaTr);
        }
        else if(i==1)
        {
            ui->boja2->setStyleSheet(bojaTr);
        }
        else if(i==2)
        {
            ui->boja3->setStyleSheet(bojaTr);
        }
        else if(i==3)
        {
            ui->boja4->setStyleSheet(bojaTr);
        }
        else if(i==4)
        {
            ui->boja5->setStyleSheet(bojaTr);
        }
        else
        {
            ui->boja6->setStyleSheet(bojaTr);
        }
    }
    this->brojBoja=brojboja;
    if(brojboja==1)
    {
        ui->tipPloce->setCurrentIndex(0);
        ui->boja2->hide();
        ui->boja3->hide();
        ui->boja4->hide();
        ui->boja5->hide();
        ui->boja6->hide();
    }
    else if(brojboja==2)
    {
        ui->tipPloce->setCurrentIndex(1);
        ui->boja2->show();
        ui->boja3->hide();
        ui->boja4->hide();
        ui->boja5->hide();
        ui->boja6->hide();
    }
    else
    {
        ui->tipPloce->setCurrentIndex(2);
        ui->boja2->show();
        ui->boja3->show();
        ui->boja4->show();
        ui->boja5->show();
        ui->boja6->show();
    }
}
void Prijava::setujPodaci(podaci *p)
{
    qDebug()<<"ja sam tvoj i kad spavam sa njom";
    pod=p;
}
Prijava::Prijava(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Prijava)
{
    this->setFocus();
    ui->setupUi(this);
    ui->fpsLineEdit->setValidator(m_fpsIntV);
    //QTimer::singleShot(0, ui, SLOT(setFocus()));
    //ui->brightLineEdit->setValidator(m_brtIntV);
    ui->tipPloce->addItem("Jednobojna");
    ui->tipPloce->addItem("Dvobojna");
    ui->tipPloce->addItem("Visebojna");
    //bojee[0]=1;
    //bojee[1]=2;
    //bojee[2]=3;
    //bojee[3]=4;
    //bojee[4]=5;
    //bojee[5]=6;
    //ui->boja1->setStyleSheet("background-color:red");
    //ui->boja2->setStyleSheet("background-color:green");
    //ui->boja3->setStyleSheet("background-color:blue");
    //ui->boja4->setStyleSheet("background-color:white");
    //ui->boja5->setStyleSheet("background-color:yellow");
    //ui->boja6->setStyleSheet("background-color:orange");
}
void Prijava::on_tipPloce_currentIndexChanged()
{
    qDebug()<<ui->tipPloce->currentText();
    if(ui->tipPloce->currentIndex()==0)
    {
        brojBoja=1;
        ui->boja2->hide();
        ui->boja3->hide();
        ui->boja4->hide();
        ui->boja5->hide();
        ui->boja6->hide();
    }
    else if(ui->tipPloce->currentIndex()==1)
    {
        brojBoja=2;
        ui->boja2->show();
        ui->boja3->hide();
        ui->boja4->hide();
        ui->boja5->hide();
        ui->boja6->hide();
    }
    else
    {
        brojBoja=3;
        ui->boja2->show();
        ui->boja3->show();
        ui->boja4->show();
        ui->boja5->show();
        ui->boja6->show();
    }
}
void Prijava::on_OK_clicked()
{
    qDebug()<<"sve ok fico";
    //sourceString = ui->sourceLineEdit->text().toUtf8().constData();
    QString kolones=ui->koloneTxt->text();
    QString redovis=ui->redoviTxt->text();
    int redovi=redovis.toInt();
    int kolone=kolones.toInt();
    if(ui->imeTestera->text().size()==0||ui->radniNalog->text().size()==0||ui->redniBrojPloce->text().size()==0 || redovi==0 || kolone == 0)
    {
        QMessageBox mesedz;
        mesedz.setText("Niste uneli sve parametre potrebne za testiranje te Vas molimo da ih unesete u najkracem roku i pokusate ponovo, hvala :)");
        mesedz.exec();
    }
    else
    {
        bojePrenos=QString::number(bojee[0])+QString::number(bojee[1])+QString::number(bojee[2])+QString::number(bojee[3])+QString::number(bojee[4])+QString::number(bojee[5]);
        imeOperatera=ui->imeTestera->text();
        radniNalog=ui->radniNalog->text();
        redniBrojPloce=ui->redniBrojPloce->text();
        fps = ui->fpsLineEdit->text().toUInt();
        brojBoja=ui->tipPloce->currentIndex()+1;
        pod->setujPodatke(imeOperatera,radniNalog,sourceString,fps,redniBrojPloce,kolone,redovi,bojePrenos,brojBoja);


        parentWidget()->setVisible(true);
        this->destroy();
    }
}

void Prijava::izabranaBoja(int ii)
{
    ind = ii;
    /*boja birac;
    birac.setModal(true);
    birac.setujPrijavu(this);
    birac.exec();*/
    boja *birac = new boja(this);
    birac->setujPrijavu(this);
    birac->show();
}

void Prijava::on_boja1_clicked()
{
    trenutni=ui->boja1;
    izabranaBoja(1);
}
void Prijava::on_boja2_clicked()
{
    trenutni=ui->boja2;
    izabranaBoja(2);
    /*ind = 2;
    boja birac;
    birac.setModal(true);
    birac.setujPrijavu(this);
    birac.exec();*/
    //qDebug() << "otvorio";
    //boja *birac = new boja();
    //birac->show();
    //birac->setujPrijavu(this);
}
void Prijava::on_boja3_clicked()
{
    trenutni=ui->boja3;
    izabranaBoja(3);
}
void Prijava::on_boja4_clicked()
{
    trenutni=ui->boja4;
    izabranaBoja(4);
}
void Prijava::on_boja5_clicked()
{
    trenutni=ui->boja5;
    izabranaBoja(5);
}
void Prijava::on_boja6_clicked()
{
    trenutni=ui->boja6;
    izabranaBoja(6);
}
Prijava::~Prijava()
{
    delete ui;
}
