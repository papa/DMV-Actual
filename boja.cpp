#include "boja.h"
#include "ui_boja.h"
#include "prijava.h"


Prijava *pp;
boja::boja(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::boja)
{
    ui->setupUi(this);
    ui->boja1->setStyleSheet("background-color:red");
    ui->boja2->setStyleSheet("background-color:green");
    ui->boja3->setStyleSheet("background-color:blue");
    ui->boja4->setStyleSheet("background-color:white");
    ui->boja5->setStyleSheet("background-color:yellow");
    ui->boja6->setStyleSheet("background-color:orange");
}
void boja::setujPrijavu(Prijava *p)
{
    pp=p;
}
void boja::on_boja1_clicked()
{
    pp->vrati(1);
    this->destroy();
}
void boja::on_boja2_clicked()
{
    pp->vrati(2);
    this->destroy();
}
void boja::on_boja3_clicked()
{
    pp->vrati(3);
    this->destroy();
}
void boja::on_boja4_clicked()
{
    pp->vrati(4);
    this->destroy();
}
void boja::on_boja5_clicked()
{
    pp->vrati(5);
    this->destroy();
}
void boja::on_boja6_clicked()
{
    pp->vrati(6);
    this->destroy();
}
boja::~boja()
{
    delete ui;
}
