#pragma once

namespace msp {

    template <typename T, typename Derived>
    class SmartPtrBase {
    public:
        T& operator*() const {
            return *static_cast<const Derived*>(this)->get();
        }

        T* operator->() const {
            return static_cast<const Derived*>(this)->get();
        }

        T* get() const {
            return static_cast<const Derived*>(this)->get();
        }

        void __cleanup__()
        {
            if (ptr != nullptr) {
                delete ptr;
            }
        }
    };

    template <typename T>
    class UniquePtr : public SmartPtrBase<T, UniquePtr<T>> {
    public:
        UniquePtr : ptr(nullptr) {};
        explicit UniquePtr(T* ptr) : ptr(ptr) {};
        ~UniquePtr() {
            __cleanup__();
        }
        
        // Move constructor
        UniquePtr(UniquePtr&& dyingObj) {
            this->ptr = dyingObj.ptr;
            dyingObj.ptr = nullptr;
        } 

        // Move assignment
        UniquePtr& operator=(UniquePtr&& dyingObj) {
            
            __cleanup__();

            this->ptr = dyingObj.ptr;
            dyingObj.ptr = nullptr;
        };

    private:
        T* ptr = nullptr;

        // delete copy constructor
        UniquePtr(const UniquePtr& obj) = delete; 

        // delete copy assignment
        UniquePtr& operator=(const UniquePtr& obj) = delete; 
    };

    template<typename T>
    class SharedPtrControlBlock {
    public:
        explicit SharedPtrControlBlock(T* ptr);
        void addRef();
        void release();
        int useCount() const;
        T* get() const;

    private:
        T* ptr;
    };

    template<typename T>
    class SharePtr : public SmartPtrBase<T, SharedPtr<T>> {
    public:
        explicit SharePtr(T* ptr = nullptr);
        SharedPtr(const SharedPtr& other);
        SharedPtr& operator=(const SharedPtr& other);
        ~SharedPtr();

        int useCount() const;
        T* get() const;

    private:
        SharedPtrControlBlock<T>* controlBlock;
    };

} // namespace msp