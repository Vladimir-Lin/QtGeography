NAME         = QtGeography
TARGET       = $${NAME}

QT           = core
QT          -= gui
QT          += network
QT          += sql
QT          += script
QT          += positioning
QT          += location
QT          += Essentials
QT          += Mathematics

load(qt_build_config)
load(qt_module)

INCLUDEPATH += $${PWD}/../../include/$${NAME}

HEADERS     += $${PWD}/../../include/$${NAME}/qtgeography.h

SOURCES     += $${PWD}/nGeolocation.cpp
SOURCES     += $${PWD}/nEarthLocation.cpp
SOURCES     += $${PWD}/nIpGeolocation.cpp

OTHER_FILES += $${PWD}/../../include/$${NAME}/headers.pri

include ($${PWD}/../../doc/Qt/Qt.pri)

TRNAME       = $${NAME}
include ($${PWD}/../../Translations.pri)
