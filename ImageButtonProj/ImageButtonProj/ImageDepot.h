#pragma once

#include <Windows.h>
#include <string>
#include "utility.hpp"

#if defined(UNICODE) || defined(_UNICODE)
#define tstring wstring
#else
#define tstring string
#endif 

class _ImageDepotImpl;
class _ImageDepot : public Singleton<_ImageDepot> {
	friend class Singleton<_ImageDepot>;
public :
	HBITMAP get(const std::tstring& filename);
	HBITMAP operator [](const std::tstring& filename);
	HBITMAP get(UINT uResourceId);
	HBITMAP operator [](UINT uResourceId);
private :
	_ImageDepot();
	~_ImageDepot();

	_ImageDepotImpl* impl;
};
#define ImageDepot (_ImageDepot::GetReference())

