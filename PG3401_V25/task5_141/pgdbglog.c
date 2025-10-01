#include "pgdbglog.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

void PgDbgLogger(unsigned long ulErrorType, int iLine,
		const char *szFile, const char *pszFormat, ...) {
			static FILE *fLogFile = NULL;
			static int iCallCounter = 1;

	if (fLogFile == NULL) {
		char szFileName[256] = {0};
		time_t tTimeAndDate = time(NULL);
		snprintf(szFileName, sizeof(szFileName) - 1, "debug_%1d.txt", tTimeAndDate);
		fLogFile = fopen(szFileName, "w");
		if (!fLogFile) return;
	}

	const char *pszType = (ulErrorType == 1) ? "Error" : "Debug";
	char szOutputString[256] = {0};

	va_list vaArgumentPointer;
	va_start(vaArgumentPointer, pszFormat);
	vsnprintf(szOutputString, sizeof(szOutputString) - 1, pszFormat, vaArgumentPointer);
	va_end(vaArgumentPointer);

	fprintf(fLogFile, "%04i: %s [%s:%d] %s\n",
		iCallCounter++, pszType, szFile, iLine, szOutputString);
}
