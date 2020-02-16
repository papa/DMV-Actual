#include "podaci.h"
#include <QDebug>
podaci::podaci()
{
    imaPodaci=false;
}
void podaci::setujPodatke(QString ime, QString radniNalog,QString sors,quint16 fps, QString rbploce,int kolones,int redovis, QString boje, int brojboja)
{
    imaPodaci=true;
    this->imeOperatera=ime;
    this->radniNalog=radniNalog;
    this->sourceString=sors;
    this->fps=fps;
    this->redniBrojPloce=rbploce;
    this->kolone=kolones;
    this->redovi=redovis;
    this->bojePrenos=boje;
    this->brojBoja=brojboja;
}
void podaci::proba()
{
    qDebug()<<"sva moja pesma je od bola";
}
