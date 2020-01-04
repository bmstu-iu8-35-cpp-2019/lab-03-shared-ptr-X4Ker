// Copyright 2018 Your Name <your_email>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_

#include <atomic>
#include <exception>
#include <iostream>
#include <utility>

class Counter {
private:
    std::atomic_int counter;

public:
    Counter() { counter = 0; }

    void inc() { counter++; }

    void dec() { counter--; }

    int get() { return counter; }

    ~Counter() = default;
};

template  <typename T>
class SharedPtr {
private:
    T* ptr;
    Counter* counter;

public:
    SharedPtr(){
        ptr = nullptr;
        counter = nullptr;
    }

    SharedPtr(const SharedPtr& other){
        counter = other.counter;
        ptr = other.ptr;
        counter->inc();
    }

    SharedPtr(SharedPtr&& other) noexcept {
        counter = other.counter;
        ptr = other.ptr;
        counter->inc();
    }

    ~SharedPtr(){
        if (*this == true) {
            counter->dec();
            if (counter->get() == 0) {
                delete counter;
                delete ptr;
            }
        }
    }

    explicit SharedPtr(T* other){
        counter = new Counter();
        counter->inc();
        ptr = other;
    }

    auto operator=(const SharedPtr& other) -> SharedPtr& {
        if (*this == true) {
            counter->dec();
            if (counter->get() == 0) {
                delete counter;
                delete ptr;
            }
        }
        counter = other.counter;
        ptr = other.ptr;
        if (*this == true) counter->inc();
        return *this;
    }

    auto operator=(SharedPtr&& other) -> SharedPtr&{
        if (*this == true) {
            counter->dec();
            if (counter->get() == 0) {
                delete counter;
                delete ptr;
            }
        }
        counter = other.counter;
        ptr = other.ptr;
        if (*this == true) counter->inc();
        return *this;
    }

    operator bool() const {
        return counter != nullptr;
    }

    auto operator*() const -> T& {
        if (*this == false) throw std::invalid_argument("no object");
        return *ptr;
    }

    auto operator-> () const -> T* {
        return ptr;
    }

    auto get() -> T* {
        return ptr;
    }

    void reset(){
        if (*this == true) {
            counter->dec();
            if (counter->get() == 0) {
                delete counter;
                delete ptr;
            }
            counter = nullptr;
            ptr = nullptr;
        }
    }

    void reset(T* other){
        if (*this == true) {
            counter->dec();
            if (counter->get() == 0) {
                delete counter;
                delete ptr;
            }
            counter = new Counter();
            counter->inc();
            ptr = other;
        }
    }

    void swap(SharedPtr& other) {
        if (ptr != other.ptr) std::swap(other, *this);
    }

    auto use_count() const -> size_t {
        if (!counter) return 0;
        return static_cast<size_t>(counter->get());
    }
};

#endif // INCLUDE_HEADER_HPP_
