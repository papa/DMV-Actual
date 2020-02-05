#ifndef PRIJAVA_H
#define PRIJAVA_H

#include <QDialog>
#include <mainwindow.h>

namespace Ui {
class Prijava;
}

class Prijava : public QDialog
{
    Q_OBJECT

private slots:
    void on_OK_clicked();
    void on_boja1_clicked();
    void on_boja2_clicked();
    void on_boja3_clicked();
    void on_boja4_clicked();
    void on_boja5_clicked();
    void on_boja6_clicked();
    void on_tipPloce_currentIndexChanged();

public:
    explicit Prijava(QWidget *parent = 0);
    void vrati(int x);
    void postaviMW(MainWindow *m);
    ~Prijava();

private:
    Ui::Prijava* ui;
};

#endif // PRIJAVA_H
