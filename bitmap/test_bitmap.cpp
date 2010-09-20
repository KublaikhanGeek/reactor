#include <ztest/ztest.h>
#include "bitmap.h"

Z_BEGIN_TEST_CLASS(Bitmap)
    Z_DECLARE_TEST_CASE(set)
    Z_DECLARE_TEST_CASE(clear)
    Z_DECLARE_TEST_CASE(size)
Z_END_TEST_CLASS()

Z_DEFINE_TEST_OBJECT(Bitmap, tester);

Z_DEFINE_TEST_CASE(Bitmap, tester, set)
{
    Bitmap bitmap;

    bitmap.Set(1000);
    Z_EXPECT_TRUE(bitmap.Test(1000));
}

Z_DEFINE_TEST_CASE(Bitmap, tester, clear)
{
    Bitmap bitmap;

    bitmap.Set(100);
    Z_EXPECT_TRUE(bitmap.Test(100));
    bitmap.Clear(100);
    Z_EXPECT_FALSE(bitmap.Test(100));
}

Z_DEFINE_TEST_CASE(Bitmap, tester, size)
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
