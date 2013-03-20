#include "AssertDefine.h"

#ifdef  __WINDOWS

void __cdecl AssertMessageBox( const char* expr )
{
	int nCode = MessageBox(NULL, expr, "Assert", MB_ABORTRETRYIGNORE|MB_ICONHAND|MB_SETFOREGROUND|MB_TASKMODAL);

	/* Abort: abort the program */
	if (nCode == IDABORT)
	{
		/* raise abort signal */
		raise(SIGABRT);

		/* We usually won't get here, but it's possible that
		SIGABRT was ignored.  So exit the program anyway. */

		_exit(3);
	}

	/* Retry: call the debugger */
	if (nCode == IDRETRY)
	{
		_DbgBreak();
		/* return to user code */
		return;
	}

	/* Ignore: continue execution */
	if (nCode == IDIGNORE)
		return;

	abort();
}

#endif

