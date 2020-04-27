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

    char *inputTest = (char *)malloc(sizeof(char)*2048);
    snprintf(inputTest, 2048, "%s/input.txt", INPUTDIRMOVE);

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
    remove_all(txt);
}



TEST(moveTestPositive, terminal) {
    char cmdline[MAXLINE + 1];
    char *cmd;
    char *arg;

    text txt = create_text();

    char *inputTest = (char *)malloc(sizeof(char)*2048);
    snprintf(inputTest, 2048, "%s/input.txt", INPUTDIRMOVE);

    load(txt, inputTest);
    free(inputTest);

    char *command = (char *)malloc(sizeof(char)*2048);
    snprintf(command, 2048, "%s/command_one.txt", INPUTDIRMOVE);

    int newSTDin = open(command, O_RDONLY);
    int oldSTDin = dup(STDIN_FILENO);
    dup2(newSTDin, STDIN_FILENO);
    free(command);

    // Original line from editor.cpp
    char* success = fgets(cmdline, MAXLINE, stdin);
    EXPECT_STRNE(success, NULL);

    close(newSTDin);
    dup2(oldSTDin, STDIN_FILENO);

    // Original lines from editor.cpp
    cmd = strtok(cmdline, " \n");
    if (strcmp(cmd, "m") == 0) {
        if ((arg = strtok(NULL, " \n")) == NULL) {
            EXPECT_TRUE(false);
            fprintf(stderr, "Usage: m line position\n");
        } else {
            int line = atoi(arg);
            if ((arg = strtok(NULL, " \n")) == NULL) {
                EXPECT_TRUE(false);
                fprintf(stderr, "Usage: m line position\n");
            } else {
                int pos = atoi(arg);
                EXPECT_TRUE(true);
                move(txt, line, pos);
            }
        }
    } else
        EXPECT_TRUE(false);

    int newPos = txt->cursor->position;

    list<string>::iterator current = txt->lines.begin();
    int newLine = 0;

    while (current != txt->cursor->line) {
        current++;
        newLine++;
    }

    ASSERT_EQ(newLine, 1);
    ASSERT_EQ(newPos, 5);
    remove_all(txt);
}



