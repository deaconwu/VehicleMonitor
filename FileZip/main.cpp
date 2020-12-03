#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <zip.h>
#include <unzip.h>
#include "iowin32.h"

#define FOPEN_FUNC(filename, mode) fopen64(filename, mode)
#define FTELLO_FUNC(stream) ftello64(stream)
#define FSEEKO_FUNC(stream, offset, origin) fseeko64(stream, offset, origin)

#define WRITEBUFFERSIZE (16384)
#define MAXFILENAME (256)

int main()
{
	//HMODULE hModule = LoadLibrary("zlibwapi.dll");
#ifdef _WIN64
	zipFile zf = zipOpen64("readme.zip", 2);
#else
	zlib_filefunc64_def ffunc;
	fill_win32_filefunc64(&ffunc);
	zipFile zf = zipOpen2_64("readme.zip", 2, NULL, &ffunc);
#endif

	if (zf == NULL)
	{
		printf("zipOpen error\n");
	}

	zip_fileinfo zi;
	zi.tmz_date.tm_sec = zi.tmz_date.tm_min = zi.tmz_date.tm_hour =
		zi.tmz_date.tm_mday = zi.tmz_date.tm_mon = zi.tmz_date.tm_year = 0;
	zi.dosDate = 0;
	zi.internal_fa = 0;
	zi.external_fa = 0;

	int err = zipOpenNewFileInZip(zf, "readme.txt", &zi,
		NULL, 0, NULL, 0, NULL,
		Z_DEFLATED,
		Z_DEFAULT_COMPRESSION);

	if (err == ZIP_OK)
	{
		FILE * fin = FOPEN_FUNC("readme.txt", "rb");
		void* buf = NULL;
		int size_buf = 0;
		int size_read = 0;

		size_buf = WRITEBUFFERSIZE;
		buf = (void*)malloc(size_buf);
		if (buf == NULL)
		{
			printf("Error allocating memory\n");
			return ZIP_INTERNALERROR;
		}

		do 
		{
			memset(buf, 0, size_buf);
			err = ZIP_OK;

			size_read = (int)fread(buf, 1, size_buf, fin);

			if (size_read < size_buf)
			{
				if (feof(fin) == 0)
				{
					printf("error in reading\n");
					err = ZIP_ERRNO;
				}
			}

			if (size_read > 0)
			{
				err = zipWriteInFileInZip(zf, buf, size_read);
				if (err < 0)
				{
					printf("error in writing in the zipfile\n");
				}
			}

		} while ((err == ZIP_OK) && (size_read > 0));

		fclose(fin);
		free(buf);
	}
	else
	{
		printf("zipOpenNewFileInZip error\n");
	}

	zipCloseFileInZip(zf);
	zipClose(zf, NULL);
	system("pause");
	return 0;
}