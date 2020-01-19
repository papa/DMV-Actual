#include "mainwindow.h"

class obelezavanjeCentara
{

private:

    int brkomp;
    int redovi;
    int kolone;
    int **centri;
    pair<int,int> **matrica;
    uint8_t* pixelPtr;
    int iMin;
    int iMax;
    int jMin;
    int jMax;

    void obelezavanje(int i,int j,cv::Mat m_mat)
    {
        brkomp++;
        if(i<iMin)iMin=i;
        else if(i>iMax)iMax=i;
        if(j<jMin)jMin=j;
        else if(j>jMax)jMax=j;

        pixelPtr[i*m_mat.cols+j]=0;

        if(i>0 && pixelPtr[(i-1)*m_mat.cols+j]!=0)
            obelezavanje(i-1,j,m_mat);
        if(i<m_mat.rows-1 && pixelPtr[(i+1)*m_mat.cols+j]!=0)
            obelezavanje(i+1,j,m_mat);
        if(j>0 && pixelPtr[i*m_mat.cols+j-1]!=0)
            obelezavanje(i,j-1,m_mat);
        if(j<m_mat.cols-1 && pixelPtr[i*m_mat.cols+j+1]!=0)
            obelezavanje(i,j+1,m_mat);
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

public:

    obelezavanjeCentara(int r,int k)
    {
        redovi = r;
        kolone = k;
        centri = new int*[r*k];
        for(int i=0;i<r*k;i++)
            centri[i] =new int[2];
        matrica = new pair<int,int>*[r];
        for(int i=0;i<r;i++)
            matrica[i]=new pair<int,int>[k];
    }

    void obeleziCentre(cv::Mat m_mat,int sens)
    {
        cv::Mat abe;
        inRange(abe, Scalar(255-sens, 255-sens, 255-sens), Scalar(255, 255, 255), m_mat);
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
                      obelezavanje(i,j,m_mat);
                      if(brkomp <= 20) continue;
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
        // preracunajPozicije();

            //imshow("Output3",m_mat);

             //QFuture<void> future = QtConcurrent::run(this,MainWindow::testiranjeAuto);
    }

};
