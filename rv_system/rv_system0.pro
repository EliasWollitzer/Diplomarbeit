include(../GESys/include.pro)
include(../GESys/include.pro)
TEMPLATE = app
TARGET = rv_system0
BUILD_PATH = .build/release

QMAKE_CFLAGS += -O2 -std=c11 -Wall -pedantic
QMAKE_CXXFLAGS += -O2 -std=c++11 -Wall -pedantic
CODECFORTR = UTF-8
CODECFORSRC = UTF-8
CONFIG *= link_pkgconfig silent

debug {
	DEFINES += DEBUG
	QMAKE_CFLAGS += -g -O0
	QMAKE_CXXFLAGS += -g -O0
	QMAKE_LFLAGS += -g -rdynamic
	BUILD_PATH = .build/debug
}

MOC_DIR = $$BUILD_PATH/moc
UI_DIR = $$BUILD_PATH/ui
OBJECTS_DIR = $$BUILD_PATH/obj
RCC_DIR = $$BUILD_PATH/res

QT += \
        core \
	quick \
	qml

SOURCES += \
	main.cpp

HEADERS +=

FORMS +=

RESOURCES += \
	resources.qrc

target.path = /tmp
INSTALLS += target

DISTFILES += \
	assets/main.qml \
    assets/Home.qml \
    assets/Newentry.qml \
    assets/Add.qml \
    assets/keyboard/keyboard_num.xml \
    assets/keyboard/keyboard_ru.xml \
    assets/keyboard/keyboard_us.xml \
    assets/keyboard/Key.qml \
    assets/keyboard/Keyboard.qml \
    assets/keyboard/KeyboardItem.qml \
    assets/Input.qml \
    assets/Detail.qml \
    assets/DateSelection.qml
