#ifndef __SHIPS_IO_IMAGEIO__
#define __SHIPS_IO_IMAGEIO__

using namespace std;

namespace Sp {
	class ImageReader{
		public:
			virtual ~ImageReader(){};
			//---------- GET ----------//
			int GetWidth() const {return width_;}
			int GetHight() const {return height_;}
			virtual string& GetURL(){return url_;}
			virtual ImageMetaData GetImageMetadata(){return metadata_;}
			//---------- TEG ----------//
			//---------- SET ----------//
			virtual void SetURL(const string& url){url_ = url;}
			//---------- TES ----------//
			template<class T> bool Read(const string& url, Image<T>* img){}

		protected:
			ImageMetaData metadata_;
			int width_;
			int height_;
			string url_;
		private:
	};

	class ImageWriter{
		public:
			virtual ~ImageWriter()=0;
	};
		
	class ImageIO {
		public:
			ImageIO();
			virtual ~ImageIO(){};
			//---------- GET ----------//
			ImageReader* GetImageReaderByFormat(const string& file_format);
			//ImageWriter* GetImageWriterByFormat(const string& file_format);
			//---------- TEG----------//
			template<class T> static bool Read(const string& file_name, Image<T>* img);
			//template<class T> static void Write(const Image<T>& image, const string& file_format, const string& file_name);
		protected:
		
		private:
			//static ImageReader* image_reader_;
			//static ImageWriter* image_writer_;
	};
}

#endif
