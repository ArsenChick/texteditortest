#ifndef SAVE_TEST_H
#define SAVE_TEST_H

#include <gtest/gtest.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdio>

extern "C++" {
#include "common.h"
#include "text/text.h"
}



TEST(saveTestPositive, functional) {
    text txt = create_text();

    char *inputTest = (char *)malloc(sizeof(char)*1024);
    sprintf(inputTest, "%s/input.txt", INPUTDIRSAVE);

    load(txt, inputTest);
    free(inputTest);

    append_line(txt, "This is a test line.");

    char *testOutput = (char *)malloc(sizeof(char)*1024);
    char *originalOutput = (char *)malloc(sizeof(char)*1024);
    sprintf(originalOutput, "%s/output.txt", INPUTDIRSAVE);
    sprintf(testOutput, "%s/outputTest.txt", INPUTDIRSAVE);

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



TEST(saveTestPositive, terminal) {
    char cmdline[MAXLINE + 1];
    char *cmd;
    char *arg;

    text txt = create_text();

    char *inputTest = (char *)malloc(sizeof(char)*1024);
    sprintf(inputTest, "%s/input.txt", INPUTDIRSAVE);

    load(txt, inputTest);
    free(inputTest);

    append_line(txt, "This is a test line.");

    char *command = (char *)malloc(sizeof(char)*1024);
    sprintf(command, "%s/command_one.txt", INPUTDIRSAVE);

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
    if (strcmp(cmd, "save") == 0) {
        if ((arg = strtok(NULL, " \n")) == NULL) {
            fprintf(stderr, "Usage: save filename\n");
        } else {
            ASSERT_TRUE(true);
            save(txt, arg);
        }
    }

    char *testOutput = (char *)malloc(sizeof(char)*64);
    char *originalOutput = (char *)malloc(sizeof(char)*1024);
    sprintf(originalOutput, "%s/output.txt", INPUTDIRSAVE);
    sprintf(testOutput, "outputTest.txt");

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



TEST(saveTestNegative, emptytext) {
    text txt = create_text();

    char *debuglog = (char *)malloc(sizeof(char)*64);
    char *output = (char *)malloc(sizeof(char)*1024);
    sprintf(debuglog, "debuglog.txt");
    sprintf(output, "%s/output2.txt", INPUTDIRSAVE);

    int newSTDerr = open(debuglog, O_WRONLY | O_CREAT | O_TRUNC, S_IWRITE | S_IREAD);
    ASSERT_NE(newSTDerr, -1);
    int oldSTDerr = dup(STDOUT_FILENO);
    close(STDOUT_FILENO);
    int success = dup2(newSTDerr, STDOUT_FILENO);
    ASSERT_TRUE(success >= 0);

    printf("Hello!\n");
    save(txt, output);

    close(newSTDerr);
    dup2(oldSTDerr, STDOUT_FILENO);

    int testFD = open(debuglog, O_RDONLY);
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

    int ret = std::remove(debuglog);
    ASSERT_EQ(ret, 0);
    ret = std::remove(output);
    ASSERT_EQ(ret, 0);

    free(debuglog);
    free(output);
}

#endif // SAVE_TEST_H
