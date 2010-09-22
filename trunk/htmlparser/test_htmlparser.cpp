#include <set>
#include <fstream>
#include <ztest/ztest.h>
#include "htmlparser.h"

#pragma warning(disable: 4996)

Z_BEGIN_TEST_CLASS(HtmlParser)
    Z_DECLARE_TEST_CASE(GetAHref1)
    Z_DECLARE_TEST_CASE(GetAHref2)
    Z_DECLARE_TEST_CASE(GetAHref3)
Z_END_TEST_CLASS()

Z_DEFINE_TEST_OBJECT(HtmlParser, tester);

class ATagHandler : public EventHandler
{
public:

    explicit ATagHandler(const char * tag) :
        EventHandler(tag)
    {}

    bool OnEvent(const char * begin_pos, size_t len, 
        const char *& cur_pos)
    {
        const char * pos = strchr(cur_pos, '>');
        if (pos != NULL)
        {
			std::string url = ParseHrefUrl(cur_pos, pos - cur_pos + 1);
            if (!url.empty() && url.find("http://") != std::string::npos)
			{
				m_urlset.insert(url);
			}
        
            cur_pos = pos + 1;
            return true;
        }
        return false;
    }

private:

    std::string ParseHrefUrl(const char * begin, size_t len)
    {
		std::string content(begin, len);
		std::string::size_type pos = content.find("href=");
		if (pos != std::string::npos)
		{
			std::string::size_type end_pos = std::string::npos;
			if (content[pos + 5] == '"')
			{
				end_pos = content.find('"', pos + 6);
				if (end_pos != std::string::npos)
				{
				    return std::string(content.substr(pos + 6, 
                        end_pos - pos - 6));
			    }
			}
			else
			{
				end_pos = content.find(' ', pos + 5);
				if (end_pos == std::string::npos)
				{
					end_pos = content.find('>', pos + 5);
				}
                if (end_pos != std::string::npos)
				{
				    return std::string(content.substr(pos + 5, 
                        end_pos - pos - 5));
                }
            }
		}
        return std::string("");
    }

public:

    std::set<std::string> m_urlset;
};

bool TestGetAHref(const char * input, const char * res)
{
    static const int s_max_page_len = (512<<10);
    FILE * fp = fopen(input, "r");
    char buffer[s_max_page_len] = {0};
    size_t len = 0;
    size_t num = 0;
    while ((num = fread(buffer + len, 1, 1024, fp)) > 0)
    {
        len += num;
    }
    fclose(fp);

	std::ifstream fin(res);
    std::set<std::string> urlset;
	std::string url;
	while (std::getline(fin, url))
	{
		urlset.insert(url);
	}
	fin.close();

    HtmlParser parser(buffer, len);
    ATagHandler * handler = new ATagHandler("<a");
    parser.RegisterHandler(handler);
    parser.Parse();

    return urlset == handler->m_urlset;
}

Z_DEFINE_TEST_CASE(HtmlParser, tester, GetAHref1)
{
    Z_EXPECT_TRUE(TestGetAHref("input1.html", "res1.txt"));
}

Z_DEFINE_TEST_CASE(HtmlParser, tester, GetAHref2)
{
    Z_EXPECT_TRUE(TestGetAHref("input2.html", "res2.txt"));
}

Z_DEFINE_TEST_CASE(HtmlParser, tester, GetAHref3)
{
    Z_EXPECT_TRUE(TestGetAHref("input3.html", "res3.txt"));
}

int main()
{
    Z_RUN_ALL_TESTCASES(tester);
}
