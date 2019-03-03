#-------------------------------------------------
#
# Project created by QtCreator 2016-07-20T19:54:30
#
#-------------------------------------------------
include($${top_srcdir}/common_pre.pri)


QT       += core gui network widgets

TARGET = AstroCameraRemote
TEMPLATE = app

QTPLUGIN += svg


DLL_DEPS += \
    AstroBase \
    BatchProcess \
    DataFormat/Fits

SOURCES += main.cpp\
        MainWindow.cpp \
    SonyAlphaRemote_Helper.cpp \
    SonyAlphaRemote_Json_Command.cpp \
    SonyAlphaRemote_Sender.cpp \
    SsdpClient.cpp \
    SonyAlphaRemote_StatusPoller.cpp \
    SonyAlphaRemote_Sequencer_StateBase.cpp \
    SonyAlphaRemote_Sequencer_StateWaitForStart.cpp \
    SonyAlphaRemote_Sequencer_StateNormalShooting.cpp \
    SonyAlphaRemote_Sequencer_StateBulbShooting.cpp \
    SonyAlphaRemote_Sequencer_StateWaitForCamReady.cpp \
    SonyAlphaRemote_Sequencer_StatePause.cpp \
    SonyAlphaRemote_Sequencer_StateFinish.cpp \
    SonyAlphaRemote_Json_GetEvent.cpp \
    SonyAlphaRemote_BatteryInfo.cpp \
    SonyAlphaRemote_Settings.cpp \
    SonyAlphaRemote_TimeUnitButton.cpp \
    SonyAlphaRemote_Sequencer_Settings.cpp \
    SonyAlphaRemote_Sequencer_BulbShootSequencer.cpp \
    SonyAlphaRemote_Sequencer_NormalShootSequencer.cpp \
    SonyAlphaRemote_Sequencer_SettingsManager.cpp \
    SonyAlphaRemote_Sequencer_Base.cpp \
    SettingsDialog.cpp \
    Settings_General.cpp \
    ToolBar.cpp \
    SonyAlphaRemote_PostView_FullScreenWindow.cpp \
    PostView_Widget.cpp \
    PostView_Info.cpp \
    LiveView_Widget.cpp \
    LiveView_Reader.cpp \
    LiveView_Commands.cpp \
    LiveView_ReaderThread.cpp \
    LiveView_Info.cpp \
    hfd/Hfd_Calculator.cpp \
    LiveView_ImageQueue.cpp \
    LiveView_Settings.cpp \
    StarTrack_Marker.cpp \
    StarTrack_GraphicsScene.cpp \
    StarTrack_Settings.cpp \
    StarTrack_LenseGraphcisScene.cpp \
    BatchProcess_FitsRepair.cpp \
    PostView_ImageLoader.cpp \
    StarTrack_StarTrackView.cpp \
    StarTrack_GraphicsView.cpp \
    StarTrack_StarInfo.cpp

HEADERS  += MainWindow.h \
    SonyAlphaRemote_Helper.h \
    SonyAlphaRemote_Json_Command.h \
    SonyAlphaRemote_Sender.h \
    SsdpClient.h \
    SonyAlphaRemote_StatusPoller.h \
    SonyAlphaRemote_Sequencer_StateBase.h \
    SonyAlphaRemote_Sequencer_StateWaitForStart.h \
    SonyAlphaRemote_Sequencer_StateNormalShooting.h \
    SonyAlphaRemote_Sequencer_StateBulbShooting.h \
    SonyAlphaRemote_Sequencer_StateWaitForCamReady.h \
    SonyAlphaRemote_Sequencer_StatePause.h \
    SonyAlphaRemote_Sequencer_StateFinish.h \
    SonyAlphaRemote_Json_GetEvent.h \
    SonyAlphaRemote_BatteryInfo.h \
    SonyAlphaRemote_Settings.h \
    SonyAlphaRemote_TimeUnitButton.h \
    SonyAlphaRemote_Sequencer_Settings.h \
    SonyAlphaRemote_Sequencer_BulbShootSequencer.h \
    SonyAlphaRemote_Sequencer_NormalShootSequencer.h \
    SonyAlphaRemote_Sequencer_SettingsManager.h \
    SonyAlphaRemote_Sequencer_Base.h \
    SettingsDialog.h \
    Settings_General.h \
    ToolBar.h \
    SonyAlphaRemote_PostView_FullScreenWindow.h \
    PostView_Widget.h \
    PostView_Info.h \
    LiveView_Widget.h \
    LiveView_Reader.h \
    LiveView_Commands.h \
    LiveView_ReaderThread.h \
    LiveView_Info.h \
    hfd/Hfd_Calculator.h \
    LiveView_ImageQueue.h \
    LiveView_Settings.h \
    StarTrack_Marker.h \
    StarTrack_GraphicsScene.h \
    StarTrack_Settings.h \
    StarTrack_LenseGraphcisScene.h \
    BatchProcess_FitsRepair.h \
    PostView_ImageLoader.h \
    StarTrack_StarTrackView.h \
    StarTrack_GraphicsView.h \
    StarTrack_StarInfo.h

FORMS    += MainWindow.ui \
    SettingsDialog.ui \
    SonyAlphaRemote_PostView_FullScreenWindow.ui \
    PostView_Widget.ui \
    LiveView_Widget.ui \
    StarTrack_StarTrackView.ui

#DEFINES += DRY_RUN

DISTFILES += \
    ToDo.txt \
    features/win32/setup.prf

RESOURCES += \
    resources.qrc \
    testdata_1.qrc \
    testdata_2.qrc \
    testdata_3.qrc \
    testdata_4.qrc \
    testdata_5.qrc

INSTALL_FILES = target

include($${top_srcdir}/common_post.pri)

