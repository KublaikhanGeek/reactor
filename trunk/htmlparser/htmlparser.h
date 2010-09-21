#ifndef _XIAO5GEPROJECT_HTML_PARSER_H_
#define _XIAO5GEPROJECT_HTML_PARSER_H_

/**
 * @file    htmlparser
 * @brief   a SAX html parser framework
 * @author  Wu Zesheng <wuzesheng@wuzesheng.com>
 * @since   2010-09-21
 * @version 1.0.0
 */

#include <map>
#include <string>

//EventHandler: the event handler base class
//In this context, event means the parser meeting a html tag, and
//event handler is used to process the specified html tag
class EventHandler
{
public:

	explicit EventHandler(const char * tag) :
		m_tag(tag)
	{}

	virtual bool OnEvent(
			const char * begin_pos, //the page's begin pointer
			size_t len,             //the page's length
			const char *& cur_pos   //the current processed position
			) = 0;

	const char * GetTag() const
	{
		return m_tag.c_str();
	}

private:

	//html tag, for example: <script, </html
	std::string m_tag;
};

class HtmlParser
{
public:

	HtmlParser(const char * begin_pos = NULL, size_t len = 0) :
		m_begin_pos(begin_pos),
		m_len(len)
	{
	}

	//set the page for the parser to parse
	void SetPage(const char * begin_pos, size_t len)
	{
		m_begin_pos = begin_pos;
		m_len = len;
	}

private:

	typedef std::map<std::string, EventHandler *> HandlerTable;

public:

	//register an event handler to the parser
	void RegisterHandler(EventHandler * handler)
	{
		m_handler_table[handler->GetTag()] = handler;
	}

	//unregister an event handler from the parser
	void UnregisterHandler(EventHandler * handler)
	{
		m_handler_table.erase(handler->GetTag());
	}

	//parse the page
	void Parse()
	{
		const char * cur_pos = m_begin_pos;
		HandlerTable::iterator iter;

		//traverse the page
		while (cur_pos < m_begin_pos + m_len)
		{
			if (*cur_pos != '<')
			{
				++ cur_pos;
				continue;
			}

			const char * pos = strchr(cur_pos, ' ');
			if (NULL != pos)
			{
				//meets a tag
				std::string tag(cur_pos, pos - cur_pos);
				AllToLower(tag);
                iter = m_handler_table.find(tag);
				if (iter != m_handler_table.end())
				{
					if (iter->second->OnEvent(m_begin_pos, 
								m_len, cur_pos))
					{
						continue;
					}
				}
			}

			++ cur_pos;
		}
	}

private:

    void AllToLower(std::string & str)
    {
        size_t size = str.size();
        for (size_t i = 0; i < size; ++ i)
        {
            if (str[i] >= 'A' && str[i] <= 'Z')
            {
                str[i] += 'a' - 'A';
            }
        }
    }

private:

	//the event handler talbe
	HandlerTable m_handler_table;

	//the page's begin pointer
	const char * m_begin_pos;

	//the page's length
	size_t m_len;
};

#endif //_XIAO5GEPROJECT_HTML_PARSER_H_
