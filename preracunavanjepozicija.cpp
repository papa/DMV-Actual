#include "mainwindow.h"
#include "pattern.cpp"

class preracunavanjepozicija
{
private:

 int redovi;
 int kolone;
 int cnt;
 int granica;
 QString stringBoja;

 void postavljanjeGranice(int brojBoja)
 {
     granica=brojBoja;
 }

 void getterStringaBoje(QString boje)
 {
     stringBoja=boje;
 }

 void prviPattern()
 {
     for(int i=0;i<redovi;i++)
             for(int j=0;j<kolone;j++)
                 for(int k = 1;k<=granica;k++)
                     sviPatterni[1]->postaviPoljeUMatrici(k-1,i,j,stringBoja[k-1]);
 }

 void drugiPattern()
 {
     cnt = 0;

        for(int k=1;k<=granica;k++)
        {
            int r1,r2,r3,r4;
            int k1,k2,k3,k4;

            for(int i=0;i<redovi/2-1;i++)
            {
               r1 = i;
               r3 = i+redovi/2+1;
               r2 = i;
               r4 = r3;
               k1 = 0;
               k2 = kolone/2+1;
               k3 = 0;
               k4 = kolone/2+1;
               while(k1 < kolone/2 -1)
               {
                    sviPatterni[2]->postaviPoljeUMatrici(cnt,r1,k1,stringBoja[k-1]);
                    sviPatterni[2]->postaviPoljeUMatrici(cnt,r2,k2,stringBoja[k-1]);
                    sviPatterni[2]->postaviPoljeUMatrici(cnt,r3,k3,stringBoja[k-1]);
                    sviPatterni[2]->postaviPoljeUMatrici(cnt,r4,k4,stringBoja[k-1]);
                    cnt++;
                    k1++;
                    k2++;
                    k3++;
                    k4++;
               }
                sviPatterni[2]->postaviPoljeUMatrici(cnt,r1,k1,stringBoja[k-1]);
                sviPatterni[2]->postaviPoljeUMatrici(cnt,r3,k1,stringBoja[k-1]);
                //trenutniRaspored[2][cnt][r1][k1] = trenutniRaspored[2][cnt][r3][k1] = k;
                cnt++;
                k1++;
                sviPatterni[2]->postaviPoljeUMatrici(cnt,r1,k1,stringBoja[k-1]);
                sviPatterni[2]->postaviPoljeUMatrici(cnt,r3,k1,stringBoja[k-1]);
                //trenutniRaspored[2][cnt][r1][k1] = trenutniRaspored[2][cnt][r3][k1] = k;
                cnt++;
            }

            for(int i=redovi/2-1;i<=redovi/2;i++)
            {
                 r1 = i;
                 k1 = 0;
                 r2 = r1;
                 k2 = kolone/2+1;
                 while(k1 < kolone/2-1)
                 {
                      sviPatterni[2]->postaviPoljeUMatrici(cnt,r1,k1,stringBoja[k-1]);
                      sviPatterni[2]->postaviPoljeUMatrici(cnt,r2,k2,stringBoja[k-1]);
                     //trenutniRaspored[2][cnt][r1][k1] =  trenutniRaspored[2][cnt][r2][k2] = k;
                     cnt++;
                     k1++;
                     k2++;
                 }
                 sviPatterni[2]->postaviPoljeUMatrici(cnt,r1,k1,stringBoja[k-1]);
                 //trenutniRaspored[2][cnt][r1][k1] = k;
                 cnt++;
                 k1++;
                 sviPatterni[2]->postaviPoljeUMatrici(cnt,r1,k1,stringBoja[k-1]);
                 //trenutniRaspored[2][cnt][r1][k1] =  k;
                 cnt++;
            }
        }
 }

 void treciPattern()
 {
     cnt = 0;
     for(int k=1;k<=granica;k++)
     {
         int k1 = 0;
         int k2 = kolone/2+1;
         while(k1<(kolone/2-1))
         {
             for(int i=0;i<redovi;i++)
             {
                 sviPatterni[3]->postaviPoljeUMatrici(cnt,i,k1,stringBoja[k-1]);
                 sviPatterni[3]->postaviPoljeUMatrici(cnt,i,k2,stringBoja[k-1]);
                 //trenutniRaspored[3][cnt][i][k1] = trenutniRaspored[3][cnt][i][k2] = k;
             }
             k1++;
             k2++;
             cnt++;
          }
          for(int i=0;i<redovi;i++)  sviPatterni[3]->postaviPoljeUMatrici(cnt,i,k1,stringBoja[k-1]);
          cnt++;
          k1++;
          for(int i=0;i<redovi;i++) sviPatterni[3]->postaviPoljeUMatrici(cnt,i,k1,stringBoja[k-1]);
          cnt++;
     }
 }

 void cetvrtiPattern()
 {
     cnt = 0;
         for(int k=1;k<=granica;k++)
         {
             int r1 = 0;
             int r2 = redovi/2+1;
             while(r1 < (redovi/2-1))
             {
                 for(int i = 0;i<kolone;i++)
                 {
                     sviPatterni[4]->postaviPoljeUMatrici(cnt,r1,i,stringBoja[k-1]);
                     sviPatterni[4]->postaviPoljeUMatrici(cnt,r2,i,stringBoja[k-1]);
                     //trenutniRaspored[4][cnt][r1][i] = trenutniRaspored[4][cnt][r2][i] = k;
                 }
                 r1++;
                 r2++;
                 cnt++;
             }
             for(int i=0;i<kolone;i++) sviPatterni[4]->postaviPoljeUMatrici(cnt,r1,i,stringBoja[k-1]);
             cnt++;
             r1++;
             for(int i=0;i<kolone;i++) sviPatterni[4]->postaviPoljeUMatrici(cnt,r1,i,stringBoja[k-1]);
             cnt++;
         }

 }

