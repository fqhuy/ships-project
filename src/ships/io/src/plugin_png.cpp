#include <png.h>
#include <ships/io/io.hpp>

namespace Sp {
PNGImageReaderPlugin::PNGImageReaderPlugin() {
}

ImageReader* PNGImageReaderPlugin::CreateImageReader() {
	return new PNGImageReader();
}

PNGImageReader::PNGImageReader() {
}

void PNGImageReader::SetURL(const string& url) {

	url_ = url;
}

template<class T> bool PNGImageReader::Read(const string& url, Image<T>* img) {
	bool result = false;
	//Image<T>* img = new Image<T>();

	return result;
}

template<class T> bool PNGImageReader::Read(const string& url,
		Image<Pixel<T> >* img) {

}

ImageMetaData PNGImageReader::GetImageMetadata() {
}

}
