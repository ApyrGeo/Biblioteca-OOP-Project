#include "GUI.h"
#include <QtWidgets/QApplication>
#include "Repository.h"
#include "Service.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    FileRepository repo = FileRepository("Data/Books.csv");
    Service service = Service(repo);


    MainWindow vsa = MainWindow(service);
    vsa.show();

    return a.exec();
}
