#include <ztest/ztest.h>
#include "bitmap.h"

Z_BEGIN_TEST_CLASS(Tester)
    Z_DECLARE_TEST_CASE(bitmap, set)
    Z_DECLARE_TEST_CASE(bitmap, clear)
    Z_DECLARE_TEST_CASE(bitmap, size)
Z_END_TEST_CLASS()

Z_DEFINE_TEST_OBJECT(Tester, tester);

Z_DEFINE_TEST_CASE(Tester, tester, bitmap, set)
{
    Bitmap bitmap;

    bitmap.Set(1000);
    Z_EXPECT_TRUE(bitmap.Test(1000));
}

Z_DEFINE_TEST_CASE(Tester, tester, bitmap, clear)
{
    Bitmap bitmap;

    bitmap.Set(100);
    Z_EXPECT_TRUE(bitmap.Test(100));
    bitmap.Clear(100);
    Z_EXPECT_FALSE(bitmap.Test(100));
}

Z_DEFINE_TEST_CASE(Tester, tester, bitmap, size)
{
    Bitmap bitmap(4096);
    Z_EXPECT_EQ(bitmap.Size(), 4096);

    Bitmap bitmap2(512);
    Z_EXPECT_NE(bitmap.Size(), 1024);
}

int main()
{
    Z_RUN_ALL_TESTCASES(tester);
}
