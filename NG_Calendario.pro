QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RESOURCES += \
    datiAttivitaFestivita.json \
    datiEventoAppuntamento.xml

SOURCES += \
    calendar.cpp \
    create.cpp \
    datafiles.cpp \
    home.cpp \
    main.cpp \
    modify.cpp \
    research.cpp \
    searchbar.cpp \
    visualize.cpp

HEADERS += \
    calendar.h \
    create.h \
    datafiles.h \
    home.h \
    modify.h \
    research.h \
    searchbar.h \
    visualize.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
