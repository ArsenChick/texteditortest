#ifndef INSERT_TEST_H
#define INSERT_TEST_H

#include <gtest/gtest.h>

#include <fcntl.h>
#include <unistd.h>
#include <cstdio>

extern "C++" {
#include "common.h"
#include "text/text.h"
}



TEST(insertTestPositive, functional) {
    text txt = create_text();

    char *inputTest = (char *)malloc(sizeof(char)*1024);
    sprintf(inputTest, "%s/input.txt", INPUTDIRINS);

    load(txt, inputTest);
    free(inputTest);

    char line[32] = {" This is a test line."};
    insert(txt, line);

    char *testOutput = (char *)malloc(sizeof(char)*1024);
    char *originalOutput = (char *)malloc(sizeof(char)*1024);
    sprintf(originalOutput, "%s/output.txt", INPUTDIRINS);
    sprintf(testOutput, "%s/outputTest.txt", INPUTDIRINS);

    save(txt, testOutput);

    int testFD = open(testOutput, O_RDONLY);
    int originalFD = open(originalOutput, O_RDONLY);
    free(originalOutput);

    char *outBuf = (char *)malloc(sizeof(char)*256);
    char *testBuf = (char *)malloc(sizeof(char)*256);
    int outCount, testCount;

    outCount = read(originalFD, outBuf, 256);
    testCount = read(testFD, testBuf, 256);
    close(originalFD);
    close(testFD);

    ASSERT_EQ(outCount, testCount);
    for(int i = 0; i < testCount; i++)
        ASSERT_EQ(outBuf[i], testBuf[i]);

    free(outBuf);
    free(testBuf);

    int ret = std::remove(testOutput);
    ASSERT_EQ(ret, 0);
    free(testOutput);
}



TEST(insertTestPositive, terminal) {
    char cmdline[MAXLINE + 1];
    char *cmd;

    text txt = create_text();

    char *inputTest = (char *)malloc(sizeof(char)*1024);
    sprintf(inputTest, "%s/input.txt", INPUTDIRINS);

    load(txt, inputTest);
    free(inputTest);

    char *command = (char *)malloc(sizeof(char)*1024);
    sprintf(command, "%s/command_one.txt", INPUTDIRINS);

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
    if (strcmp(cmd, "i") == 0) {
        char *addstr = cmd + strlen(cmd) + 1;
        if (addstr[strlen(addstr) - 1] == '\n')
            addstr[strlen(addstr) - 1] = '\0';
        if (*addstr == '\0') {
            fprintf(stderr, "Usage: i textstring\n");
        } else {
            insert(txt, addstr);
        }
    }

    char *testOutput = (char *)malloc(sizeof(char)*64);
    char *originalOutput = (char *)malloc(sizeof(char)*1024);
    sprintf(originalOutput, "%s/output.txt", INPUTDIRINS);
    sprintf(testOutput, "%s/outputTest.txt", INPUTDIRINS);

    save(txt, testOutput);

    int testFD = open(testOutput, O_RDONLY);
    int originalFD = open(originalOutput, O_RDONLY);
    free(originalOutput);

    char *outBuf = (char *)malloc(sizeof(char)*256);
    char *testBuf = (char *)malloc(sizeof(char)*256);
    int outCount, testCount;

    outCount = read(originalFD, outBuf, 256);
    testCount = read(testFD, testBuf, 256);
    close(originalFD);
    close(testFD);

    ASSERT_EQ(outCount, testCount);
    for(int i = 0; i < testCount; i++)
        ASSERT_EQ(outBuf[i], testBuf[i]);

    free(outBuf);
    free(testBuf);
    remove_all(txt);

    int ret = std::remove(testOutput);
    ASSERT_EQ(ret, 0);
    free(testOutput);
}



