/***********************************************************************************************
	created: 		2020-06-22

	author:			chensong

	purpose:		2.alloc
我可能会遇到很多的人，听他们讲好2多的故事，我来写成故事或编成歌，用我学来的各种乐器演奏它。
然后还可能在一个国家遇到一个心仪我的姑娘，她可能会被我帅气的外表捕获，又会被我深邃的内涵吸引，在某个下雨的夜晚，她会全身淋透然后要在我狭小的住处换身上的湿衣服。
3小时候后她告诉我她其实是这个国家的公主，她愿意向父皇求婚。我不得已告诉她我是穿越而来的男主角，我始终要回到自己的世界。
然后我的身影慢慢消失，我看到她眼里的泪水，心里却没有任何痛苦，我才知道，原来我的心被丢掉了，我游历全世界的原因，就是要找回自己的本心。
于是我开始有意寻找各种各样失去心的人，我变成一块砖头，一颗树，一滴水，一朵白云，去听大家为什么会失去自己的本心。
我发现，刚出生的宝宝，本心还在，慢慢的，他们的本心就会消失，收到了各种黑暗之光的侵蚀。
从一次争论，到嫉妒和悲愤，还有委屈和痛苦，我看到一只只无形的手，把他们的本心扯碎，蒙蔽，偷走，再也回不到主人都身边。
我叫他本心猎手。他可能是和宇宙同在的级别 但是我并不害怕，我仔细回忆自己平淡的一生 寻找本心猎手的痕迹。
沿着自己的回忆，一个个的场景忽闪而过，最后发现，我的本心，在我写代码的时候，会回来。
安静，淡然，代码就是我的一切，写代码就是我本心回归的最好方式，我还没找到本心猎手，但我相信，顺着这个线索，我一定能顺藤摸瓜，把他揪出来。
************************************************************************************************/
//

#ifndef CSTL_SOURCE_CALLOC_H
#define CSTL_SOURCE_CALLOC_H

#include <iostream>
//#include <new>
#include "calloc_adaptor.h"
#include "calloc_mem_pool.h"
namespace  chen {

    namespace callocate
    {
        template <class T>
        inline T* __allocate(ptrdiff_t size, T*/*T**/)
        {
//        set_new_handler(0);
            std::cout << "size = " <<size << ", sizeof(T) = " << sizeof(T) << std::endl;
//            T* temp = (T*)::operator new ((size_t)(size * sizeof(T)));

           //()
            T* temp = (T*)  calloc_adaptor<T, csingle_client_alloc>::allocate ((size_t)(size /** sizeof(T)*/));// csingle_client_alloc::allocate((size_t)(size * sizeof(T)));
            printf("alloc address =%p\n", temp);
            calloc_adaptor<T, csingle_client_alloc>::show_info();
            if (!temp)
            {
                std::cerr << "alloc failed !!!" << std::endl;
                exit(1);
            }
            return temp;
        }
        template <class T>
        inline void __deallocate(T* buffer, size_t size)
        {
//            ::operator delete (buffer);
            // 释放的内存的大小
            std::cout << "delete size = " << size * sizeof(T) << std::endl;
            calloc_adaptor<T, csingle_client_alloc>::deallocate(buffer, size /** sizeof(T)*/);
            //csingle_client_alloc::deallocate(buffer, size * sizeof(T));
            calloc_adaptor<T, csingle_client_alloc>::show_info();
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
//
        template <class U>
        struct rebind
        {
            typedef callocator<U/*, callocator*/> other;
        };
        pointer allocate(size_type n, const void * hint=0)
        {
            return callocate::__allocate((difference_type)n, (pointer)0);
        }

        void deallocate(pointer p, size_type n)
        {
            callocate::__deallocate(p, n);
        }

        void construct(pointer p, const T& value)
        {
            callocate::__construct(p, value);
        }

        void destroy(pointer ptr)
        {
            callocate::__destroy(ptr);
        }

        pointer address(reference x)
        {
            return (pointer)&x;
        }

        const_pointer  const_address(const_reference x)
        {
            return (const_pointer)&x;
        }

        size_type max_size() const
        {
            return size_type(UINT_MAX/ sizeof(T));
        }
    };











}// chen
#endif //CSTL_SOURCE_CALLOC_H
