#include "save_test.h"
#include "copybegin_test.h"
#include "insert_test.h"
#include "slns_test.h"
#include "move_test.h"


#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
