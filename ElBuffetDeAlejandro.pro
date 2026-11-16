QT       += widgets sql

CONFIG += c++17

# Librería de C de MySQL (mysql.h y libmysql)
win32 {
    # Windows: la que viene con MySQL Server (las DLL hay que copiarlas al lado del .exe)
    MYSQL_DIR = "C:/Program Files/MySQL/MySQL Server 8.0"
    INCLUDEPATH += $$MYSQL_DIR/include
    LIBS += -L$$MYSQL_DIR/lib -lmysql
}
unix {
    # Ubuntu: paquete libmysqlclient-dev, igual que "g++ ... -lmysqlclient" de la guía del profesor
    INCLUDEPATH += /usr/include/mysql
    LIBS += -lmysqlclient
}

SOURCES += \
    app/main.cpp \
    app/sistemabuffet.cpp \
    app/basedatos.cpp \
    app/loginwindow.cpp \
    app/registerdialog.cpp \
    app/mainwindow.cpp \
    app/carritodialog.cpp \
    app/pagodialog.cpp \
    app/productdetaildialog.cpp \
    app/comprobantedialog.cpp \
    app/perfildialog.cpp \
    app/cajerowindow.cpp \
    app/adminwindow.cpp \
    app/devwindow.cpp

HEADERS += \
    app/sistemabuffet.h \
    app/basedatos.h \
    app/loginwindow.h \
    app/registerdialog.h \
    app/mainwindow.h \
    app/carritodialog.h \
    app/pagodialog.h \
    app/productdetaildialog.h \
    app/comprobantedialog.h \
    app/perfildialog.h \
    app/cajerowindow.h \
    app/adminwindow.h \
    app/devwindow.h

FORMS += \
    app/loginwindow.ui \
    app/registerdialog.ui \
    app/mainwindow.ui \
    app/carritodialog.ui \
    app/pagodialog.ui \
    app/productdetaildialog.ui \
    app/comprobantedialog.ui \
    app/perfildialog.ui \
    app/cajerowindow.ui \
    app/adminwindow.ui \
    app/devwindow.ui

RESOURCES += \
    app/recursos.qrc