TEST(moveTestNegative, emptyText) {
    text txt = create_text();

    char *debug = (char *)malloc(sizeof(char)*64);
    snprintf(debug, 64, "file.log");

    int newSTDerr = open(debug, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    ASSERT_NE(newSTDerr, -1);
    int oldSTDerr = dup(STDERR_FILENO);
    close(STDERR_FILENO);
    dup2(newSTDerr, STDERR_FILENO);

    move(txt, 0, 0);

    fflush(stderr);
    close(newSTDerr);
    dup2(oldSTDerr, STDERR_FILENO);

    int testFD = open(debug, O_RDONLY);
    char *outBuf = (char *)malloc(sizeof(char)*64);
    char *testBuf = (char *)malloc(sizeof(char)*64);
    int testCount;

    testCount = read(testFD, testBuf, 64);
    snprintf(outBuf, 64, "There is no text to work with!\n");
    ASSERT_TRUE(testCount > 0);
    close(testFD);

    for(int i = 0; i < testCount; i++)
        ASSERT_EQ(outBuf[i], testBuf[i]);

    free(outBuf);
    free(testBuf);

    int ret = std::remove(debug);
    ASSERT_EQ(ret, 0);
    free(debug);
    remove_all(txt);
}



TEST(moveTestNegative, wrongPos) {
    text txt = create_text();

    char *input = (char *)malloc(sizeof(char)*2048);
    char *debug = (char *)malloc(sizeof(char)*64);
    snprintf(input, 2048, "%s/input.txt", INPUTDIRMOVE);
    snprintf(debug, 64, "file.log");

    load(txt, input);
    free(input);

    int newSTDerr = open(debug, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    ASSERT_NE(newSTDerr, -1);
    int oldSTDerr = dup(STDERR_FILENO);
    close(STDERR_FILENO);
    dup2(newSTDerr, STDERR_FILENO);

    move(txt, -1, 0);
    move(txt, 10, 0);
    move(txt, 0, -1);
    move(txt, 0, 99);

    fflush(stderr);
    close(newSTDerr);
    dup2(oldSTDerr, STDERR_FILENO);

    int testFD = open(debug, O_RDONLY);
    char *outBuf = (char *)malloc(sizeof(char)*256);
    char *testBuf = (char *)malloc(sizeof(char)*256);
    int testCount;

    testCount = read(testFD, testBuf, 256);
    snprintf(outBuf, 256, "Can't place cursor here! No such line!\n"
                    "Can't place cursor here! No such line!\n"
                    "Can't place cursor here! Incorrect position!\n"
                    "Can't place cursor here! Incorrect position!\n");
    ASSERT_TRUE(testCount > 0);
    close(testFD);

    for(int i = 0; i < testCount; i++)
        ASSERT_EQ(outBuf[i], testBuf[i]);

    free(outBuf);
    free(testBuf);

    int ret = std::remove(debug);
    ASSERT_EQ(ret, 0);
    free(debug);
    remove_all(txt);
}



/*TEST(moveTestNegative, noArguments) {
    char cmdline[MAXLINE + 1];
    char *cmd;
    char *arg;

    text txt = create_text();
    append_line(txt, "This is a test line.");

    char *debug = (char *)malloc(sizeof(char)*64);
    char *command = (char *)malloc(sizeof(char)*2048);
    sprintf(command, "%s/command_two.txt", INPUTDIRMOVE);
    sprintf(debug, "file.log");

    int newSTDin = open(command, O_RDONLY);
    int oldSTDin = dup(STDIN_FILENO);
    dup2(newSTDin, STDIN_FILENO);
    free(command);

    // Original line from editor.cpp
    char* success = fgets(cmdline, MAXLINE, stdin);
    EXPECT_STRNE(success, NULL);

    close(newSTDin);
    dup2(oldSTDin, STDIN_FILENO);

    int newSTDout = open(debug, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    ASSERT_NE(newSTDout, -1);
    int oldSTDout = dup(STDOUT_FILENO);
    close(STDOUT_FILENO);
    dup2(newSTDout, STDOUT_FILENO);

    // Original lines from editor.cpp
    cmd = strtok(cmdline, " \n");
    if (strcmp(cmd, "m") == 0) {
        if ((arg = strtok(NULL, " \n")) == NULL) {
            EXPECT_TRUE(true);
            fprintf(stdout, "Usage: m line position\n");
        } else {
            int line = atoi(arg);
            if ((arg = strtok(NULL, " \n")) == NULL) {
                EXPECT_TRUE(true);
                fprintf(stdout, "Usage: m line position\n");
            } else {
                int pos = atoi(arg);
                EXPECT_TRUE(false);
                move(txt, line, pos);
            }
        }
    } else
        EXPECT_TRUE(false);

    fflush(stdout);
    close(newSTDout);
    dup2(oldSTDout, STDOUT_FILENO);

    int testFD = open(debug, O_RDONLY);
    char *outBuf = (char *)malloc(sizeof(char)*64);
    char *testBuf = (char *)malloc(sizeof(char)*64);
    int testCount;

    testCount = read(testFD, testBuf, 64);
    sprintf(outBuf, "Usage: m line position\n");
    ASSERT_TRUE(testCount > 0);
    close(testFD);

    for(int i = 0; i < testCount; i++)
        ASSERT_EQ(outBuf[i], testBuf[i]);

    free(outBuf);
    free(testBuf);

    int ret = std::remove(debug);
    ASSERT_EQ(ret, 0);
    free(debug);
    remove_all(txt);
}*/

#endif // MOVE_TEST_H
