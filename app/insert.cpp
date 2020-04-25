 
/**
 * insert.cpp -- функция вставки строки от курсора
 *
 * Copyright (c) 2020, Arseniy Fomin <fomin@mail.cs.petrsu.ru>
 *
 * This code is licensed under a MIT-style license.
 */

#include <cstdio>
#include <cstring>
#include "common.h"
#include "text/text.h"



static void insert_line(int index, string& contents, int cursor, void *data);

/**
 * Вставляет текст в строку от курсора
 */
void insert(text txt, char *contents)
{
    get_current(txt, insert_line, (void *)contents);
}

static void insert_line(int index, string& contents, int cursor, void *data)
{    
    /* Декларируем неиспользуемые параметры */
    UNUSED(index);

    char *inclusion = (char *)data;
  
    /* Проверка на переполнение */
    if (contents.size() + strlen(inclusion) > MAXLINE) {
        printf("The insert line is too long!\n");
        return;
    }
    
    contents.insert(cursor, inclusion);    
}
