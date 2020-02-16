#ifndef PODACI_H
#define PODACI_H
#include <QString>

class podaci
{
public:
    QString imeOperatera;
    QString radniNalog;
    QString redniBrojPloce;
    quint16 fps;
    bool imaPodaci;
    int kolone;
    int redovi;
    int brojBoja;
    QString sourceString;
    QString bojePrenos;
    void setujPodatke(QString ime, QString radniNalog,QString sors,quint16 fps, QString rbploce,int kolones,int redovis, QString boje, int brojboja);
    podaci();
    void proba();
};

#endif // PODACI_H
