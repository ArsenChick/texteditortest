 /**
 * m.c -- функция перемещения курсора
 * 
 * Copyright (c) 2020, Arseniy Fomin <fomin@mail.cs.petrsu.ru>
 *
 * This code is licensed under a MIT-style license.
 */

#include "common.h"
#include "text/text.h"

/**
 * Перемещает курсор на указанную позицию
 */
void move(text txt, int line, int pos)
{
    move_cursor(txt, line, pos);
}
