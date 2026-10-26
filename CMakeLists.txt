cmake_minimum_required(VERSION 3.16)

project(ElBuffetDeAlejandro LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Habilitar la generación automática de MOC, UIC y RCC para Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Buscar paquetes Qt (Widgets y Sql para conexión BDD MySQL)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

# Lista de fuentes, encabezados e interfaces gráficas
set(PROJECT_SOURCES
    main.cpp
    sistemabuffet.cpp
    sistemabuffet.h
    loginwindow.cpp
    loginwindow.h
    loginwindow.ui
    registerdialog.cpp
    registerdialog.h
    registerdialog.ui
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    carritodialog.cpp
    carritodialog.h
    carritodialog.ui
    pagodialog.cpp
    pagodialog.h
    pagodialog.ui
    productdetaildialog.cpp
    productdetaildialog.h
    productdetaildialog.ui
    comprobantedialog.cpp
    comprobantedialog.h
    comprobantedialog.ui
    cajerowindow.cpp
    cajerowindow.h
    cajerowindow.ui
    adminwindow.cpp
    adminwindow.h
    adminwindow.ui
    devwindow.cpp
    devwindow.h
    devwindow.ui
    styles.qss
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

# Copiar el archivo de estilos styles.qss a la carpeta de compilación para su ejecución
add_custom_command(TARGET ElBuffetDeAlejandro POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/styles.qss"
        $<TARGET_FILE_DIR:ElBuffetDeAlejandro>
    COMMENT "Copiando styles.qss al directorio del ejecutable..."
)

# Finalización para Qt6
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_finalize_executable(ElBuffetDeAlejandro)
endif()
