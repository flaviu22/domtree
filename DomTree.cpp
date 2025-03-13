// DomTree.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>
#include <fstream>

#include "DomTree.h"

#include <gtest/gtest.h>

using namespace domtree;

domtree::Tag* GenerateHeader(domtree::CDomTree& dom)
{
	return nullptr;
//	CDomTreeWork::Tag html("html");
//	html.AddChild({ "style", std::move(html_style.data())});
//	html.AddChild({ "body" });
//	dom.GetTags().push_back(html);
//	return &dom.GetTags().back().m_childs.back();
}

domtree::Tag* GenerateSplitLeft(domtree::Tag* body)
{
	return nullptr;
//	CDomTreeWork::Tag tag("div", "bibi", { {{"class"}, {"split left"}} });
//	std::vector<CDomTreeWork::Attribute> att({ {{"repeat"}, {"no"}} });
//	tag.AddAttributes(att);
//	body->AddChild(tag);
//	return &body->m_childs.back();
}

TEST(TestInvalidTable, invalidSmallTable)
{
	std::ifstream ifs(std::filesystem::current_path().generic_string() + "/html/invalid_small_table.html");
	std::string html_file((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	CDomTree dt{};
	dt.Parse(std::move(html_file));
	EXPECT_EQ(1, dt.GetTags().size());
	if (1 == dt.GetTags().size())				// html
	{
		auto& childs = dt.GetTags().at(0);
		EXPECT_EQ(1, childs->m_childs.size());	// body
		if (1 == childs->m_childs.size())
		{
			if (1 == childs.get()->m_childs.size())
				EXPECT_EQ(4, childs.get()->m_childs.at(0)->m_childs.size());	// body's childs
		}
	}
}

TEST(TestInvalidTable, invalidHugeTable)
{
	std::ifstream ifs(std::filesystem::current_path().generic_string() + "/html/invalid_huge_table.html");
	std::string html_file((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	CDomTree dt{};
	dt.Parse(std::move(html_file));
//	std::ofstream ofs(std::filesystem::current_path().generic_string() + "/html/invalid_huge_table_out.html");
//	ofs << dt.GetData();
	EXPECT_EQ(2, dt.GetTags().size());
	if (2 == dt.GetTags().size())				// doctype, html
	{
		auto& childs = dt.GetTags().at(1);
		EXPECT_EQ(2, childs->m_childs.size());	// head, body
		if (2 == childs->m_childs.size())
			EXPECT_EQ(1009, childs->m_childs.at(1)->m_childs.size());	// all tables
	}
}

TEST(TestInvalidTable, imbricatedTable)
{
	std::ifstream ifs(std::filesystem::current_path().generic_string() + "/html/imbricated_tables.html");
	std::string html_file((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	CDomTree dt{};
	dt.Parse(std::move(html_file));
//	std::ofstream ofs(std::filesystem::current_path().generic_string() + "/html/imbricated_tables_out.html");
//	ofs << dt.GetData();
	EXPECT_EQ(2, dt.GetTags().size());
	if (2 == dt.GetTags().size())				// doctype, html
	{
		auto& childs = dt.GetTags().at(1);
		EXPECT_EQ(2, childs->m_childs.size());	// head, body
		if (2 == childs->m_childs.size())
			EXPECT_EQ(1, childs->m_childs.at(1)->m_childs.size());	// all tables
	}
}

TEST(TestInvalidTable, imbricatedInvalidTablesSmall)
{
	std::ifstream ifs(std::filesystem::current_path().generic_string() + "/html/imbricated_invalid_tables_small.html");
	std::string html_file((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	CDomTree dt{};
	dt.Parse(std::move(html_file));
//	std::ofstream ofs(std::filesystem::current_path().generic_string() + "/html/imbricated_invalid_tables_small_out.html");
//	ofs << dt.GetData();
	EXPECT_EQ(2, dt.GetTags().size());
	if (2 == dt.GetTags().size())				// doctype, html
	{
		auto& childs = dt.GetTags().at(1);
		EXPECT_EQ(2, childs->m_childs.size());	// head, body
		if (2 == childs->m_childs.size())
			EXPECT_EQ(1, childs->m_childs.at(1)->m_childs.size());	// all tables
	}
}

TEST(TestInvalidTable, imbricatedInvalidTables)
{
	std::ifstream ifs(std::filesystem::current_path().generic_string() + "/html/imbricated_invalid_tables.html");
	std::string html_file((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	CDomTree dt{};
	dt.Parse(std::move(html_file));
//	std::ofstream ofs(std::filesystem::current_path().generic_string() + "/html/imbricated_invalid_tables_out.html");
//	ofs << dt.GetData();
	EXPECT_EQ(2, dt.GetTags().size());
	if (2 == dt.GetTags().size())				// doctype, html
	{
		auto& childs = dt.GetTags().at(1);
		EXPECT_EQ(2, childs->m_childs.size());	// head, body
		if (2 == childs->m_childs.size())
			EXPECT_EQ(1, childs->m_childs.at(1)->m_childs.size());	// all tables
	}
}

TEST(TestBigSite, modernescpp_com)
{
	std::ifstream ifs(std::filesystem::current_path().generic_string() + "/html/modernescpp_com.html");
	std::string html_file((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	CDomTree dt{};
	dt.Parse(std::move(html_file));
//	std::ofstream ofs(std::filesystem::current_path().generic_string() + "/html/modernescpp_com_out.html");
//	ofs << dt.GetData();
	EXPECT_EQ(2, dt.GetTags().size());
	if (2 == dt.GetTags().size())
	{
		EXPECT_EQ(2, dt.GetTags().at(1)->m_childs.size());
	}
}

TEST(TestBigSite, codingforums)
{
	std::ifstream ifs(std::filesystem::current_path().generic_string() + "/html/codingforums.html");
	std::string html_file((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	CDomTree dt{};
	dt.Parse(std::move(html_file));
//	std::ofstream ofs(std::filesystem::current_path().generic_string() + "/html/codingforums_out.html");
//	ofs << dt.GetData();
	EXPECT_EQ(2, dt.GetTags().size());
	if (2 == dt.GetTags().size())
	{
		EXPECT_EQ(2, dt.GetTags().at(1)->m_childs.size());
		if (2 == dt.GetTags().at(1)->m_childs.size())
		{
			EXPECT_EQ(2, dt.GetTags().at(1)->m_childs.at(1)->m_childs.size());
			if (2 == dt.GetTags().at(1)->m_childs.at(1)->m_childs.size())
			{
				EXPECT_EQ(2, dt.GetTags().at(1)->m_childs.at(1)->m_childs.at(1)->m_childs.size());
			}
		}
	}
}

TEST(TestBigSite, myradioonline_ro)
{
	std::ifstream ifs(std::filesystem::current_path().generic_string() + "/html/myradioonline_ro.html");
	std::string html_file((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	CDomTree dt{};
	dt.Parse(std::move(html_file));
	EXPECT_EQ(2, dt.GetTags().size());
	if (2 == dt.GetTags().size())
	{
		auto& childs = dt.GetTags().at(1);
		EXPECT_EQ(2, childs->m_childs.size());	// head, body
	}
}

TEST(TestBigSite, adevarul_ro)
{
	std::ifstream ifs(std::filesystem::current_path().generic_string() + "/html/adevarul_ro.html");
	std::string html_file((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	CDomTree dt{};
	dt.Parse(std::move(html_file));
//	std::ofstream ofs(std::filesystem::current_path().generic_string() + "/html/adevarul_ro_out.html");
//	ofs << dt.GetData();
	EXPECT_EQ(2, dt.GetTags().size());
	if (2 == dt.GetTags().size())
	{
		auto& childs = dt.GetTags().at(1);
		EXPECT_EQ(2, childs->m_childs.size());	// head, body
	}
}

TEST(TestBigSite, dailymail)
{
	std::ifstream ifs(std::filesystem::current_path().generic_string() + "/html/dailymail.html");
	std::string html_file((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	CDomTree dt{};
	dt.Parse(std::move(html_file));
//	std::ofstream ofs(std::filesystem::current_path().generic_string() + "/html/dailymail_out.html");
//	ofs << dt.GetData();
	EXPECT_EQ(2, dt.GetTags().size());
	if (2 == dt.GetTags().size())
	{
		auto& childs = dt.GetTags().at(1);
		EXPECT_EQ(2, childs->m_childs.size());	// head, body
	}
}

TEST(TestBigSite, cppreference_com)
{
	std::ifstream ifs(std::filesystem::current_path().generic_string() + "/html/cppreference_com.html");
	std::string html_file((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	CDomTree dt{};
	dt.Parse(std::move(html_file));
//	std::ofstream ofs(std::filesystem::current_path().generic_string() + "/html/cppreference_com_out.html");
//	ofs << dt.GetData();
	EXPECT_EQ(2, dt.GetTags().size());
	if (2 == dt.GetTags().size())
	{
		auto& childs = dt.GetTags().at(1);
		EXPECT_EQ(3, childs->m_childs.size());	// head, body, comment
	}
}

TEST(TestSite, icomoon)
{
	std::ifstream ifs(std::filesystem::current_path().generic_string() + "/html/icomoon.html");
	std::string html_file((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	CDomTree dt{};
	dt.Parse(std::move(html_file));
//	std::ofstream ofs(std::filesystem::current_path().generic_string() + "/html/icomoon_out.html");
//	ofs << dt.GetData();
	EXPECT_EQ(3, dt.GetTags().size());
	if (3 == dt.GetTags().size())
	{
		auto& childs = dt.GetTags().at(2);
		EXPECT_EQ(2, childs->m_childs.size());	// head, body
	}
}

TEST(TestSite, multi_comments)
{
	std::ifstream ifs(std::filesystem::current_path().generic_string() + "/html/multi_comments.html");
	std::string html_file((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	CDomTree dt{};
	dt.Parse(std::move(html_file));
	EXPECT_EQ(4, dt.GetTags().size());
	if (4 == dt.GetTags().size())
	{
		auto& childs = dt.GetTags().at(2);
		EXPECT_EQ(4, childs->m_childs.size());	// head, body
	}
}

TEST(TestSite, multi_spaces)
{
	std::ifstream ifs(std::filesystem::current_path().generic_string() + "/html/multi_spaces.html");
	std::string html_file((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	CDomTree dt{};
	dt.Parse(std::move(html_file));
	EXPECT_EQ(2, dt.GetTags().size());
	if (2 == dt.GetTags().size())
	{
		auto& childs = dt.GetTags().at(1);
		EXPECT_EQ(2, childs->m_childs.size());	// head, body
	}
}

TEST(TestSite, multi_self_closing_tags)
{
	std::ifstream ifs(std::filesystem::current_path().generic_string() + "/html/multi_self_closing_tags.html");
	std::string html_file((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	CDomTree dt{};
	dt.Parse(std::move(html_file));
	EXPECT_EQ(1, dt.GetTags().size());
	if (1 == dt.GetTags().size())
	{
		auto& childs = dt.GetTags().at(0);
		EXPECT_EQ(1, childs->m_childs.size());
		if (1 == childs->m_childs.size())
			EXPECT_EQ(8, childs.get()->m_childs.at(0)->m_childs.size());
	}
}

TEST(TestSite, style_with_comments)
{
	std::ifstream ifs(std::filesystem::current_path().generic_string() + "/html/style_with_comments.html");
	std::string html_file((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	CDomTree dt{};
	dt.Parse(std::move(html_file));
	EXPECT_EQ(1, dt.GetTags().size());
	if (1 == dt.GetTags().size())
	{
		auto& childs = dt.GetTags().at(0);
		EXPECT_EQ(2, childs->m_childs.size());
		if (2 == childs->m_childs.size())
			EXPECT_EQ(1, childs.get()->m_childs.at(0)->m_childs.size());
	}
}

int main()
{
	testing::InitGoogleTest();
	return RUN_ALL_TESTS();
}
