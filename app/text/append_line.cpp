/**
 * append_line.cpp -- функции для добавления строк
 * 
 * Copyright (c) 2020, Arseniy Fomin <fomin@mail.cs.petrsu.ru>
 *
 * This code is licensed under a MIT-style license.
 */

#include "_text.h"

/**
 * Добавляет одну строку к тексту
 * 
 * @param txt текст
 * @param contents новая строка
 * @returns none
 */
void append_line(text txt, string contents)
{
    /* Если список пуст, создаем первый элемент 
     * независимо, пустой он или нет */
    txt->lines.push_back(contents);
    
    /* Устанавливаем курсор в конец добавленной строки */
    txt->cursor->line = --(txt->lines.end());
    txt->cursor->position = txt->lines.back().size();
}
