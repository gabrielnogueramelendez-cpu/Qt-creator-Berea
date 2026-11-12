#include "loginwindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Cargar estilos de la app
    QFile styleFile("styles.qss");
    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        QString style = QLatin1String(styleFile.readAll());
        a.setStyleSheet(style);
        styleFile.close();
    }

    LoginWindow w;
    w.show();

    return a.exec();
}
