// Copyright (C) 2025, Flaviu Marc.
//
// Licensed under the MIT License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// http://opensource.org/licenses/MIT
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#pragma once

#include <array>
#include <stack>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <string_view>

namespace domtree
{
	constexpr std::string_view whitespace{ " \n\r\t" };

	constexpr std::array<std::string_view, 16> self_closing_tags
	{
		"area",
		"base",
		"br",
		"col",
		"command",
		"embed",
		"hr",
		"img",
		"input",
		"keygen",
		"link",
		"meta",
		"param",
		"source",
		"track",
		"wbr"
	};

	constexpr std::array<std::string_view, 1> non_valid_tags
	{
		"align"
	};

	struct Attribute
	{
		std::string m_key{};
		std::string m_value{};
		char m_quote{ '\"' };
	};

	struct Tag
	{
	public:
		Tag() = default;
		Tag(const std::string& name)
			: m_name(name)
		{
		}
		Tag(const std::string& name, const std::string& value)
			: m_name(name)
			, m_value(value)
		{
		}
		Tag(const std::string& name, const std::string& value, const std::vector<Attribute>& attributes)
			: m_name(name)
			, m_value(value)
			, m_attributes(attributes)
		{
		}
		Tag(const std::string& name, const std::vector<Attribute>& attributes)
			: m_name(name)
			, m_attributes(attributes)
		{
		}
		Tag(std::string&& name)
			: m_name(std::move(name))
		{
		}
		Tag(std::string&& name, std::string&& value)
			: m_name(std::move(name))
			, m_value(std::move(value))
		{
		}
		Tag(std::string&& name, std::string&& value, std::vector<Attribute>&& attributes)
			: m_name(std::move(name))
			, m_value(std::move(value))
			, m_attributes(std::move(attributes))
		{
		}
		Tag(std::string&& name, std::vector<Attribute>&& attributes)
			: m_name(std::move(name))
			, m_attributes(std::move(attributes))
		{
		}
		Tag(const Tag& rhs)
			: m_parent(rhs.m_parent)
			, m_name(rhs.m_name)
			, m_value(rhs.m_value)
			, m_childs(rhs.m_childs)
			, m_attributes(rhs.m_attributes)
		{
		}
		Tag& operator=(const Tag& rhs)
		{
			if (this != &rhs)
			{
				m_parent = rhs.m_parent;
				m_name = rhs.m_name;
				m_value = rhs.m_value;
				m_childs = rhs.m_childs;
				m_attributes = rhs.m_attributes;
			}
			return *this;
		}
		Tag(Tag&& rhs) noexcept
			: m_parent(std::move(rhs.m_parent))
			, m_name(std::move(rhs.m_name))
			, m_value(std::move(rhs.m_value))
			, m_childs(std::move(rhs.m_childs))
			, m_attributes(std::move(rhs.m_attributes))
		{
			rhs.m_parent = nullptr;
		}
		Tag& operator=(Tag&& rhs) noexcept
		{
			if (this != &rhs)
			{
				m_parent = std::move(rhs.m_parent);
				m_name = std::move(rhs.m_name);
				m_value = std::move(rhs.m_value);
				m_childs = std::move(rhs.m_childs);
				m_attributes = std::move(rhs.m_attributes);

				rhs.m_parent = nullptr;
			}
			return *this;
		}
		~Tag() = default;

	public:
		std::string m_name{};
		std::string m_value{};
		std::vector<Attribute> m_attributes{};
		std::vector<std::shared_ptr<Tag>> m_childs{};
		Tag* m_parent{};

