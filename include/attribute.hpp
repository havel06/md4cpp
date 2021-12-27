#pragma once

#include <md4c.h>
#include <utility>
#include <string>
#include <vector>

namespace md4cpp
{
	struct substring
	{
		std::string text;
		MD_TEXTTYPE type;
		substring(const std::string& t_text, MD_TEXTTYPE t_type) : text(t_text), type(t_type) {}
	};

	struct attribute : public MD_ATTRIBUTE
	{
		attribute()	= delete;
		attribute(const MD_ATTRIBUTE& t_attribute) : MD_ATTRIBUTE(t_attribute) {}
		std::vector<substring> get_substrings()
		{
			if (this->text == nullptr)
			{
				return {};
			}

			std::vector<substring> retval{};
			for(auto i = 0; this->substr_offsets[i] < this->size; i++)
			{
				size_t text_size = this->substr_offsets[i + 1] - this->substr_offsets[i];
				retval.emplace_back(std::string(this->text, text_size), this->substr_types[i]);
			}
			return retval;
		}
	};
} //namespace md4cpp
