#ifndef __ATL_STRUTILS_H__
#define __ATL_STRUTILS_H__

/*!  
*  @file	atl_str_utils.h
*  @brief	Basic string utilities
*  @author	syounan
*  @sa		ATWILC_OSWrapper.h top level OS wrapper file
*  @date	16 Aug 2010
*  @version	1.0
*/

#ifndef CONFIG_ATL_STRING_UTILS
#error the feature CONFIG_ATL_STRING_UTILS must be supported to include this file
#endif

/*!
*  @brief	Compares two memory buffers
*  @param[in]	pvArg1 pointer to the first memory location
*  @param[in]	pvArg2 pointer to the second memory location
*  @param[in]	u32Count the size of the memory buffers
*  @return	0 if the 2 buffers are equal, 1 if pvArg1 is bigger than pvArg2,
		-1 if pvArg1 smaller than pvArg2
*  @note	this function repeats the functionality of standard memcmp
*  @author	syounan
*  @date	18 Aug 2010
*  @version	1.0
*/
ATL_Sint32 ATL_memcmp(const void* pvArg1, const void* pvArg2, ATL_Uint32 u32Count);

/*!
*  @brief	Internal implementation for memory copy
*  @param[in]	pvTarget the target buffer to which the data is copied into
*  @param[in]	pvSource pointer to the second memory location
*  @param[in]	u32Count the size of the data to copy
*  @note	this function should not be used directly, use ATL_memcpy instead
*  @author	syounan
*  @date	18 Aug 2010
*  @version	1.0
*/
void ATL_memcpy_INTERNAL(void* pvTarget, const void* pvSource, ATL_Uint32 u32Count);

/*!
*  @brief	Copies the contents of a memory buffer into another
*  @param[in]	pvTarget the target buffer to which the data is copied into
*  @param[in]	pvSource pointer to the second memory location
*  @param[in]	u32Count the size of the data to copy
*  @return	ATL_SUCCESS if copy is successfully handeled
		ATL_FAIL if copy failed
*  @note	this function repeats the functionality of standard memcpy,
		however memcpy is undefined if the two buffers overlap but this 
		implementation will check for overlap and report error
*  @author	syounan
*  @date	18 Aug 2010
*  @version	1.0
*/
static ATL_ErrNo ATL_memcpy(void* pvTarget, const void* pvSource, ATL_Uint32 u32Count)
{
	if(
		(((ATL_Uint8*)pvTarget <= (ATL_Uint8*)pvSource) 
			&& (((ATL_Uint8*)pvTarget+u32Count) > (ATL_Uint8*)pvSource))

		|| (((ATL_Uint8*)pvSource <= (ATL_Uint8*)pvTarget) 
			&& (((ATL_Uint8*)pvSource+u32Count) > (ATL_Uint8*)pvTarget))
	)
	{
		/* ovelapped memory, return Error */
		return ATL_FAIL;
	}
	else
	{
		ATL_memcpy_INTERNAL(pvTarget, pvSource, u32Count);
		return ATL_SUCCESS;
	}
}

/*!
*  @brief	Sets the contents of a memory buffer with the given value
*  @param[in]	pvTarget the target buffer which contsnts will be set
*  @param[in]	u8SetValue the value to be used
*  @param[in]	u32Count the size of the memory buffer
*  @return	value of pvTarget
*  @note	this function repeats the functionality of standard memset
*  @author	syounan
*  @date	18 Aug 2010
*  @version	1.0
*/
void* ATL_memset(void* pvTarget, ATL_Uint8 u8SetValue, ATL_Uint32 u32Count);

/*!
*  @brief	Concatenates the contents of 2 strings up to a given count
*  @param[in]	pcTarget the target string, its null character will be overwritten
		and contents of pcSource will be concatentaed to it
*  @param[in]	pcSource the source string the will be concatentaed
*  @param[in]	u32Count copying will proceed until a null character in pcSource
		is encountered or u32Count of bytes copied
*  @return	value of pcTarget
*  @note	this function repeats the functionality of standard strncat
*  @author	syounan
*  @date	18 Aug 2010
*  @version	1.0
*/
ATL_Char* ATL_strncat(ATL_Char* pcTarget, const ATL_Char* pcSource,
	ATL_Uint32 u32Count); 

