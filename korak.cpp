#include "mainwindow.h"

class Korak
{
private:
    int matrica[10][10];
public:

    Korak()
    {

    }

    Korak(int redovi,int kolone)
    {
       for(int i=0;i<redovi;i++)
           for(int j=0;j<kolone;j++) matrica[i][j] = 0;
    }

    void obelezi(int i,int j,int b)
    {
        matrica[i][j] = b;
    }

    int uzmiBoju(int red,int kol)
    {
        return matrica[red][kol];
    }

};
