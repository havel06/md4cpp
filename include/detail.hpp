#pragma once

#include "attribute.hpp"
#include <variant>

namespace md4cpp
{
	using block_ul_detail = MD_BLOCK_UL_DETAIL;
	using block_ol_detail = MD_BLOCK_OL_DETAIL;
	using block_li_detail = MD_BLOCK_LI_DETAIL;
	using block_h_detail = MD_BLOCK_H_DETAIL;
	using block_table_detail = MD_BLOCK_TABLE_DETAIL;
	using block_td_detail = MD_BLOCK_TD_DETAIL;

	struct block_code_detail
	{
		attribute info;
		attribute lang;
		MD_CHAR fence_char;
		block_code_detail(const MD_BLOCK_CODE_DETAIL& t_detail) : info(t_detail.info),
														   lang(t_detail.lang),
														   fence_char(t_detail.fence_char) {}
		block_code_detail() = delete;
	};

	struct span_a_detail
	{
		attribute href;
		attribute title;
		span_a_detail(const MD_SPAN_A_DETAIL& t_detail) : href(t_detail.href), title(t_detail.title) {} 
		span_a_detail() = delete;
	};

	struct span_img_detail
	{
		attribute src;
		attribute title;
		span_img_detail(const MD_SPAN_IMG_DETAIL& t_detail) : src(t_detail.src), title(t_detail.title) {}
		span_img_detail() = delete;
	};

	struct span_wikilink_detail
	{
		attribute target;
		span_wikilink_detail(const MD_SPAN_WIKILINK_DETAIL& t_detail) : target(t_detail.target) {}
		span_wikilink_detail() = delete;
	};

	using detail_variant = std::variant<std::monostate, block_ul_detail,
										block_ol_detail, block_li_detail,
										block_h_detail, block_code_detail,
										block_table_detail, block_td_detail,
										span_a_detail, span_img_detail,
										span_wikilink_detail>;

	inline detail_variant cast_detail_block(void* detail, MD_BLOCKTYPE type) //NOLINT
	{
		switch (type)
		{
		case MD_BLOCK_UL:
			return *static_cast<block_ul_detail*>(detail);
		case MD_BLOCK_OL:
			return *static_cast<block_ol_detail*>(detail);
		case MD_BLOCK_LI:
			return *static_cast<block_li_detail*>(detail);
		case MD_BLOCK_H:
			return *static_cast<block_h_detail*>(detail);
		case MD_BLOCK_CODE:
			return block_code_detail(*static_cast<MD_BLOCK_CODE_DETAIL*>(detail));
		case MD_BLOCK_TABLE:
			return *static_cast<block_table_detail*>(detail);
		case MD_BLOCK_TD:
			return *static_cast<block_td_detail*>(detail);
		default:
			return {};
		}
	}
	
	inline detail_variant cast_detail_span(void* detail, MD_SPANTYPE type) //NOLINT
	{
		switch (type)
		{
		case MD_SPAN_A:
			return span_a_detail(*static_cast<MD_SPAN_A_DETAIL*>(detail));
		case MD_SPAN_IMG:
			return span_img_detail(*static_cast<MD_SPAN_IMG_DETAIL*>(detail));
		case MD_SPAN_WIKILINK:
			return span_wikilink_detail(*static_cast<MD_SPAN_WIKILINK_DETAIL*>(detail));
		default:
			return {};
		}
	}
} //namesapce md4cpp
