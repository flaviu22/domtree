CDomTree is a C++ header only class that parse and handle a HTML DOM.

Installation: Just include DomTree.h in your application.
Because the class is using string_view you need to compile with C++17.

Examples:

#include "DomTree.h"

using namespace domtree;


std::stirng html_file{"<html><body>abc</div></html>"};
CDomTree dt{};
dt.Parse(html_file);	// now all html tags reside in CDomTree structure
