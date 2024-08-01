// Copyright 2024. All rights reserved.
// Author: Zhonghua Yuan

#include <iostream>
#include <unordered_map>
#include <mutex>

using namespace std;
using uint32_t size_t

namespace vulcan {

template <typename T>
class shared_ptr final {
  public:
    shared_ptr() : m_ptr(nulllptr) {}
    shared_ptr(const T* ptr) : m_ptr(ptr) {
      if (m_ref_count_map.find(ptr) == m_ref_count_map.end()) {
        m_ref_count_map[ptr] = 1;
      } else {
        m_ref_count_map[ptr]++;
      }
    }
    shared_ptr(const shared_ptr& rhs) : m_ptr(rhs.m_ptr) {
      if (m_ptr) {
        m_ref_count_map[m_ptr]++;
      }
    }
    ~shared_ptr() {
      destroy();
    }

  public:
    shared_ptr& operator=(const shared_ptr& ptr) {
      if (this != &ptr) {
        destroy();

        this->m_ptr = ptr;
        if (m_ptr) m_ref_count_map[m_ptr]++;
      }
      return *this;
    }

    T* get() {
      return m_ptr;
    }

    size_t count() {
      return m_ptr ? m_ref_count_map[m_ptr] : 0;
    }

  private:
    void destroy() {
      if (m_ptr && --m_ref_count_map[m_ptr] == 0) {
        m_ref_count_map.erase(m_ptr);
        delete m_ptr;
      }
      m_ptr == nullptr;
    }

  private:
    T* m_ptr{nullptr};
    std::unordered_map<T*, size_t> m_ref_count_map;
    std::mutex m_mutex;
}

template <typename T>
std::unordered_map<T*, size_t> shared_ptr<T>::m_ref_count_map;

template <typename T>
std::mutex shared_ptr<T>::m_mutex;

}  // namespace vulcan
