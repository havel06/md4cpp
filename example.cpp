#include "include/md4cpp.hpp"
#include <iostream>

class my_parser : public md4cpp::parser
{
public:
	const std::string& get_result() const
	{
		return this->m_result;
	}
private:
	std::string m_result{};

	void on_enter_block(MD_BLOCKTYPE type, const md4cpp::detail_variant& detail) override
	{
		switch(type)
		{
			case MD_BLOCK_DOC: break;
			case MD_BLOCK_HTML: break;
			case MD_BLOCK_QUOTE:
				m_result +=  "<blockquote>";
				break;
			case MD_BLOCK_UL:
				m_result += "<ul>";
				break;
			case MD_BLOCK_OL:
				m_result += "<ol start=\"" + std::to_string(std::get<md4cpp::block_ol_detail>(detail).start) + "\">";
				break;
			case MD_BLOCK_LI:
				m_result += "<li>";
				break;
			case MD_BLOCK_HR:
				m_result +=  "<hr>";
				break;
			case MD_BLOCK_H:
				m_result += "<h" + std::to_string(std::get<md4cpp::block_h_detail>(detail).level) + ">";
				break;
			case MD_BLOCK_CODE:
				m_result += "<pre><code>";
				break;
			case MD_BLOCK_P:
				m_result +=  "<p>";
				break;
			default:
				break;
		}
	}

	void on_leave_block(MD_BLOCKTYPE type, const md4cpp::detail_variant& detail) override
	{
		switch(type)
		{
			case MD_BLOCK_DOC: break;
			case MD_BLOCK_HR: break;
			case MD_BLOCK_HTML: break;
			case MD_BLOCK_QUOTE:
				m_result += "</blockquote>";
				break;
			case MD_BLOCK_UL:
				m_result += "</ul>";
				break;
			case MD_BLOCK_OL:
				m_result += "</ol>";
				break;
			case MD_BLOCK_LI:
				m_result += "</li>";
				break;
			case MD_BLOCK_H:
				m_result += "</h" + std::to_string(std::get<md4cpp::block_h_detail>(detail).level) + ">";
			case MD_BLOCK_CODE:
				m_result += "</code></pre>";
				break;
			case MD_BLOCK_P:
				m_result += "</p>";
				break;
			default:
				break;
		}
	}

	void on_enter_span(MD_SPANTYPE type, const md4cpp::detail_variant& detail) override
	{
		switch(type)
		{
			case MD_SPAN_EM:
				m_result += "<em>";
				break;
			case MD_SPAN_STRONG:
				m_result += "<strong>";
				break;
			case MD_SPAN_U:
				m_result += "<u>";
				break;
			case MD_SPAN_A:
				m_result += "<a href=\"";
				for (auto& s : std::get<md4cpp::span_a_detail>(detail).href.get_substrings())
				{
					if (s.type == MD_TEXT_NORMAL)
					{
						m_result += s.text;
					}
					break;
				}
				m_result += "\">";
				break;
			case MD_SPAN_IMG:
				m_result += "<img src=\"";
				for (auto& s : std::get<md4cpp::span_img_detail>(detail).src.get_substrings())
				{
					if (s.type == MD_TEXT_NORMAL)
					{
						m_result += s.text;
					}
					break;
				}
				break;
			case MD_SPAN_CODE:
				m_result += "<code>";
				break;
			case MD_SPAN_DEL:
				m_result += "<del>";
				break;
			default:
				break;
		}
	}

	void on_leave_span(MD_SPANTYPE type, const md4cpp::detail_variant& detail) override
	{
		switch(type)
		{
			case MD_SPAN_EM:
				m_result += "</em>";
				break;
			case MD_SPAN_STRONG:
				m_result += "</strong>";
				break;
			case MD_SPAN_U:
				m_result += "</u>";
				break;
			case MD_SPAN_A:
				m_result += "</a>";
				break;
			case MD_SPAN_CODE:
				m_result += "</code>";
				break;
			case MD_SPAN_DEL:
				m_result += "</del>";
				break;
			case MD_SPAN_IMG:
				m_result += "<img src=\"";
				for (auto& s : std::get<md4cpp::span_img_detail>(detail).title.get_substrings())
				{
					if (s.type == MD_TEXT_NORMAL)
					{
						m_result += s.text;
					}
					break;
				}
				break;
			default:
				break;
		}
	}

	void on_text(std::string_view text, MD_TEXTTYPE type) override
	{
		m_result += text;
	}
};

int main()
{
	my_parser parser;
	std::string source = R"(
# Section 1

This is the **first** *paragraph*.

- list item 
- another list item

## Section 2

This is ~the~ **second** *paragraph*.

1. list item
1. another list item
	1. nested list item
	1. another nested list item
1. final list item

This is the ***third paragraph***.
It even contains a [link!](https://www.youtube.com)

```
int main()
{
	return 0;
}
```
	)";
	parser.parse(source);

	std::cout << parser.get_result() << std::endl;

	return 0;

}
