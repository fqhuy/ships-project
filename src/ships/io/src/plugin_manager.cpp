#include <ships/io/io.hpp>

using namespace std;

namespace Sp {

map<string, PluginBase*> PluginManager::plugins_;

PluginManager::PluginManager() {
	//let register some built in plugins.

	PNGImageReaderPlugin* png_plugin = new PNGImageReaderPlugin();
	PluginManager::Register(png_plugin->GetFormatName(),
			static_cast<PluginBase*> (png_plugin));
}

void PluginManager::Register(const string& plugin_name, PluginBase* plugin) {
	//PluginManager::GetPlugins();
	PluginManager::GetPlugins().insert(pair<string,PluginBase*>(plugin_name,plugin));
}

void PluginManager::Deregister(const string& plugin_name) {
}

}
