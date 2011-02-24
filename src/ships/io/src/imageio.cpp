#include <ships/io/io.hpp>

namespace Sp {

ImageIO::ImageIO() {
}

template<class T1, class T2> bool ImageIO::Read(const string& file_name, Image<T1, T2>* img) {
	//only PNG is supported by now
	return GetImageReaderByFormat("png")->Read(file_name, img);
}

ImageReader* ImageIO::GetImageReaderByFormat(const string& file_format) {
	//if (ImageIO::image_reader_ == NULL)
	//	ImageIO::image_reader_
	//			= static_cast<ImageReaderPlugin*> (PluginManager::GetPlugin(
	//					file_format))->CreateImageReader();

	//return static_cast<ImageReaderPlugin*> (PluginManager::GetPlugin(file_format))->CreateImageReader();
}

}
