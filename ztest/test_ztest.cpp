#define Z_NEED_ACCESS_PRIVATE_MEMBER
#include "ztest.h"

//if you want to use ztest, just follow 4 steps described below,
//have fun with it :)

//step 1: Declare the test class and the test cases
Z_BEGIN_TEST_CLASS(Test)
	Z_DECLARE_TEST_CASE(Global, Add)
	Z_DECLARE_TEST_CASE(Class, Add)
	Z_DECLARE_TEST_CASE(Class, Sub)
Z_END_TEST_CLASS()

//step 2: define a object of the type of the test class declared int step 1
Z_DEFINE_TEST_OBJECT(Test, tester)

//step 3: define the test case
Z_DEFINE_TEST_CASE(Test, tester, Global, Add)
{
	Z_EXPECT_EQ(1 + 3, 4);
}

class Test
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

	Z_DECLARE_FRIEND(Test);
};

Z_DEFINE_TEST_CASE(Test, tester, Class, Add)
{
	Test test;
	Z_EXPECT_TRUE(test.Add(3, 4) == 8);
}

Z_DEFINE_TEST_CASE(Test, tester, Class, Sub)
{
	Test test;
	Z_EXPECT_TRUE(test.Sub(10, 20) == -10);
}

int main()
{
	//step 4: run all the test cases
	Z_RUN_ALL_TESTCASES(tester);
}
