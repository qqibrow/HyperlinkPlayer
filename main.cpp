#include "player.h"
#include <QtGui/QApplication>
#include <fstream>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Player w;
	w.show();
	return a.exec();
}
