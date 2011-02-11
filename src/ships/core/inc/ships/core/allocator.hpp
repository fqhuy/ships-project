#ifndef __SHIPS_CORE_ALLOCATOR_HPP__
#define __SHIPS_CORE_ALLOCATOR_HPP__

#include  <cstddef>
#include <memory>

namespace Sp {
    //typedef std::allocator Allocator;
    //fake device memory allocator, used to qualify the storage type (unbounded_array, bounded_array...)
    /*
    template<class T>
    class DeviceAllocator:
    public std::allocator<T> {
        typedef DeviceAllocator<T> SelfType;
    public:
        typedef typename Allocator::size_type size_type;
        typedef typename Allocator::difference_type difference_type;
        typedef T value_type;
        typedef const T &const_reference;
        typedef T &reference;
        typedef const T *const_pointer;
        typedef T *pointer;
    };
    */
}

#endif
