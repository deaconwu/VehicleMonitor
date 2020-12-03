#include <windows.h>
#include <stdio.h>

typedef NTSTATUS(WINAPI* typedef_RtlCompressBuffer)(
	_In_  USHORT CompressionFormatAndEngine,
	_In_  PUCHAR UncompressedBuffer,
	_In_  ULONG  UncompressedBufferSize,
	_Out_ PUCHAR CompressedBuffer,
	_In_  ULONG  CompressedBufferSize,
	_In_  ULONG  UncompressedChunkSize,
	_Out_ PULONG FinalCompressedSize,
	_In_  PVOID  WorkSpace
	);

typedef NTSTATUS(WINAPI* typedef_RtlDecompressBuffer)(
	_In_  USHORT CompressionFormat,
	_Out_ PUCHAR UncompressedBuffer,
	_In_  ULONG  UncompressedBufferSize,
	_In_  PUCHAR CompressedBuffer,
	_In_  ULONG  CompressedBufferSize,
	_Out_ PULONG FinalUncompressedSize
	);

typedef NTSTATUS(WINAPI* typedef_RtlGetCompressionWorkSpaceSize)(
	_In_  USHORT CompressionFormatAndEngine,
	_Out_ PULONG CompressBufferWorkSpaceSize,
	_Out_ PULONG CompressFragmentWorkSpaceSize
	);

extern "C" _declspec(dllexport) bool DataCompress(unsigned char* pUncompressData, unsigned long dwUncompressDataLength, unsigned char** ppCompressData, unsigned long* pdwCompressDataLength)
{
	HMODULE hModule = LoadLibrary("ntdll.dll");
	if (NULL == hModule)
	{
		printf("Load dll error!\n");
		return FALSE;
	}

	//获取函数地址
	typedef_RtlGetCompressionWorkSpaceSize RtlGetCompressionWorkSpaceSize = (typedef_RtlGetCompressionWorkSpaceSize)GetProcAddress(hModule, "RtlGetCompressionWorkSpaceSize");;
	if (RtlGetCompressionWorkSpaceSize == NULL)
	{
		printf("Load function error!\n");
	}

	typedef_RtlCompressBuffer RtlCompressBuffer = (typedef_RtlCompressBuffer)::GetProcAddress(hModule, "RtlCompressBuffer");
	if (RtlCompressBuffer == NULL)
	{
		printf("Load function error!\n");
	}

	DWORD dwWorkSpaceSize = 0, dwFragmentWorkSpaceSize = 0;
	NTSTATUS status = RtlGetCompressionWorkSpaceSize(COMPRESSION_FORMAT_LZNT1 | COMPRESSION_ENGINE_STANDARD, &dwWorkSpaceSize, &dwFragmentWorkSpaceSize);
	if (status != 0)
	{
		printf("run the first function error!\n");
	}

	BYTE* pWorkSpace = new BYTE[dwWorkSpaceSize];
	if (pWorkSpace == NULL)
	{
		printf("new memory error!\n");
	}
	RtlZeroMemory(pWorkSpace, dwWorkSpaceSize);

	//用0来填充一块内存区域,第一个参数指向指针，第二个参数指向大小
	DWORD dwFinalCompressSize = 0;
	DWORD dwCompressDataLength = 4096;
	BYTE* pCompressData = NULL;

	while (TRUE)
	{
		pCompressData = new BYTE[dwCompressDataLength];
		if (pCompressData == NULL)
		{
			printf("new memory error!\n");
			break;
		}
		RtlZeroMemory(pCompressData, dwCompressDataLength);
		RtlCompressBuffer(COMPRESSION_FORMAT_LZNT1, pUncompressData, dwUncompressDataLength, pCompressData, dwCompressDataLength, 4096, &dwFinalCompressSize, (PVOID)pWorkSpace);
		if (dwCompressDataLength < dwFinalCompressSize)
		{
			if (pCompressData)
				delete[] pCompressData;
			dwCompressDataLength = dwFinalCompressSize;
		}
		else
		{
			break;
		}
	}

	*ppCompressData = pCompressData;
	*pdwCompressDataLength = dwFinalCompressSize;

	if (pWorkSpace)
	{
		delete[]pWorkSpace;
	}
	if (hModule)
	{
		FreeLibrary(hModule);
	}

	return TRUE;
}

extern "C" _declspec(dllexport) bool UncompressData(unsigned char *pCompressData, unsigned long dwCompressDataLength, unsigned char **ppUncompressData, unsigned long *pdwUncompressDataLength)
{
	HMODULE hModule = LoadLibrary("ntdll.dll");
	if (NULL == hModule)
	{
		printf("Load dll error!\n");
		return FALSE;
	}

	typedef_RtlDecompressBuffer RtlDecompressBuffer = (typedef_RtlDecompressBuffer)GetProcAddress(hModule, "RtlDecompressBuffer");
	if (RtlDecompressBuffer == NULL)
	{
		printf("Load function error!\n");
	}

	DWORD dwFinalUncompressSize = 0;
	DWORD dwUncompressDataLength = 4096;
	BYTE* pUncompressData = NULL;
	while (TRUE)
	{
		// 申请动态内存
		pUncompressData = new BYTE[dwUncompressDataLength];
		if (NULL == pUncompressData)
		{
			printf("new memory error!\n");
			break;
		}
		::RtlZeroMemory(pUncompressData, dwUncompressDataLength);

		// 调用RtlDecompressBuffer解压缩数据
		RtlDecompressBuffer(COMPRESSION_FORMAT_LZNT1, pUncompressData, dwUncompressDataLength, pCompressData, dwCompressDataLength, &dwFinalUncompressSize);
		if (dwUncompressDataLength < dwFinalUncompressSize)
		{
			// 释放内存
			if (pUncompressData)
			{
				delete[]pUncompressData;
			}
			dwUncompressDataLength = dwFinalUncompressSize;
		}
		else
		{
			break;
		}
	}

	*ppUncompressData = pUncompressData;
	*pdwUncompressDataLength = dwFinalUncompressSize;

	if (hModule)
	{
		FreeLibrary(hModule);
	}

	return TRUE;
}