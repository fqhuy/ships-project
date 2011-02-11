#ifndef __SHIPS_CORE_COMMON_HPP__
#define __SHIPS_CORE_COMMON_HPP__

#if (defined WIN32 || defined _WIN32 || defined WINCE) && defined SPAPI_EXPORTS
#define SP_EXPORTS __declspec(dllexport)
#else
#define SP_EXPORTS
#endif

/****************************************************************************************\
*                                  Matrix type                                            *
\****************************************************************************************/

#define SP_CN_MAX     512
#define SP_CN_SHIFT   3
#define SP_DEPTH_MAX  (1 << SP_CN_SHIFT)

#define SP_8U   0
#define SP_8S   1
#define SP_16U  2
#define SP_16S  3
#define SP_32S  4
#define SP_32F  5
#define SP_64F  6
#define SP_USRTYPE1 7

#define SP_MAT_DEPTH_MASK       (SP_DEPTH_MAX - 1)
#define SP_MAT_DEPTH(flags)     ((flags) & SP_MAT_DEPTH_MASK)

#define SP_MAKETYPE(depth,cn) (SP_MAT_DEPTH(depth) + (((cn)-1) << SP_CN_SHIFT))
#define SP_MAKE_TYPE SP_MAKETYPE

#define SP_8UC1 SP_MAKETYPE(SP_8U,1)
#define SP_8UC2 SP_MAKETYPE(SP_8U,2)
#define SP_8UC3 SP_MAKETYPE(SP_8U,3)
#define SP_8UC4 SP_MAKETYPE(SP_8U,4)
#define SP_8UC(n) SP_MAKETYPE(SP_8U,(n))

#define SP_8SC1 SP_MAKETYPE(SP_8S,1)
#define SP_8SC2 SP_MAKETYPE(SP_8S,2)
#define SP_8SC3 SP_MAKETYPE(SP_8S,3)
#define SP_8SC4 SP_MAKETYPE(SP_8S,4)
#define SP_8SC(n) SP_MAKETYPE(SP_8S,(n))

#define SP_16UC1 SP_MAKETYPE(SP_16U,1)
#define SP_16UC2 SP_MAKETYPE(SP_16U,2)
#define SP_16UC3 SP_MAKETYPE(SP_16U,3)
#define SP_16UC4 SP_MAKETYPE(SP_16U,4)
#define SP_16UC(n) SP_MAKETYPE(SP_16U,(n))

#define SP_16SC1 SP_MAKETYPE(SP_16S,1)
#define SP_16SC2 SP_MAKETYPE(SP_16S,2)
#define SP_16SC3 SP_MAKETYPE(SP_16S,3)
#define SP_16SC4 SP_MAKETYPE(SP_16S,4)
#define SP_16SC(n) SP_MAKETYPE(SP_16S,(n))

#define SP_32SC1 SP_MAKETYPE(SP_32S,1)
#define SP_32SC2 SP_MAKETYPE(SP_32S,2)
#define SP_32SC3 SP_MAKETYPE(SP_32S,3)
#define SP_32SC4 SP_MAKETYPE(SP_32S,4)
#define SP_32SC(n) SP_MAKETYPE(SP_32S,(n))

#define SP_32FC1 SP_MAKETYPE(SP_32F,1)
#define SP_32FC2 SP_MAKETYPE(SP_32F,2)
#define SP_32FC3 SP_MAKETYPE(SP_32F,3)
#define SP_32FC4 SP_MAKETYPE(SP_32F,4)
#define SP_32FC(n) SP_MAKETYPE(SP_32F,(n))

#define SP_64FC1 SP_MAKETYPE(SP_64F,1)
#define SP_64FC2 SP_MAKETYPE(SP_64F,2)
#define SP_64FC3 SP_MAKETYPE(SP_64F,3)
#define SP_64FC4 SP_MAKETYPE(SP_64F,4)
#define SP_64FC(n) SP_MAKETYPE(SP_64F,(n))

#define SP_AUTO_STEP  0x7fffffff
#define SP_WHOLE_ARR  cvSlice( 0, 0x3fffffff )

#define SP_MAT_CN_MASK          ((SP_CN_MAX - 1) << SP_CN_SHIFT)
#define SP_MAT_CN(flags)        ((((flags) & SP_MAT_CN_MASK) >> SP_CN_SHIFT) + 1)
#define SP_MAT_TYPE_MASK        (SP_DEPTH_MAX*SP_CN_MAX - 1)
#define SP_MAT_TYPE(flags)      ((flags) & SP_MAT_TYPE_MASK)
#define SP_MAT_CONT_FLAG_SHIFT  14
#define SP_MAT_CONT_FLAG        (1 << SP_MAT_CONT_FLAG_SHIFT)
#define SP_IS_MAT_CONT(flags)   ((flags) & SP_MAT_CONT_FLAG)
#define SP_IS_CONT_MAT          SP_IS_MAT_CONT
#define SP_SUBMAT_FLAG_SHIFT    15
#define SP_SUBMAT_FLAG          (1 << SP_SUBMAT_FLAG_SHIFT)
#define SP_IS_SUBMAT(flags)     ((flags) & SP_MAT_SUBMAT_FLAG)

#define SP_MAGIC_MASK       0xFFFF0000
#define SP_MAT_MAGIC_VAL    0x42420000
#define SP_TYPE_NAME_MAT    "ships-matrix"

#endif
