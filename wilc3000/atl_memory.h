#ifndef __ATL_MEMORY_H__
#define __ATL_MEMORY_H__

/*!  
*  @file	atl_memory.h
*  @brief	Memory OS wrapper functionality
*  @author	syounan
*  @sa		ATWILC_OSWrapper.h top level OS wrapper file
*  @date	16 Aug 2010
*  @version	1.0
*/

#ifndef CONFIG_ATL_MEMORY_FEATURE
#error the feature CONFIG_ATL_MEMORY_FEATURE must be supported to include this file
#endif

/*!
*  @struct 		tstrATL_MemoryAttrs
*  @brief		Memory API options 
*  @author		syounan
*  @date		16 Aug 2010
*  @version		1.0
*/
typedef struct {
	#ifdef CONFIG_ATL_MEMORY_POOLS
	/*!< the allocation pool to use for this memory, NULL for system 
	allocation. Default is NULL
	*/
	ATL_MemoryPoolHandle* pAllocationPool;
	#endif

	/* a dummy member to avoid compiler errors*/
	ATL_Uint8 dummy;
}tstrATL_MemoryAttrs;

/*!
*  @brief	Fills the tstrATL_MemoryAttrs with default parameters
*  @param[out]	pstrAttrs structure to be filled
*  @sa		tstrATL_MemoryAttrs
*  @author	syounan
*  @date	16 Aug 2010
*  @version	1.0
*/
static void ATL_MemoryFillDefault(tstrATL_MemoryAttrs* pstrAttrs)
{
	#ifdef CONFIG_ATL_MEMORY_POOLS
	pstrAttrs->pAllocationPool = ATL_NULL;
	#endif
}

/*!
*  @brief	Allocates a given size of bytes
*  @param[in]	u32Size size of memory in bytes to be allocated
*  @param[in]	strAttrs Optional attributes, NULL for default
		if not NULL, pAllocationPool should point to the pool to use for
		this allocation. if NULL memory will be allocated directly from
		the system
*  @param[in]	pcFileName file name of the calling code for debugging
*  @param[in]	u32LineNo line number of the calling code for debugging
*  @return	The new allocated block, NULL if allocation fails
*  @note	It is recommended to use of of the wrapper macros instead of 
		calling this function directly 
*  @sa		sttrATL_MemoryAttrs
*  @sa		ATL_MALLOC
*  @sa		ATL_MALLOC_EX
*  @sa		ATL_NEW
*  @sa		ATL_NEW_EX
*  @author	syounan
*  @date	16 Aug 2010
*  @version	1.0
*/
void* ATL_MemoryAlloc(ATL_Uint32 u32Size, tstrATL_MemoryAttrs* strAttrs,
	ATL_Char* pcFileName, ATL_Uint32 u32LineNo);

/*!
*  @brief	Allocates a given size of bytes and zero filling it
*  @param[in]	u32Size size of memory in bytes to be allocated
*  @param[in]	strAttrs Optional attributes, NULL for default
		if not NULL, pAllocationPool should point to the pool to use for
		this allocation. if NULL memory will be allocated directly from
		the system
*  @param[in]	pcFileName file name of the calling code for debugging
*  @param[in]	u32LineNo line number of the calling code for debugging
*  @return	The new allocated block, NULL if allocation fails
*  @note	It is recommended to use of of the wrapper macros instead of 
		calling this function directly 
*  @sa		sttrATL_MemoryAttrs
*  @sa		ATL_CALLOC
*  @sa		ATL_CALLOC_EX
*  @sa		ATL_NEW_0
*  @sa		ATL_NEW_0_EX
*  @author	syounan
*  @date	16 Aug 2010
*  @version	1.0
*/
void* ATL_MemoryCalloc(ATL_Uint32 u32Size, tstrATL_MemoryAttrs* strAttrs,
	ATL_Char* pcFileName, ATL_Uint32 u32LineNo);

