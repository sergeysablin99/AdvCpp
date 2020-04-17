//
// Created by Sergey Sablin on 17.04.2020.
// @serewkasablin@outlook.com
//

#ifndef LINEAR_ALLOCATOR_H
#define LINEAR_ALLOCATOR_H

#include <cstddef>
#include <sys/mman.h>

namespace shmem {
    struct Mmap {
        void* begin;
        std::size_t size;
        Mmap(std::size_t memory_size);
        ~Mmap();
    };

    template <typename T>
    class MMapAllocator {
    public:
        typedef size_t    size_type;
        typedef ptrdiff_t difference_type;
        typedef T*        pointer;
        typedef const T*  const_pointer;
        typedef T&        reference;
        typedef const T&  const_reference;
        typedef T         value_type;
        typedef MMapAllocator allocator_type;

        T* allocate(std::size_t size);
        void deallocate(T* p, std::size_t size);

        static Mmap mmap;

        MMapAllocator() = default;

        template<typename U>
        MMapAllocator(const MMapAllocator<U>& );
    };

    template<typename T>
    Mmap MMapAllocator<T>::mmap{1024};

    Mmap::Mmap(std::size_t memory_size): size(memory_size)
    {
        begin = ::mmap(nullptr, size,
                       PROT_READ | PROT_WRITE,
                       MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        if (begin == MAP_FAILED)
            throw std::bad_alloc();
    }

    Mmap::~Mmap() {
        ::munmap(begin, size);
    }

    template<typename T>
    T *MMapAllocator<T>::allocate(std::size_t size) {
        char* new_mem = new (mmap.begin) char[size];
        return reinterpret_cast<T*>(new_mem);
    }

    template<typename T>
    void MMapAllocator<T>::deallocate(T *p, std::size_t size) {
        p->~T();
    }

    template<typename T>
    template<typename U>
    MMapAllocator<T>::MMapAllocator(const MMapAllocator<U>& other) {
        mmap.begin = other.begin;
        mmap.size = other.size;
    }
}



#endif //LINEAR_ALLOCATOR_H
