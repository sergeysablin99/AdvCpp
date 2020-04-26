//
// Created by Sergey Sablin on 17.04.2020.
// @serewkasablin@outlook.com
//

#ifndef SHMAP_H
#define SHMAP_H

#include <map>

namespace shmem {
    class Semaphore {
        sem_t* sem_;
    public:
        Semaphore(sem_t* sem);
        ~Semaphore();
    };

    Semaphore::Semaphore(sem_t* sem) : sem_(sem)
    {
        sem_wait(sem_);
    }

    Semaphore::~Semaphore() {
        sem_post(sem_);
    }

    template<typename Key,
            typename T,
            typename Compare = std::less<Key>,
            typename Allocator = MMapAllocator< std::pair<Key, T>, std::pair<Key, T>[5]> >
    class Shmap {
        std::map<Key, T, Compare, Allocator> map_;
    public:
        void insert(Key key, T value);
        void erase(size_t pos);
        void erase(const Key& key);
        void reset(Key key, T value);
        T at(const Key& key) const;
        Shmap() = default;
        Shmap(const Allocator& alloc);
    };

    template<typename Key, typename T, typename Compare,  typename Allocator>
    Shmap<Key, T, Compare, Allocator>::Shmap(const Allocator &alloc):
    map_(alloc)
    {
    }

    template<typename Key, typename T, typename Compare,  typename Allocator>
    void Shmap<Key, T, Compare,  Allocator>::insert(Key key, T value) {
        Semaphore sem(map_.get_allocator().get_sem());
        map_.insert(key, value);
    }

     template<typename Key, typename T, typename Compare,  typename Allocator>
    void Shmap<Key, T,Compare,  Allocator>::erase(size_t pos) {
        Semaphore sem(map_.get_allocator().get_sem());
        map_.erase(pos);
    }

     template<typename Key, typename T, typename Compare,  typename Allocator>
    void Shmap<Key, T,Compare,  Allocator>::erase(const Key &key) {
        Semaphore sem(map_.get_allocator().get_sem());
        map_.erase(key);
    }

     template<typename Key, typename T, typename Compare,  typename Allocator>
    T Shmap<Key, T,Compare,  Allocator>::at(const Key &key) const {
        Semaphore sem(map_.get_allocator().get_sem());
        return map_.at(key);
    }

     template<typename Key, typename T, typename Compare,  typename Allocator>
    void Shmap<Key, T,Compare,  Allocator>::reset(Key key, T value) {
        Semaphore sem(map_.get_allocator().get_sem());
        map_[key] = value;
    }
}

#endif //SHMAP_H
