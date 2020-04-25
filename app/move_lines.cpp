 
 /**
 * move_lines.cpp -- функция перемещения текущей строки в начало текста
 * 
 * Copyright (c) 2020, Arseniy Fomin <fomin@mail.cs.petrsu.ru>
 *
 * This code is licensed under a MIT-style license.
 */

#include <cstdio>
#include <cassert>
#include "common.h"
#include "text/text.h"

static void move_begin(int index, string& contents, int cursor, void *data);

/**
 * Перемещает текущую строку в начало текста
 */
void copy_begin(text txt)
{
    get_current(txt, move_begin, (void *)txt);
}

static void move_begin(int index, string& contents, int cursor, void *data)
{    
    /* Декларируем неиспользуемые параметры */
    UNUSED(cursor);
    UNUSED(contents);
    
    text txt = (text)data;
    
    move_line(txt, index, 0);
    
}
