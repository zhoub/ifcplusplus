/* -*-c++-*- IfcQuery www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include "ifcpp/model/GlobalDefines.h"
#include "ifcpp/model/BuildingObject.h"
#include "WriterUtil.h"

void appendRealWithoutTrailingZeros(std::stringstream& stream, const double number)
{
	std::ostringstream temp;
	temp.imbue(std::locale("C"));
	temp.precision(15);
	temp << std::fixed << number;
	std::string str = temp.str();
	size_t pos_dot = str.find_last_of('.');
	if (pos_dot != std::string::npos)
	{
		// 1.000 -> 1.
		size_t pos_last_non_zero = str.find_last_not_of('0');
		if (pos_last_non_zero != std::string::npos && pos_last_non_zero >= pos_dot)
		{
			str.erase(pos_last_non_zero + 1, std::string::npos);
		}
	}
	stream << str;
}

void writeIntList(std::stringstream& stream, const std::vector<int>& vec)
{
	// example: (3,23,039)
	if (vec.size() == 0)
	{
		stream << "$";
		return;
	}
	stream << "(";
	for (size_t ii = 0; ii < vec.size(); ++ii)
	{
		if (ii > 0)
		{
			stream << ",";
		}

		stream << vec[ii];
	}
	stream << ")";
}

void writeIntList2D(std::stringstream& stream, const std::vector<std::vector<int> >& vec)
{
	// example: ((1,2,4),(3,23,039),(938,3,-3,6))
	if (vec.size() == 0)
	{
		stream << "$";
		return;
	}

	stream << "(";
	for (size_t ii = 0; ii < vec.size(); ++ii)
	{
		const std::vector<int>& inner_vec = vec[ii];
		if (ii > 0)
		{
			stream << ",";
		}
		writeIntList(stream, inner_vec);
	}
	stream << ")";
}

void writeIntList3D(std::stringstream& stream, const std::vector<std::vector<std::vector<int> > >& vec)
{
	// example: ((1,2,4),(3,23,39),(938,3,-35,6))
	if (vec.size() == 0)
	{
		stream << "$";
		return;
	}

	stream << "(";
	for (size_t ii = 0; ii < vec.size(); ++ii)
	{
		const std::vector<std::vector<int> >& inner_vec = vec[ii];
		if (ii > 0)
		{
			stream << ",";
		}
		writeIntList2D(stream, inner_vec);
	}
	stream << ")";
}

void writeRealList(std::stringstream& stream, const std::vector<double>& vec)
{
	// example: (3,23,039)
	if (vec.size() == 0)
	{
		stream << "$";
		return;
	}
	stream << "(";
	for (size_t ii = 0; ii < vec.size(); ++ii)
	{
		if (ii > 0)
		{
			stream << ",";
		}

		appendRealWithoutTrailingZeros(stream, vec[ii]);
	}
	stream << ")";
}
void writeRealList2D(std::stringstream& stream, const std::vector<std::vector<double> >& vec)
{
	// example: ((1,2,4),(3,23,039),(938,3,-3,6))
	if (vec.size() == 0)
	{
		stream << "$";
		return;
	}

	stream << "(";
	for (size_t ii = 0; ii < vec.size(); ++ii)
	{
		const std::vector<double>& inner_vec = vec[ii];
		if (ii > 0)
		{
			stream << ",";
		}
		writeRealList(stream, inner_vec);
	}
	stream << ")";
}

void writeRealList3D(std::stringstream& stream, const std::vector<std::vector<std::vector<double> > >& vec)
{
	// example: ((1,2,4),(3,23,39),(938,3,-35,6))
	if (vec.size() == 0)
	{
		stream << "$";
		return;
	}

	stream << "(";
	for (size_t ii = 0; ii < vec.size(); ++ii)
	{
		const std::vector<std::vector<double> >& inner_vec = vec[ii];
		if (ii > 0)
		{
			stream << ",";
		}
		writeRealList2D(stream, inner_vec);
	}
	stream << ")";
}


std::string encodeStepString( const std::wstring& str )
{
	wchar_t* stream_pos = const_cast<wchar_t*>(str.c_str());
	std::string result_str;
	std::string beginUnicodeTag = "\\X2\\";
	std::string endUnicodeTag = "\\X0\\";
	bool hasOpenedUnicodeTag = false;

	auto closeUnicodeBlockIfOpened = [&]
	{
		if( hasOpenedUnicodeTag )
		{
			result_str += endUnicodeTag;
			hasOpenedUnicodeTag = false;
		}
	};

	while( *stream_pos != '\0' )
	{
		wchar_t append_char = *stream_pos;
		if (append_char == 9)
		{
			closeUnicodeBlockIfOpened();
			// encode tab
			result_str.append("\\X\\09");
		}
		else if( append_char == 10 )
		{
			closeUnicodeBlockIfOpened();
			// encode new line
			result_str.append( "\\X\\0A" );
		}
		else if( append_char == 13 )
		{
			closeUnicodeBlockIfOpened();
			// encode carriage return
			result_str.append( "\\X\\0D" );
		}
		else if( append_char == 39 )
		{
			closeUnicodeBlockIfOpened();
			// encode apostrophe
			result_str.append( "\\X\\27" );
		}
		else if( append_char == 92 )
		{
			closeUnicodeBlockIfOpened();
			// encode backslash
			result_str.append( "\\\\" );
		}
		else if( append_char > 0 && append_char < 128 )
		{
			closeUnicodeBlockIfOpened();

			result_str.push_back( static_cast<char>(append_char) );
		}
		else
		{
			int value = static_cast<int>(append_char);
			wchar_t temporary[8];
			swprintf( temporary, 5, L"%04X", value );

			if( !hasOpenedUnicodeTag )
			{
				result_str += beginUnicodeTag;
				hasOpenedUnicodeTag = true;
			}

			char mb[8];
			wctomb( mb, temporary[0] );
			result_str.push_back( mb[0] );
			wctomb( mb, temporary[1] );
			result_str.push_back( mb[0] );
			wctomb( mb, temporary[2] );
			result_str.push_back( mb[0] );
			wctomb( mb, temporary[3] );
			result_str.push_back( mb[0] );
		}
		++stream_pos;
	}

	closeUnicodeBlockIfOpened();

	return result_str;
}
