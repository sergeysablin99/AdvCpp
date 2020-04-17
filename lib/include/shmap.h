//
// Created by Sergey Sablin on 17.04.2020.
// @serewkasablin@outlook.com
//

#ifndef SHMAP_H
#define SHMAP_H

#include <semaphore.h>
#include <map>

namespace shmem {
    template<typename Key,
            typename T,
            typename Allocator>
    class Shmap {
        std::map<Key, T, std::less<Key>, Allocator> map_;
        sem_t* sem_;
    public:
        void insert(Key key, T value);
        void erase(size_t pos);
        void erase(const Key& key);
        void reset(const Key& key, T value);
        const T& at(const Key& key) const;
        Shmap();
    };

    template<typename Key, typename T, typename Allocator>
    void Shmap<Key, T, Allocator>::insert(Key key, T value) {
        sem_wait(sem_);
        map_.insert(key, value);
        sem_post(sem_);
    }

    template<typename Key, typename T, typename Allocator>
    void Shmap<Key, T, Allocator>::erase(size_t pos) {
        sem_wait(sem_);
        map_.erase(pos);
        sem_post(sem_);
    }

    template<typename Key, typename T, typename Allocator>
    void Shmap<Key, T, Allocator>::erase(const Key &key) {
        sem_wait(sem_);
        map_.erase(key);
        sem_post(sem_);
    }

    template<typename Key, typename T, typename Allocator>
    const T& Shmap<Key, T, Allocator>::at(const Key &key) const {
        sem_wait(sem_);
        const T& reply = map_.at(key);
        sem_post(sem_);
        return reply;
    }

    template<typename Key, typename T, typename Allocator>
    void Shmap<Key, T, Allocator>::reset(const Key &key, T value) {
        sem_wait(sem_);
        map_[key] = value;
        sem_post(sem_);
    }

    template<typename Key, typename T, typename Allocator>
    Shmap<Key, T, Allocator>::Shmap() {
        MMapAllocator<sem_t> alloc;
        sem_ = alloc.allocate(sizeof(sem_t));
        sem_init(sem_, 1, 1);
    }
}

#endif //SHMAP_H