	public:
		void AddAttributes(const std::vector<Attribute>& attributes)
		{
			std::copy(std::begin(attributes), std::end(attributes), std::back_inserter(m_attributes));
		}
		void AddAttributes(std::vector<Attribute>&& attributes)
		{
			if (m_attributes.empty())
			{
				m_attributes = std::move(attributes);
			}
			else
			{
				m_attributes.reserve(m_attributes.size() + attributes.size());
				std::move(std::begin(attributes), std::end(attributes), std::back_inserter(m_attributes));
				attributes.clear();
			}
		}
		void SetValue(const std::string& text)
		{
			m_value = text;
		}
		void SetValue(std::string&& text)
		{
			m_value = std::move(text);
		}
		void AddText(const std::string& text)
		{
			Tag tag{};
			tag.m_value = text;
			tag.m_parent = this;
			m_childs.push_back(std::make_shared<Tag>(std::move(tag)));
		}
		void AddText(std::string&& text)
		{
			Tag tag{};
			tag.m_value = std::move(text);
			tag.m_parent = this;
			m_childs.push_back(std::make_shared<Tag>(std::move(tag)));
		}
		void AddChild(Tag& tag)
		{
			tag.m_parent = this;
			m_childs.push_back(std::make_shared<Tag>(tag));
		}
		void AddChild(Tag&& tag)
		{
			tag.m_parent = this;
			m_childs.push_back(std::make_shared<Tag>(std::move(tag)));
		}
	};

	class CDomTree
	{
	public:
		CDomTree() = default;
		CDomTree(const CDomTree& rhs) = delete;
		CDomTree& operator=(const CDomTree& rhs) = delete;
		CDomTree(CDomTree&& rhs) noexcept
			: m_currentTag(std::move(rhs.m_currentTag))
			, m_data(std::move(rhs.m_data))
			, m_tags(std::move(rhs.m_tags))
			, m_tables(std::move(rhs.m_tables))
			, m_bufferIndex(std::move(rhs.m_bufferIndex))
			, m_svg(std::move(rhs.m_svg))
			, m_style(std::move(rhs.m_style))
			, m_script(std::move(rhs.m_script))
			, m_td(std::move(rhs.m_td))
			, m_tr(std::move(rhs.m_tr))
			, m_table(std::move(rhs.m_table))
			, m_p(std::move(rhs.m_p))
			, m_a(std::move(rhs.m_a))
			, m_label(std::move(rhs.m_label))
		{
			rhs.m_currentTag = nullptr;
			rhs.m_bufferIndex = 0;
			rhs.m_svg = false;
			rhs.m_style = false;
			rhs.m_script = false;
			rhs.m_td = TagState::closed;
			rhs.m_tr = TagState::closed;
			rhs.m_table = TagState::closed;
			rhs.m_p = TagState::closed;
			rhs.m_a = TagState::closed;
			rhs.m_label = TagState::closed;
		}
		CDomTree& operator=(CDomTree&& rhs) noexcept
		{
			if (this != &rhs)
			{
				m_currentTag = std::move(rhs.m_currentTag);
				m_data = std::move(rhs.m_data);
				m_tags = std::move(rhs.m_tags);
				m_tables = std::move(rhs.m_tables);
				m_bufferIndex = std::move(rhs.m_bufferIndex);
				m_svg = std::move(rhs.m_svg);
				m_style = std::move(rhs.m_style);
				m_script = std::move(rhs.m_script);
				m_td = std::move(rhs.m_td);
				m_tr = std::move(rhs.m_tr);
				m_table = std::move(rhs.m_table);
				m_p = std::move(rhs.m_p);
				m_a = std::move(rhs.m_a);
				m_label = std::move(rhs.m_label);

				rhs.m_currentTag = nullptr;
				rhs.m_bufferIndex = 0;
				rhs.m_svg = false;
				rhs.m_style = false;
				rhs.m_script = false;
				rhs.m_td = TagState::closed;
				rhs.m_tr = TagState::closed;
				rhs.m_table = TagState::closed;
				rhs.m_p = TagState::closed;
				rhs.m_a = TagState::closed;
				rhs.m_label = TagState::closed;
			}
			return *this;
		}
		~CDomTree() = default;

	public:
		std::vector<std::shared_ptr<Tag>>& GetTags() { return m_tags; }
		const std::vector<std::shared_ptr<Tag>>& GetTags() const { return m_tags; }
		void Parse(const std::string& data)
		{
			m_data = data;
			Parse();
		}

		void Parse(std::string&& data)
		{
			m_data = std::move(data);
			Parse();
		}

		std::string GetData() const
		{
			std::string out{};
			PrintData(m_tags, out);
			return out + "\n";
		}

	private:
		void Parse()
		{
			m_bufferIndex = 0;
			while (m_bufferIndex < m_data.length())
			{
				if (!ParseNextToken())
					break;
			}
		}