 void petiPattern()
 {
     cnt = 0;
         for(int k=1;k<=granica;k++)
         {

             int r1 = 0;
             int r2 = 5;
             int k1 = 3;
             while(r2 < redovi)
             {
                 int ko = 0;
                 int t = r1;
                 int re;
                 while(t >= 0)
                 {
                     sviPatterni[5]->postaviPoljeUMatrici(cnt,t,ko,stringBoja[k-1]);
                     //trenutniRaspored[5][cnt][t][ko] = k;
                     t--;
                     ko++;
                 }
                 t = r2;
                 ko = 0;
                 while(t >= 0)
                 {
                     sviPatterni[5]->postaviPoljeUMatrici(cnt,t,ko,stringBoja[k-1]);
                     //trenutniRaspored[5][cnt][t][ko] = k;
                     t--;
                     ko++;
                 }
                 re = redovi-1;
                 t = k1;
                 while(t < kolone)
                 {
                     sviPatterni[5]->postaviPoljeUMatrici(cnt,re,t,stringBoja[k-1]);
                    // trenutniRaspored[5][cnt][re][t] = k;
                     re--;
                     t++;
                 }
                 cnt++;
                 r1++;
                 r2++;
                 k1++;
             }

             int k2 = 1;
             while(k1 < kolone)
             {
                 int ko = 0;
                 int t = r1;
                 int re;
                 while(t >= 0)
                 {
                     sviPatterni[5]->postaviPoljeUMatrici(cnt,t,ko,stringBoja[k-1]);
                     //trenutniRaspored[5][cnt][t][ko] = k;
                     t--;
                     ko++;
                 }
                 re = redovi-1;
                 t = k1;
                 while(t < kolone)
                 {
                     sviPatterni[5]->postaviPoljeUMatrici(cnt,re,t,stringBoja[k-1]);
                    // trenutniRaspored[5][cnt][re][t] = k;
                     re--;
                     t++;
                 }
                 re = redovi-1;
                 t = k2;
                 while(t < kolone)
                 {
                     sviPatterni[5]->postaviPoljeUMatrici(cnt,re,t,stringBoja[k-1]);
                     //trenutniRaspored[5][cnt][re][t] = k;
                     re--;
                     t++;
                 }
                 r1++;
                 k1++;
                 k2++;
                 cnt++;
             }
         }
 }

 void sestiPattern()
 {
     cnt = 0;
     for(int k=1;k<=6;k++)
     {
         int k1 = 0;
         int k2 = 5;
         int r1 = 4;
         int temp;
         int ko;
         int re;
         while(k2<kolone)
         {
             re = redovi-1;
             temp = k1;
             while(temp >= 0)
             {
                 sviPatterni[6]->postaviPoljeUMatrici(cnt,re,temp,stringBoja[k-1]);
                // trenutniRaspored[6][cnt][re][temp] = k;
                 temp--;
                 re--;
             }
             re = redovi-1;
             temp = k2;
             while(temp >= 0)
             {
                 sviPatterni[6]->postaviPoljeUMatrici(cnt,re,temp,stringBoja[k-1]);
                 temp--;
                 re--;
             }
             temp = r1;
             ko = kolone-1;
             while(temp >= 0)
             {
                 sviPatterni[6]->postaviPoljeUMatrici(cnt,temp,ko,stringBoja[k-1]);
                 //trenutniRaspored[6][cnt][temp][ko] = k;
                 temp--;
                 ko--;
             }
             r1--;
             k1++;
             k2++;
             cnt++;
         }

         int r2 = redovi-2;

         while(r1 >= 0)
         {
              re = redovi-1;
              temp = k1;
              while(temp >= 0)
              {
                  sviPatterni[6]->postaviPoljeUMatrici(cnt,re,temp,stringBoja[k-1]);
                  //trenutniRaspored[6][cnt][re][temp] = k;
                  temp--;
                  re--;
              }
              temp = r1;
              ko = kolone-1;
              while(temp >= 0)
              {
                  sviPatterni[6]->postaviPoljeUMatrici(cnt,temp,ko,stringBoja[k-1]);
                  //trenutniRaspored[6][cnt][temp][ko] = k;
                  temp--;
                  ko--;
              }
              temp = r2;
              ko = kolone-1;
              while(temp >= 0)
              {
                  sviPatterni[6]->postaviPoljeUMatrici(cnt,temp,ko,stringBoja[k-1]);
                  //trenutniRaspored[6][cnt][temp][ko] = k;
                  temp--;
                  ko--;
              }
              r1--;
              r2--;
              k1++;
              cnt++;
          }
      }
 }

public:

   Pattern **sviPatterni;

   preracunavanjepozicija(int redovi,int kolone,int brpatterna)
   {
       this->redovi = redovi;
       this->kolone = kolone;
       sviPatterni=new Pattern*[brpatterna];
       sviPatterni[0] = new Pattern(6,redovi,kolone);
       sviPatterni[1] = new Pattern(6*(redovi/2+1)*(kolone/2+1),redovi,kolone);
       sviPatterni[2] = new Pattern(6*(kolone/2+1),redovi,kolone);
       sviPatterni[3] = new Pattern(6*(redovi/2+1),redovi,kolone);
       sviPatterni[4] = new Pattern(6*(redovi+kolone-1)/3,redovi,kolone);
       sviPatterni[5] = new Pattern(6*(redovi+kolone-1)/3,redovi,kolone);
   }

   void preracunajSvePozicije()
   {
      prviPattern();
      drugiPattern();
      treciPattern();
      cetvrtiPattern();
      petiPattern();
      sestiPattern();
   }

};
