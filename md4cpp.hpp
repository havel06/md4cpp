#pragma once

#include <string>
#include <md4c.h>

namespace md4cpp
{
	class parser
	{
	public:
		parser(unsigned int flags = MD_DIALECT_COMMONMARK);
		void parse(const std::string& source);
	protected:
		virtual void on_enter_block(MD_BLOCKTYPE type) {}
		virtual void on_leave_block(MD_BLOCKTYPE type) {}
		virtual void on_enter_span(MD_SPANTYPE type) {}
		virtual void on_leave_span(MD_SPANTYPE type) {}
		virtual void on_text(const std::string& text, MD_TEXTTYPE type) {}
	private:
		unsigned int m_flags;
		friend int enter_block(MD_BLOCKTYPE type, void* detail, void* object);
		friend int leave_block(MD_BLOCKTYPE type, void* detail, void* object);
		friend int enter_span(MD_SPANTYPE type, void* detail, void* object);
		friend int leave_span(MD_SPANTYPE type, void* detail, void* object);
		friend int text(MD_TEXTTYPE type, const MD_CHAR* text, MD_SIZE size, void* object);
	};

	parser::parser(unsigned int t_flags) //NOLINT
	{
		this->m_flags = t_flags;
	}

	int enter_block(MD_BLOCKTYPE type, void* detail, void* object) //NOLINT
	{
		static_cast<parser*>(object)->on_enter_block(type);
		return 0;
	}

	int leave_block(MD_BLOCKTYPE type, void* detail, void* object) //NOLINT
	{
		static_cast<parser*>(object)->on_leave_block(type);
		return 0;
	}

	int enter_span(MD_SPANTYPE type, void* detail, void* object) //NOLINT
	{
		static_cast<parser*>(object)->on_enter_span(type);
		return 0;
	}
	
	int leave_span(MD_SPANTYPE type, void* detail, void* object) //NOLINT
	{
		static_cast<parser*>(object)->on_leave_span(type);
		return 0;
	}

	int text(MD_TEXTTYPE type, const MD_CHAR* text, MD_SIZE size, void* object) //NOLINT
	{
		static_cast<parser*>(object)->on_text(std::string(text, size), type);
		return 0;
	}

	void parser::parse(const std::string& source) //NOLINT
	{
		MD_PARSER parser_data {
			0,
			this->m_flags,
			enter_block,
			leave_block,
			enter_span,
			leave_span,
			text,
			nullptr,
			nullptr
		};

		md_parse(source.c_str(), source.size(), &parser_data, this);
	}
} //namespace md4cpp
