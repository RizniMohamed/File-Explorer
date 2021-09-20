QT       += core gui

#QT += winextras



greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

VERSION=1.0
TARGET=RC_FileManager
DEFINES += APP_VERSION=$$VERSION

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

QT+= androidextras

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

ANDROID_ABIS = armeabi-v7a arm64-v8a x86 x86_64

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle.properties \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/org/qtproject/example/FilesCPDemo.java \
    android/org/qtproject/example/LegacyCompatFileProvider.java \
    android/res/values/libs.xml \
    android/res/xml/fileprovider.xml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
android: include(C:/Users/user/AppData/Local/Android/Sdk/android_openssl-master/openssl.pri)
