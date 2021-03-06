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

    char *inputTest = (char *)malloc(sizeof(char)*2048);
    snprintf(inputTest, 2048, "%s/input.txt", INPUTDIRSAVE);

    load(txt, inputTest);
    free(inputTest);

    append_line(txt, "This is a test line.");

    char *testOutput = (char *)malloc(sizeof(char)*2048);
    char *originalOutput = (char *)malloc(sizeof(char)*2048);
    snprintf(originalOutput, 2048, "%s/output.txt", INPUTDIRSAVE);
    snprintf(testOutput, 2048, "%s/outputTest.txt", INPUTDIRSAVE);

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



TEST(saveTestPositive, terminal) {
    char cmdline[MAXLINE + 1];
    char *cmd;
    char *arg;

    text txt = create_text();

    char *inputTest = (char *)malloc(sizeof(char)*2048);
    snprintf(inputTest, 2048, "%s/input.txt", INPUTDIRSAVE);

    load(txt, inputTest);
    free(inputTest);

    append_line(txt, "This is a test line.");

    char *command = (char *)malloc(sizeof(char)*2048);
    snprintf(command, 2048, "%s/command_one.txt", INPUTDIRSAVE);

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
    if (strcmp(cmd, "save") == 0) {
        if ((arg = strtok(NULL, " \n")) == NULL) {
            fprintf(stderr, "Usage: save filename\n");
        } else {
            EXPECT_TRUE(true);
            save(txt, arg);
        }
    } else
        EXPECT_TRUE(false);

    char *testOutput = (char *)malloc(sizeof(char)*64);
    char *originalOutput = (char *)malloc(sizeof(char)*2048);
    snprintf(originalOutput, 2048, "%s/output.txt", INPUTDIRSAVE);
    snprintf(testOutput, 64, "outputTest.txt");

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



TEST(saveTestNegative, emptyText) {
    text txt = create_text();

    char *debug = (char *)malloc(sizeof(char)*64);
    char *output = (char *)malloc(sizeof(char)*1024);
    snprintf(debug, 64, "file.log");
    snprintf(output, 1024, "%s/output2.txt", INPUTDIRSAVE);

    int newSTDerr = open(debug, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    ASSERT_NE(newSTDerr, -1);
    int oldSTDerr = dup(STDERR_FILENO);
    close(STDERR_FILENO);
    dup2(newSTDerr, STDERR_FILENO);

    save(txt, output);

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
    ret = std::remove(output);
    ASSERT_EQ(ret, 0);

    free(debug);
    free(output);
}



TEST(saveTestNegative, securedFile) {
    text txt = create_text();

    char *debug = (char *)malloc(sizeof(char)*64);
    char *output = (char *)malloc(sizeof(char)*1024);
    snprintf(debug, 64, "file.log");
    snprintf(output, 1024, "%s/outputSecured.txt", INPUTDIRSAVE);

    // Creating a temporary file
    int locked = creat(output, 0000);
    ASSERT_NE(locked, -1);
    close(locked);

    int newSTDout = open(debug, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    ASSERT_NE(newSTDout, -1);
    int oldSTDout = dup(STDOUT_FILENO);
    close(STDOUT_FILENO);
    dup2(newSTDout, STDOUT_FILENO);

    save(txt, output);

    fflush(stdout);
    close(newSTDout);
    dup2(oldSTDout, STDOUT_FILENO);

    int testFD = open(debug, O_RDONLY);
    char *outBuf = (char *)malloc(sizeof(char)*1024);
    char *testBuf = (char *)malloc(sizeof(char)*1024);
    int testCount;

    testCount = read(testFD, testBuf, 1024);
    snprintf(outBuf, 1024, "The file %s cannot be opened\n", output);
    ASSERT_TRUE(testCount > 0);
    close(testFD);

    for(int i = 0; i < testCount; i++)
        ASSERT_EQ(outBuf[i], testBuf[i]);

    free(outBuf);
    free(testBuf);
    remove_all(txt);

    chmod(output, 0777);

    int ret = std::remove(debug);
    ASSERT_EQ(ret, 0);
    ret = std::remove(output);
    ASSERT_EQ(ret, 0);

    free(debug);
    free(output);
}



TEST(saveTestNegative, noFilename) {
    char cmdline[MAXLINE + 1];
    char *cmd;
    char *arg;

    text txt = create_text();
    append_line(txt, "This is a test line.");

    char *command = (char *)malloc(sizeof(char)*2048);
    snprintf(command, 2048, "%s/command_two.txt", INPUTDIRSAVE);
    char *debug = (char *)malloc(sizeof(char)*64);
    snprintf(debug, 64, "file.log");

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
    if (strcmp(cmd, "save") == 0) {
        if ((arg = strtok(NULL, " \n")) == NULL) {
            EXPECT_TRUE(true);
            fprintf(stderr, "Usage: save filename\n");
        } else {
            EXPECT_TRUE(false);
            save(txt, arg);
        }
    }

    fflush(stderr);
    close(newSTDerr);
    dup2(oldSTDerr, STDERR_FILENO);

    int testFD = open(debug, O_RDONLY);
    char *outBuf = (char *)malloc(sizeof(char)*64);
    char *testBuf = (char *)malloc(sizeof(char)*64);
    int testCount;

    testCount = read(testFD, testBuf, 64);
    snprintf(outBuf, 64, "Usage: save filename\n");
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

#endif // SAVE_TEST_H
