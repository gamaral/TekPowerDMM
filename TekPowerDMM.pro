TEMPLATE = app
TARGET = TekPowerDMM
INCLUDEPATH += .

QT += serialport widgets

HEADERS += TekPowerDMM.h QTekPowerDMM.h DMMDisplay.h
SOURCES += TekPowerDMM.cpp QTekPowerDMM.cpp DMMDisplay.cpp
FORMS   += DMMDisplay.ui

RC_ICONS   = TekPowerDMM.ico
RESOURCES += TekPowerDMM.qrc
SOURCES   += main.cpp
