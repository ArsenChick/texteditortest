/**
 * process_forward.cpp -- реализует обобщенную функцию обработки текста
 *
 * Copyright (c) 2020, Arseniy Fomin <fomin@mail.cs.petrsu.ru>
 *
 * This code is licensed under a MIT-style license.
 */

#include "_text.h"

/**
 * Обрабатывает список, применяя заданную функцию к каждой строке
 * 
 * @param txt текст
 * @param process функция-обработчик
 * @returns none
 */
void process_forward(
    text txt,
    void (*process)(int index, string& contents, int cursor, void *data),
    void *data
)
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
    int index = 0;
    int cursor_position = -1;
    
    /* К каждой строке текста применяем обработчик */
    while (current != txt->lines.end()) {
        if (txt->cursor->line == current) 
            cursor_position = txt->cursor->position;
        else
            cursor_position = -1;
        process(index, *current, cursor_position, data);
        current++;
        index++;
    }
}
