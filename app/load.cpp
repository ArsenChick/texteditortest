/**
 * load.cpp -- функция загрузки текста из файла
 * 
 * Copyright (c) 2020, Arseniy Fomin <fomin@mail.cs.petrsu.ru>
 *
 * This code is licensed under a MIT-style license.
 */

#include <cstdio>
#include <cstring>
#include "common.h"
#include "text/text.h"

/**
 * Загружает содержимое указанного файла
 */
void load(text txt, char *filename)
{
    FILE *f;

    char buf[MAXLINE + 1];

    /* Открываем файл для чтения, контролируя ошибки */
    if ((f = fopen(filename, "r")) == NULL) {
        printf("The file %s cannot be opened\n", filename);
        return;
    }

    /* Удаляем предыдущее содержимое */
    remove_all(txt);

    /* Считываем содержимое строка за строкой */
    while (fgets(buf, MAXLINE, f)) {
        if (buf[strlen(buf) - 1] == '\n')
            buf[strlen(buf) - 1] = '\0';
        append_line(txt, buf);
    }

    fclose(f);
}
