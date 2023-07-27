
QT       += core gui texttospeech network sql location charts printsupport multimedia multimediawidgets serialport qml quick positioning



greaterThan(QT_MAJOR_VERSION, 4): QT += widgets quickwidgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    ambulance.cpp \
    animate.cpp \
    arduino.cpp \
    communication.cpp \
    connsql.cpp \
    dashboard_ambulance.cpp \
    dashboard_comms.cpp \
    dashboard_employee.cpp \
    dashboard_hospital.cpp \
    dashboard_patient.cpp \
    employee.cpp \
    faceid.cpp \
    fingerprint.cpp \
    hospital.cpp \
    libraries/qcustomplot/qcustomplot.cpp \
    main.cpp \
    mainwindow.cpp \
    libraries/framelesswindow.cpp \
    modifyambulance.cpp \
    modifycomms.cpp \
    modifyemployee.cpp \
    patient.cpp \
    selectmap.cpp \
    sms.cpp \
    userselection.cpp

HEADERS += \
    ambulance.h \
    animate.h \
    arduino.h \
    communication.h \
    connsql.h \
    dashboard_ambulance.h \
    dashboard_comms.h \
    dashboard_employee.h \
    dashboard_hospital.h \
    dashboard_patient.h \
    employee.h \
    faceid.h \
    fingerprint.h \
    hospital.h \
    libraries/qcustomplot/qcustomplot.h \
    mainwindow.h \
    libraries/framelesswindow.h \
    modifyambulance.h \
    modifycomms.h \
    modifyemployee.h \
    patient.h \
    selectmap.h \
    sms.h \
    userselection.h


INCLUDEPATH +=$$PWD\libraries\opencv\release\install\include
INCLUDEPATH +=$$PWD\libraries\ZBar\include
INCLUDEPATH +=$$PWD\libraries\SimpleMail\include\simplemail2-qt5\SimpleMail
INCLUDEPATH +=$$PWD\libraries\qcustomplot

LIBS+= -L$$PWD\libraries\SimpleMail\lib -lSimpleMail2Qt5.dll
LIBS+= -L$$PWD\libraries\ZBar\lib -lzbar.dll



LIBS += -L$$PWD\libraries\opencv\release\bin \
    -lopencv_core320 \
    -lopencv_highgui320 \
    -lopencv_imgcodecs320 \
    -lopencv_imgproc320 \
    -lopencv_videoio320

FORMS += \
    dashboard_ambulance.ui \
    dashboard_comms.ui \
    dashboard_employee.ui \
    dashboard_hospital.ui \
    dashboard_patient.ui \
    faceid.ui \
    fingerprint.ui \
    mainwindow.ui \
    modifyambulance.ui \
    modifycomms.ui \
    modifyemployee.ui \
    selectmap.ui \
    userselection.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


CONFIG(debug, debug|release) {
    my_copy_target.commands = robocopy \"$$PWD/libraries/\" \"$$OUT_PWD/debug\" *.dll /S /LEV:1
}

# Release build
CONFIG(release, debug|release) {
    my_copy_target.commands = robocopy \"$$PWD/libraries/\" \"$$OUT_PWD/release\" *.dll /S /LEV:1
}

#QMAKE_POST_LINK += $$quote(robocopy \"$$PWD/libraries/\" \"$$OUT_PWD/debug\" *.dll /S /LEV:1)
#my_copy_target.commands = robocopy \"$$PWD/libraries/\" \"$$OUT_PWD/debug\" *.dll /S /LEV:1

QMAKE_EXTRA_TARGETS += my_copy_target

POST_TARGETDEPS += my_copy_target

RESOURCES += \
    assets.qrc \
    qml.qrc