/*!
*  @brief	copies the contents of source string into the target string
*  @param[in]	pcTarget the target string buffer
*  @param[in]	pcSource the source string the will be copied
*  @param[in]	u32Count copying will proceed until a null character in pcSource
		is encountered or u32Count of bytes copied
*  @return	value of pcTarget
*  @note	this function repeats the functionality of standard strncpy
*  @author	syounan
*  @date	18 Aug 2010
*  @version	1.0
*/
ATL_Char* ATL_strncpy(ATL_Char* pcTarget, const ATL_Char* pcSource,
	ATL_Uint32 u32Count); 

/*!
*  @brief	Compares two strings
*  @details	Compares 2 strings reporting which is bigger, ATL_NULL is considered
		the smallest string, then a zero length string then all other 
		strings depending on thier ascii characters order
*  @param[in]	pcStr1 the first string, ATL_NULL is valid and considered smaller
		than any other non-NULL string (incliding zero lenght strings)
*  @param[in]	pcStr2 the second string, ATL_NULL is valid and considered smaller
		than any other non-NULL string (incliding zero lenght strings)
*  @return	0 if the 2 strings are equal, 1 if pcStr1 is bigger than pcStr2,
		-1 if pcStr1 smaller than pcStr2
*  @note	this function repeats the functionality of standard strcmp
*  @author	syounan
*  @date	18 Aug 2010
*  @version	1.0
*/
ATL_Sint32 ATL_strcmp(const ATL_Char* pcStr1, const ATL_Char* pcStr2);

/*!
*  @brief	Compares two strings up to u32Count characters
*  @details	Compares 2 strings reporting which is bigger, ATL_NULL is considered
		the smallest string, then a zero length string then all other 
		strings depending on thier ascii characters order with small case 
		converted to uppder case
*  @param[in]	pcStr1 the first string, ATL_NULL is valid and considered smaller
		than any other non-NULL string (incliding zero lenght strings)
*  @param[in]	pcStr2 the second string, ATL_NULL is valid and considered smaller
		than any other non-NULL string (incliding zero lenght strings)
*  @param[in]	u32Count copying will proceed until a null character in pcStr1 or
		pcStr2 is encountered or u32Count of bytes copied
*  @return	0 if the 2 strings are equal, 1 if pcStr1 is bigger than pcStr2,
		-1 if pcStr1 smaller than pcStr2
*  @author	aabozaeid
*  @date	7 Dec 2010
*  @version	1.0
*/
ATL_Sint32 ATL_strncmp(const ATL_Char* pcStr1, const ATL_Char* pcStr2,
	ATL_Uint32 u32Count); 

/*!
*  @brief	Compares two strings ignoring the case of its latin letters
*  @details	Compares 2 strings reporting which is bigger, ATL_NULL is considered
		the smallest string, then a zero length string then all other 
		strings depending on thier ascii characters order with small case 
		converted to uppder case
*  @param[in]	pcStr1 the first string, ATL_NULL is valid and considered smaller
		than any other non-NULL string (incliding zero lenght strings)
*  @param[in]	pcStr2 the second string, ATL_NULL is valid and considered smaller
		than any other non-NULL string (incliding zero lenght strings)
*  @return	0 if the 2 strings are equal, 1 if pcStr1 is bigger than pcStr2,
		-1 if pcStr1 smaller than pcStr2
*  @author	syounan
*  @date	1 Nov 2010
*  @version	2.0
*/
ATL_Sint32 ATL_strcmp_IgnoreCase(const ATL_Char* pcStr1, const ATL_Char* pcStr2);

