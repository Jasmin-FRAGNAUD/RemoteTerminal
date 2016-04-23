#include <QtWidgets/QApplication>
#include "fenetre_rt.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    fenetre_rt fenetre;
    fenetre.show();

    return app.exec();
}
