#ifndef __ATL_LOG_H__
#define __ATL_LOG_H__

/* Errors will always get printed */
#define ATL_ERROR(...) do {  ATL_PRINTF("(ERR)(%s:%d) ",__ATL_FUNCTION__,__ATL_LINE__);\
	ATL_PRINTF(__VA_ARGS__); } while(0)

/* Wraning only printed if verbosity is 1 or more */	
#if (ATL_LOG_VERBOSITY_LEVEL > 0)
#define ATL_WARN(...) do { ATL_PRINTF("(WRN)");\
	ATL_PRINTF(__VA_ARGS__);  } while(0)
#else
#define ATL_WARN(...) (0)
#endif

/* Info only printed if verbosity is 2 or more */
#if (ATL_LOG_VERBOSITY_LEVEL > 1)
#define ATL_INFO(...) do {  ATL_PRINTF("(INF)");\
	ATL_PRINTF(__VA_ARGS__);  } while(0)
#else
#define ATL_INFO(...) (0)
#endif

/* Debug is only printed if verbosity is 3 or more */
#if (ATL_LOG_VERBOSITY_LEVEL > 2)
#define ATL_DBG(...) do { ATL_PRINTF("(DBG)(%s:%d) ",__ATL_FUNCTION__,__ATL_LINE__);\
	ATL_PRINTF(__VA_ARGS__);  } while(0)

#else
#define ATL_DBG(...) (0)
#endif

/* Function In/Out is only printed if verbosity is 4 or more */
#if (ATL_LOG_VERBOSITY_LEVEL > 3)
#define ATL_FN_IN do { ATL_PRINTF("(FIN) (%s:%d) \n", __ATL_FUNCTION__, __ATL_LINE__);  } while(0)
#define ATL_FN_OUT(ret) do { ATL_PRINTF("(FOUT) (%s:%d) %d.\n",__ATL_FUNCTION__,__ATL_LINE__,(ret));  } while(0)
#else
#define ATL_FN_IN (0)
#define ATL_FN_OUT(ret) (0)
#endif


#endif