#ifndef __SHIPS_IO_PLUGIN_PNG__
#define __SHIPS_IO_PLUGIN_PNG__

namespace Sp {

class PNGImageReaderPlugin: public ImageReaderPlugin {
public:
	PNGImageReaderPlugin();
	ImageReader* CreateImageReader();
};

class PNGImageReader: public ImageReader {
public:
	PNGImageReader();

	virtual ~PNGImageReader() {
	}
	void SetURL(const string& url);
	template<class T> bool Read(const string& url,Image<T>* img);
	template<class T> bool Read(const string& url,Image<Pixel<T> >* img	);
	ImageMetaData GetImageMetadata();
};
}

#endif
