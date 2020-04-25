/**
 * common.h -- прототипы функций реализации команд редактора
 * 
 * Copyright (c) 2020, Arseniy Fomin <fomin@mail.cs.petrsu.ru>
 *
 * This code is licensed under a MIT-style license.
 */

#ifndef COMMON_H
#define COMMON_H

#include "text/text.h"

/* Декларирует неиспользуемый аргумент,
 * корректно подавляя предупреждение вида "unused parameter" */
#define UNUSED(x) (void)(x)

/**
 * Выводит содержимое указанного файла на экран
 */
void show(text txt);

/**
 * Выводит каждый последний непробельный символ
 * строки в прямом порядке следования строк
 */
void showlastnonspace(text txt);

/**
 * Загружает содержимое указанного файла
 */
void load(text txt, char *filename);

/**
 * Сохраняет содержимое в указанный файл
 */
void save(text txt, char *filename);

/**
 * Перемещает курсор в указанное место
 */
void move(text txt, int line, int pos);

/**
 * Вставляет текст после курсора
 */
void insert(text txt, char *contents);

/**
 * Перемещает текущую строку в начало текста
 */
void copy_begin(text txt);

#endif
