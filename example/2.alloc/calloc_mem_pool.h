/***********************************************************************************************
	created: 		2020-06-23

	author:			chensong

	purpose:		calloc_mem_pool
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

#ifndef CSTL_SOURCE_CALLOC_MEM_POOL_H
#define CSTL_SOURCE_CALLOC_MEM_POOL_H

#include "cmalloc_alloc.h"
#include <mutex>
namespace chen {

    union cnode_alloc_obj;
    union cnode_alloc_obj
    {
        union cnode_alloc_obj* free_list_link;
        char client_data[1]; // info address
    };


    template <bool threads, int inst>
    class calloc
    {
    public:
        // this one is needed for proper simple_alloc wrapping
        typedef char value_type;
        typedef cnode_alloc_obj obj;
    public:

        //内存对齐最小字节
        enum {__ALIGN = 8};
        // 内存池中最大内存节点 128
        enum {__MAX_BYTES = 128};
        // 内存
        enum {__NFREELISTS = __MAX_BYTES/__ALIGN};
    private:
        // 内存字节对齐 8的倍数
        static size_t  round_up(size_t bytes)
        {
            return ((bytes + __ALIGN -1) & ~(__ALIGN -1));
        }
        // 内存字节数组的下标
        static size_t free_list_index(size_t bytes)
        {
            return ((bytes + __ALIGN -1)/__ALIGN -1);//
        }
        //
        static void *refill(size_t n);

        static char * chunk_alloc(size_t size, int & nobjes);

        // Chunk allocation state.
        static char *start_free;
        static char *end_free;
        static size_t heap_size;
    public:
        /* n must be > 0      */
        static void * allocate(size_t n);
        /* p may not be 0 */
        static void deallocate(void *p, size_t n);
        static void * reallocate(void *p, size_t old_sz, size_t new_sz);
        static void show_info()
        {
            printf("mem_pool node size = %llu\n", (size_t)(end_free - start_free));
            printf("mem_pool use size = %llu\n", m_use_size);
            printf("mem_pool size = %llu\n", heap_size);
        }
    private :
        static obj *  free_list[__NFREELISTS];
        static  std::mutex m_lock;
        static size_t  m_use_size;
    };



    typedef calloc<false, 0 > csingle_client_alloc;
    typedef calloc<true, 0 >  cmultithreaded_alloc;


    template <bool threads, int inst>
    inline void * calloc<threads,inst>::allocate(size_t n)
    {
        obj **  my_free_list;
        obj *  result;

        m_use_size +=n;
        if (n > (size_t)__MAX_BYTES)
        {
            return(malloc_alloc::allocate(n));
        }
        printf("allocate index = %llu\n", free_list_index(n));
        my_free_list = free_list + free_list_index(n);
        // Acquire the lock here with a constructor call.
        // This ensures that it is released in exit or during stack
        // unwinding.
        /*REFERENCED*/
        if (threads)
        {
            m_lock.lock();
        }
        result = *my_free_list;
        if (result == 0)
        {
            void *r = refill(round_up(n));
            return r;
        }
        *my_free_list = result -> free_list_link;
        if (threads)
        {
            m_lock.unlock();
        }
        return (result);
    }
    template <bool threads, int inst>
    inline void calloc<threads, inst>::deallocate(void *p, size_t n)
    {
        obj *q = (obj *)p;
        obj  ** my_free_list;

        m_use_size -=n;
        if (n > (size_t)__MAX_BYTES)
        {

            malloc_alloc::deallocate(p, n);
            return;
        }
        printf("deallocate  index = %llu\n", free_list_index(n));
        my_free_list = free_list + free_list_index(n);
        // acquire lock

        if (threads)
        {
            m_lock.lock();
        }
        q->free_list_link = *my_free_list;
        *my_free_list = q;
        // lock is released here
        if (threads)
        {
            m_lock.unlock();
        }
    }




