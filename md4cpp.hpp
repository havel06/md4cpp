#pragma once

#include <string>
#include <md4c.h>
#include <variant>

namespace md4cpp
{
	using detail_variant = std::variant<std::monostate,
										MD_BLOCK_UL_DETAIL,
										MD_BLOCK_OL_DETAIL,
										MD_BLOCK_LI_DETAIL,
										MD_BLOCK_H_DETAIL,
										MD_BLOCK_CODE_DETAIL,
										MD_BLOCK_TABLE_DETAIL,
										MD_BLOCK_TD_DETAIL,
										MD_SPAN_A_DETAIL,
										MD_SPAN_IMG_DETAIL,
										MD_SPAN_WIKILINK_DETAIL>;

	class parser
	{
	public:
		parser(unsigned int flags = MD_DIALECT_COMMONMARK);
		virtual ~parser() = default;
		void parse(const std::string& source);
	protected:
		virtual void on_enter_block(MD_BLOCKTYPE type, detail_variant detail) {}
		virtual void on_leave_block(MD_BLOCKTYPE type, detail_variant detail) {}
		virtual void on_enter_span(MD_SPANTYPE type, detail_variant detail) {}
		virtual void on_leave_span(MD_SPANTYPE type, detail_variant detail) {}
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

	detail_variant cast_detail_block(void* detail, MD_BLOCKTYPE type) //NOLINT
	{
		switch (type)
		{
		case MD_BLOCK_UL:
			return *static_cast<MD_BLOCK_UL_DETAIL*>(detail);
		case MD_BLOCK_OL:
			return *static_cast<MD_BLOCK_OL_DETAIL*>(detail);
		case MD_BLOCK_LI:
			return *static_cast<MD_BLOCK_LI_DETAIL*>(detail);
		case MD_BLOCK_H:
			return *static_cast<MD_BLOCK_H_DETAIL*>(detail);
		case MD_BLOCK_CODE:
			return *static_cast<MD_BLOCK_CODE_DETAIL*>(detail);
		case MD_BLOCK_TABLE:
			return *static_cast<MD_BLOCK_TABLE_DETAIL*>(detail);
		case MD_BLOCK_TD:
			return *static_cast<MD_BLOCK_TD_DETAIL*>(detail);
		default:
			return {};
		}
	}
	
	detail_variant cast_detail_span(void* detail, MD_SPANTYPE type) //NOLINT
	{
		switch (type)
		{
		case MD_SPAN_A:
			return *static_cast<MD_SPAN_A_DETAIL*>(detail);
		case MD_SPAN_IMG:
			return *static_cast<MD_SPAN_IMG_DETAIL*>(detail);
		case MD_SPAN_WIKILINK:
			return *static_cast<MD_SPAN_WIKILINK_DETAIL*>(detail);
		default:
			return {};
		}
	}

	int enter_block(MD_BLOCKTYPE type, void* detail, void* object) //NOLINT
	{

		static_cast<parser*>(object)->on_enter_block(type, cast_detail_block(detail, type));
		return 0;
	}

	int leave_block(MD_BLOCKTYPE type, void* detail, void* object) //NOLINT
	{
		static_cast<parser*>(object)->on_leave_block(type, cast_detail_block(detail, type));
		return 0;
	}

	int enter_span(MD_SPANTYPE type, void* detail, void* object) //NOLINT
	{
		static_cast<parser*>(object)->on_enter_span(type, cast_detail_span(detail, type));
		return 0;
	}
	
	int leave_span(MD_SPANTYPE type, void* detail, void* object) //NOLINT
	{
		static_cast<parser*>(object)->on_leave_span(type, cast_detail_span(detail, type));
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
