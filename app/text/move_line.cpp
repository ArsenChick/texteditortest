/**
 * move_line.cpp -- функции для перемещения строк
 *
 * Copyright (c) 2020, Arseniy Fomin <fomin@mail.cs.petrsu.ru>
 *
 * This code is licensed under a MIT-style license.
 */

#include "_text.h"

/**
 * Меняет позицию строки в тексте
 *
 * @param txt текст
 * @param old_pos старая позиция
 * @param new_pos новая позиция
 * @returns none
 */
void move_line(text txt, int old_pos, int new_pos)
{
    /* Проверяем, имеется ли текст */
    if (txt == NULL || txt->lines.size() == 0) {
        fprintf(stderr, "There is no text to work with!\n");
        return;
    } 
    
    /* Текст ненулевой длины должен содержать хотя бы одну строку */
    assert(!(txt->lines.empty()));

    /* Параметры должны попадать в диапазон */
    if ((old_pos < 0 || (size_t)old_pos >= txt->lines.size()) ||
        (new_pos < 0 || (size_t)new_pos >= txt->lines.size())) {
        fprintf(stderr, "Incorrect parameters!\n");
        return;
    }

    /* Переходим на строку с номером old_pos */
    list<string>::iterator current = txt->lines.begin();
    advance(current, old_pos);
    
    /* Запоминаем содержание и удаляем строку */
    string buf = *current;
    txt->lines.erase(current);
    
    /* Переходим на строку с номером new_pos */
    current = txt->lines.begin();
    advance(current, new_pos);
    
    /* Вставляем строку на новое место и обновляем курсор */
    txt->lines.insert(current, buf);
    txt->cursor->line = --current;
}