		bool ParseNextToken()
		{
			if (m_bufferIndex >= m_data.length())
				return false;

			if (!m_currentTag || !m_script || !m_style || !m_svg)
				SkipWhiteSpaces();

			if (m_svg)
				return ParseValue();

			if ('<' == m_data[m_bufferIndex])
				return ParseTag();
			else
				return ParseValue();
		}

		bool ParseTag()
		{
			if (m_bufferIndex >= m_data.length())
				return false;

			m_bufferIndex++;

			if (m_bufferIndex >= m_data.length())
				return false;

			switch (m_data[m_bufferIndex])
			{
			case '/':
				m_bufferIndex++;
				ParseClosingTag();
				break;
			case '!':
			case '?':
				if ('!' == m_data[m_bufferIndex]
					&& m_bufferIndex < m_data.length() - 3
					&& '-' == m_data[m_bufferIndex + 1]
					&& '-' == m_data[m_bufferIndex + 2])
					ParseCommentTag();
				else
					ParseSpecialTag();
				break;
			default:
				if (!ParseOpeningTag())
					return false;
			}

			return true;
		}

		bool ParseValue()
		{
			if (!m_currentTag)
				return false;

			std::string value{};

			do
			{
				if (!m_script && !m_style && !m_svg)
				{
					while (m_bufferIndex < m_data.length() - 1 && '<' != m_data[m_bufferIndex])
						value += m_data[m_bufferIndex++];
					break;
				}

				if (m_script)
				{
					while (m_bufferIndex < m_data.length() - 5
						&& !('<' == m_data[m_bufferIndex]
							&& '/' == m_data[m_bufferIndex + 1]
							&& 's' == std::tolower(m_data[m_bufferIndex + 2])
							&& 'c' == std::tolower(m_data[m_bufferIndex + 3])
							&& 'r' == std::tolower(m_data[m_bufferIndex + 4])))
						value += m_data[m_bufferIndex++];
					m_script = false;
					break;
				}

				if (m_style)
				{
					while (m_bufferIndex < m_data.length() - 5
						&& !('<' == m_data[m_bufferIndex]
							&& '/' == m_data[m_bufferIndex + 1]
							&& 's' == std::tolower(m_data[m_bufferIndex + 2])
							&& 't' == std::tolower(m_data[m_bufferIndex + 3])
							&& 'y' == std::tolower(m_data[m_bufferIndex + 4])))
						value += m_data[m_bufferIndex++];
					m_style = false;
					break;
				}

				if (m_svg)
				{
					while (m_bufferIndex < m_data.length() - 5
						&& !('<' == m_data[m_bufferIndex]
							&& '/' == m_data[m_bufferIndex + 1]
							&& 's' == std::tolower(m_data[m_bufferIndex + 2])
							&& 'v' == std::tolower(m_data[m_bufferIndex + 3])
							&& 'g' == std::tolower(m_data[m_bufferIndex + 4])))
						value += m_data[m_bufferIndex++];
					m_svg = false;
				}
			} while (false);

			Tag tag{};
			tag.m_parent = m_currentTag;
			tag.m_value = value;
			m_currentTag->m_childs.push_back(std::make_shared<Tag>(std::move(tag)));

			return true;
		}

		bool ParseSpecialTag()
		{
			Tag tag{};

			while (m_bufferIndex < m_data.length() && '>' != m_data[m_bufferIndex])
				tag.m_name += m_data[m_bufferIndex++];

			if (m_tags.empty() || !m_currentTag)
			{
				m_tags.push_back(std::make_shared<Tag>(std::move(tag)));
			}
			else
			{
				tag.m_parent = m_currentTag;
				tag.m_parent->m_childs.push_back(std::make_shared<Tag>(std::move(tag)));
			}

			if ('>' == m_data[m_bufferIndex] || m_bufferIndex >= m_data.length())
				m_bufferIndex++;

			return true;
		}

