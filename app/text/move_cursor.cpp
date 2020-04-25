 
/**
 * move_cursor.cpp -- реализует функцию перемещения курсора
 *
 * Copyright (c) 2020, Arseniy Fomin <fomin@mail.cs.petrsu.ru>
 *
 * This code is licensed under a MIT-style license.
 */

#include "_text.h"

/**
 * Перемещает указатель
 * 
 * @param txt текст
 * @param line номер строки
 * @param pos номер позиции
 * @returns none
 */
void move_cursor(text txt, int line, int pos)
{   
    /* Проверяем, имеется ли текст */
    if (txt == NULL || txt->lines.size() == 0) {
        fprintf(stderr, "There is no text to work with!\n");
        return;
    } 
    
    /* Текст ненулевой длины должен содержать хотя бы одну строку */
    assert(!(txt->lines.empty()));
    
    /* Стартуем с начальной строки текста */
    list<string>::iterator current = txt->lines.begin();
    
    /* Параметр должен попадать в диапазон */
    if (line < 0 || (size_t)line >= txt->lines.size()) {
        fprintf(stderr, "Can't place cursor here! No such line!\n");
        return;
    }
    
    /* Переходим на строку с номером line */
    advance(current, line);
    
    /* Параметр должен попадать в диапазон */
    if (pos < 0 || (size_t)pos > (*current).size()) {
        fprintf(stderr, "Can't place cursor here! Incorrect position!\n");
        return;
    }
    
    /* Обновляем позицию курсора */
    txt->cursor->line = current;
    txt->cursor->position = pos;
}