TEST(insertTestNegative, emptyText) {
    text txt = create_text();

    char *debug = (char *)malloc(sizeof(char)*64);
    char *string = (char *)malloc(sizeof(char)*32);
    sprintf(debug, "file.log");
    sprintf(string, "This is a test line.");

    int newSTDerr = open(debug, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    ASSERT_NE(newSTDerr, -1);
    int oldSTDerr = dup(STDERR_FILENO);
    close(STDERR_FILENO);
    dup2(newSTDerr, STDERR_FILENO);

    insert(txt, string);
    free(string);

    fflush(stderr);
    close(newSTDerr);
    dup2(oldSTDerr, STDERR_FILENO);

    int testFD = open(debug, O_RDONLY);
    char *outBuf = (char *)malloc(sizeof(char)*128);
    char *testBuf = (char *)malloc(sizeof(char)*128);
    int testCount;

    testCount = read(testFD, testBuf, 128);
    sprintf(outBuf, "There is no text to work with!\n");
    ASSERT_TRUE(testCount > 0);
    close(testFD);

    for(int i = 0; i < testCount; i++)
        ASSERT_EQ(outBuf[i], testBuf[i]);

    free(outBuf);
    free(testBuf);
    remove_all(txt);

    int ret = std::remove(debug);
    ASSERT_EQ(ret, 0);
    free(debug);
}



TEST(insertTestNegative, longString) {
    text txt = create_text();
    append_line(txt, "This is a test line.");

    char *debug = (char *)malloc(sizeof(char)*64);
    char *string = (char *)malloc(sizeof(char)*256);
    sprintf(debug, "file.log");
    for(int i = 0; i < 255; i++)
        string[i] = 'a';
    string[255] = '\0';

    int newSTDout = open(debug, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    ASSERT_NE(newSTDout, -1);
    int oldSTDout = dup(STDOUT_FILENO);
    close(STDOUT_FILENO);
    dup2(newSTDout, STDOUT_FILENO);

    insert(txt, string);

    fflush(stdout);
    close(newSTDout);
    dup2(oldSTDout, STDOUT_FILENO);

    int testFD = open(debug, O_RDONLY);
    char *outBuf = (char *)malloc(sizeof(char)*512);
    char *testBuf = (char *)malloc(sizeof(char)*512);
    int testCount;

    testCount = read(testFD, testBuf, 512);
    sprintf(outBuf, "The insert line is too long!\n");
    ASSERT_TRUE(testCount > 0);
    close(testFD);

    for(int i = 0; i < testCount; i++)
        ASSERT_EQ(outBuf[i], testBuf[i]);

    free(outBuf);
    free(testBuf);
    remove_all(txt);

    int ret = std::remove(debug);
    ASSERT_EQ(ret, 0);
    free(debug);
}



TEST(insertTestNegative, noString) {
    char cmdline[MAXLINE + 1];
    char *cmd;

    text txt = create_text();
    append_line(txt, "This is a test line.");

    char *command = (char *)malloc(sizeof(char)*1024);
    sprintf(command, "%s/command_two.txt", INPUTDIRINS);
    char *debug = (char *)malloc(sizeof(char)*64);
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

    int newSTDerr = open(debug, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    ASSERT_NE(newSTDerr, -1);
    int oldSTDerr = dup(STDERR_FILENO);
    close(STDERR_FILENO);
    dup2(newSTDerr, STDERR_FILENO);

    // Original lines from editor.cpp
    cmd = strtok(cmdline, " \n");
    if (strcmp(cmd, "i") == 0) {
        char *addstr = cmd + strlen(cmd) + 1;
        if (addstr[strlen(addstr) - 1] == '\n')
            addstr[strlen(addstr) - 1] = '\0';
        if (*addstr == '\0') {
            EXPECT_TRUE(true);
            fprintf(stderr, "Usage: i textstring\n");
        } else {
            EXPECT_TRUE(false);
            insert(txt, addstr);
        }
    } else
        EXPECT_TRUE(false);

    fflush(stderr);
    close(newSTDerr);
    dup2(oldSTDerr, STDERR_FILENO);

    int testFD = open(debug, O_RDONLY);
    char *outBuf = (char *)malloc(sizeof(char)*64);
    char *testBuf = (char *)malloc(sizeof(char)*64);
    int testCount;

    testCount = read(testFD, testBuf, 64);
    sprintf(outBuf, "Usage: i textstring\n");
    ASSERT_TRUE(testCount > 0);
    close(testFD);

    for(int i = 0; i < testCount; i++)
        ASSERT_EQ(outBuf[i], testBuf[i]);

    free(outBuf);
    free(testBuf);
    remove_all(txt);

    int ret = std::remove(debug);
    ASSERT_EQ(ret, 0);
    free(debug);
}

#endif // INSERT_TEST_H
