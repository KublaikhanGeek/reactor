<li><strong>1. ZTest是什么？</strong></li>
ZTest是一个轻量级的c++程序单元测试框架

<li><strong>2. 如何获得ZTest?</strong></li>
可以从google code：<a href='http://xiao5geproject.googlecode.com/svn/trunk/ztest'><a href='http://xiao5geproject.googlecode.com/svn/trunk/ztest'>http://xiao5geproject.googlecode.com/svn/trunk/ztest</a></a> 处获得ZTest的源码。

<li><strong>3. 如何使用ZTest?</strong></li>使用ZTest要遵循以下几个步骤：

（1）包含ztest.h:
<pre lang='cpp'>
#include "ztest.h"<br>
</pre>
（2）声明测试类和测试用例
<pre lang='cpp'>
Z_BEGIN_TEST_CLASS(Test)<br>
Z_DECLARE_TEST_CASE(Global, Add)<br>
Z_DECLARE_TEST_CASE(Class, Add)<br>
Z_END_TEST_CLASS()<br>
</pre>
（3）定义测试对象
<pre lang='cpp'>
Z_DEFINE_TEST_OBJECT(Test, tester)<br>
</pre>
（4）定义测试用例
<pre lang='cpp'>
Z_DEFINE_TEST_CASE(Test, tester, Global, Add)<br>
{<br>
Z_EXPECT_EQ(1 - 3, 2);<br>
}<br>
<br>
class Test<br>
{<br>
public:<br>
int Add(int a, int b)<br>
{<br>
return a + b;<br>
}<br>
};<br>
<br>
Z_DEFINE_TEST_CASE(Test, tester, Class, Add)<br>
{<br>
Test test;<br>
Z_EXPECT_TRUE(test.Add(3, -4) == -1);<br>
}<br>
</pre>
（5）运行测试用例
<pre lang='cpp'>
int main()<br>
{<br>
Z_RUN_ALL_TESTCASES(tester);<br>
}<br>
</pre>
（6）编译运行单元测试程序：
<pre lang='shell'>
Running test case: Global/Add    [Check failed] 1 - 3 == 2<br>
Running test case: Class/Add     [Passed]<br>
Tatal 2 cases, passed 1<br>
</pre>

4. 如果你在用的过程中发现什么bug或者有什么好的改进建议，请到<a href='http://www.wuzesheng.com/?p=1400'><a href='http://www.wuzesheng.com/?p=1400'>http://www.wuzesheng.com/?p=1400</a></a>留言，谢谢！