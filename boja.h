#ifndef BOJA_H
#define BOJA_H

#include <QDialog>
#include "prijava.h"
namespace Ui {
class boja;
}

class boja : public QDialog
{
    Q_OBJECT
private slots:
    void on_boja1_clicked();
    void on_boja2_clicked();
    void on_boja3_clicked();
    void on_boja4_clicked();
    void on_boja5_clicked();
    void on_boja6_clicked();

public slots:
    void setujPrijavu(Prijava *p);
public:
    explicit boja(QWidget *parent = 0);
    ~boja();

private:
    Ui::boja *ui;
};

#endif // BOJA_H
