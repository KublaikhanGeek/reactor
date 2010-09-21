#include <ztest/ztest.h>
#include "htmlparser.h"

Z_BEGIN_TEST_CLASS(HtmlParser)
    Z_DECLARE_TEST_CASE(GetAHref)
Z_END_TEST_CLASS()

Z_DEFINE_TEST_OBJECT(HtmlParser, tester);

Z_DEFINE_TEST_CASE(HtmlParser, tester, GetAHref)
{

}

int main()
{
    Z_RUN_ALL_TESTCASES(tester);
}