/*!
*  @brief	Reallocates a given block to a new size
*  @param[in]	pvOldBlock the old memory block, if NULL then this function 
		behaves as a new allocation function
*  @param[in]	u32NewSize size of the new memory block in bytes, if zero then
		this function behaves as a free function
*  @param[in]	strAttrs Optional attributes, NULL for default
		if pAllocationPool!=NULL and pvOldBlock==NULL, pAllocationPool 
		should point to the pool to use for this allocation. 
		if pAllocationPool==NULL and pvOldBlock==NULL memory will be 
		allocated directly from	the system
		if and pvOldBlock!=NULL, pAllocationPool will not be inspected 
		and reallocation is done from the same pool as the original block
*  @param[in]	pcFileName file name of the calling code for debugging
*  @param[in]	u32LineNo line number of the calling code for debugging
*  @return	The new allocated block, possibly same as pvOldBlock
*  @note	It is recommended to use of of the wrapper macros instead of 
		calling this function directly 
*  @sa		sttrATL_MemoryAttrs
*  @sa		ATL_REALLOC
*  @sa		ATL_REALLOC_EX
*  @author	syounan
*  @date	16 Aug 2010
*  @version	1.0
*/
void* ATL_MemoryRealloc(void* pvOldBlock, ATL_Uint32 u32NewSize, 
	tstrATL_MemoryAttrs* strAttrs, ATL_Char* pcFileName, ATL_Uint32 u32LineNo);

/*!
*  @brief	Frees given block
*  @param[in]	pvBlock the memory block to be freed
*  @param[in]	strAttrs Optional attributes, NULL for default
*  @param[in]	pcFileName file name of the calling code for debugging
*  @param[in]	u32LineNo line number of the calling code for debugging
*  @note	It is recommended to use of of the wrapper macros instead of 
		calling this function directly 
*  @sa		sttrATL_MemoryAttrs
*  @sa		ATL_FREE
*  @sa		ATL_FREE_EX
*  @sa		ATL_FREE_SET_NULL
*  @sa		ATL_FREE_IF_TRUE
*  @author	syounan
*  @date	16 Aug 2010
*  @version	1.0
*/
void ATL_MemoryFree(void* pvBlock, tstrATL_MemoryAttrs* strAttrs,
	ATL_Char* pcFileName, ATL_Uint32 u32LineNo);

/*!
*  @brief	Creates a new memory pool
*  @param[out]	pHandle the handle to the new Pool
*  @param[in]	u32PoolSize The pool size in bytes
*  @param[in]	strAttrs Optional attributes, NULL for default
*  @return	Error code indicating sucess/failure
*  @sa		sttrATL_MemoryAttrs
*  @author	syounan
*  @date	16 Aug 2010
*  @version	1.0
*/
ATL_ErrNo ATL_MemoryNewPool(ATL_MemoryPoolHandle* pHandle, ATL_Uint32 u32PoolSize,
	tstrATL_MemoryAttrs* strAttrs);

/*!
*  @brief	Deletes a memory pool, freeing all memory allocated from it as well
*  @param[in]	pHandle the handle to the deleted Pool
*  @param[in]	strAttrs Optional attributes, NULL for default
*  @return	Error code indicating sucess/failure
*  @sa		sttrATL_MemoryAttrs
*  @author	syounan
*  @date	16 Aug 2010
*  @version	1.0
*/
ATL_ErrNo ATL_MemoryDelPool(ATL_MemoryPoolHandle* pHandle, tstrATL_MemoryAttrs* strAttrs);


#ifdef CONFIG_ATL_MEMORY_DEBUG

	/*!
	@brief	standrad malloc wrapper with custom attributes
	*/
	#define ATL_MALLOC_EX(__size__, __attrs__)\
		(ATL_MemoryAlloc(\
			(__size__), __attrs__,\
			(ATL_Char*)__ATL_FILE__, (ATL_Uint32)__ATL_LINE__))

	/*!
	@brief	standrad calloc wrapper with custom attributes
	*/
	#define ATL_CALLOC_EX(__size__, __attrs__)\
		(ATL_MemoryCalloc(\
			(__size__), __attrs__,\
			(ATL_Char*)__ATL_FILE__, (ATL_Uint32)__ATL_LINE__))
	
	/*!
	@brief	standrad realloc wrapper with custom attributes
	*/
	#define ATL_REALLOC_EX(__ptr__, __new_size__, __attrs__)\
		(ATL_MemoryRealloc(\
			(__ptr__), (__new_size__), __attrs__,\
			(ATL_Char*)__ATL_FILE__, (ATL_Uint32)__ATL_LINE__))

	/*!
	@brief	standrad free wrapper with custom attributes
	*/
	#define ATL_FREE_EX(__ptr__, __attrs__)\
		(ATL_MemoryFree(\
			(__ptr__), __attrs__,\
			(ATL_Char*)__ATL_FILE__, (ATL_Uint32)__ATL_LINE__))

