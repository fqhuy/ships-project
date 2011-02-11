#ifndef __SHIPS_IO_PLUGIN_MANAGER__
#define __SHIPS_IO_PLUGIN_MANAGER__

using namespace std;

namespace Sp{
	class PluginBase{
		public:
			virtual ~PluginBase(){};
		protected:
			string vendorName;
			string version;
		private:

	};
        
	class PluginManager{
		public:
			PluginManager();
			virtual ~PluginManager(){};
			
			static PluginBase* GetPlugin(const string& name){return plugins_[name];
			}
			static map<string, PluginBase*>& GetPlugins(){return plugins_;
			}
			//bool Contains(const PluginBase& plugin);
			static void Register(const string& plugin_name, PluginBase* plugin);
			static void Deregister(const string& plugin_name);
		protected:
		
		private:
			static map<string, PluginBase*> plugins_;
	};

	


	class ImageReaderWriterPlugin:
	public PluginBase {
		public:
			virtual ~ImageReaderWriterPlugin(){};
			
			//---------- GET ----------//
			string& GetFormatName(){return format_name_;}
			string& GetMIMEType(){return MIME_type_;}
			//---------- TEG ----------//
			//---------- SET ----------//
			//---------- TES ----------//
			
			
		protected:
			string format_name_;
			string MIME_type_;

		private:
	};
	
	class ImageReaderPlugin:
	public ImageReaderWriterPlugin{
		public:
			virtual ~ImageReaderPlugin(){};
			
			virtual ImageReader* CreateImageReader()=0;
		protected:
		private:
	};
	
	class ImageWriterPlugin:
	public ImageReaderWriterPlugin{
		public:
			virtual ~ImageWriterPlugin(){};
			
			ImageWriter* CreateImageWriter();
		protected:
		private:	
	};
	
	class DescriptorReaderWriterPlugin {
		public:
			virtual ~DescriptorReaderWriterPlugin(){};
	};
	
	class DescriptorReaderPlugin:
	public DescriptorReaderWriterPlugin {
		public:
			virtual ~DescriptorReaderPlugin(){};
	};

}

#endif
