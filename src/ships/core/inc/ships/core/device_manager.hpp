#ifndef __SHIPS_CORE_DEVICE_MANAGER_HPP__
#define __SHIPS_CORE_DEVICE_MANAGER_HPP__

namespace Sp {
/*
 class Device {
 public:
 typedef cl_device_type DeviceType;
 typedef cl_command_queue_properties CommandQueueProperties;
 typedef cl_device_exec_capabilities DeviceExecutionCapabilities;
 typedef cl_device_local_mem_type DeviceLocalMemType;
 typedef cl_device_mem_cache_type DeviceMemCacheType;

 SHIPS_INLINE Device(cl::Device* device) {
 device_ = device;
 }
 virtual ~Device() {
 }

 DeviceType GetType(){return device_->getInfo<CL_DEVICE_TYPE>();}
 uint32_t GetVendorID();
 uint32_t GetMaxComputeUnits(){return device_->getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>();}
 uint32_t GetMaxWorkItemDimensions();
 size_t* GetMaxWorkItemSizes();
 size_t GetMaxWorkGroupSize();
 std::string GetOpenCLCVersion();
 std::string GetVersion();
 std::string GetProfile();
 std::string GetDriverVersion();
 std::string GetVendor();
 std::string GetName();
 cl::Platform* GetPlatform();
 CommandQueueProperties GetCommandQueueProperties();
 DeviceExecutionCapabilities GetDeviceExecutionCapabilities();
 bool IsCompilerAvailable();
 bool IsAvailable();
 bool IsEndianLittle();
 size_t GetProfilingTimerResolution();
 bool HasHostUnifiedMemory();
 bool HasErrorCorrectionSupport();
 uint64_t GetLocalMemSize();
 DeviceLocalMemType GetLocalMemType();
 uint32_t GetMaxConstantArgs();
 uint64_t GetMaxConstantBufferSize();
 uint64_t GetGlobalMemSize();
 uint64_t GetGlobalMemCacheSize();
 uint64_t GetGlobalMemCachelineSize();
 DeviceMemCacheType GetGlobalMemCacheType();
 bool HasImageSupport();
 uint32_t GetImage2DMaxWidth();
 uint32_t GetImage2DMaxHeight();
 SHIPS_INLINE
 cl::Device* Get() {
 return device_;
 }
 protected:
 cl::Device* device_;
 cl::Platform* platform_;
 };

 class Platform {
 public:
 typedef cl_device_type DeviceType;
 SHIPS_INLINE Platform(cl::Platform* platform) {
 platform_ = platform;
 }
 virtual ~Platform() {
 }

 SHIPS_INLINE std::vector<cl::Device> GetDevices(){return devices_;}
 std::vector<cl::Device> GetGPUDevices();
 cl::Device GetDefaultDevice();
 std::vector<cl::Device> GetDeviceByType(DeviceType device_type);

 std::string GetProfile();
 std::string GetVersion();
 std::string GetName();
 std::string GetVendor();
 std::vector<std::string> GetExtensions();
 SHIPS_INLINE cl::Platform* Get() {
 return platform_;
 }
 protected:
 cl::Platform* platform_;
 std::vector<cl::Device> devices_;
 };

 class Context{
 public:
 typedef cl_context_properties ContextProperties;

 SHIPS_INLINE Context(cl::Context* context){context_ = context;}

 uint32_t GetReferenceCount();
 uint32_t GetNumDevices();
 std::vector<cl::Device> GetDevices();
 std::vector<ContextProperties> GetProperties();
 SHIPS_INLINE cl::Context* Get(){return context_;}

 protected:
 cl::Context* context_;

 };
 *
 */

class DeviceManager {

public:
	static DeviceManager& Instance() {
		static DeviceManager obj;
		return obj;
	}

	SHIPS_INLINE
	std::vector<cl::Device>& GetDefaultDevices() {
		return devices_;
	}
	SHIPS_INLINE
	cl::Device GetDefaultDevice() {
		return devices_[0];
	}
	SHIPS_INLINE
	cl::Context GetDefaultContext(){
		return contexts_[0];
	}

	SHIPS_INLINE
	cl::Kernel GetKernel(std::string name) {
		return kernels_[name];
	}
	SHIPS_INLINE
	cl::Program GetProgram(std::string name) {
		return programs_[name];
	}

	SHIPS_INLINE
	std::vector<cl::Platform>& GetPlatforms() {
		return platforms_;
	}

	SHIPS_INLINE
	cl::Platform GetPlatform(){
		return platforms_[0];
	}

	SHIPS_INLINE
	void SetProgramSrcPaths(const std::vector<std::string>& paths) {
		program_src_paths_ = paths;
	}
	SHIPS_INLINE
	void SetProgramBinPaths(const std::vector<std::string>& paths) {
		program_bin_paths_ = paths;
	}

	void Initialize();
private:

	void Cleanup();

	virtual ~DeviceManager() {
		Cleanup();
	}
	SHIPS_INLINE DeviceManager() {
		Initialize();
	}

	DeviceManager(const DeviceManager& devie_manager) {
	}

	DeviceManager& operator=(const DeviceManager&);

	std::vector<cl::Device> devices_; /**< CL devices belong to the default platform*/
	//cl::Device* device_; /**< CL device to be used */
	std::vector<cl::Platform> platforms_;
	std::vector<cl::Context> contexts_;
	//cl::Context* context_;

	std::map<std::string, cl::Program> programs_;
	std::map<std::string, cl::Kernel> kernels_;

	std::vector<std::string> program_src_paths_;
	std::vector<std::string> program_bin_paths_;
};
}
#endif
