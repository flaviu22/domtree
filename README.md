## Overview

A C++ header only library for parsing a `HTML` source.  It is designed to parse and represent HTML/XML-like data structures. The class `CDomTree` represents the DOM tree and provides parsing functionality. The `CDomTree` class parses input data (HTML/XML) into a hierarchical DOM tree structure, allowing manipulation and serialization of the data.

**Public Methods**

**Parsing:**
`void Parse(const std::string& data)`: Parses input data by copying it.
`void Parse(std::string&& data)`: Parses input data by moving it.

**Serialization:**
`std::string GetData() const`: Serializes the DOM tree into a formatted string.

**Accessors:**
`std::vector<std::shared_ptr<Tag>>& GetTags()`: Returns a reference to the root tags (modifiable).
`const std::vector<std::shared_ptr<Tag>>& GetTags() const`: Returns a const reference to the root tags.

**Summary**
The `CDomTree` class is a robust and extensible DOM tree parser that supports parsing, correctness validation, and serialization of HTML/XML-like data. It ensures proper handling of nested tags, multi-line tags, and special cases, making it suitable for building or manipulating DOM structures programmatically.

## Getting Started

In order to use this library, just include the header, like:

```cpp
#include "DomTree.h"
```

You can parse a HTML string as follows:

```cpp
#include "DomTree.h"

	std::ifstream ifs(std::filesystem::current_path().generic_string() + "/html/style_with_comments.html");
	std::string html_file((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	CDomTree dt{};
	dt.Parse(std::move(html_file));
```

You can easily generate a HTML source using this:

```cpp
#include "DomTree.h"

constexpr std::string_view html_style = R"(
body {
	font-family: Arial;

	color: #f9f9ff;
	background-color:#161B1F;
}
)";

	CDomTree dom{};

	dom.GetTags().push_back(std::make_shared<Tag>("!DOCTYPE html"));
	Tag head("head");
	head.AddChild({ "meta", { {{"http-equiv"}, {"X-UA-Compatible"}}, {{"content"}, {"IE=edge"}} } });
	head.AddChild({ "meta", { {{"http-equiv"}, {"content-type"}}, {{"content"}, {"text/html; charset=utf-8"}} } });
	head.AddChild({ "meta", { {{"name"}, {"viewport"}}, {{"content"}, {"width=device-width, initial-scale=1"}} } });
	head.AddChild({ "style", html_style.data() });

    dom.GetTags().push_back(std::make_shared<Tag>(std::move(head)));

    // return HTML source as string
    std::clog << dom.GetData() << std::endl;

```

The project parse several HTML sources using google test, the outcome is like:

```cpp
[==========] Running 16 tests from 3 test suites.
[----------] Global test environment set-up.
[----------] 5 tests from TestInvalidTable
[ RUN      ] TestInvalidTable.invalidSmallTable
[       OK ] TestInvalidTable.invalidSmallTable (0 ms)
[ RUN      ] TestInvalidTable.invalidHugeTable
[       OK ] TestInvalidTable.invalidHugeTable (82 ms)
[ RUN      ] TestInvalidTable.imbricatedTable
[       OK ] TestInvalidTable.imbricatedTable (1 ms)
[ RUN      ] TestInvalidTable.imbricatedInvalidTablesSmall
[       OK ] TestInvalidTable.imbricatedInvalidTablesSmall (0 ms)
[ RUN      ] TestInvalidTable.imbricatedInvalidTables
[       OK ] TestInvalidTable.imbricatedInvalidTables (0 ms)
[----------] 5 tests from TestInvalidTable (90 ms total)

[----------] 6 tests from TestBigSite
[ RUN      ] TestBigSite.modernescpp_com
[       OK ] TestBigSite.modernescpp_com (57 ms)
[ RUN      ] TestBigSite.codingforums
[       OK ] TestBigSite.codingforums (4 ms)
[ RUN      ] TestBigSite.myradioonline_ro
[       OK ] TestBigSite.myradioonline_ro (29 ms)
[ RUN      ] TestBigSite.adevarul_ro
[       OK ] TestBigSite.adevarul_ro (115 ms)
[ RUN      ] TestBigSite.dailymail
[       OK ] TestBigSite.dailymail (245 ms)
[ RUN      ] TestBigSite.cppreference_com
[       OK ] TestBigSite.cppreference_com (36 ms)
[----------] 6 tests from TestBigSite (493 ms total)

[----------] 5 tests from TestSite
[ RUN      ] TestSite.icomoon
[       OK ] TestSite.icomoon (163 ms)
[ RUN      ] TestSite.multi_comments
[       OK ] TestSite.multi_comments (0 ms)
[ RUN      ] TestSite.multi_spaces
[       OK ] TestSite.multi_spaces (0 ms)
[ RUN      ] TestSite.multi_self_closing_tags
[       OK ] TestSite.multi_self_closing_tags (0 ms)
[ RUN      ] TestSite.style_with_comments
[       OK ] TestSite.style_with_comments (0 ms)
[----------] 5 tests from TestSite (168 ms total)

[----------] Global test environment tear-down
[==========] 16 tests from 3 test suites ran. (756 ms total)
[  PASSED  ] 16 tests.
```
## Contributing

If you have suggestions for improvement or if you've identified a bug, please don't hesitate to [open an issue](https://github.com/flaviu22/domtree/issues) or contribute by creating a pull request. When reporting a bug, provide comprehensive details about your environment, including compiler version and other relevant information, to facilitate issue reproduction. Additionally, if you're introducing a new feature, ensure that you include corresponding test cases to validate its functionality.

### Dependencies

No dependencies, just a C++ compiler which supports C++17.
This one has been tested on Windows only.