		bool ParseCommentTag()
		{
			Tag tag{};

			while (m_bufferIndex < m_data.length() - 3 &&
				!('>' == m_data[m_bufferIndex]
					&& '-' == m_data[m_bufferIndex - 1]
					&& '-' == m_data[m_bufferIndex - 2]))
				tag.m_name += m_data[m_bufferIndex++];

			if (m_tags.empty() || !m_currentTag)
			{
				m_tags.push_back(std::make_shared<Tag>(std::move(tag)));
			}
			else
			{
				tag.m_parent = m_currentTag;
				tag.m_parent->m_childs.push_back(std::make_shared<Tag>(std::move(tag)));
			}

			if ('>' == m_data[m_bufferIndex] || m_bufferIndex >= m_data.length())
				m_bufferIndex++;

			return true;
		}

		bool ParseOpeningTag()
		{
			SkipWhiteSpaces();

			std::shared_ptr<Tag> tag = std::make_shared<Tag>();
			// parse tag name
			while (m_bufferIndex < m_data.length()
				&& !IsWhiteSpace(m_data[m_bufferIndex])
				&& '>' != m_data[m_bufferIndex]
				&& '/' != m_data[m_bufferIndex])
				tag->m_name += std::tolower(m_data[m_bufferIndex++]);

			if (std::binary_search(non_valid_tags.cbegin(), non_valid_tags.cend(), tag->m_name))
			{
				SkipCurrentTag();
				return true;
			}

			const bool isSelfClosingTag = (std::binary_search(self_closing_tags.cbegin(), self_closing_tags.cend(), tag->m_name));

			if (m_tags.empty() || !m_currentTag)
			{
				m_tags.push_back(tag);
				m_currentTag = m_tags.back().get();
			}
			else
			{
				if (!isSelfClosingTag && IsWatched(tag->m_name))
					PerformCorrectnessOnOpen(tag->m_name);

				if (m_currentTag)
				{
					tag->m_parent = m_currentTag;							// put m_currentTag to local tag as parent
					tag->m_parent->m_childs.push_back(tag);					// add to local tag parent childs the actual local tag
					m_currentTag = tag->m_parent->m_childs.back().get();	// setup m_currentTag as local tag
					SetupMultiLineTags();
				}
			}

			ParseAttributes();

			if (isSelfClosingTag)
			{
				m_currentTag = m_currentTag->m_parent;
			}
			else if (IsWatched(m_currentTag->m_name))
			{
				UpdateWatched(m_currentTag->m_name, TagState::opened);
			}

			if ('>' == m_data[m_bufferIndex] || m_bufferIndex >= m_data.length())
				m_bufferIndex++;

			return true;
		}

		void ParseClosingTag()
		{
			SkipWhiteSpaces();

			std::string tagName{};
			while (m_bufferIndex < m_data.length() && !IsWhiteSpace(m_data[m_bufferIndex]) && '>' != m_data[m_bufferIndex])
				tagName += std::tolower(m_data[m_bufferIndex++]);

			if ('>' == m_data[m_bufferIndex] || m_data[m_bufferIndex] >= m_data.length())
				m_bufferIndex++;

			if (binary_search(non_valid_tags.cbegin(), non_valid_tags.cend(), tagName))
				return;

			if (m_currentTag)
			{
				bool valid_close{ true };
				if (IsWatched(tagName))
				{
					UpdateWatched(tagName, TagState::closed);
					PerformCorrectnessOnClose(tagName);
					valid_close = CloseParagraphes(tagName);
				}
				if (m_currentTag && valid_close)
					m_currentTag = m_currentTag->m_parent;
				if ("table" == tagName && !m_tables.empty())
					RestoreCurrentTable();
			}
		}
		// corrected tags: tr, td
		void PerformCorrectnessOnOpen(const std::string& tagName)
		{
			if ("td" == tagName)
			{
				if (TagState::opened == m_td && m_currentTag)
				{
					m_currentTag = m_currentTag->m_parent;
					m_td = TagState::closed;
				}
				return;
			}
			if ("tr" == tagName)
			{
				if (TagState::opened == m_td && m_currentTag)
				{
					m_currentTag = m_currentTag->m_parent;
					m_td = TagState::closed;
				}
				if (TagState::opened == m_tr && m_currentTag)
				{
					m_currentTag = m_currentTag->m_parent;
					m_tr = TagState::closed;
				}
				return;
			}
			if ("table" == tagName && TagState::opened == m_td && m_currentTag)
			{
				m_tables.push({ m_table, m_tr, m_td });
				m_tr = m_td = TagState::closed;
				m_table = TagState::opened;
				return;
			}
		}
		// corrected tags: table
		void PerformCorrectnessOnClose(const std::string& tagName)
		{
			if ("table" == tagName)
			{
				if (TagState::opened == m_td && m_currentTag)
				{
					m_currentTag = m_currentTag->m_parent;
					m_td = TagState::closed;
				}
				if (TagState::opened == m_tr && m_currentTag)
				{
					m_currentTag = m_currentTag->m_parent;
					m_tr = TagState::closed;
				}
				return;
			}
		}

