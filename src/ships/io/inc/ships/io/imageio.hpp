#ifndef __SHIPS_IO_IMAGEIO__
#define __SHIPS_IO_IMAGEIO__

using namespace std;

namespace Sp {

class ImageReader {
public:
	virtual ~ImageReader() {
	}

	//---------- GET ----------//
	int GetWidth() const {
		return width_;
	}
	int GetHight() const {
		return height_;
	}
	virtual std::string& GetURL() {
		return url_;
	}
	virtual ImageMetadata GetImageMetadata() {
		return metadata_;
	}
	//---------- TEG ----------//
	//---------- SET ----------//
	virtual void SetURL(const std::string& url) {
		url_ = url;
	}
	//---------- TES ----------//
	template<class T1, class T2> bool Read(const std::string& url, Image<T1, T2>* img) {
	}

protected:
	ImageMetadata metadata_;
	int width_;
	int height_;
	std::string url_;
private:
};

class ImageWriter {
public:
	virtual ~ImageWriter()=0;
};

class ImageReaderWriterPlugin: public PluginBase {
public:
	virtual ~ImageReaderWriterPlugin() {
	}

	//---------- GET ----------//
	std::vector<std::string> GetFormatName() {
		return format_names_;
	}
	std::string GetMIMEType() {
		return MIME_type_;
	}
	//---------- TEG ----------//
	//---------- SET ----------//
	//---------- TES ----------//


protected:
	std::vector<std::string> format_names_;

	std::string MIME_type_;

private:
};

class ImageReaderPlugin: public ImageReaderWriterPlugin {
public:
	virtual ~ImageReaderPlugin() {
	}

	virtual ImageReader* CreateImageReader()=0;
protected:
private:
};

class ImageWriterPlugin: public ImageReaderWriterPlugin {
public:
	virtual ~ImageWriterPlugin() {
	}

	ImageWriter* CreateImageWriter();
protected:
private:
};

class ImageIO {
public:
	ImageIO();
	virtual ~ImageIO() {
	}
	;
	//---------- GET ----------//
	ImageReader* GetImageReaderByFormat(const std::string& file_format);
	//ImageWriter* GetImageWriterByFormat(const std::string& file_format);
	//---------- TEG----------//
	template<class T1, class T2> static bool Read(const std::string& file_name,
			Image<T1, T2>* img);
	//template<class T> static void Write(const Image<T>& image, const std::string& file_format, const std::string& file_name);
protected:

private:
	//static ImageReader* image_reader_;
	//static ImageWriter* image_writer_;
};
}

#endif
