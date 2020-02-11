#include "mainwindow.h"
#include "prijava.h"
#include <QApplication>

static Prijava *prijava;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    MainWindow w;
    w.show();

    prijava = new Prijava();
    //w.podesavanjeRef(prijava);
    prijava->show();

	int ret = a.exec();

	cv::destroyAllWindows();
    //w.close();
	return ret;
}
