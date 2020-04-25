#ifndef MOVE_TEST_H
#define MOVE_TEST_H

#include <gtest/gtest.h>

#include <fcntl.h>
#include <unistd.h>
#include <cstdio>

extern "C++" {
#include "common.h"
#include "text/text.h"
#include "text/_text.h"
}



TEST(moveTestPositive, functional) {
    text txt = create_text();

    char *inputTest = (char *)malloc(sizeof(char)*1024);
    sprintf(inputTest, "%s/input.txt", INPUTDIRMOVE);

    load(txt, inputTest);
    free(inputTest);

    move(txt, 1, 5);

    int newPos = txt->cursor->position;

    list<string>::iterator current = txt->lines.begin();
    int newLine = 0;

    while (current != txt->cursor->line) {
        current++;
        newLine++;
    }

    ASSERT_EQ(newLine, 1);
    ASSERT_EQ(newPos, 5);
}



TEST(moveTestPositive, terminal) {
    char cmdline[MAXLINE + 1];
    char *cmd;
    char *arg;

    text txt = create_text();

    char *inputTest = (char *)malloc(sizeof(char)*1024);
    sprintf(inputTest, "%s/input.txt", INPUTDIRMOVE);

    load(txt, inputTest);
    free(inputTest);

    char *command = (char *)malloc(sizeof(char)*1024);
    sprintf(command, "%s/command_one.txt", INPUTDIRMOVE);

    int newSTDin = open(command, O_RDONLY);
    int oldSTDin = dup(STDIN_FILENO);
    dup2(newSTDin, STDIN_FILENO);
    free(command);

    // Original line from editor.cpp
    fgets(cmdline, MAXLINE, stdin);

    close(newSTDin);
    dup2(oldSTDin, STDIN_FILENO);

    // Original lines from editor.cpp
    cmd = strtok(cmdline, " \n");
    if (strcmp(cmd, "m") == 0) {
        if ((arg = strtok(NULL, " \n")) == NULL) {
            fprintf(stderr, "Usage: m line position\n");
        } else {
            int line = atoi(arg);
            if ((arg = strtok(NULL, " \n")) == NULL) {
                fprintf(stderr, "Usage: m line position\n");
            } else {
                int pos = atoi(arg);
                ASSERT_TRUE(true);
                move(txt, line, pos);
            }
        }
    }

    int newPos = txt->cursor->position;

    list<string>::iterator current = txt->lines.begin();
    int newLine = 0;

    while (current != txt->cursor->line) {
        current++;
        newLine++;
    }

    ASSERT_EQ(newLine, 1);
    ASSERT_EQ(newPos, 5);
}

#endif // MOVE_TEST_H
