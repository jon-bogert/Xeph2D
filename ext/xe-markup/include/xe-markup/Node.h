#pragma once

#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
#include <type_traits>

namespace xe::Markup
{
	class Node final
	{
	public:
		Node() = default;
		~Node()
		{
			Clear();
		}

		Node& operator=(const Node& other)
		{
			Clear();
			//m_key = other.m_key;
			if (other.m_value != nullptr)
			{
				m_value = new std::string;
				*m_value = *other.m_value;
			}
			if (other.m_array != nullptr)
			{
				m_array = new std::vector<Node>;
				*m_array = *other.m_array;
			}
			if (other.m_children != nullptr)
			{
				m_children = new std::vector<Node>;
				*m_children = *other.m_children;
			}
			return *this;
		}

		template <typename T>
		typename std::enable_if<std::is_same<bool, T>::value, Node&>::type
		operator=(const T& other)
		{
			Clear();

			m_value = new std::string;

			*m_value = (other) ? "true" : "false";
			return *this;
		}

		template <typename T>
		typename std::enable_if<std::is_same<char, T>::value || std::is_base_of_v<std::string, T>, Node&>::type
		operator=(const T& other)
		{
			Clear();
			m_value = new std::string;
			*m_value = other;
			return *this;
		}

		template <typename T>
		typename std::enable_if<(std::is_integral_v<T> || std::is_floating_point_v<T>)
			&& !std::is_same<bool, T>::value && !std::is_same<char, T>::value, Node&>::type
			operator=(const T& other)
		{
			Clear();

			m_value = new std::string;
			*m_value = std::to_string(other);
			TrimFloat();
			return *this;
		}
		
		//Move Assignment
		Node& operator=(Node&& other) noexcept
		{
			if (&other == this)
				return *this;

			Clear();
			//m_key = other.m_key;
			if (other.m_value != nullptr)
			{
				m_value = new std::string;
				*m_value = *other.m_value;
			}
			if (other.m_array != nullptr)
			{
				m_array = new std::vector<Node>;
				*m_array = *other.m_array;
			}
			if (other.m_children != nullptr)
			{
				m_children = new std::vector<Node>;
				*m_children = *other.m_children;
			}

			other.Clear();
			other.m_key = "";
			return *this;
		}

		Node(const Node& other)
		{
			if (&other == this)
				return;

			Clear();
			m_key = other.m_key;
			if (other.m_value != nullptr)
			{
				m_value = new std::string;
				*m_value = *other.m_value;
			}
			if (other.m_array != nullptr)
			{
				m_array = new std::vector<Node>;
				*m_array = *other.m_array;
			}
			if (other.m_children != nullptr)
			{
				m_children = new std::vector<Node>;
				*m_children = *other.m_children;
			}
		}

		Node(Node&& other) noexcept
		{
			if (&other == this)
				return;

			Clear();
			m_key = other.m_key;
			if (other.m_value != nullptr)
			{
				m_value = new std::string;
				*m_value = *other.m_value;
			}
			if (other.m_array != nullptr)
			{
				m_array = new std::vector<Node>;
				*m_array = *other.m_array;
			}
			if (other.m_children != nullptr)
			{
				m_children = new std::vector<Node>;
				*m_children = *other.m_children;
			}

			other.Clear();
			other.m_key = "";
		}

		class iterator
		{
			std::vector<Node>::iterator iter;

		public:
			iterator(std::vector<Node>::iterator iter) : iter(iter) {}

			Node& operator*()
			{
				return *iter;
			}

			iterator& operator++()
			{
				++iter;
				return *this;
			}

			bool operator!=(const iterator& other) const
			{
				return iter != other.iter;
			}

			bool operator==(const iterator& other) const
			{
				return iter == other.iter;
			}
		};

		class const_iterator
		{
			std::vector<Node>::const_iterator iter;

		public:
			const_iterator(std::vector<Node>::const_iterator iter) : iter(iter) {}

			const Node& operator*() const
			{
				return *iter;
			}

			const_iterator& operator++()
			{
				++iter;
				return *this;
			}

			bool operator!=(const const_iterator& other) const
			{
				return iter != other.iter;
			}

			bool operator==(const const_iterator& other) const
			{
				return iter == other.iter;
			}
		};

		iterator begin()
		{
			if (IsValue())
				throw std::exception("node is not of collection type");
			if (IsArray())
			{
				auto iter = iterator(m_array->begin());
				return iterator(iter);
			}
			return iterator(m_children->begin());
		}

		iterator end()
		{
			if (IsValue())
				throw std::exception("node is not of collection type");
			if (IsArray())
				return iterator(m_array->end());
			return iterator(m_children->end());
		}

		const_iterator begin() const
		{
			if (IsValue())
				throw std::exception("node is not of collection type");
			if (IsArray())
				return const_iterator(m_array->begin());
			return const_iterator(m_children->begin());
		}

		const_iterator end() const
		{
			if (IsValue())
				throw std::exception("node is not of collection type");
			if (IsArray())
				return const_iterator(m_array->end());
			return const_iterator(m_children->end());
		}

		void Clear()
		{
			delete m_value;
			delete m_array;
			delete m_children;

			m_value = nullptr;
			m_array = nullptr;
			m_children = nullptr;
		}

		bool IsDefined() const
		{
			return (m_value != nullptr
				|| m_children != nullptr
				|| m_array != nullptr);
		}

		bool IsArray() const
		{
			return m_array != nullptr;	
		}

		bool IsObject() const
		{
			return m_children != nullptr;
		}