/* We allocate memory in large chunks in order to avoid fragmenting     */
/* the malloc heap too much.                                            */
/* We assume that size is properly aligned.                             */
/* We hold the allocation lock.                                         */
    template <bool threads, int inst>
    char* calloc<threads, inst>::chunk_alloc(size_t size, int& nobjs)
    {
        char * result;
        size_t total_bytes = size * nobjs;
        size_t bytes_left = end_free - start_free;

        if (bytes_left >= total_bytes)
        {
            result = start_free;
            start_free += total_bytes;
            return(result);
        }
        else if (bytes_left >= size)
        {
            nobjs = bytes_left/size;
            total_bytes = size * nobjs;
            result = start_free;
            start_free += total_bytes;
            return(result);
        }
        else
            {
                // 这边 heap_size  >> 4 ====> 扩容作准备的  内存池
            size_t bytes_to_get = 2 * total_bytes + round_up(heap_size >> 4);
            // Try to make use of the left-over piece.
            if (bytes_left > 0)
            {
                obj  ** my_free_list = free_list + free_list_index(bytes_left);

                ((obj *)start_free) -> free_list_link = *my_free_list;
                *my_free_list = (obj *)start_free;
            }
            start_free = (char *)malloc_alloc::allocate(bytes_to_get);
            if (0 == start_free)
            {
                int i;
                obj  ** my_free_list;
                obj *p;
                // Try to make do with what we have.  That can't
                // hurt.  We do not try smaller requests, since that tends
                // to result in disaster on multi-process machines.
                for (i = size; i <= __MAX_BYTES; i += __ALIGN)
                {
                    my_free_list = free_list + free_list_index(i);
                    p = *my_free_list;
                    if (0 != p)
                    {
                        *my_free_list = p -> free_list_link;
                        start_free = (char *)p;
                        end_free = start_free + i;
                        return(chunk_alloc(size, nobjs));
                        // Any leftover piece will eventually make it to the
                        // right free list.
                    }
                }
                end_free = 0;	// In case of exception.
                start_free = (char *)malloc_alloc::allocate(bytes_to_get);
                // This should either throw an
                // exception or remedy the situation.  Thus we assume it
                // succeeded.
            }
            heap_size += bytes_to_get;
            end_free = start_free + bytes_to_get;
            return(chunk_alloc(size, nobjs));
        }
    }

