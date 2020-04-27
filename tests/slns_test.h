#ifndef SLNS_TEST_H
#define SLNS_TEST_H

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

TEST(slnsTestPositive, functional) {
    text txt = create_text();

    char *outputTest = (char *)malloc(sizeof(char)*2048);
    char *inputTest = (char *)malloc(sizeof(char)*2048);
    snprintf(outputTest, 2048, "%s/outputTest.txt", INPUTDIRSLNS);
    snprintf(inputTest, 2048, "%s/input.txt", INPUTDIRSLNS);

    load(txt, inputTest);
    free(inputTest);

    int newSTDout = open(outputTest, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    ASSERT_NE(newSTDout, -1);
    int oldSTDout = dup(STDOUT_FILENO);
    close(STDOUT_FILENO);
    dup2(newSTDout, STDOUT_FILENO);

    showlastnonspace(txt);

    fflush(stdout);
    close(newSTDout);
    dup2(oldSTDout, STDOUT_FILENO);

    char *originalOutput = (char *)malloc(sizeof(char)*2048);
    snprintf(originalOutput, 2048, "%s/output.txt", INPUTDIRSLNS);

    int testFD = open(outputTest, O_RDONLY);
    int originalFD = open(originalOutput, O_RDONLY);
    char *outBuf = (char *)malloc(sizeof(char)*16);
    char *testBuf = (char *)malloc(sizeof(char)*16);
    int testCount, outputCount;
    free(originalOutput);

    testCount = read(testFD, testBuf, 16);
    outputCount = read(originalFD, outBuf, 16);
    close(testFD);
    close(originalFD);

    ASSERT_EQ(testCount, outputCount);
    for(int i = 0; i < testCount; i++)
        ASSERT_EQ(outBuf[i], testBuf[i]);

    free(outBuf);
    free(testBuf);
    remove_all(txt);

    int ret = std::remove(outputTest);
    ASSERT_EQ(ret, 0);
    free(outputTest);
}



TEST(slnsTestPositive, terminal) {
    char cmdline[MAXLINE + 1];
    char *cmd;

    text txt = create_text();

    char *inputTest = (char *)malloc(sizeof(char)*2048);
    char *outputTest = (char *)malloc(sizeof(char)*2048);
    snprintf(inputTest, 2048, "%s/input.txt", INPUTDIRSLNS);
    snprintf(outputTest, 2048, "%s/outputTest.txt", INPUTDIRSLNS);

    load(txt, inputTest);
    free(inputTest);

    char *command = (char *)malloc(sizeof(char)*2048);
    snprintf(command, 2048, "%s/command_one.txt", INPUTDIRSLNS);

    int newSTDin = open(command, O_RDONLY);
    int oldSTDin = dup(STDIN_FILENO);
    dup2(newSTDin, STDIN_FILENO);
    free(command);

    // Original line from editor.cpp
    char* success = fgets(cmdline, MAXLINE, stdin);
    EXPECT_STRNE(success, NULL);

    close(newSTDin);
    dup2(oldSTDin, STDIN_FILENO);

    int newSTDout = open(outputTest, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    ASSERT_NE(newSTDout, -1);
    int oldSTDout = dup(STDOUT_FILENO);
    close(STDOUT_FILENO);
    dup2(newSTDout, STDOUT_FILENO);

    // Original lines from editor.cpp
    cmd = strtok(cmdline, " \n");
    if (strcmp(cmd, "showlastnonspace") == 0) {
        EXPECT_TRUE(true);
        showlastnonspace(txt);
    } else
        EXPECT_TRUE(false);

    fflush(stdout);
    close(newSTDout);
    dup2(oldSTDout, STDOUT_FILENO);

    char *originalOutput = (char *)malloc(sizeof(char)*2048);
    snprintf(originalOutput, 2048, "%s/output.txt", INPUTDIRSLNS);

    int testFD = open(outputTest, O_RDONLY);
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

    int ret = std::remove(outputTest);
    ASSERT_EQ(ret, 0);
    free(outputTest);
}



TEST(slnsTestNegative, emptyText) {
    text txt = create_text();

    char *debug = (char *)malloc(sizeof(char)*64);
    snprintf(debug, 64, "file.log");

    int newSTDerr = open(debug, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    ASSERT_NE(newSTDerr, -1);
    int oldSTDerr = dup(STDERR_FILENO);
    close(STDERR_FILENO);
    dup2(newSTDerr, STDERR_FILENO);

    showlastnonspace(txt);

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
    remove_all(txt);

    int ret = std::remove(debug);
    ASSERT_EQ(ret, 0);
    free(debug);
}

#endif // SLNS_TEST_H
