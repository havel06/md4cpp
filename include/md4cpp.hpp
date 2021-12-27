#pragma once

#include "detail.hpp"

namespace md4cpp
{
	/**
	 * A class that encapsulates the library, and acts as a front end.
	 */
	class parser
	{
	public:
		/**
		 * Constructs the parser.
		 *
		 * @param flags Options for the parser. Please refer to
		 * <a href="https://github.com/mity/md4c/blob/master/src/md4c.h#L300">md4c headers</a>
		 * to view the possible flags.
		 */
		parser(unsigned int flags = MD_DIALECT_COMMONMARK);
		virtual ~parser() = default;
		/**
		 * Begins the parsing process.
		 *
		 * @param source Markdown source text.
		 */
		void parse(const std::string& source);
	protected:
		/**
		 * Gets called when the parser encounters the beginning of a block element.
		 *
		 * @param Type the type of the block element. Please refer to 
		 * <a href="https://github.com/mity/md4c/blob/master/src/md4c.h#L54">md4c headers</a>
		 * to view the possible block types.
		 *
		 * @param Detail an std::variant containing
		 * details about the block element.
		 */
		virtual void on_enter_block(MD_BLOCKTYPE type, detail_variant detail) {}
		/**
		 * Gets called when the parser encounters the end of a block element.
		 *
		 * @param type The type of the block element. Please refer to 
		 * <a href="https://github.com/mity/md4c/blob/master/src/md4c.h#L54">md4c headers</a>
		 * to view the possible block types.
		 *
		 * @param Detail an std::variant containing
		 * details about the block element.
		 */
		virtual void on_leave_block(MD_BLOCKTYPE type, detail_variant detail) {}
		/**
		 * Gets called when the parser encounters the beginning of a span element.
		 *
		 * @param type The type of the span element. Please refer to 
		 * <a href="https://github.com/mity/md4c/blob/master/src/md4c.h#L108">md4c headers</a>
		 * to view the possible span types.
		 *
		 * @param Detail an std::variant containing
		 * details about the span element.
		 */
		virtual void on_enter_span(MD_SPANTYPE type, detail_variant detail) {}
		/**
		 * Gets called when the parser encounters the end of a span element.
		 *
		 * @param type The type of the span element. Please refer to 
		 * <a href="https://github.com/mity/md4c/blob/master/src/md4c.h#L108">md4c headers</a>
		 * to view the possible span types.
		 *
		 * @param detail An std::variant containing
		 * details about the span element.
		 */
		virtual void on_leave_span(MD_SPANTYPE type, detail_variant detail) {}
		/**
		 * Gets called when the parser encounters plain text.
		 *
		 * @param text The text to be outputted.
		 *
		 * @param type The type of the text. Please refer to
		 * <a href="https://github.com/mity/md4c/blob/master/src/md4c.h#L152">md4c headers</a>
		 * to view the possible text types.
		 */
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

		md_parse(source.data(), source.size(), &parser_data, this);
	}
} //namespace md4cpp
