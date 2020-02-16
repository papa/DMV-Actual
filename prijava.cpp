#include "prijava.h"
#include "ui_prijava.h"
#include "QDebug"
#include "QMessageBox"
#include "boja.h"
#include "QPushButton"
#include "mainwindow.h"

MainWindow *mw;
QPushButton *trenutni;
int ind;
void Prijava::postaviMW(MainWindow *m)
{
    mw=m;
   // m->close();
}

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
void Prijava::on_tipPloce_currentIndexChanged()
{
    qDebug()<<ui->tipPloce->currentText();
    if(ui->tipPloce->currentIndex()==0)
    {
        ui->boja2->hide();
        ui->boja3->hide();
        ui->boja4->hide();
        ui->boja5->hide();
        ui->boja6->hide();
    }
    else if(ui->tipPloce->currentIndex()==1)
    {
        ui->boja2->show();
        ui->boja3->hide();
        ui->boja4->hide();
        ui->boja5->hide();
        ui->boja6->hide();
    }
    else
    {
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
    sourceString = ui->sourceLineEdit->text().toUtf8().constData();

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

         this->setVisible(false);

        if(ui->tipPloce->currentIndex()==0)
        {
            mw->vratiBoje(QString::number(bojee[0]));
        }
        else if(ui->tipPloce->currentIndex()==1)
        {
            mw->vratiBoje(QString::number(bojee[0])+QString::number(bojee[1]));
        }
        else
        {
            mw->vratiBoje(QString::number(bojee[0])+QString::number(bojee[1])+QString::number(bojee[2])+QString::number(bojee[3])+QString::number(bojee[4])+QString::number(bojee[5]));
        }

        imeOperatera=ui->imeTestera->text();
        qDebug() << imeOperatera;
        radniNalog=ui->radniNalog->text();
        qDebug() << radniNalog;
        redniBrojPloce=ui->redniBrojPloce->text();
        qDebug() << redniBrojPloce;


        fps = ui->fpsLineEdit->text().toUInt();

        mw->setDisabled(false);
    }
}
void Prijava::on_boja1_clicked()
{
    boja *birac = new boja();
    birac->show();
    birac->setujPrijavu(this);
    trenutni=ui->boja1;
    ind = 1;
}
void Prijava::on_boja2_clicked()
{
    boja *birac = new boja();
    birac->show();
    birac->setujPrijavu(this);
    trenutni=ui->boja2;
    ind = 2;
}
void Prijava::on_boja3_clicked()
{
    boja *birac = new boja();
    birac->show();
    birac->setujPrijavu(this);
    trenutni=ui->boja3;
    ind = 3;
}
void Prijava::on_boja4_clicked()
{
    boja *birac = new boja();
    birac->show();
    birac->setujPrijavu(this);
    trenutni=ui->boja4;
    ind =4;
}
void Prijava::on_boja5_clicked()
{
    boja *birac = new boja();
    birac->show();
    birac->setujPrijavu(this);
    trenutni=ui->boja5;
    ind = 5;
}
void Prijava::on_boja6_clicked()
{
    boja *birac = new boja();
    birac->show();
    birac->setujPrijavu(this);
    trenutni=ui->boja6;
    ind = 6;
}
Prijava::~Prijava()
{
    delete ui;
}