		void ParseAttributes()
		{
			char quote{ '\"' };
			while (m_bufferIndex < m_data.length() && '>' != m_data[m_bufferIndex])
			{
				SkipWhiteSpaces();

				if (m_bufferIndex < m_data.length() && '>' != m_data[m_bufferIndex] && '/' != m_data[m_bufferIndex])
				{
					std::string key{}, value{};
					while (m_bufferIndex < m_data.length()
						&& '=' != m_data[m_bufferIndex]
						&& '>' != m_data[m_bufferIndex]
						&& !IsWhiteSpace(m_data[m_bufferIndex]))
						key += m_data[m_bufferIndex++];

					SkipWhiteSpaces();
					if (m_bufferIndex < m_data.length() && '=' == m_data[m_bufferIndex])
					{
						m_bufferIndex++;
						SkipWhiteSpaces();
						if (m_bufferIndex < m_data.length() && ('\"' == m_data[m_bufferIndex] || '\'' == m_data[m_bufferIndex]))
						{
							quote = m_data[m_bufferIndex++];
							while (m_bufferIndex < m_data.length() && quote != m_data[m_bufferIndex])
								value += m_data[m_bufferIndex++];
						}
					}
					else
					{
						m_bufferIndex--;
					}
					m_currentTag->m_attributes.push_back(Attribute{ key, value, quote });
				}

				if ('>' != m_data[m_bufferIndex])
					m_bufferIndex++;
			}
		}

		void SkipWhiteSpaces()
		{
			while (m_bufferIndex < m_data.length() && IsWhiteSpace(m_data[m_bufferIndex]))
				m_bufferIndex++;
		}

		constexpr bool IsWhiteSpace(const char c) const
		{
			return (' ' == c || '\r' == c || '\n' == c || '\t' == c);
		}

		std::string GetIndent(size_t tabs) const
		{
			std::ostringstream buffer{};
			while (tabs--)
				buffer << "\t";
			return buffer.str();
		}

		void PrintData(const std::vector<std::shared_ptr<Tag>>& tags, std::string& data, const size_t level = 0) const
		{
			for (const auto& it : tags)
			{
				if (it->m_name.empty())
					continue;

				if ('!' != it->m_name.front() && '?' != it->m_name.front())
				{
					PrintName(*it, data, level);
					PrintValue(*it, data, level);
					PrintClose(*it, data, level);
				}
				else
				{
					if (!data.empty())
						data += "\n";
					data += GetIndent(level) + "<" + it->m_name + ">";
				}
			}
		}

		void PrintName(const Tag& tag, std::string& data, const size_t level) const
		{
			if (!data.empty())
				data += "\n";
			data += GetIndent(level) + "<" + tag.m_name;
			for (const auto& attr : tag.m_attributes)
				data += " " + attr.m_key + "=" + attr.m_quote + attr.m_value + attr.m_quote;

			if (std::binary_search(self_closing_tags.cbegin(), self_closing_tags.cend(), tag.m_name))
				data += "/";
			data += ">";
		}

		void PrintValue(const Tag& tag, std::string& data, const size_t level) const
		{
			if (0 == tag.m_childs.size())
			{
				data += tag.m_value.substr(0, tag.m_value.find_last_not_of(whitespace) + 1);
				return;
			}

			for (auto&& it : tag.m_childs)
			{
				if (it->m_name.empty())	// is value
				{
					if (1 == tag.m_childs.size() &&
						tag.m_childs.front()->m_name.empty() &&
						"script" != tag.m_name &&
						"svg" != tag.m_name &&
						"style" != tag.m_name)
						data += it->m_value.substr(0, it->m_value.find_last_not_of(whitespace) + 1);
					else
						data += "\n" + GetIndent(level + 1) + it->m_value.substr(0, it->m_value.find_last_not_of(whitespace) + 1);
				}
				else
				{
					PrintData({ it }, data, level + 1);
				}
			}
		}

