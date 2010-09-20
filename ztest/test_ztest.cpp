#define Z_NEED_ACCESS_PRIVATE_MEMBER
#include "ztest.h"

//if you want to use ztest, just follow 4 steps described below,
//have fun with it :)

//step 1: Declare the test class and the test cases
Z_BEGIN_TEST_CLASS(Calculator)
    Z_DECLARE_TEST_CASE(Add)
    Z_DECLARE_TEST_CASE(Sub)
Z_END_TEST_CLASS()

//step 2: define a object of the type of the test class declared int step 1
Z_DEFINE_TEST_OBJECT(Calculator, tester)

//step 3: define the test case
class Calculator
{
public:
    int Add(int a, int b)
    {
        return a + b;
    }

private:

    int Sub(int a, int b)
    {
        return a - b;
    }

    Z_DECLARE_FRIEND(Calculator);
};

Z_DEFINE_TEST_CASE(Calculator, tester, Add)
{
    Calculator cal;
    Z_EXPECT_TRUE(cal.Add(3, -4) == -1);
}

Z_DEFINE_TEST_CASE(Calculator, tester, Sub)
{
    Calculator cal;
    Z_EXPECT_TRUE(cal.Sub(10, 20) == 10);
}

int main()
{
    //step 4: run all the test cases
    Z_RUN_ALL_TESTCASES(tester);
}
