QT       += core gui widgets mqtt

TARGET = qtmqtt_cloud_connector
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        providerbase.cpp \
        providerconnectionview.cpp \
        providerselector.cpp

HEADERS += \
        providerbase.h \
        providerconnectionview.h \
        providerselector.h

FORMS += \
        providerconnectionview.ui \
        providerselector.ui

# AWS
!contains(CONFIG, NO_AWS) {
    SOURCES += \
            awsprovider.cpp
    HEADERS += \
            awsprovider.h
    FORMS += \
            awsconfiguration.ui
    DEFINES += WITH_AWS
}

# Azure
!contains(CONFIG, NO_AZURE) {
    SOURCES += \
            azureprovider.cpp
    HEADERS += \
            azureprovider.h
    FORMS += \
            azureconfiguration.ui
    DEFINES += WITH_AZURE
}

# Google
!contains(CONFIG, NO_GOOGLE) {
    INCLUDEPATH += 3rdparty/google
    DEPENDPATH += 3rdparty/google
    SOURCES += \
            3rdparty/google/jwtcreation.cpp \
            googleprovider.cpp
    HEADERS += \
            3rdparty/google/jwtcreation.h \
            googleprovider.h
    FORMS += \
            googleconfiguration.ui
    LIBS += -ljwt
    DEFINES += WITH_GOOGLE
}

# Alibaba
!contains(CONFIG, NO_ALIBABA) {
    INCLUDEPATH += 3rdparty/alibaba
    DEPENDPATH += 3rdparty/alibaba
    SOURCES += \
            3rdparty/alibaba/sign_sha256.cpp \
            3rdparty/alibaba/sign_mqtt.cpp \
            alibabaprovider.cpp
    HEADERS += \
            3rdparty/alibaba/sign_api.h \
            alibabaprovider.h
    FORMS += \
            alibabaconfiguration.ui
    DEFINES += WITH_ALIBABA
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
