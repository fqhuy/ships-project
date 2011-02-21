#ifndef __SHIPS_IO_PLUGIN_MANAGER__
#define __SHIPS_IO_PLUGIN_MANAGER__

namespace Sp {

class PluginBase {
public:
	PluginBase(){}
	virtual ~PluginBase() {
	}

protected:
	std::string vendor_name_;
	std::string version_;
private:

};

class PluginManager {
public:
	typedef PluginManager SelfType;
	virtual ~PluginManager() {
	}

	PluginBase* GetPlugin(const std::string& name) {
		return plugins_[name];
	}

	//bool Contains(const PluginBase& plugin);
	void Register(const std::string& plugin_name, PluginBase* plugin);
	void Deregister(const std::string& plugin_name);

	static SelfType Instance(){return instance_;}
protected:
	PluginManager();
private:
	static SelfType instance_;
	std::map<std::string, PluginBase*> plugins_;
};



}

#endif