/*!
*  @brief	Compares two strings ignoring the case of its latin letters up to 
*		u32Count characters
*  @details	Compares 2 strings reporting which is bigger, ATL_NULL is considered
		the smallest string, then a zero length string then all other 
		strings depending on thier ascii characters order with small case 
		converted to uppder case
*  @param[in]	pcStr1 the first string, ATL_NULL is valid and considered smaller
		than any other non-NULL string (incliding zero lenght strings)
*  @param[in]	pcStr2 the second string, ATL_NULL is valid and considered smaller
		than any other non-NULL string (incliding zero lenght strings)
*  @param[in]	u32Count copying will proceed until a null character in pcStr1 or
		pcStr2 is encountered or u32Count of bytes copied
*  @return	0 if the 2 strings are equal, 1 if pcStr1 is bigger than pcStr2,
		-1 if pcStr1 smaller than pcStr2
*  @author	aabozaeid
*  @date	7 Dec 2010
*  @version	1.0
*/
ATL_Sint32 ATL_strncmp_IgnoreCase(const ATL_Char* pcStr1, const ATL_Char* pcStr2,
	ATL_Uint32 u32Count); 

/*!
*  @brief	gets the length of a string
*  @param[in]	pcStr the string
*  @return	the length
*  @note	this function repeats the functionality of standard strlen
*  @author	syounan
*  @date	18 Aug 2010
*  @version	1.0
*/
ATL_Uint32 ATL_strlen(const ATL_Char* pcStr); 

/*!
*  @brief	convert string to integer
*  @param[in]	pcStr the string
*  @return	the value of string
*  @note	this function repeats the functionality of the libc atoi
*  @author	bfahmy
*  @date	28 Aug 2010
*  @version	1.0
*/
ATL_Sint32 ATL_strtoint(const ATL_Char* pcStr);

/*!
*  @brief	print a formatted string into a buffer
*  @param[in]	pcTarget the buffer where the resulting string is written
*  @param[in]	u32Size size of the output beffer including the \0 terminating
		character
*  @param[in]	pcFormat format of the string
*  @return	number of character written or would have been written if the
		string were not truncated
*  @note	this function repeats the functionality of standard snprintf
*  @author	syounan
*  @date	1 Nov 2010
*  @version	2.0
*/
ATL_Sint32 ATL_snprintf(ATL_Char* pcTarget, ATL_Uint32 u32Size, 
	const ATL_Char* pcFormat, ...);


#ifdef CONFIG_ATL_EXTENDED_STRING_OPERATIONS


/**
*  @brief 		
*  @details 	Searches for the first occurrence of the character c in the first n bytes
				of the string pointed to by the argument str.
				Returns a pointer pointing to the first matching character,
				or null if no match was found.
*  @param[in] 	
*  @return 		 
*  @note 		
*  @author		remil
*  @date		3 Nov 2010
*  @version		1.0
*/
ATL_Char* ATL_memchr(const void *str, ATL_Char c, ATL_Sint32 n);

/**
*  @brief 		
*  @details 	Searches for the first occurrence of the character c (an unsigned char)
				in the string pointed to by the argument str. 
				The terminating null character is considered to be part of the string.
				Returns a pointer pointing to the first matching character, 
				or null if no match was found.
*  @param[in] 	
*  @return 		 
*  @note 		
*  @author		remil
*  @date		3 Nov 2010
*  @version		1.0
*/
ATL_Char* ATL_strchr(const ATL_Char *str, ATL_Char c);

/**
*  @brief 		
*  @details 	Appends the string pointed to by str2 to the end of the string pointed to by str1.
				The terminating null character of str1 is overwritten. 
				Copying stops once the terminating null character of str2 is copied. If overlapping occurs, the result is undefined.
				The argument str1 is returned.
*  @param[in] 	ATL_Char* str1,
*  @param[in] 	ATL_Char* str2,
*  @return 		ATL_Char* 
*  @note 		
*  @author		remil
*  @date		3 Nov 2010
*  @version		1.0
*/
ATL_Char* ATL_strcat(ATL_Char *str1, const ATL_Char *str2);


