QT += gui
QT += widgets
QT += sql

TARGET = client
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

CONFIG(debug, debug|release): DEFINES += _DEBUG
CONFIG(release, debug|release): DEFINES += NDEBUG

CONFIG(release, debug|release): DESTDIR = $$PWD/../build/MOBILE_CLIENT/release
CONFIG(debug, debug|release): DESTDIR = $$PWD/../build/MOBILE_CLIENT/debugs

INCLUDEPATH += $$PWD/../common \
               $$PWD/Config

LIBS += -lcrypto \
        -lssl \
        -lconfig++ \
        -lprotobuf


TEMPLATE = app

SOURCES += main.cpp \
    ../common/Worker.cpp \
    Client.cpp \
    ../common/define.cpp \
    ../common/Helper.cpp \
    MessageManager.cpp \
    Config/ConfigManager.cpp \
    Config/GlobalParams.cpp \
    RRManager.cpp \
    $$PWD/Protobuf/Message.pb.cc \
    Protobuf/MessageType.pb.cc \
    authReg.cpp \
    Crypto/CryptoHelper.cpp \
    GuestView.cpp \
    AdminView.cpp \
    ManagerView.cpp \
    ReceptionistView.cpp \
    RegisterView.cpp

HEADERS += \
    ../common/Worker.h \
    Client.h \
    ../common/define.h \
    ../common/Helper.h \
    MessageManager.h \
    Config/ConfigManager.h \
    Config/GlobalParams.h \
    RRManager.h \
    $$PWD/Protobuf/Message.pb.h \
    Protobuf/MessageType.pb.h \
    authReg.h \
    Crypto/CryptoHelper.h \
    GuestView.h \
    AdminView.h \
    ManagerView.h \
    ReceptionistView.h \
    RegisterView.h

system(cp $$PWD/SSL/* $PWD/../build/MOBILE_CLIENT/debugs)
system(cp $$PWD/Config/config.cfg $PWD/../build/MOBILE_CLIENT/debugs)
system(rm -rf Protobuf && mkdir Protobuf)

PROTOPATH = $$PWD/../mobileoperator_proto/protoSourse
system(protoc --proto_path=$${PROTOPATH} --cpp_out=./Protobuf $${PROTOPATH}/Message.proto)
system(protoc --proto_path=$${PROTOPATH} --cpp_out=./Protobuf $${PROTOPATH}/MessageType.proto)


DISTFILES += \
    Config/config.cfg \
    ../mobileoperator_proto/protoSourse/MessageType.proto \
    ../mobileoperator_proto/protoSourse/Message.proto

FORMS += \
    authReg.ui \
    GuestView.ui \
    AdminView.ui \
    ManagerView.ui \
    ReceptionistView.ui \
    RegisterView.ui
