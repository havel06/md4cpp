#include "md4cpp.hpp"
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
	void on_enter_block(MD_BLOCKTYPE type) override
	{
		m_result += "<block>";
	}

	void on_leave_block(MD_BLOCKTYPE type) override
	{
		m_result += "</block>";
	}

	void on_enter_span(MD_SPANTYPE type) override
	{
		if (type == MD_SPAN_EM)
		{
			m_result += "<em>";
		}
		else
		{
			m_result += "<span>";
		}
	}

	void on_leave_span(MD_SPANTYPE type) override
	{
		if (type == MD_SPAN_EM)
		{
			m_result += "</em>";
		}
		else
		{
			m_result += "</span>";
		}
	}

	void on_text(const std::string& text, MD_TEXTTYPE type) override
	{
		m_result += text;
	}
};

int main()
{
	my_parser parser;
	parser.parse("fjfj *fdfdf* **lol** fdf");

	std::cout << parser.get_result() << std::endl;

	return 0;
}
