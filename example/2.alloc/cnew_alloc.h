/***********************************************************************************************
	created: 		2020-06-24

	author:			chensong

	purpose:		cnew_alloc
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
#ifndef CSTL_SOURCE_CNEW_ALLOC_H
#define CSTL_SOURCE_CNEW_ALLOC_H

namespace chen {
// New-based allocator.  Typically slower than default alloc below.
// Typically thread-safe and more storage efficient.
    template <int inst>
    class cnew_alloc {
    public:
        // this one is needed for proper simple_alloc wrapping
        typedef char value_type;
        static void*  allocate(size_t n)
        {
            return 0 == n ? 0 : ::operator new(n);
        }
        static void*  reallocate(void *p, size_t old_sz, size_t new_sz)
        {
            void* result = allocate(new_sz);
            size_t copy_sz = new_sz > old_sz? old_sz : new_sz;
            memcpy(result, p, copy_sz);
            deallocate(p, old_sz);
            return result;
        }
        static void deallocate(void* p)
        {
            ::operator delete(p);
        }
        static void deallocate(void* p, size_t)
        {
            ::operator delete(p);
        }
    };

    typedef cnew_alloc<0> new_alloc;


} // namespace chen
#endif //CSTL_SOURCE_CNEW_ALLOC_H
