#ifndef COPYBEGIN_TEST_H
#define COPYBEGIN_TEST_H

#include <gtest/gtest.h>

#include <fcntl.h>
#include <unistd.h>
#include <cstdio>

extern "C++" {
#include "common.h"
#include "text/text.h"
}



TEST(cbeginTestPositive, functional) {
    text txt = create_text();

    char *inputTest = (char *)malloc(sizeof(char)*1024);
    sprintf(inputTest, "%s/input.txt", INPUTDIRCB);

    load(txt, inputTest);
    free(inputTest);

    copy_begin(txt);

    char *testOutput = (char *)malloc(sizeof(char)*1024);
    char *originalOutput = (char *)malloc(sizeof(char)*1024);
    sprintf(originalOutput, "%s/output.txt", INPUTDIRCB);
    sprintf(testOutput, "%s/outputTest.txt", INPUTDIRCB);

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



TEST(cbeginTestPositive, terminal) {
    char cmdline[MAXLINE + 1];
    char *cmd;

    text txt = create_text();

    char *inputTest = (char *)malloc(sizeof(char)*1024);
    sprintf(inputTest, "%s/input.txt", INPUTDIRCB);

    load(txt, inputTest);
    free(inputTest);

    char *command = (char *)malloc(sizeof(char)*1024);
    sprintf(command, "%s/command_one.txt", INPUTDIRCB);

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
    if (strcmp(cmd, "cb") == 0) {
        copy_begin(txt);
    }

    char *testOutput = (char *)malloc(sizeof(char)*64);
    char *originalOutput = (char *)malloc(sizeof(char)*1024);
    sprintf(originalOutput, "%s/output.txt", INPUTDIRCB);
    sprintf(testOutput, "%s/outputTest.txt", INPUTDIRCB);

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

#endif // COPYBEGIN_TEST_H
