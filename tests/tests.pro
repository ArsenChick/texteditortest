include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

QMAKE_CXXFLAGS += -Wall -Wextra -Werror
QMAKE_CFLAGS += -Wall -Wextra -Werror

QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
QMAKE_CFLAGS += -fprofile-arcs -ftest-coverage
LIBS += -lgcov

DEFINES += INPUTDIRSAVE=\\\"$$PWD/testsave\\\" \
    INPUTDIRMOVE=\\\"$$PWD/testmove\\\" \
    INPUTDIRINS=\\\"$$PWD/testinsert\\\" \
    INPUTDIRCB=\\\"$$PWD/testcbegin\\\"

HEADERS +=     \
    ../app/common.h \
    ../app/text/text.h \
    ../app/text/_text.h \
    copybegin_test.h \
    insert_test.h \
    move_test.h \
    save_test.h

SOURCES +=     main.cpp \
    ../app/load.cpp \
    ../app/show.cpp \
    ../app/save.cpp \
    ../app/showlastnonspace.cpp \
    ../app/move.cpp \
    ../app/move_lines.cpp \
    ../app/insert.cpp \
    ../app/text/append_line.cpp \
    ../app/text/create_text.cpp \
    ../app/text/get_current.cpp \
    ../app/text/move_cursor.cpp \
    ../app/text/move_line.cpp \
    ../app/text/process_forward.cpp \
    ../app/text/remove_all.cpp

INCLUDEPATH += ../app
