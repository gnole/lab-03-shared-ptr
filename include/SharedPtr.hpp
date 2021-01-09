#ifndef INCLUDE_SHAREDPTR_HPP_
#define INCLUDE_SHAREDPTR_HPP_

#include <atomic>
#include <iostream>
#include <utility>

struct Counter {
  std::atomic<unsigned int> reference_count;
  explicit Counter(const int& number) : reference_count(number) {}
};

template <class T>
class SharedPtr {
  Counter* count;
  T* main_ptr;
 public:
  SharedPtr();
  explicit SharedPtr(T* ptr);
  SharedPtr(SharedPtr& r_ptr);
  SharedPtr(SharedPtr&& r_ptr) noexcept;
  ~SharedPtr();
  SharedPtr<T>& operator=(SharedPtr& r_ptr);
  SharedPtr& operator=(SharedPtr&& r_ptr) noexcept;

  explicit operator bool() const;
  T& operator*() const;
  T* operator->() const;

  T* get();
  void reset();
  void reset(T* ptr);
  void swap(SharedPtr& r_ptr) noexcept;

  void print_info() const;
  [[nodiscard]] size_t use_count() const;
};

template <class T>
SharedPtr<T>::SharedPtr() {
  main_ptr = nullptr;
  count = nullptr;
}

template <class T>
SharedPtr<T>::SharedPtr(T* ptr) {
  main_ptr = ptr;
  count = new Counter(1);
}

template <class T>
SharedPtr<T>::SharedPtr(SharedPtr<T>& r_ptr) {
  main_ptr = r_ptr.main_ptr;
  count = r_ptr.count;
  count->reference_count.fetch_add(1, std::memory_order_relaxed);
}

template <class T>
void SharedPtr<T>::swap(SharedPtr<T>& r_ptr) noexcept {
  std::swap(main_ptr, r_ptr.main_ptr);
  std::swap(count, r_ptr.count);
}

template <class T>
void SharedPtr<T>::reset() {
  SharedPtr<T>().swap(*this);
}

template <class T>
void SharedPtr<T>::reset(T* ptr) {
  SharedPtr<T>(ptr).swap(*this);
}

template <typename T>
SharedPtr<T>::SharedPtr(SharedPtr<T>&& r_ptr) noexcept {
  main_ptr = nullptr;
  count = nullptr;
  swap(r_ptr);
}

template <class T>
T * SharedPtr<T>::get() {
  return main_ptr;
}

template <class T>
T & SharedPtr<T>::operator*() const {
  return *main_ptr;
}

template <class T>
T * SharedPtr<T>::operator->() const {
  return main_ptr;
}

template <class T>
size_t SharedPtr<T>::use_count() const {
  return count->reference_count.load();
}

template <class T>
SharedPtr<T>::operator bool() const {
  return main_ptr ? true : false;
}

template <class T>
SharedPtr<T> & SharedPtr<T>::operator=(SharedPtr<T>&& r_ptr) noexcept {
  if (this != &r_ptr) return *this;
  swap(r_ptr);
  r_ptr.reset();
  return *this;
}

template <class T>
SharedPtr<T>::~SharedPtr<T>() {
  if (count != nullptr) {
    if (count->reference_count.fetch_sub(1, std::memory_order_relaxed) == 1) {
      delete main_ptr;
      delete count;
      main_ptr = nullptr;
      count = nullptr;
    }
  }
}

template <class T>
SharedPtr<T> & SharedPtr<T>::operator=(SharedPtr<T>& r_ptr) {
  if (this == &r_ptr) return *this;
  reset();
  main_ptr = r_ptr.main_ptr;
  count = r_ptr.count;
  count->reference_count.fetch_add(1, std::memory_order_relaxed);
  return *this;
}

template <class T>
void SharedPtr<T>::print_info() const {
  std::cout << "refs: " << count->reference_count.load() << std::endl;
  std::cout << "object: " << main_ptr << std::endl;
}

#endif // INCLUDE_SHAREDPTR_HPP_
