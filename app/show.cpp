/**
 * show.cpp -- реализует команду вывода хранимого текста на экран
 *
 * Copyright (c) 2020, Arseniy Fomin <fomin@mail.cs.petrsu.ru>
 *
 * This code is licensed under a MIT-style license.
 */

#include <cstdio>
#include "common.h"
#include "text/text.h"

static void show_line(int index, string& contents, int cursor, void *data);

/**
 * Выводит содержимое указанного файла на экран
 */
void show(text txt)
{
    /* Применяем функцию show_line к каждой строке текста */
    process_forward(txt, show_line, NULL);
}

/**
 * Выводит содержимое указанного файла на экран
 */
static void show_line(int index, string& contents, int cursor, void *data)
{
    /* Декларируем неиспользуемые параметры */
    UNUSED(index);
    UNUSED(data);
    
    string line = contents;

    if (cursor != -1) {
        line.insert(cursor, 1, '|');
    }
    
    /* Выводим строку на экран */
    printf("%s\n", line.c_str());
}
