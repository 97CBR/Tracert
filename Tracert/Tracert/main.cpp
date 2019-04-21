#include "Tracert.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Tracert w;
	w.show();
	return a.exec();
}
