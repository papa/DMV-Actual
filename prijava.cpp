#include "prijava.h"
#include "ui_prijava.h"
#include "QDebug"
#include "QMessageBox"
#include "boja.h"
#include "QPushButton"
#include "mainwindow.h"

MainWindow *mw;
QPushButton *trenutni;
void Prijava::postaviMW(MainWindow *m)
{
    mw=m;
}
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
}
Prijava::Prijava(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Prijava)
{
    ui->setupUi(this);
    ui->tipPloce->addItem("Jednobojna");
    ui->tipPloce->addItem("Dvobojna");
    ui->tipPloce->addItem("Visebojna");
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
    if(ui->imeTestera->text().size()==0||ui->radniNalog_2->text().size()==0||ui->redniBrojPloce->text().size()==0)
    {
        QMessageBox mesedz;
        mesedz.setText("Niste uneli sve parametre potrebne za testiranje te Vas molimo da ih unesete u najkracem roku i pokusate ponovo, hvala :)");
        mesedz.exec();
    }
    else
    {
        //this->hide();
        mw->vratiBoje("AAAAAAAAAAAAAAAAAAAAAAAAAA");
        //todo stavi boje ovde, ocitaj iz ui->tipPloce koji je tip i onda parsuj prvih n boja u obliku 1 2 3 4 5 6
        mw->setDisabled(false);
        this->destroy();
    }
}
void Prijava::on_boja1_clicked()
{
    boja *birac = new boja();
    birac->show();
    birac->setujPrijavu(this);
    trenutni=ui->boja1;
}
void Prijava::on_boja2_clicked()
{
    boja *birac = new boja();
    birac->show();
    birac->setujPrijavu(this);
    trenutni=ui->boja2;
}
void Prijava::on_boja3_clicked()
{
    boja *birac = new boja();
    birac->show();
    birac->setujPrijavu(this);
    trenutni=ui->boja3;
}
void Prijava::on_boja4_clicked()
{
    boja *birac = new boja();
    birac->show();
    birac->setujPrijavu(this);
    trenutni=ui->boja4;
}
void Prijava::on_boja5_clicked()
{
    boja *birac = new boja();
    birac->show();
    birac->setujPrijavu(this);
    trenutni=ui->boja5;
}
void Prijava::on_boja6_clicked()
{
    boja *birac = new boja();
    birac->show();
    birac->setujPrijavu(this);
    trenutni=ui->boja6;
}
Prijava::~Prijava()
{
    delete ui;
}
