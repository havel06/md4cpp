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
	void on_enter_block(MD_BLOCKTYPE type, md4cpp::detail_variant detail) override
	{
		if (type == MD_BLOCK_H)
		{
			m_result += std::to_string(std::get<MD_BLOCK_H_DETAIL>(detail).level) + " ";
		}
		else
		{
			m_result += "<block>";
		}
	}

	void on_leave_block(MD_BLOCKTYPE type, md4cpp::detail_variant detail) override
	{
		m_result += "</block>";
	}

	void on_enter_span(MD_SPANTYPE type, md4cpp::detail_variant detail) override
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

	void on_leave_span(MD_SPANTYPE type, md4cpp::detail_variant detail) override
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
	parser.parse("# jdkjfkdjf \n \n ## fjkdjflsjdfls \n \n # jfkdflsjf \n \n fjfj *fdfdf* **lol** fdf");

	std::cout << parser.get_result() << std::endl;

	return 0;
}
