/**
 * remove_all.cpp -- реализует функцию очистки текста
 *
 * Copyright (c) 2020, Arseniy Fomin <fomin@mail.cs.petrsu.ru>
 *
 * This code is licensed under a MIT-style license.
 */

#include "_text.h"

/**
 * Удаляет весь текст
 * 
 * @param txt текст
 * @returns none
 */
void remove_all(text txt)
{
    /* Проверяем, имеется ли текст */
    if (txt == NULL) {
        fprintf(stderr, "The text doesn't exist!\n");
        return;
    }    
    
    /* Если список изначально пуст, делать ничего не надо */
    if (txt->lines.size() == 0) {
        return;
    }

    /* Текст ненулевой длины должен содержать хотя бы одну строку */
    assert(!(txt->lines.empty()));
    
    /* Очищаем контейнер со строками */
    txt->lines.clear();

    /* Реинициализируем курсор */
    txt->cursor->line = txt->lines.end();
    txt->cursor->position = 0;
}
