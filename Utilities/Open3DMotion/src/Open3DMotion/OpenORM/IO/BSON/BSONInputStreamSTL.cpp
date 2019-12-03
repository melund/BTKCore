/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "BSONInputStreamSTL.h"

namespace Open3DMotion
{
	BSONInputStreamSTL::BSONInputStreamSTL(std::istream& _input) :
		input(_input)
	{
	}

	void BSONInputStreamSTL::SkipBytes(UInt32 count)  throw(BSONReadException)
	{
    size_t start_pos = static_cast<size_t>(input.tellg());
		input.seekg(count, std::ios::cur);
		if (input.fail() || (static_cast<size_t>(input.tellg()) != (start_pos+count)))
			throw BSONReadException("error skipping bytes in BSON stream");
	}

	void BSONInputStreamSTL::ReadBinary(void* binary, UInt32 size)  throw(BSONReadException)
	{
		input.read((char*)binary, size);
		if (input.fail())
			throw BSONReadException("error reading from BSON stream");
	}

}
