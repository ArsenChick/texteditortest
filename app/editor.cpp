/**
 * editor.cpp -- строковый текстовый редактор
 * 
 * Copyright (c) 2020, Arseniy Fomin <fomin@mail.cs.petrsu.ru>
 *
 * This code is licensed under a MIT-style license.
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "text/text.h"
#include "common.h"

#define MAXLINE 255

int main()
{
    char cmdline[MAXLINE + 1];
    char *cmd;
    char *arg;
    
    /* Создаем объект для представления текста */
    text txt = create_text();

    /* Цикл обработки команд */
    while (1) {
        printf("ed > ");
        
        /* Получаем команду */
        fgets(cmdline, MAXLINE, stdin);

        /* Извлекаем имя команды */
        if ((cmd = strtok(cmdline, " \n")) == NULL) {
            continue;
        }

        /* Распознаем поддерживаемую команду */
        
        /* Завершаем работу редактора */
        if (strcmp(cmd, "quit") == 0) {
            fprintf(stderr, "Bye!\n");
            break;
        }

        /* Загружаем содержимое файла, заданного параметром */
        if (strcmp(cmd, "load") == 0) {
            if ((arg = strtok(NULL, " \n")) == NULL) {
                fprintf(stderr, "Usage: load filename\n");
            } else {
                load(txt, arg);
            }
            continue;
        }

        /* Перемещаем курсор в указанные строку и позицию */
        if (strcmp(cmd, "save") == 0) {
            if ((arg = strtok(NULL, " \n")) == NULL) {
                fprintf(stderr, "Usage: save filename\n");
            } else {
                save(txt, arg);
            }
            continue;
        }

        /* Выводим текст */
        if (strcmp(cmd, "show") == 0) {
            show(txt);
            continue;
        }
        
        /* Выводим последние непробельные символы строк */
        if (strcmp(cmd, "showlastnonspace") == 0) {
            showlastnonspace(txt);
            continue;
        }
        
        /* Перемещаем курсор в указанные строку и позицию */
        if (strcmp(cmd, "m") == 0) {
            if ((arg = strtok(NULL, " \n")) == NULL) {
                fprintf(stderr, "Usage: m line position\n");
            } else {
                int line = atoi(arg);
                if ((arg = strtok(NULL, " \n")) == NULL) {
                    fprintf(stderr, "Usage: m line position\n");
                } else {
                    int pos = atoi(arg);
                    move(txt, line, pos);
                }
            }
            continue;
        }
        
        /* Перемещаем текущую строку в начало текста */
        if (strcmp(cmd, "cb") == 0) {
            copy_begin(txt);
            continue;
        }
        
        /* Вставляем текст от курсора */
        if (strcmp(cmd, "i") == 0) {
            /* Читаем все символы полсе i */
            char *addstr = cmd + strlen(cmd) + 1;
            if (addstr[strlen(addstr) - 1] == '\n')
                addstr[strlen(addstr) - 1] = '\0';
            /* Проверка на правильность использования */
            if (*addstr == '\0') {
                fprintf(stderr, "Usage: i textstring\n");
            } else {
                insert(txt, addstr);
            }
            continue;
        }
        
        /* Если команда не известна */
        fprintf(stderr, "Unknown command: %s\n", cmd);
    }

    return 0;
}
