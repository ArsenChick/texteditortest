TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    editor.cpp \
    insert.cpp \
    load.cpp \
    move.cpp \
    move_lines.cpp \
    save.cpp \
    show.cpp \
    showlastnonspace.cpp \
    text/append_line.cpp \
    text/create_text.cpp \
    text/get_current.cpp \
    text/move_cursor.cpp \
    text/move_line.cpp \
    text/process_forward.cpp \
    text/remove_all.cpp

HEADERS += \
    common.h \
    text/_text.h \
    text/text.h

QMAKE_CFLAGS += -Wall -Wextra -Werror

# gcov
QMAKE_CFLAGS += -fprofile-arcs -ftest-coverage
LIBS += -lgcov
