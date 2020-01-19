#include "mainwindow.h"
#include "korak.cpp"

class Pattern
{
private:
Korak **koraci;
public:
    Pattern(int brKoraka,int redovi,int kolone)
    {
       koraci=new Korak*[brKoraka];
       for(int i=0;i<brKoraka;i++)
       {
           koraci[i] = new Korak(redovi,kolone);
       }
    }

    void postaviPoljeUMatrici(int kor,int red,int kol,int boja)
    {
        koraci[kor]->obelezi(red,kol,boja);
    }


};
