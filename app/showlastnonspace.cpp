/**
 * showlastnonspace.cpp -- вывод последних непробельных символов
 *
 * Copyright (c) 2020, Arseniy Fomin <fomin@mail.cs.petrsu.ru>
 *
 * This code is licensed under a MIT-style license.
 */

#include <cstdio>
#include "common.h"
#include "text/text.h"

static void show_last(int index, string& contents, int cursor, void *data);

/**
 * Выводит последние непробельные символы на экран
 */
void showlastnonspace(text txt)
{
    /* Применяем функцию show_last к каждой строке текста */
    process_forward(txt, show_last, NULL);
}

static void show_last(int index, string& contents, int cursor, void *data)
{    
    /* Декларируем неиспользуемые параметры */
    UNUSED(index);
    UNUSED(data);
    UNUSED(cursor);
    
    /* Находим последний непробельный символ */
    int last_char = contents.size() - 1;
    while ((contents[last_char] == ' ') && (last_char >= 0))
        last_char--;
    
    /* Выводим символ на экран */
    if (last_char > -1)
        printf("%c\n", contents[last_char]);
}
