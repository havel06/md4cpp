# MD4CPP

A C++ wrapper for the [md4c](https://github.com/mity/md4c) Markdown parser.

The wrapper should provide a way to write more idiomatic C++ code when using the library.

## Usage

```cpp
#include <iostream>
#include "include/md4cpp.hpp"

class my_parser : public md4cpp::parser
{
public:
	std::string m_result{};
private:
	void on_enter_block(MD_BLOCKTYPE type, const md4cpp::detail_variant& detail) override
	{
		// gets called when entering a block
	}
	void on_leave_block(MD_BLOCKTYPE type, const md4cpp::detail_variant& detail) override
	{
		// gets called when exiting a block
	}
	void on_enter_span(MD_SPANTYPE type, const md4cpp::detail_variant& detail) override
	{
		// gets called when entering a span
	}
	void on_leave_span(MD_SPANTYPE type, const md4cpp::detail_variant& detail) override
	{
		// gets called when exiting a span
	}
	void on_text(const std::string& text, MD_TEXTTYPE type) override
	{
		// gets called when outputting text
	}
};

int main()
{
	my_parser parser;
	parser.parse("markdown source");
	std::cout << parser.m_result << std::endl;
}
```

Also refer to example.cpp, which shows the implementation of a simple html renderer.
It is not feature complete, and it does not handle all edge cases,
but it is good enough as an example.
