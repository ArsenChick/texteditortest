/**
 * text.h -- внешний интерфейс библиотеки для работы с текстом
 *
 * Copyright (c) 2017, Alexander Borodin <aborod@petrsu.ru>
 *
 * This code is licensed under a MIT-style license.
 */

#ifndef TEXT_H
#define TEXT_H

#include <string>

#define MAXLINE 255

using namespace std;

/**
 * Абстрактный тип текстовых объектов
 */
typedef struct _list *text;

/**
 * Абстрактный тип курсора
 */
typedef struct _crsr *cursor;


/**
 * Создает новый текстовый объект
 * 
 * @returns текст
 */
text create_text();

/**
 * Добавляет одну строку к тексту
 * 
 * @param txt текст
 * @param contents новая строка
 * @returns none
 */
void append_line(text txt, string contents);

/**
 * Перемещает указатель
 * 
 * @param txt текст
 * @param line номер строки
 * @param pos номер позиции
 * @returns none
 */
void move_cursor(text txt, int line, int pos);

/**
 * Возвращает данные о строке с курсором
 * и применяет к ней заданную функцию
 * 
 * @param txt текст
 * @param process функция-обработчик
 * @returns none
 */
void get_current(
    text txt,
    void (*process)(int index, string& contents, int cursor_position, void *data),
    void *data
);

/**
 * Меняет позицию строки в тексте
 * 
 * @param txt текст
 * @param old_pos старая позиция
 * @param new_pos новая позиция
 * @returns none
 */
void move_line(text txt, int old_pos, int new_pos);

/**
 * Обрабатывает текст, применяя заданную функцию к каждой строке
 * 
 * @param txt текст
 * @param process функция-обработчик
 * @returns none
 */
void process_forward(
    text txt,
    void (*process)(int index, string& contents, int cursor_position, void *data),
    void *data
);


/**
 * Удаляет весь текст
 * 
 * @param txt текст
 * @returns none
 */
void remove_all(text txt);

#endif
