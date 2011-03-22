/*
 * device_manager.cpp
 *
 *  Created on: Feb 14, 2011
 *      Author: fqhuy
 */
#include <ships/core/core.hpp>

namespace Sp {
Package::Package() {

}

Package::Package(std::string name, std::string vendor, std::string version) {
	this->name_ = name;
	this->vendor_ = vendor;
	this->version_ = version;
}

void Package::AddPackage(Package package) {
	this->packages_.insert(std::pair<std::string, Package>(package.GetName(),
			package));
}

void Package::AddProgram(Program program) {
	this->programs_.insert(std::pair<std::string, Program>(program.GetName(),
			program));
}

Program* Package::FindProgram(const std::string& name) {
	std::map<std::string, Program>::iterator it;
	it = this->programs_.find(name);
	if (it == this->programs_.end())
		return NULL;
	return &it->second;
}

Package* Package::FindPackage(const std::string& name) {
	std::map<std::string, Package>::iterator it;
	it = this->packages_.find(name);
	if (it == this->packages_.end())
		return NULL;
	return &it->second;
}

Program::Program() {

}

Program::Program(std::string name) {
	this->name_ = name;
}

void Program::AddKernel(cl::Kernel kernel, const std::string& name) {
	//char str[256];
	//int err;
	//err = kernel.getInfo(CL_KERNEL_FUNCTION_NAME, str);
	//if(!err)
	//	LOG4CXX_ERROR(core_logger, "error in getting kernel info, error code is: "<<err);

	//LOG4CXX_INFO(core_logger, "adding kernel : " << str);
	this->kernels_.insert(std::pair<std::string, cl::Kernel>(name, kernel));
}

int Program::Build(const cl::Context& context) {
	cl::Program::Sources sources;
	//TODO: should I delete this after pushing it back to the vector ?
	std::ifstream* sourceFile;
	std::string* allCode = new std::string("");
	std::string* sourceCode;
	for (int i = 0; i < includes_.size(); i++) {
		sourceFile = new std::ifstream(includes_[i].c_str());
		sourceCode = new std::string(
				std::istreambuf_iterator<char>(*sourceFile),
				(std::istreambuf_iterator<char>()));

		allCode->append(*sourceCode);
	}
	int err;
	sources.push_back(std::make_pair(allCode->c_str(), allCode->length() + 1));

	program_ = cl::Program(context, sources, &err);
	if (err != CL_SUCCESS) {
		LOG4CXX_ERROR(core_logger, "unable to create program " << name_
				<< "error code is " << err);
		return err;
	}
	std::vector<cl::Device> devices;
	context.getInfo(CL_CONTEXT_DEVICES, &devices);
	err = program_.build(devices);

	if (err != CL_SUCCESS) {
		if (err == CL_BUILD_PROGRAM_FAILURE) {
			std::string elog = program_.getBuildInfo<CL_PROGRAM_BUILD_LOG> (
					devices[0]);
			LOG4CXX_ERROR(core_logger, "unable to build program " << name_
					<< "error code is " << err <<" ,error log: \n" << elog);
		}
		return err;
	}

	return 0;
}

cl::Kernel* Program::FindKernel(const std::string& name) {
	std::map<std::string, cl::Kernel>::iterator it;

	it = this->kernels_.find(name);
	if (it == this->kernels_.end()) {
		return NULL;
	}
	return &it->second;
}

void DeviceManager::Initialize() {
	LOG4CXX_INFO(Sp::core_logger, "Initializing DeviceManager...\n");

	cl::Platform::get(&this->platforms_);
	if (this->platforms_.size() > 0)
		this->platforms_[0].getDevices(CL_DEVICE_TYPE_ALL, &this->devices_);

	cl_context_properties cps[3] = { CL_CONTEXT_PLATFORM,
			(cl_context_properties) this->platforms_[0](), 0 };

	cl::Context context = cl::Context(CL_DEVICE_TYPE_ALL, cps);
	this->contexts_.push_back(context);
	this->ParsePackageConfig(this->program_src_paths_[0] + "/config.xml");
}

void DeviceManager::ReadProgram(Program* prg, const TiXmlHandle& root) {
	TiXmlElement* pElem;
	pElem = root.FirstChild("sp:include").Element();
	//find all kernels.
	if (pElem) {
		for (pElem; pElem; pElem = pElem->NextSiblingElement()) {

			const char* cname = pElem->GetText();

			if (!cname)
				continue;

			std::string name(cname);
			name = StringUtils::replaceAll(name, std::string("."), std::string(
					"/"));
			name.append(".cl");
			name.insert(0, "/");
			name.insert(0, this->program_src_paths_[0]);
			prg->Include(name);
		}
	}
	//TODO: Build program

	prg->Build(this->GetDefaultContext());

	pElem = root.FirstChild("sp:kernel").Element();
	if (!pElem) {
		LOG4CXX_WARN(core_logger,"no kernel has been found! ");
		return;
	}
	int err;
	for (pElem; pElem; pElem = pElem->NextSiblingElement()) {
		const char* kname = pElem->Attribute("name");
		if (!kname)
			continue;

		cl::Kernel *kernel = new cl::Kernel(prg->GetProgram(), kname, &err);

		if (err != CL_SUCCESS) {
			LOG4CXX_ERROR(core_logger,"unable to create kernel "<< kname <<", error code is "<< err);
			continue;
		}
		//LOG4CXX_INFO(Sp::core_logger, "adding kernel " << (*kernel)() << '\n');
		prg->AddKernel(*kernel, std::string(kname));
	}

}

void DeviceManager::ReadPackage(Package* package, const TiXmlHandle& root) {
	TiXmlElement* pElem;
	pElem = root.FirstChild("sp:program").Element();
	if (pElem) {
		for (pElem; pElem; pElem = pElem->NextSiblingElement()) {
			//LOG4CXX_INFO(Sp::core_logger, "parsing program: " << pElem->Attribute("name") << '\n');
			const std::string* name = pElem->Attribute(std::string("name"));
			Program* prg = new Program(*name);
			this->ReadProgram(prg, TiXmlHandle(pElem));
			package->AddProgram(*prg);
		}
	}
	pElem = root.FirstChild("sp:package").Element();
	if (!pElem)
		return;
	for (pElem; pElem; pElem = pElem->NextSiblingElement()) {
		//LOG4CXX_INFO(Sp::core_logger, "parsing package: " << pElem->Attribute("name") << '\n');
		const std::string* name = pElem->Attribute(std::string("name"));
		if (!name) {
			LOG4CXX_ERROR(core_logger,"Error parsing program configure file: package name is needed ");
			return;
		}
		const std::string* version = pElem->Attribute(std::string("version"));
		if (!version)
			version = new std::string("");
		const std::string* vendor = pElem->Attribute(std::string("vendor"));
		if (!vendor)
			vendor = new std::string("");
		Package* pkg = new Package(*name, *vendor, *version);

		//this->packages_.insert(std::pair<std::string,Package>(name,*pkg));
		this->ReadPackage(pkg, TiXmlHandle(pElem));
		package->AddPackage(*pkg);
	}

}
void DeviceManager::ParsePackageConfig(const std::string& filename) {
	TiXmlDocument doc(filename);
	if (!doc.LoadFile())
		return;
	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;
	TiXmlHandle hRoot(0);
	//handle root package
	pElem = hDoc.FirstChildElement().Element();
	if (!pElem)
		return;
	//TODO: fix this , use pointer for safety.
	std::string name(pElem->Attribute("name"));
	std::string version(pElem->Attribute("version"));
	std::string vendor(pElem->Attribute("vendor"));

	//LOG4CXX_INFO(Sp::core_logger, "parsing root package: "<<name <<" "<<version << '\n');
	hRoot = TiXmlHandle(pElem);

	Package* pkg = new Package(name, vendor, version);
	this->ReadPackage(pkg, hRoot);
	this->packages_.insert(std::pair<std::string, Package>(name, *pkg));
}

cl::Kernel* DeviceManager::FindKernel(const std::string& name) {

	boost::char_separator<char> sep(".");
	boost::tokenizer<boost::char_separator<char> > tok(name, sep);
	//quota for nested package is 16...
	std::string str[16];
	int count = 0;
	for (boost::tokenizer<boost::char_separator<char> >::iterator beg =
			tok.begin(); beg != tok.end(); ++beg) {
		str[count] = *beg;
		count++;
	}
	if (count < 3)
		return NULL;

	//LOG4CXX_INFO(Sp::core_logger,"count = " << count << " requested kernel: " << str[count-1]);
	std::map<std::string, Package>::iterator it;
	it = this->packages_.find(str[0]);
	if (it == this->packages_.end())
		return NULL;
	Package* pkg = &this->packages_[str[0]];
	//LOG4CXX_INFO(Sp::core_logger,"found package 0 = " << pkg->GetName());
	for (int i = 1; i < count - 2; i++) {
		pkg = pkg->FindPackage(str[i]);
		if (pkg == NULL)
			return NULL;
		//LOG4CXX_INFO(Sp::core_logger,"found package "<< i <<" = " << pkg->GetName());
	}
	Program* prg = pkg->FindProgram(str[count - 2]);
	//LOG4CXX_INFO(Sp::core_logger,"found program  = " << prg->GetName());
	return prg->FindKernel(str[count - 1]);
	//return kernels_[name];
}
cl::Program* DeviceManager::FindProgram(const std::string& name) {
	//TODO: need DeviceManager::FindProgram
	return NULL;
}
Package* DeviceManager::FindPackage(const std::string& name) {
	std::map<std::string, Package>::iterator it;
	it = packages_.find(name);
	if (it == packages_.end())
		return NULL;
	return &it->second;
}

void DeviceManager::Cleanup() {

}

}