		void PrintClose(const Tag& tag, std::string& data, const size_t level) const
		{
			if (!std::binary_search(self_closing_tags.cbegin(), self_closing_tags.cend(), tag.m_name))
			{
				if (tag.m_childs.size() > 1 || (0 != tag.m_childs.size() && !tag.m_childs.front()->m_name.empty()) ||
					"script" == tag.m_name || "svg" == tag.m_name || "style" == tag.m_name)
					data += "\n" + GetIndent(level);
				data += "</" + tag.m_name + ">";
			}
		}

	private:
		enum class TagState
		{
			closed = 0,
			opened
		};

	private:
		void SetupMultiLineTags()
		{
			m_script = m_style = m_svg = false;
			if ("script" == m_currentTag->m_name)
			{
				m_script = true;
				return;
			}
			if ("style" == m_currentTag->m_name)
			{
				m_style = true;
				return;
			}
			if ("svg" == m_currentTag->m_name)
			{
				m_svg = true;
			}
		}
		// watched tags: p, a, table, tr, td
		bool IsWatched(const std::string& tag) const
		{
			if ("p" == tag || "a" == tag || "td" == tag || "tr" == tag || "table" == tag || "label" == tag)
				return true;
			return false;
		}
		// update tags: table, tr, td
		// tags updated on open only: p, a 
		void UpdateWatched(const std::string& tagName, TagState state)
		{
			if ("p" == tagName && TagState::opened == state)	// update the p tag at openning only
			{
				m_p = state;
				return;
			}
			if ("a" == tagName && TagState::opened == state)	// update the a tag at openning only
			{
				m_a = state;
				return;
			}
			if ("td" == tagName)
			{
				m_td = state;
				return;
			}
			if ("tr" == tagName)
			{
				m_tr = state;
				return;
			}
			if ("table" == tagName)
			{
				m_table = state;
				return;
			}
			if ("label" == tagName && TagState::opened == state)
			{
				m_label = state;
				return;
			}
		}
		// return false if the current closing tag should remain at the same level
		bool CloseParagraphes(const std::string& tagName)
		{
			if ("p" == tagName)
			{
				if (TagState::opened == m_p)
					m_p = TagState::closed;
				else
					return false;
			}
			if ("a" == tagName)
			{
				if (TagState::opened == m_a)
					m_a = TagState::closed;
				else
					return false;
			}
			if ("label" == tagName)
			{
				if (TagState::opened == m_label)
					m_label = TagState::closed;
				else
					return false;
			}
			return true;
		}
		// restore the current table state
		void RestoreCurrentTable()
		{
			m_table = m_tables.top().m_table;
			m_tr = m_tables.top().m_tr;
			m_td = m_tables.top().m_td;
			m_tables.pop();
		}
		// skip the current tag
		void SkipCurrentTag() { while (m_bufferIndex < m_data.length() && '>' != m_data[m_bufferIndex])
				m_bufferIndex++;
			if ('>' == m_data[m_bufferIndex] || m_bufferIndex >= m_data.length())
				m_bufferIndex++;
		}

	private:
		struct TableState
		{
			TagState m_table{ TagState::closed };
			TagState m_tr{ TagState::closed };
			TagState m_td{ TagState::closed };
		};

	private:
		std::string m_data{};
		std::stack<TableState> m_tables;
		std::vector<std::shared_ptr<Tag>> m_tags{};
		size_t m_bufferIndex{};
		Tag* m_currentTag{};

	// tags for correctness
	private:
		TagState m_td{ TagState::closed };
		TagState m_tr{ TagState::closed };
		TagState m_table{ TagState::closed };
		TagState m_p{ TagState::closed };	// paragraph tag
		TagState m_a{ TagState::closed };
		TagState m_label{ TagState::closed };

	// multilines tags
	private:
		bool m_svg{ false };
		bool m_style{ false };
		bool m_script{ false };
	};
}
