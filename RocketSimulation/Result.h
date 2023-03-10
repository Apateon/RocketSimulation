#pragma once
#include <windows.h>
struct Result
{
	static const HRESULT OK = static_cast<HRESULT>(0L);
	static const HRESULT INVALIDARGS = static_cast<HRESULT>(0x80070057L);
	static const HRESULT FAIL = static_cast<HRESULT>(0x80004005L);
};
