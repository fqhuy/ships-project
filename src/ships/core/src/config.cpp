/*
 * config.cpp
 *
 *  Created on: Feb 15, 2011
 *      Author: fqhuy
 */
#include <ships/core/core.hpp>

namespace Sp{

ShipsConfig::ShipsConfig(){

}

void ShipsConfig::Config(const string& file_name){
	this->file_name_ = file_name;
	this->config_.readFile(file_name.c_str());
	this->ships_home_path = SHIPS_HOME_RPATH;
	this->ships_home_path.append("/.ships");

	libconfig::Setting& ships = this->config_.lookup("ships");
	//log4cxx config
	std::string log_config_file = ships["log_config_file"];
	log4cxx::xml::DOMConfigurator::configure(log_config_file);
	//program source config
	LOG4CXX_INFO(Sp::core_logger, "config program source path");
	int count = ships["program_source_paths"].getLength();
	std::vector<std::string> program_source_paths;
	for (int i=0;i<count;i++){
		std::string s = ships["program_source_paths"][i];
		program_source_paths.push_back(s);
	}
	//program binary config
	LOG4CXX_INFO(Sp::core_logger, "config program binary path");
	count = ships["program_binary_paths"].getLength();
	std::vector<std::string> program_bin_paths;
	for (int i=0;i<count;i++){
		std::string s = ships["program_binary_paths"][i];
		program_bin_paths.push_back(s);
	}
	LOG4CXX_INFO(Sp::core_logger, "Initialize device manager");
	DeviceManager::Instance().SetProgramSrcPaths(program_source_paths);
	DeviceManager::Instance().SetProgramBinPaths(program_bin_paths);
	DeviceManager::Instance().Initialize();
}

void ShipsConfig::Config(){
	this->ships_home_path = SHIPS_HOME_RPATH;
	this->ships_home_path.append("/.ships");
	Config(this->ships_home_path + "/ships.cfg");
}
}
