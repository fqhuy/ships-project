#ifndef __SHIPS_CORE_CONFIG_HPP__
#define __SHIPS_CORE_CONFIG_HPP__

#if defined WIN32 || defined _WIN32
#define SHIPS_HOME "HOMEPATH"
#define SHIPS_HOME_DRIVE "HOMEDRIVE"
#else
#define SHIPS_HOME "HOME"
//#define SHIPS_HOME_DRIVE ""
#endif

#if defined WIN32 || defined _WIN32
#define SHIPS_CDECL __cdecl
#define SHIPS_STDCALL __stdcall
#else
#define SHIPS_CDECL
#define SHIPS_STDCALL
#endif

#ifndef SHIPS_EXTERN_C
#ifdef __cplusplus
#define SHIPS_EXTERN_C extern "C"
#define SHIPS_DEFAULT(val) = val
#else
#define SHIPS_EXTERN_C
#define SHIPS_DEFAULT(val)
#endif
#endif

#ifndef SHIPS_EXTERN_C_FUNCPTR
#ifdef __cplusplus
#define SHIPS_EXTERN_C_FUNCPTR(x) extern "C" { typedef x; }
#else
#define SHIPS_EXTERN_C_FUNCPTR(x) typedef x
#endif
#endif

#ifndef SHIPS_INLINE
#if defined __cplusplus
#define SHIPS_INLINE inline
#elif (defined WIN32 || defined _WIN32 || defined WINCE) && !defined __GNUC__
#define SHIPS_INLINE __inline
#else
#define SHIPS_INLINE static
#endif
#endif /* SHIPS_INLINE */

#if (defined WIN32 || defined _WIN32 || defined WINCE) && defined SPAPI_EXPORTS
#define SHIPS_EXPORTS __declspec(dllexport)
#else
#define SHIPS_EXPORTS
#endif

#ifndef SPAPI
#define SPAPI(rettype) SHIPS_EXTERN_C SHIPS_EXPORTS rettype SHIPS_CDECL
#endif

namespace Sp{
class ShipsConfig{
public:
	SHIPS_INLINE static ShipsConfig& Instance(){
		static ShipsConfig config;
		return config;
	}
	void Config(const std::string& file_name);
	void Config();

	SHIPS_INLINE std::string GetHome(){return ships_home_path;}
private:
	ShipsConfig();
	virtual ~ShipsConfig(){}
	ShipsConfig& operator=(const ShipsConfig& config);

	std::string file_name_;
	std::string ships_home_path;
	libconfig::Config config_;
};
}
#endif
