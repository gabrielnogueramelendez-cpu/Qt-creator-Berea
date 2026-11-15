cmake_minimum_required(VERSION 3.16)

project(ElBuffetDeAlejandro LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Habilitar la generación automática de MOC, UIC y RCC para Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Buscar paquetes Qt (Widgets y Sql)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

# Lista de fuentes, encabezados e interfaces gráficas
set(PROJECT_SOURCES
    app/main.cpp
    app/basedatos.cpp
    app/basedatos.h
    app/sistemabuffet.cpp
    app/sistemabuffet.h
    app/loginwindow.cpp
    app/loginwindow.h
    app/loginwindow.ui
    app/registerdialog.cpp
    app/registerdialog.h
    app/registerdialog.ui
    app/mainwindow.cpp
    app/mainwindow.h
    app/mainwindow.ui
    app/carritodialog.cpp
    app/carritodialog.h
    app/carritodialog.ui
    app/pagodialog.cpp
    app/pagodialog.h
    app/pagodialog.ui
    app/productdetaildialog.cpp
    app/productdetaildialog.h
    app/productdetaildialog.ui
    app/comprobantedialog.cpp
    app/comprobantedialog.h
    app/comprobantedialog.ui
    app/perfildialog.cpp
    app/perfildialog.h
    app/perfildialog.ui
    app/cajerowindow.cpp
    app/cajerowindow.h
    app/cajerowindow.ui
    app/adminwindow.cpp
    app/adminwindow.h
    app/adminwindow.ui
    app/devwindow.cpp
    app/devwindow.h
    app/devwindow.ui
    app/recursos.qrc
)

# Definición del ejecutable
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ElBuffetDeAlejandro
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(ElBuffetDeAlejandro
        ${PROJECT_SOURCES}
    )
endif()

# Enlazar con las librerías de Qt Widgets y Qt Sql
target_link_libraries(ElBuffetDeAlejandro PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
)

# Librería de C de MySQL (mysql.h y libmysql)
if(WIN32)
    # Windows: la que viene con MySQL Server
    set(MYSQL_DIR "C:/Program Files/MySQL/MySQL Server 8.0")
    target_include_directories(ElBuffetDeAlejandro PRIVATE "${MYSQL_DIR}/include")
    target_link_libraries(ElBuffetDeAlejandro PRIVATE "${MYSQL_DIR}/lib/libmysql.lib")

    # libmysql.dll y las dos de OpenSSL que usa tienen que estar al lado del .exe
    add_custom_command(TARGET ElBuffetDeAlejandro POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${MYSQL_DIR}/lib/libmysql.dll"
            "${MYSQL_DIR}/bin/libssl-3-x64.dll"
            "${MYSQL_DIR}/bin/libcrypto-3-x64.dll"
            $<TARGET_FILE_DIR:ElBuffetDeAlejandro>
    )
else()
    # Ubuntu: paquete libmysqlclient-dev, igual que "g++ ... -lmysqlclient" de la guía del profesor
    target_include_directories(ElBuffetDeAlejandro PRIVATE /usr/include/mysql)
    target_link_libraries(ElBuffetDeAlejandro PRIVATE mysqlclient)
endif()

# Finalización para Qt6
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_finalize_executable(ElBuffetDeAlejandro)
endif()
