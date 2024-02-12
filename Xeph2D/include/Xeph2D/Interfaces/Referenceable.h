#ifndef __X2D_INTERFACES_REFERENCEABLE_H__
#define __X2D_INTERFACES_REFERENCEABLE_H__

#include <cstdint>

namespace Xeph2D
{
	class Referenceable
	{
	public:
		virtual uint32_t InstID() = 0;
	};
}

#endif //!__X2D_INTERFACES_REFERENCEABLE_H__
