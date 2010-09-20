#include <ztest/ztest.h>
#include "bitmap.h"

Z_BEGIN_TEST_CLASS(TestBitmap)
    Z_DECLARE_TEST_CASE(Set, IsSet)
    Z_DECLARE_TEST_CASE(Bitmap, Size)
Z_END_TEST_CLASS()

Z_DEFINE_TEST_OBJECT(TestBitmap, test);

Z_DEFINE_TEST_CASE(TestBitmap, test, Set, IsSet)
{
    Bitmap bitmap;

    bitmap.Set(1023);
    Z_EXPECT_FALSE(bitmap.IsSet(1021));
    Z_EXPECT_TRUE(bitmap.IsSet(1023));
}

Z_DEFINE_TEST_CASE(TestBitmap, test, Bitmap, Size)
{
    Bitmap bitmap(512);

    Z_EXPECT_EQ(bitmap.Size(), 512);
}

int main()
{
    Z_RUN_ALL_TESTCASES(test);
}
