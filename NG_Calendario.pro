QT += widgets

CONFIG += c++17

DEFINES += PROJECT_SOURCE_DIR=\\\"$$PWD\\\"

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DISTFILES += \
    datiAttivitaFestivita.json \
    datiEventoAppuntamento.xml

RESOURCES += \
    risorse.qrc

SOURCES += \
    appuntamento.cpp \
    archivioimpegni.cpp \
    attivita.cpp \
    calendar.cpp \
    consegna.cpp \
    create.cpp \
    datafiles.cpp \
    evento.cpp \
    festivita.cpp \
    home.cpp \
    agenda.cpp \
    main.cpp \
    modify.cpp \
    research.cpp \
    searchbar.cpp \
    visualize.cpp

HEADERS += \
    appuntamento.h \
    archivioimpegni.h \
    attivita.h \
    calendar.h \
    consegna.h \
    create.h \
    datafiles.h \
    evento.h \
    festivita.h \
    home.h \
    agenda.h \
    modify.h \
    research.h \
    searchbar.h \
    visualize.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
