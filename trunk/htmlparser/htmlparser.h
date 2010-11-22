#ifndef _XIAO5GEPROJECT_HTML_PARSER_H_
#define _XIAO5GEPROJECT_HTML_PARSER_H_

/// @file    htmlparser.h
/// @brief   a SAX html parser framework
/// @author  Wu Zesheng <wuzesheng@wuzesheng.com>
/// @since   2010-09-21
/// @version 1.0.0

#include <map>
#include <string>

/// @brief the event handler base class
/// In this context, event means the parser meeting a html tag, and
/// event handler is used to process the specified html tag
class EventHandler
{
public:

    explicit EventHandler(const char * tag, bool flag = true) :
        m_tag(tag),
        m_delete(flag)
    {}

	/// @brief the event callback
	/// @note  if the specified event is processed successfully, the cur_pos
	///        should points to the position just after the processed tag
	///        e.g: 
	///            "<head>....</head><body>..."
	///            before OnEvent: cur_pos -> "<head.."
	///            after OnEvent: cur_pos -> "<body.."
    virtual bool OnEvent(
            const char * begin_pos, ///< the page's begin pointer
            size_t len,             ///< the page's length
            const char *& cur_pos   ///< the current processed position
            ) = 0;

	/// @brief get the tag of this handler
    const char * GetTag() const
    {
        return m_tag.c_str();
    }

	/// @brief determine whether the tag is need to be deleted
    bool IsNeedDelete() const
    {
        return m_delete;
    }

private:

    /// html tag, for example: <script, </html
    std::string m_tag;

    /// the flag of whether the handler is need to be deleted
    bool m_delete;
};

#ifndef STRNCASECMP
#ifdef _WIN32
#define STRNCASECMP _strnicmp
#else
#define STRNCASECMP strncasecmp
#endif
#endif

/// @brief  the parser class
class HtmlParser
{
private:

    typedef std::map<std::string, EventHandler *> HandlerTable;

public:

    HtmlParser(const char * begin_pos = NULL, size_t len = 0) :
        m_begin_pos(begin_pos),
        m_len(len)
    {
    }

    ~HtmlParser()
    {
        HandlerTable::iterator it = m_handler_table.begin();
        while (it != m_handler_table.end())
        {
            if (it->second->IsNeedDelete())
            {
                delete it->second;
            }
            ++ it;
        }
    }

    /// @brief set the page for the parser to parse
	/// @param begin_pos [in] the page's begin pointer
	/// @param len [in] the page length
    void SetPage(const char * begin_pos, size_t len)
    {
        m_begin_pos = begin_pos;
        m_len = len;
    }

public:

    /// @brief register an event handler to the parser
	/// @param handler [in] the handler to be registered
    void RegisterHandler(EventHandler * handler)
    {
        m_handler_table[handler->GetTag()] = handler;
    }

    /// @brief unregister an event handler from the parser
	/// @param handler [in] the handler to be unregistered
    void UnregisterHandler(EventHandler * handler)
    {
        HandlerTable::iterator it = m_handler_table.find(handler->GetTag());
        if (it != m_handler_table.end())
        {
            delete it->second;
            m_handler_table.erase(it);
        }
    }

    /// @brief parse the page
	/// the user should call this function after register 
	/// your EventHandlers to drive the paser
    void Parse()
    {
        const char * cur_pos = m_begin_pos;
        HandlerTable::iterator iter;

        /// traverse the page
        while (cur_pos < m_begin_pos + m_len)
        {
            if (*cur_pos != '<') ///< not a tag's begin
            {
                ++ cur_pos;
                continue;
            }

            iter = m_handler_table.begin();
            bool ret = false;
            while (iter != m_handler_table.end())
            {
                if (!STRNCASECMP(iter->first.c_str(),
                    cur_pos, iter->first.size()))
                {
					/// find the handler for this tag
                    ret = iter->second->OnEvent(
                        m_begin_pos, m_len, cur_pos);
                    break;
                }
                ++ iter;
            }
            if (!ret)
            {
                ++ cur_pos;
            }
        }
    }

private:

    /// the event handler talbe
    HandlerTable m_handler_table;

    /// the page's begin pointer
    const char * m_begin_pos;

    /// the page's length
    size_t m_len;
};

#undef STRNCASECMP
#endif //_XIAO5GEPROJECT_HTML_PARSER_H_