/* Returns an object of size n, and optionally adds to size n free list.*/
/* We assume that n is properly aligned.                                */
/* We hold the allocation lock.                                         */
    template <bool threads, int inst>
    void* calloc<threads, inst>::refill(size_t n)
    {
        int nobjs = 20;
        char * chunk = chunk_alloc(n, nobjs);
        obj  *  * my_free_list;
        obj * result;
        obj * current_obj, * next_obj;
        int i;

        if (1 == nobjs)
        {
            return(chunk);
        }
        my_free_list = free_list + free_list_index(n);

        /* Build free list in chunk */
        // node list -->>>>>>  8 * ? = 128
        result = (obj *)chunk;
        *my_free_list = next_obj = (obj *)(chunk + n);
        for (i = 1; ; i++)
        {
            current_obj = next_obj;
            next_obj = (obj *)((char *)next_obj + n);
            if (nobjs - 1 == i)
            {
                current_obj -> free_list_link = 0;
                break;
            }
            else
                {
                current_obj -> free_list_link = next_obj;
            }
        }
        return(result);
    }

    template <bool threads, int inst>
    void* calloc<threads, inst>::reallocate(void *p, size_t old_sz, size_t new_sz)
    {
        void * result;
        size_t copy_sz;

        if (old_sz > (size_t) __MAX_BYTES && new_sz > (size_t) __MAX_BYTES)
        {
            return(malloc_alloc::reallocate(p, old_sz, new_sz));
        }
        if (round_up(old_sz) == round_up(new_sz))
        {
            return(p);
        }
        result = allocate(new_sz);
        copy_sz = new_sz > old_sz? old_sz : new_sz;
        ::memcpy(result, p, copy_sz);
        deallocate(p, old_sz);
        return(result);
    }

    template <bool threads, int inst>
    char *calloc<threads, inst>::start_free = 0;

    template <bool threads, int inst>
    char *calloc<threads, inst>::end_free = 0;

    template <bool threads, int inst>
    size_t calloc<threads, inst>::heap_size = 0;
    template <bool threads, int inst>
    size_t calloc<threads, inst>::m_use_size = 0;
    template <bool threads, int inst>
    cnode_alloc_obj * calloc<threads, inst>::free_list[__NFREELISTS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//    struct cnode_alloc_obj {
//        cnode_alloc_obj * _M_next;
//    };
//    static const size_t _ALIGN = 8;
//    static const size_t _ALIGN_SHIFT = 3;
//    static const size_t _STLP_NFREELISTS = 16; //内存节点 的数组
//    inline size_t FREELIST_INDEX(size_t __bytes)
//    {
//        return ((__bytes - size_t(1)) >> (int)_ALIGN_SHIFT);
//    }
//    class cnode_alloc_impl
//    {
//    public:
//        /**
//         * 内存对齐 4
//         * @param __bytes
//         * @return
//         */
//        static inline size_t round_up(size_t __bytes)
//        {
//            return (((__bytes) + (size_t)_ALIGN-1) & ~((size_t)_ALIGN - 1));
//        }
//
//
//        typedef cnode_alloc_obj       _Obj;
//        typedef _Obj*                 _Freelist;
//        typedef _Obj*                 _ChunkList;
//
//
//        private:
//            // Returns an object of size __n, and optionally adds to size __n free list.
//            static _Obj* _S_refill(size_t __n);
//            // Allocates a chunk for nobjs of size __p_size.  nobjs may be reduced
//            // if it is inconvenient to allocate the requested number.
//            static char* _S_chunk_alloc(size_t __p_size, int& __nobjs);
//            // Chunk allocation state.
//            static _Freelist _S_free_list[_STLP_NFREELISTS];
//            // Amount of total allocated memory
//
//            static size_t _S_heap_size;
//
//            // Start of the current free memory buffer
//            static char* _S_start_free;
//            // End of the current free memory buffer
//            static char* _S_end_free;
//
//
//
//
//        public:
//            /* __n must be > 0      */
//            static void* _M_allocate(size_t& __n);
//            /* __p may not be 0 */
//            static void _M_deallocate(void *__p, size_t __n);
//
//
//    };
//
//
//
//    void* cnode_alloc_impl::_M_allocate(size_t& __n) {
//        __n = round_up(__n);
//        _Obj  * __my_free_list = _S_free_list + FREELIST_INDEX(__n);
//        _Obj *__r;
//
//        // Acquire the lock here with a constructor call.
//        // This ensures that it is released in exit or during stack
//        // unwinding.
//
//
//        if ( (__r  = *__my_free_list) != 0 ) {
//            *__my_free_list = __r->_M_next;
//        } else {
//            __r = _S_refill(__n);
//        }
//        // lock is released here
//        return __r;
//    }
//
//
//
//    void cnode_alloc_impl::_M_deallocate(void *__p, size_t __n) {
//        _Obj * /*_STLP_VOLATILE **/ __my_free_list = _S_free_list + FREELIST_INDEX(__n);
//        _Obj * __pobj = static_cast<_Obj * >(__p);//__STATIC_CAST(_Obj*, __p);
//
//        // acquire lock
//      //  _Node_Alloc_Lock __lock_instance;
//        __pobj->_M_next = *__my_free_list;
//        *__my_free_list = __pobj;
//
////#  if defined (_STLP_DO_CLEAN_NODE_ALLOC)
////        _S_dealloc_call();
////#  endif
//        // lock is released here
//    }
//
//    /* We allocate memory in large chunks in order to avoid fragmenting     */
///* the malloc heap too much.                                            */
///* We assume that size is properly aligned.                             */
///* We hold the allocation lock.                                         */
//    char* cnode_alloc_impl::_S_chunk_alloc(size_t _p_size, int& __nobjs) {
//        char* __result;
//        size_t __total_bytes = _p_size * __nobjs;
//        size_t __bytes_left = _S_end_free - _S_start_free;
//
//        if (__bytes_left > 0) {
//            if (__bytes_left >= __total_bytes) {
//                __result = _S_start_free;
//                _S_start_free += __total_bytes;
//                return __result;
//            }
//
//            if (__bytes_left >= _p_size) {
//                __nobjs = (int)(__bytes_left / _p_size);
//                __total_bytes = _p_size * __nobjs;
//                __result = _S_start_free;
//                _S_start_free += __total_bytes;
//                return __result;
//            }
//
//            // Try to make use of the left-over piece.
//            _Obj* /*_STLP_VOLATILE**/ __my_free_list = _S_free_list + FREELIST_INDEX(__bytes_left);
//            reinterpret_cast<_Obj*>(/*__REINTERPRET_CAST(_Obj*,*/ _S_start_free)->_M_next = *__my_free_list;
//            *__my_free_list = reinterpret_cast<_Obj*>(/*__REINTERPRET_CAST(_Obj*,*/ _S_start_free);
//        }
//
//        size_t __bytes_to_get =
//                2 * __total_bytes + _S_round_up(_S_heap_size >> 4)
////#  if defined (_STLP_DO_CLEAN_NODE_ALLOC)
//        + sizeof(_Obj)
////#  endif
//        ;
//
//        _S_start_free = static_cast<char *> (/*__STATIC_CAST(char*,*/__stlp_chunk_malloc(__bytes_to_get));
//        if (0 == _S_start_free) {
//            _Obj* /*_STLP_VOLATILE**/ __my_free_list;
//            _Obj* __p;
//            // Try to do with what we have.  That can't hurt.
//            // We do not try smaller requests, since that tends
//            // to result in disaster on multi-process machines.
//            for (size_t __i = _p_size; __i <= (size_t)_MAX_BYTES; __i += (size_t)_ALIGN) {
//                __my_free_list = _S_free_list + FREELIST_INDEX(__i);
//                __p = *__my_free_list;
//                if (0 != __p) {
//                    *__my_free_list = __p -> _M_next;
//                    _S_start_free = reinterpret_cast<char*>( __p);
//                    _S_end_free = _S_start_free + __i;
//                    return _S_chunk_alloc(_p_size, __nobjs);
//                    // Any leftover piece will eventually make it to the
//                    // right free list.
//                }
//            }
//            _S_end_free = 0;    // In case of exception.
//            _S_start_free = static_cast<char *>(__stlp_chunk_malloc(__bytes_to_get));
//            /*
//            (char*)malloc_alloc::allocate(__bytes_to_get);
//            */
//
//            // This should either throw an
//            // exception or remedy the situation.  Thus we assume it
//            // succeeded.
//        }
//
//        _S_heap_size += __bytes_to_get;
////#  if defined (_STLP_DO_CLEAN_NODE_ALLOC)
////        __REINTERPRET_CAST(_Obj*, _S_start_free)->_M_next = _S_chunks;
////  _S_chunks = __REINTERPRET_CAST(_Obj*, _S_start_free);
////#  endif
//        _S_end_free = _S_start_free + __bytes_to_get;
////#  if defined (_STLP_DO_CLEAN_NODE_ALLOC)
////        _S_start_free += sizeof(_Obj);
////#  endif
//        return _S_chunk_alloc(_p_size, __nobjs);
//    }
//
///* Returns an object of size __n, and optionally adds to size __n free list.*/
///* We assume that __n is properly aligned.                                  */
///* We hold the allocation lock.                                             */
//    cnode_alloc_obj* cnode_alloc_impl::_S_refill(size_t __n) {
//        int __nobjs = 20;
//        char* __chunk = _S_chunk_alloc(__n, __nobjs);
//
//        if (1 == __nobjs) return reinterpret_cast<_Obj*> (/* __REINTERPRET_CAST(_Obj*,*/__chunk);
//
//        _Obj* /*_STLP_VOLATILE**/ __my_free_list = _S_free_list + FREELIST_INDEX(__n);
//        _Obj* __result;
//        _Obj* __current_obj;
//        _Obj* __next_obj;
//
//        /* Build free list in chunk */
//        __result = reinterpret_cast<_Obj*>(/*__REINTERPRET_CAST(_Obj*,*/ __chunk);
//        *__my_free_list = __next_obj = reinterpret_cast<_Obj*>(/*__REINTERPRET_CAST(_Obj*,*/ __chunk + __n);
//        for (--__nobjs; --__nobjs; ) {
//            __current_obj = __next_obj;
//            __next_obj =reinterpret_cast<_Obj*>(/*__REINTERPRET_CAST(_Obj*,*/ reinterpret_cast<char *>(/*__REINTERPRET_CAST(char*,*/ __next_obj) + __n);
//            __current_obj->_M_next = __next_obj;
//        }
//        __next_obj->_M_next = 0;
//        return __result;
//    }
//
//    cnode_alloc_obj *
//            cnode_alloc_impl::_S_free_list[_STLP_NFREELISTS]
//    = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//// The 16 zeros are necessary to make version 4.1 of the SunPro
//// compiler happy.  Otherwise it appears to allocate too little
//// space for the array.
//
//
//cnode_alloc_impl::_S_start_free = 0;
//cnode_alloc_impl::_S_end_free = 0;
//
//
//
//    cnode_alloc_impl::_S_heap_size = 0;



}
#endif //CSTL_SOURCE_CALLOC_MEM_POOL_H
