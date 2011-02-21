/*
 * device_manager.cpp
 *
 *  Created on: Feb 14, 2011
 *      Author: fqhuy
 */
#include <ships/core/core.hpp>

namespace Sp {

void DeviceManager::Initialize() {
	cl::Platform::get(&this->platforms_);
	if (this->platforms_.size() > 0)
		this->platforms_[0].getDevices(CL_DEVICE_TYPE_ALL, &this->devices_);

	cl_context_properties cps[3] =
    {
        CL_CONTEXT_PLATFORM,
        (cl_context_properties) this->platforms_[0](),
        0
    };

	cl::Context context = cl::Context(CL_DEVICE_TYPE_ALL, cps);
	this->contexts_.push_back(context);

}

void DeviceManager::Cleanup() {

}

}