/**
*  @brief 		
*  @details 	Copy pcSource to pcTarget
*  @param[in] 	ATL_Char* pcTarget
*  @param[in] 	const ATL_Char* pcSource
*  @return 		ATL_Char* 
*  @note 		
*  @author		remil
*  @date		3 Nov 2010
*  @version		1.0
*/
ATL_Char* ATL_strcpy(ATL_Char* pcTarget, const ATL_Char* pcSource);



/**
*  @brief 		
*  @details 	Finds the first sequence of characters in the string str1 that
				does not contain any character specified in str2.
				Returns the length of this first sequence of characters found that 
				do not match with str2.
*  @param[in] 	const ATL_Char *str1
*  @param[in] 	const ATL_Char *str2
*  @return 		ATL_Uint32
*  @note 		
*  @author		remil
*  @date		3 Nov 2010
*  @version		1.0
*/
ATL_Uint32 ATL_strcspn(const ATL_Char *str1, const ATL_Char *str2);


/**
*  @brief 		
*  @details 	Searches an internal array for the error number errnum and returns a pointer
				to an error message string.
				Returns a pointer to an error message string.
*  @param[in] 	ATL_Sint32 errnum
*  @return 		ATL_Char*
*  @note 		
*  @author		remil
*  @date		3 Nov 2010
*  @version		1.0
*/
ATL_Char* ATL_strerror(ATL_Sint32 errnum);

/**
*  @brief 		
*  @details 	Finds the first occurrence of the entire string str2 
				(not including the terminating null character) which appears in the string str1.
				Returns a pointer to the first occurrence of str2 in str1. 
				If no match was found, then a null pointer is returned. 
				If str2 points to a string of zero length, then the argument str1 is returned.
*  @param[in] 	const ATL_Char *str1
*  @param[in] 	const ATL_Char *str2
*  @return 		ATL_Char*
*  @note 		
*  @author		remil
*  @date		3 Nov 2010
*  @version		1.0
*/
ATL_Char* ATL_strstr(const ATL_Char *str1, const ATL_Char *str2);

/**
*  @brief 		
*  @details 	Searches for the first occurrence of the character c (an unsigned char)
				in the string pointed to by the argument str. 
				The terminating null character is considered to be part of the string.
				Returns a pointer pointing to the first matching character, 
				or null if no match was found.
*  @param[in] 	
*  @return 		 
*  @note 		
*  @author		remil
*  @date		3 Nov 2010
*  @version		1.0
*/
ATL_Char* ATL_strchr(const ATL_Char *str, ATL_Char c);


/**
*  @brief 		
*  @details 	Parses the C string str interpreting its content as a floating point 
				number and returns its value as a double. 
				If endptr is not a null pointer, the function also sets the value pointed
				by endptr to point to the first character after the number.
*  @param[in] 	const ATL_Char* str
*  @param[in] 	ATL_Char** endptr
*  @return 		ATL_Double
*  @note 		
*  @author		remil
*  @date		11 Nov 2010
*  @version		1.0
*/
ATL_Double ATL_StringToDouble(const ATL_Char* str,
							  ATL_Char** endptr);


/**
*  @brief 		Parses the C string str interpreting its content as an unsigned integral
				number of the specified base, which is returned as an unsigned long int value.
*  @details 	The function first discards as many whitespace characters as necessary
				until the first non-whitespace character is found. 
				Then, starting from this character, takes as many characters as possible
				that are valid following a syntax that depends on the base parameter,
				and interprets them as a numerical value. 
				Finally, a pointer to the first character following the integer
				representation in str is stored in the object pointed by endptr.
*  @param[in] 	const ATL_Char *str
*  @param[in]	ATL_Char **endptr
*  @param[in]	ATL_Sint32 base
*  @return 		ATL_Uint32
*  @note 		
*  @author		remil
*  @date		11 Nov 2010
*  @version		1.0
*/
ATL_Uint32 ATL_StringToUint32(const ATL_Char *str,
							  ATL_Char **endptr,
							  ATL_Sint32 base);



#endif

#endif
