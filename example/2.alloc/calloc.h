/**********************************************************
create: 2020/6/22.

author: chensong 
**********************************************************/
//

#ifndef CSTL_SOURCE_CALLOC_H
#define CSTL_SOURCE_CALLOC_H

#include <iostream>
#include <new>
namespace  chen {

    template <class T>
    inline T* __allocate(ptrdiff_t size, T*)
    {
//        set_new_handler(0);
        T* temp = (T*)::operator new ((size_t)(size * sizeof(T)));

        if (!temp)
        {
            std::cerr << "alloc failed !!!" << std::endl;
            exit(1);
        }
        return temp;
    }
    template <class T>
    inline void __deallocate(T* buffer)
    {
        ::operator delete (buffer);
    }

    template <class T1, class T2>
    inline void __construct (T1*p, const T2& value)
    {
        new (p)T1(value);
    }

    template <class T>
    inline void __destroy(T*ptr)
    {
        ptr->~T();
    }

    template <class T>
    class callocator
    {
    public:
        typedef T          value_type;
        typedef T*         pointer;
        typedef const T*   const_pointer;
        typedef T&          reference;
        typedef const T&    const_reference;
        typedef size_t      size_type;
        typedef ptrdiff_t   difference_type;
        template <class U>
        struct rebind
        {
            typedef callocator<U/*, callocator*/> other;
        };
        pointer allocate(size_type n, const void * hint=0)
        {
            return __allocate((difference_type)n, (pointer)0);
        }

        void deallocate(pointer p, size_type n)
        {
            __deallocate(p);
        }

        void construct(pointer p, const T& value)
        {
            __construct(p, value);
        }

        void destroy(pointer ptr)
        {
            __destroy(ptr);
        }

        pointer address(reference x)
        {
            return (pointer)&x;
        }

        const_pointer  const_address(const_reference x)
        {
            return (const_pointer0)&x;
        }

        size_type max_size() const
        {
            return size_type(UINT_MAX/ sizeof(T));
        }


    };

}// chen
#endif //CSTL_SOURCE_CALLOC_H
