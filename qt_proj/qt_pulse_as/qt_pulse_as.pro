#-------------------------------------------------
#
# Project created by QtCreator 2016-04-07T09:39:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT       += printsupport

TARGET = qt_pulse_as
TEMPLATE = app


SOURCES += \
    source/stdafx.cpp \
    source/QTabManagerDlg.cpp \
    source/QShowMeasureResult.cpp \
    source/QSerialRecvThread.cpp \
    source/QSavePressureWorker.cpp \
    source/QSavePressureThread.cpp \
    source/QSavePressure.cpp \
    source/QResultEvaluate.cpp \
    source/QReadAndWriteXml.cpp \
    source/QPersonBasicInfoDlg.cpp \
    source/QPainterResult.cpp \
    source/QPackageReceiveWorker.cpp \
    source/QMedicalRecordDlg.cpp \
    source/QMeasureDlg.cpp \
    source/QFilterData.cpp \
    source/QDrawWaveWorker.cpp \
    source/QDrawWaveWidget.cpp \
    source/QDrawMeasure.cpp \
    source/PackageSendCmd.cpp \
    source/PackageRecvCmd.cpp \
    source/packagecommon.cpp \
    source/measurearteriosclerosis.cpp \
    source/main.cpp

HEADERS  += \
    header/stdafx.h \
    header/QTabManagerDlg.h \
    header/QShowMeasureResult.h \
    header/QSerialRecvThread.h \
    header/QSavePressureWorker.h \
    header/QSavePressureThread.h \
    header/QSavePressure.h \
    header/QResultEvaluate.h \
    header/QReadAndWriteXml.h \
    header/QPersonBasicInfoDlg.h \
    header/QPainterResult.h \
    header/QPackageReceiveWorker.h \
    header/QMedicalRecordDlg.h \
    header/QMeasureDlg.h \
    header/QFilterData.h \
    header/QDrawWaveWorker.h \
    header/QDrawWaveWidget.h \
    header/QDrawMeasure.h \
    header/QConstValue.h \
    header/PackageSendCmd.h \
    header/PackageRecvCmd.h \
    header/packagecommon.h \
    header/measurearteriosclerosis.h \
    header/fenlishuju_and_lvbo_2.h

FORMS    += \
    MedicalRecordDlg.ui \
    PersonBasicInfo.ui