		bool IsValue() const
		{
			return m_value != nullptr;
		}

		bool HasKey() const
		{
			return !m_key.empty();
		}

		std::string Key() const
		{
			return m_key;
		}

		bool IsNumeric() const
		{
			if (!IsValue())
				return false;

			std::regex pattern("^-?\\d+(\\.\\d+)?$");
			return std::regex_match(*m_value, pattern);
		}

		bool IsDecimal() const
		{
			if (!IsValue())
				return false;

			std::regex pattern("^-?\\d+\\.\\d+$");
			return std::regex_match(*m_value, pattern);
		}

		bool IsIntegral() const
		{
			if (!IsValue())
				return false;

			std::regex pattern("^-?\\d+$");
			return std::regex_match(*m_value, pattern);
		}

		bool IsBool() const
		{
			if (!IsValue())
				return false;

			return *m_value == "true" || *m_value == "false";
		}

		bool IsChar() const
		{
			if (!IsValue())
				return false;

			return m_value->length() == 1;
		}

		//As bool
		template <typename T>
		typename std::enable_if<std::is_same<bool, T>::value, T>::type
		As() const
		{
			if (!IsValue())
				throw std::exception("Node is not of value type");

			if (!IsBool())
				throw std::exception("Node is not of bool type");
			return *m_value == "true";
		}

		//As char
		template <typename T>
		typename std::enable_if<std::is_same<char, T>::value, T>::type
		As() const
		{
			if (!IsValue())
				throw std::exception("Node is not of value type");

			if (!IsChar())
				throw std::exception("Node is not of bool type");
			return (*m_value)[0];
		}

		//As Numeric
		template <typename T>
		typename std::enable_if<(std::is_integral_v<T> || std::is_floating_point_v<T>)
			&& !std::is_same<bool, T>::value && !std::is_same<char, T>::value, T>::type
		As() const
		{
			if (!IsValue())
				throw std::exception("Node is not of value type");

			std::stringstream stream(*m_value);
			T result;

			stream >> result;
			return result;
		}

		//As String
		template <typename T>
		typename std::enable_if<std::is_base_of_v<std::string, T>, T>::type
		As() const
		{
			if (!IsValue())
				throw std::exception("Node is not of value type");

			return *m_value;
		}

		void PushBack(const Node& node)
		{
			if (IsValue() && IsObject())
				throw std::exception("Node is not of type Array");
			if (!IsArray())
			{
				m_array = new std::vector<Node>;
			}

			m_array->push_back(node);
		}

		//Trims all nodes that are not defined
		void Trim()
		{
			if (!IsDefined() || IsValue())
				return;

			if (IsArray())
			{
				for (auto iter = m_array->begin(); iter != m_array->end();)
				{
					if (iter->IsDefined())
					{
						iter->Trim();
						++iter;
					}
					else
					{
						iter = m_array->erase(iter);
					}
				}
			}
			else //IsObject
			{
				for (auto iter = m_children->begin(); iter != m_children->end();)
				{
					if (iter->IsDefined())
					{
						iter->Trim();
						++iter;
					}
					else
					{
						iter = m_children->erase(iter);
					}
				}
			}
		}

		size_t Size() const
		{
			if (IsArray())
				return m_array->size();
			else if (IsObject())
				return m_children->size();
			else
				return 0;
		}

		Node& operator[](const std::string& key)
		{
			if (!IsDefined())
			{
				Clear();
				m_children = new std::vector<Node>;
			}
			if (!IsObject())
				throw std::exception("node is not of object type");

			Node* node = FindKey(key);
			if (node == nullptr)
			{
				node = &m_children->emplace_back();
				node->m_key = key;
			}
			return *node;
		}

		Node& operator[](const size_t index)
		{
			if (!IsObject() && !IsArray())
				throw std::exception("node is not an indxed type");
			if (index >= Size())
				throw std::exception("index out of range");

			if (IsObject())
				return m_children->at(index);
			else
				return m_array->at(index);
		}

		const Node& operator[](const size_t index) const
		{
			if (!IsObject() && !IsArray())
				throw std::exception("node is not an indxed type");
			if (index >= Size())
				throw std::exception("index out of range");

			if (IsObject())
				return m_children->at(index);
			else
				return m_array->at(index);
		}

		const Node& At(const std::string key) const
		{
			if (!IsObject())
				throw std::exception("node is not of object type");

			const Node* node = FindKey(key);
			if (node == nullptr)
				throw std::exception("key not found");

			return *node;
		}

		const Node& At(const size_t index) const
		{
			return operator[](index);
		}

	private:
		Node* FindKey(const std::string& key)
		{
			auto iter = std::find_if(m_children->begin(), m_children->end(),
				[&](const Node& n) { return n.m_key == key; });

			if (iter == m_children->end())
				return nullptr;
			return &(*iter);
		}

		const Node* FindKey(const std::string& key) const
		{
			const auto iter = std::find_if(m_children->begin(), m_children->end(),
				[&](const Node& n) { return n.m_key == key; });

			if (iter == m_children->end())
				return nullptr;
			return &(*iter);
		}

		void TrimFloat()
		{
			if (IsIntegral())
				return;

			while (!m_value->empty())
			{
				if (m_value->back() == '.')
				{
					m_value->pop_back();
					break;
				}
				if (m_value->back() == '0')
				{
					m_value->pop_back();
				}
				else
				{
					break;
				}
			}
		}

		std::string m_key = "";
		std::string* m_value = nullptr;
		std::vector<Node>* m_array = nullptr;
		std::vector<Node>* m_children = nullptr;
	};
}