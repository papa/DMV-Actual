#include "mainwindow.h"
#include "prijava.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.show();

	int ret = a.exec();

	cv::destroyAllWindows();
    //w.close();
	return ret;
}

