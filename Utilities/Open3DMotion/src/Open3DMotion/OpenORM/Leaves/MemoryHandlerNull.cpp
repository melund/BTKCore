/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Leaves/MemoryHandlerNull.h"
#include <stdlib.h>

namespace Open3DMotion
{
	DEFINE_CLASS_NAME(MemoryHandlerNull);

	void MemoryHandlerNull::Free()
	{
	}

	size_t MemoryHandlerNull::SizeBytes() const
	{
		return 0;
	}

	UInt8* MemoryHandlerNull::Data() const
	{
		return NULL;
	}

	MemoryHandler* MemoryHandlerNull::Clone() const
	{
		return new MemoryHandlerNull();
	}
}
