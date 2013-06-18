#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <assert.h>

///> Function call with parameters that may be unsafe - this call relies
///> on the caller to check that the passed values are correct. To disable
///> this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how
///> to use Visual C++ 'Checked Iterators'
#pragma warning(disable:4996)

///> 该文件包含不能在当前代码页(936)中表示的字符。
///> 请将该文件保存为 Unicode 格式以防止数据丢失
#pragma warning(disable:4819)

///> “初始化”: 从“std::streamsize”转换到“size_t”，可能丢失数据
#pragma warning(disable:4244)

///> zpublic！
#include "zpublic.hpp"

///> 标准容器
#include <array>
#include <string>
#include <vector>
#include <map>
#include <list>
using std::string;

///> 输入输出相关
#include <iomanip>
#include <iostream>
using std::cout;
using std::endl;

///> 智能指针
#include <memory>

///> 正则表达式
#include <regex>

///> 原子操作
#include <atomic>

///> 时间
#include <chrono>
#include <ratio>
#include <ctime>

///> 位操作
#include <bitset>

///> Conversion
///> Polymorphic and lexical casts.
#include <boost/cast.hpp>
#include <boost/lexical_cast.hpp>

///> String Algo
///> String algorithms library.
#include <boost/algorithm/string.hpp>

///> Format
///> The format library provides a class for formatting arguments 
///> according to a format-string, as does printf, but with two major 
///> differences: format sends the arguments to an internal stream, and so
///> is entirely type-safe and naturally supports all user-defined types; 
///> the ellipsis (...) can not be used correctly in the strongly typed 
///> context of format, and thus the function call with arbitrary arguments
///> is replaced by successive calls to an argument feeding operator%.
#include <boost/format.hpp>

///> Tokenizer
///> Break of a string or other character sequence into a series of tokens.
#include <boost/tokenizer.hpp>

///> Bimap
///> Bidirectional maps library for C++. With Boost.Bimap you can create
///> associative containers in which both types can be used as key.
#include <boost/bimap.hpp>

///> Circular Buffer
///> A STL compliant container also known as ring or cyclic buffer.
#include <boost/circular_buffer.hpp>

///> Dynamic Bitset
///> The dynamic_bitset class represents a set of bits. It provides
///> accesses to the value of individual bits via an operator[] and
///> provides all of the bitwise operators that one can apply to builtin
///> integers, such as operator& and operator<<. The number of bits in the
///> set is specified at runtime via a parameter to the constructor of the
///> dynamic_bitset.
#include <boost/dynamic_bitset.hpp>

///> Property Tree
///> A tree data structure especially suited to storing configuration data.
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/info_parser.hpp>

///> Variant
///> Safe, generic, stack-based discriminated union container.
#include <boost/variant.hpp>

///> Algorithm
///> A collection of useful generic algorithms.
#include <boost/algorithm/searching/boyer_moore.hpp>
#include <boost/algorithm/searching/boyer_moore_horspool.hpp>
#include <boost/algorithm/searching/knuth_morris_pratt.hpp>
#include <boost/algorithm/clamp.hpp>
#include <boost/algorithm/hex.hpp>