#else

	/*!
	@brief	standrad malloc wrapper with custom attributes
	*/
	#define ATL_MALLOC_EX(__size__, __attrs__)\
		(ATL_MemoryAlloc(\
			(__size__), __attrs__, ATL_NULL, 0))

	/*!
	@brief	standrad calloc wrapper with custom attributes
	*/
	#define ATL_CALLOC_EX(__size__, __attrs__)\
		(ATL_MemoryCalloc(\
			(__size__), __attrs__, ATL_NULL, 0))

	/*!
	@brief	standrad realloc wrapper with custom attributes
	*/
	#define ATL_REALLOC_EX(__ptr__, __new_size__, __attrs__)\
		(ATL_MemoryRealloc(\
			(__ptr__), (__new_size__), __attrs__, ATL_NULL, 0))
	/*!
	@brief	standrad free wrapper with custom attributes
	*/
	#define ATL_FREE_EX(__ptr__, __attrs__)\
		(ATL_MemoryFree(\
			(__ptr__), __attrs__, ATL_NULL, 0))

#endif

/*!
@brief	Allocates a block (with custom attributes) of given type and number of 
elements 
*/
#define ATL_NEW_EX(__struct_type__, __n_structs__, __attrs__)\
	((__struct_type__*)ATL_MALLOC_EX(\
		sizeof(__struct_type__) * (ATL_Uint32)(__n_structs__), __attrs__))

/*!
@brief	Allocates a block (with custom attributes) of given type and number of 
elements and Zero-fills it
*/
#define ATL_NEW_0_EX(__struct_type__, __n_structs__, __attrs__)\
	((__struct_type__*)ATL_CALLOC_EX(\
		sizeof(__struct_type__) * (ATL_Uint32)(__n_structs__), __attrs__))

/*!
@brief	Frees a block (with custom attributes), also setting the original pointer
to NULL
*/
#define ATL_FREE_SET_NULL_EX(__ptr__, __attrs__) do{\
	if(__ptr__ != ATL_NULL){\
		ATL_FREE_EX(__ptr__, __attrs__);\
		__ptr__ = ATL_NULL ;\
	}\
}while(0)

/*!
@brief	Frees a block (with custom attributes) if the pointer expression evaluates 
to true
*/
#define ATL_FREE_IF_TRUE_EX(__ptr__, __attrs__) do{\
	if (__ptr__ != ATL_NULL){\
		ATL_FREE_EX(__ptr__, __attrs__);\
	}\
} while(0)

/*!
@brief	standrad malloc wrapper with default attributes
*/
#define ATL_MALLOC(__size__)\
	ATL_MALLOC_EX(__size__, ATL_NULL)

/*!
@brief	standrad calloc wrapper with default attributes
*/
#define ATL_CALLOC(__size__)\
	ATL_CALLOC_EX(__size__, ATL_NULL)
	
/*!
@brief	standrad realloc wrapper with default attributes
*/
#define ATL_REALLOC(__ptr__, __new_size__)\
	ATL_REALLOC_EX(__ptr__, __new_size__, ATL_NULL)
	
/*!
@brief	standrad free wrapper with default attributes
*/
#define ATL_FREE(__ptr__)\
	ATL_FREE_EX(__ptr__, ATL_NULL)

/*!
@brief	Allocates a block (with default attributes) of given type and number of 
elements 
*/
#define ATL_NEW(__struct_type__, __n_structs__)\
	ATL_NEW_EX(__struct_type__, __n_structs__, ATL_NULL)

/*!
@brief	Allocates a block (with default attributes) of given type and number of 
elements and Zero-fills it
*/
#define ATL_NEW_0(__struct_type__, __n_structs__)\
	ATL_NEW_O_EX(__struct_type__, __n_structs__, ATL_NULL)

/*!
@brief	Frees a block (with default attributes), also setting the original pointer
to NULL
*/
#define ATL_FREE_SET_NULL(__ptr__)\
	ATL_FREE_SET_NULL_EX(__ptr__, ATL_NULL)

/*!
@brief	Frees a block (with default attributes) if the pointer expression evaluates 
to true
*/
#define ATL_FREE_IF_TRUE(__ptr__)\
	ATL_FREE_IF_TRUE_EX(__ptr__, ATL_NULL)

	
#endif

