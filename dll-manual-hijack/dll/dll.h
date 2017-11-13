//Setting export-import header
#ifdef DLL_IMPORT
#define DLL_IE __declspec(dllimport)
#else
#define DLL_IE __declspec(dllexport)
#endif

typedef struct _MESSAGE {
	char *msg;
} MSG, *PMSG;

void DLL_IE send(PMSG pMsg);