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
    # Lógica (programación estructurada: variables globales y funciones, sin clases propias)
    app/main.cpp
    app/basedatos.cpp
    app/basedatos.h
    app/datos.cpp
    app/datos.h
    app/pantallas.h
    # Una función abrir_xxx() por pantalla; el diseño sigue en el .ui de Qt Designer
    app/login.cpp
    app/loginwindow.ui
    app/registro.cpp
    app/registerdialog.ui
    app/catalogo.cpp
    app/mainwindow.ui
    app/detalle.cpp
    app/productdetaildialog.ui
    app/carrito.cpp
    app/carritodialog.ui
    app/pago.cpp
    app/pagodialog.ui
    app/comprobante.cpp
    app/comprobantedialog.ui
    app/perfil.cpp
    app/perfildialog.ui
    app/caja.cpp
    app/cajerowindow.ui
    app/admin.cpp
    app/adminwindow.ui
    app/desarrollo.cpp
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
    # Ubuntu: paquete libmysqlclient-dev (equivale a "g++ ... -lmysqlclient")
    target_include_directories(ElBuffetDeAlejandro PRIVATE /usr/include/mysql)
    target_link_libraries(ElBuffetDeAlejandro PRIVATE mysqlclient)
endif()

# Finalización para Qt6
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_finalize_executable(ElBuffetDeAlejandro)
endif()
