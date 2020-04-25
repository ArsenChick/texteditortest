/**
 * _text.h -- внутренние структуры представления текста
 * 
 * Этот файл не должен включаться в клиентские модули
 *
 * Copyright (c) 2020, Arseniy Fomin <fomin@mail.cs.petrsu.ru>
 *
 * This code is licensed under a MIT-style license.
 */

#ifndef _TEXT_H
#define _TEXT_H

#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <iostream>
#include <list>

#include "text.h"

/**
 * @struct 
 * @brief Представление курсора
 * 
 */
typedef struct _crsr {
    list<string>::iterator line;        /**< указатель на строку с курсором */
    int position;                       /**< позиция курсора в строке */
} crsr;


/**
 * @struct 
 * @brief Линейный двунаправленный список строк текста
 * 
 */
typedef struct _list {
    list<string> lines;         /**< линейный список строк */
    struct _crsr *cursor;       /**< позиция в тексте, курсор */
} myList;

typedef struct _list *text;

#endif
