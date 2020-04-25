/**
 * create_text.cpp -- создает в памяти структуры для представления текста
 *
 * Copyright (c) 2020, Arseniy Fomin <fomin@mail.cs.petrsu.ru>
 *
 * This code is licensed under a MIT-style license.
 */

#include "_text.h"

/**
 * Создает новый текстовый объект
 * 
 * @returns текст
 */
text create_text()
{  
    /* Создаем структуру описания текста */
    text txt = new myList;
    
    if (txt == NULL) {
        fprintf(stderr, "Not enough memory!\n");
        exit(EXIT_FAILURE);
    }
    
    /* Создаем структуру описания курсора */
    txt->cursor = new crsr;

    if (txt == NULL) {
        fprintf(stderr, "Not enough memory!\n");
        exit(EXIT_FAILURE);
    }
    
    /* Курсор не настроен ни на какой символ текста */
    txt->cursor->line = txt->lines.end();
    txt->cursor->position = 0;
    
    return txt;
}
