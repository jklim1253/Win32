#include "ImageDepot.h"
#include <map>

typedef std::map<std::tstring, HBITMAP> Container;
typedef typename Container::iterator Iterator;

class _ImageDepotImpl {
public :
	std::map<std::tstring, HBITMAP> depot;

	HBITMAP load(const std::tstring& filename) {
		return (HBITMAP)::LoadImage(NULL, filename.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);
	}
	void clear() {
		for (Iterator it = depot.begin(); it != depot.end(); it++) {
			::DeleteObject(it->second);
			it->second = NULL;
		}
		depot.clear();
	}
};
_ImageDepot::_ImageDepot()
	: impl(new _ImageDepotImpl){

}
_ImageDepot::~_ImageDepot() {
	impl->clear();
	if (impl) {
		delete impl;
		impl = nullptr;
	}
}
HBITMAP _ImageDepot::get(const std::tstring& filename) {
	HBITMAP hBitmap = NULL;
	Iterator it = impl->depot.find(filename);
	if (it == impl->depot.end()) {
		if ((hBitmap = impl->load(filename)) == NULL) {
			// TODO : load error.
			return NULL;
		}
		impl->depot.insert(std::make_pair(filename, hBitmap));

		return hBitmap;
	}
	return it->second;
}
HBITMAP _ImageDepot::operator [](const std::tstring& filename) {
	return get(filename);
}
