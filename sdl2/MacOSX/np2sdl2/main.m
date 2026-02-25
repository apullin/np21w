/**
 * @file	main.m
 * @brief	���C��
 */

#include "compiler.h"
#include "../../np2.h"
#include "../../dosio.h"

/**
 * ���C��
 * @param[in] argc ����
 * @param[in] argv ����
 * @return ���U���g �R�[�h
 */
int main(int argc, char * argv[])
{
	/* For a .app bundle, bundlePath is "/path/to/Foo.app" and file_setcd
	   strips the last component to get "/path/to/" as the ROM search dir.
	   For a raw binary, bundlePath is the directory (no trailing slash),
	   so file_setcd would strip one level too many. Use argv[0] instead
	   which is the actual binary path, giving us the correct directory. */
	NSString *pstrBundlePath = [[NSBundle mainBundle] bundlePath];
	if ([pstrBundlePath hasSuffix:@".app"]) {
		file_setcd([pstrBundlePath UTF8String]);
	} else {
		file_setcd(argv[0]);
	}

	char** q = &argv[1];
	for (int i = 1; i < argc; i++)
	{
		if (strncmp(argv[i], "-psn_", 5) == 0)
		{
		}
		else if (strcasecmp(argv[i], "-NSDocumentRevisionsDebugMode") == 0)
		{
			i++;
		}
		else
		{
			*q++ = argv[i];
		}
	}
	*q = NULL;

	return np2_main((int)(q - argv), argv);
}
