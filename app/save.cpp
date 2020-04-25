/**
 * save.cpp -- функция сохранения текста в файл
 *
 * Copyright (c) 2020, Arseniy Fomin <fomin@mail.cs.petrsu.ru>
 *
 * This code is licensed under a MIT-style license.
 */

#include <cstdio>
#include "common.h"
#include "text/text.h"



static void save_line(int index, string& contents, int cursor, void *data);

/**
 * Сохраняет содержимое в указанный файл
 */
void save(text txt, char *filename)
{
    FILE *f;

    /* Открываем файл для записи контролируя ошибки */
    if ((f = fopen(filename, "w")) == NULL) {
        printf("The file %s cannot be opened\n", filename);
        return;
    }

    /* Сохраняет содержимое текста в файл */
    process_forward(txt, save_line, (void *)f);

    fclose(f);

}

static void save_line(int index, string& contents, int cursor, void *data)
{
    FILE *f = (FILE *)data;

    /* Декларируем неиспользуемые параметры */
    UNUSED(index);
    UNUSED(cursor);

    /* Выводим строку на экран */
    fprintf(f, "%s", (contents + "\n").c_str());
}
