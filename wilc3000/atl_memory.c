
#include "atl_os_wrapper.h"

#ifdef CONFIG_ATL_MEMORY_FEATURE


/*!
*  @author	syounan
*  @date	18 Aug 2010
*  @version	1.0
*/
void* ATL_MemoryAlloc(ATL_Uint32 u32Size, tstrATL_MemoryAttrs* strAttrs,
	ATL_Char* pcFileName, ATL_Uint32 u32LineNo)
{
	if(u32Size > 0)
	{
		return kmalloc(u32Size, GFP_ATOMIC);
	}
	else
	{
		return ATL_NULL;
	}
}

/*!
*  @author	syounan
*  @date	18 Aug 2010
*  @version	1.0
*/
void* ATL_MemoryCalloc(ATL_Uint32 u32Size, tstrATL_MemoryAttrs* strAttrs,
	ATL_Char* pcFileName, ATL_Uint32 u32LineNo)
{
	return kcalloc(u32Size, 1,GFP_KERNEL);
}

/*!
*  @author	syounan
*  @date	18 Aug 2010
*  @version	1.0
*/
void* ATL_MemoryRealloc(void* pvOldBlock, ATL_Uint32 u32NewSize, 
	tstrATL_MemoryAttrs* strAttrs, ATL_Char* pcFileName, ATL_Uint32 u32LineNo)
{
	if(u32NewSize==0)
	{
		kfree(pvOldBlock);
		return ATL_NULL;
	}
	else if(pvOldBlock==ATL_NULL)
	{
		return kmalloc(u32NewSize, GFP_KERNEL);
	}
	else
	{
		return krealloc(pvOldBlock, u32NewSize, GFP_KERNEL);
	}

}

/*!
*  @author	syounan
*  @date	18 Aug 2010
*  @version	1.0
*/
void ATL_MemoryFree(void* pvBlock, tstrATL_MemoryAttrs* strAttrs,
	ATL_Char* pcFileName, ATL_Uint32 u32LineNo)
{
	kfree(pvBlock);
}

#endif
