#include <assert.h>

#include <set>

#include <ztest/ztest.h>
#include "htmlparser.h"

#pragma warning(disable: 4996)

Z_BEGIN_TEST_CLASS(HtmlParser)
    Z_DECLARE_TEST_CASE(GetTitle)
Z_END_TEST_CLASS()

Z_DEFINE_TEST_OBJECT(HtmlParser, tester);

//commnent event handler, used to skip the comment
class CommentHandler : public EventHandler
{
public:

    explicit CommentHandler(const char * tag) :
        EventHandler(tag)
    {}

    bool OnEvent(const char * begin_pos, size_t len, 
        const char *& cur_pos)
    {
        const char * pos = cur_pos;
        while (pos < begin_pos + len)
        {
            if (pos + 2 < begin_pos + len &&
                    pos[0] == '-' && pos[1] == '-' &&
                    pos[2] == '>')
            {
                cur_pos = pos + 3;
                return true;
            }
            ++ pos;
        }
        return false;
    }
};

//title evnent handler, used to process the <title tag
class TitleHandler : public EventHandler
{
public:

    explicit TitleHandler(const char * tag) :
        EventHandler(tag)
    {}

    bool OnEvent(const char * begin_pos, size_t len,
        const char *& cur_pos)
    {
        const char * pos = cur_pos + 1;
        const char * begin = NULL;
        const char * end = NULL;
        while (pos < begin_pos + len)
        {
            if ('>' == *pos)
            {
                begin = pos + 1;
            }
            else if ('<' == *pos)
            {
                end = pos;
                break;
            }
            ++ pos;
        }

        if (NULL != begin && NULL != end)
        {
            m_title = std::string(begin, end - begin);
            cur_pos = pos + 1;
            return true;
        }
        return false;
    }

    const std::string & GetTitle() const
    {
        return m_title;
    }

private:

    std::string m_title;
};

size_t ReadInput(const char * input, char * buffer, size_t buf_len)
{
    FILE * fp = fopen(input, "r");
    memset(buffer, 0, buf_len);
    size_t len = 0;
    size_t num = 0;
    while ((num = fread(buffer + len, 1, 1024, fp)) > 0)
    {
        assert(len + num < buf_len);
        len += num;
    }
    fclose(fp);

    return len;
}

Z_DEFINE_TEST_CASE(HtmlParser, tester, GetTitle)
{
    static char buffer[512<<20];
    size_t len = ReadInput("input.html", buffer, 512<<10);

    HtmlParser parser(buffer, len);
    parser.RegisterHandler(new CommentHandler("<!--"));
    TitleHandler * title_handler = new TitleHandler("<title");
    parser.RegisterHandler(title_handler);

    parser.Parse();

    Z_EXPECT_EQ(title_handler->GetTitle(), 
        std::string("cplusplus.com - The C++ Resources Network"));
}

int main()
{
    Z_RUN_ALL_TESTCASES(tester);
}
