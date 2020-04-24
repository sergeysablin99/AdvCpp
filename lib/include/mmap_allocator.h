//
// Created by Sergey Sablin on 17.04.2020.
// @serewkasablin@outlook.com
//

#ifndef LINEAR_ALLOCATOR_H
#define LINEAR_ALLOCATOR_H

#include <cstddef>
#include <memory>
#include <semaphore.h>
#include <sys/mman.h>

namespace shmem {
    struct Mmap {
        void* begin;
        std::size_t size;
        sem_t* sem;
        explicit Mmap(std::size_t memory_size);
        Mmap(const Mmap& other) = delete;
        Mmap(Mmap&& other);
        ~Mmap();
    };

    template <typename T, typename MEM_SIZE>
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

        MMapAllocator<T, MEM_SIZE> select_on_container_copy_construction();

        std::shared_ptr<Mmap> mmap;

        MMapAllocator();
        sem_t* get_sem();

        template <typename U, typename U_MEM>
        MMapAllocator(const MMapAllocator<U, U_MEM>& other);
    };

    Mmap::Mmap(std::size_t memory_size): size(memory_size)
    {
        begin = ::mmap(nullptr, size,
                       PROT_READ | PROT_WRITE,
                       MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        if (begin == MAP_FAILED)
            throw std::bad_alloc();
        sem = new (begin) sem_t;
        sem_init(sem, 1, 1);
    }

    Mmap::~Mmap() {
        ::munmap(begin, size);
    }

    Mmap::Mmap(Mmap&& other) :
    begin(other.begin), size(other.size), sem(other.sem)
    {
        other.begin = nullptr;
        other.sem = nullptr;
        other.size = 0;
    }

    template<typename T, typename MEM_SIZE>
    T *MMapAllocator<T, MEM_SIZE>::allocate(std::size_t size) {
        return {new (mmap->begin) T[size]};
    }

    template<typename T, typename MEM_SIZE>
    void MMapAllocator<T, MEM_SIZE>::deallocate(T *p, std::size_t size) {
    }

    template<typename T, typename MEM_SIZE>
    template <typename U, typename U_MEM>
    MMapAllocator<T, MEM_SIZE>::MMapAllocator(const MMapAllocator<U, U_MEM>& other) {
        mmap = other.mmap;
    }

    template<typename T, typename MEM_SIZE>
    MMapAllocator<T, MEM_SIZE>::MMapAllocator() : mmap{std::make_shared<Mmap>(sizeof(MEM_SIZE))}
    {
    }

    template<typename T, typename MEM_SIZE>
    sem_t *MMapAllocator<T, MEM_SIZE>::get_sem() {
        return mmap->sem;
    }

    template<typename T, typename MEM_SIZE>
    MMapAllocator<T, MEM_SIZE> MMapAllocator<T, MEM_SIZE>::select_on_container_copy_construction() {
        return this;
    }
}



#endif //LINEAR_ALLOCATOR_H
