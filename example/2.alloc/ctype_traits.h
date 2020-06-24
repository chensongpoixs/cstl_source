/***********************************************************************************************
	created: 		2020-06-24

	author:			chensong

	purpose:		ctype_traits
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

#ifndef CSTL_SOURCE_CTYPE_TRAITS_H
#define CSTL_SOURCE_CTYPE_TRAITS_H
namespace chen
{

    struct ctrue_type {
    };

    struct cfalse_type {
    };

    template <class type>
    struct ctype_traits {
        typedef ctrue_type     this_dummy_member_must_be_first;
        /* Do not remove this member. It informs a compiler which
           automatically specializes __type_traits that this
           __type_traits template is special. It just makes sure that
           things work if an implementation is using a template
           called __type_traits for something unrelated. */

        /* The following restrictions should be observed for the sake of
           compilers which automatically produce type specific specializations
           of this class:
               - You may reorder the members below if you wish
               - You may remove any of the members below if you wish
               - You must not rename members without making the corresponding
                 name change in the compiler
               - Members you add will be treated like regular members unless
                 you add the appropriate support in the compiler. */


        typedef cfalse_type    has_trivial_default_constructor;
        typedef cfalse_type    has_trivial_copy_constructor;
        typedef cfalse_type    has_trivial_assignment_operator;
        typedef cfalse_type    has_trivial_destructor;
        typedef cfalse_type    is_POD_type;
    };

// Provide some specializations.  This is harmless for compilers that
//  have built-in __types_traits support, and essential for compilers
//  that don't.

    template <>
    struct ctype_traits<char> {
        typedef ctrue_type    has_trivial_default_constructor;
        typedef ctrue_type    has_trivial_copy_constructor;
        typedef ctrue_type    has_trivial_assignment_operator;
        typedef ctrue_type    has_trivial_destructor;
        typedef ctrue_type    is_POD_type;
    };

    template <>
    struct ctype_traits<signed char> {
        typedef ctrue_type    has_trivial_default_constructor;
        typedef ctrue_type    has_trivial_copy_constructor;
        typedef ctrue_type    has_trivial_assignment_operator;
        typedef ctrue_type    has_trivial_destructor;
        typedef ctrue_type    is_POD_type;
    };

    template <>
    struct ctype_traits<unsigned char> {
        typedef ctrue_type    has_trivial_default_constructor;
        typedef ctrue_type    has_trivial_copy_constructor;
        typedef ctrue_type    has_trivial_assignment_operator;
        typedef ctrue_type    has_trivial_destructor;
        typedef ctrue_type    is_POD_type;
    };

    template <>
    struct ctype_traits<short> {
        typedef ctrue_type    has_trivial_default_constructor;
        typedef ctrue_type    has_trivial_copy_constructor;
        typedef ctrue_type    has_trivial_assignment_operator;
        typedef ctrue_type    has_trivial_destructor;
        typedef ctrue_type    is_POD_type;
    };

    template <>
    struct ctype_traits<unsigned short> {
        typedef ctrue_type    has_trivial_default_constructor;
        typedef ctrue_type    has_trivial_copy_constructor;
        typedef ctrue_type    has_trivial_assignment_operator;
        typedef ctrue_type    has_trivial_destructor;
        typedef ctrue_type    is_POD_type;
    };

    template <>
    struct ctype_traits<int> {
        typedef ctrue_type    has_trivial_default_constructor;
        typedef ctrue_type    has_trivial_copy_constructor;
        typedef ctrue_type    has_trivial_assignment_operator;
        typedef ctrue_type    has_trivial_destructor;
        typedef ctrue_type    is_POD_type;
    };

    template <>
    struct ctype_traits<unsigned int> {
        typedef ctrue_type    has_trivial_default_constructor;
        typedef ctrue_type    has_trivial_copy_constructor;
        typedef ctrue_type    has_trivial_assignment_operator;
        typedef ctrue_type    has_trivial_destructor;
        typedef ctrue_type    is_POD_type;
    };

    template <>
    struct ctype_traits<long> {
        typedef ctrue_type    has_trivial_default_constructor;
        typedef ctrue_type    has_trivial_copy_constructor;
        typedef ctrue_type    has_trivial_assignment_operator;
        typedef ctrue_type    has_trivial_destructor;
        typedef ctrue_type    is_POD_type;
    };

    template <>
    struct ctype_traits<unsigned long> {
        typedef ctrue_type    has_trivial_default_constructor;
        typedef ctrue_type    has_trivial_copy_constructor;
        typedef ctrue_type    has_trivial_assignment_operator;
        typedef ctrue_type    has_trivial_destructor;
        typedef ctrue_type    is_POD_type;
    };

    template <>
    struct ctype_traits<long long> {
       typedef ctrue_type    has_trivial_default_constructor;
       typedef ctrue_type    has_trivial_copy_constructor;
       typedef ctrue_type    has_trivial_assignment_operator;
       typedef ctrue_type    has_trivial_destructor;
       typedef ctrue_type    is_POD_type;
    };

    template <>
    struct ctype_traits<unsigned long long> {
       typedef ctrue_type    has_trivial_default_constructor;
       typedef ctrue_type    has_trivial_copy_constructor;
       typedef ctrue_type    has_trivial_assignment_operator;
       typedef ctrue_type    has_trivial_destructor;
       typedef ctrue_type    is_POD_type;
    };


    template <>
    struct ctype_traits<float> {
        typedef ctrue_type    has_trivial_default_constructor;
        typedef ctrue_type    has_trivial_copy_constructor;
        typedef ctrue_type    has_trivial_assignment_operator;
        typedef ctrue_type    has_trivial_destructor;
        typedef ctrue_type    is_POD_type;
    };

    template <>
    struct ctype_traits<double> {
        typedef ctrue_type    has_trivial_default_constructor;
        typedef ctrue_type    has_trivial_copy_constructor;
        typedef ctrue_type    has_trivial_assignment_operator;
        typedef ctrue_type    has_trivial_destructor;
        typedef ctrue_type    is_POD_type;
    };

    template <>
    struct ctype_traits<long double> {
       typedef ctrue_type    has_trivial_default_constructor;
       typedef ctrue_type    has_trivial_copy_constructor;
       typedef ctrue_type    has_trivial_assignment_operator;
       typedef ctrue_type    has_trivial_destructor;
       typedef ctrue_type    is_POD_type;
    };



    template <class T>
    struct ctype_traits<T*> {
       typedef ctrue_type    has_trivial_default_constructor;
       typedef ctrue_type    has_trivial_copy_constructor;
       typedef ctrue_type    has_trivial_assignment_operator;
       typedef ctrue_type    has_trivial_destructor;
       typedef ctrue_type    is_POD_type;
    };

// provide a mechanism for partial specialization of various
// type_traits. No reason for __true_dominate for now.

template <class T1, class T2>
struct cfalse_dominate {
    typedef cfalse_type resulting_type;
};

    template <>
struct cfalse_dominate<ctrue_type, ctrue_type> {
    typedef ctrue_type  resulting_type;
};

// a type_traits for two types composition
template <class T1, class T2>
struct ctype_traits_compose {
    typedef ctype_traits<T1> type1;
    typedef ctype_traits<T2> type2;
    typedef typename cfalse_dominate<typename type1::has_trivial_default_constructor,
                          typename type2::has_trivial_default_constructor>::resulting_type
            has_trivial_default_constructor;
    typedef typename cfalse_dominate<typename type1::has_trivial_copy_constructor,
                          typename type2::has_trivial_copy_constructor>::resulting_type
            has_trivial_copy_constructor;
    typedef typename cfalse_dominate<typename type1::has_trivial_assignment_operator,
                          typename type2::has_trivial_assignment_operator>::resulting_type
            has_trivial_assignment_operator;
    typedef typename cfalse_dominate<typename type1::has_trivial_destructor,
                             typename type2::has_trivial_destructor>::resulting_type
            has_trivial_destructor;
    typedef typename cfalse_dominate<typename type1::is_POD_type,
                             typename type2::is_POD_type>::resulting_type
            is_POD_type;
};



    template <>
    struct ctype_traits<char*> {
        typedef ctrue_type    has_trivial_default_constructor;
        typedef ctrue_type    has_trivial_copy_constructor;
        typedef ctrue_type    has_trivial_assignment_operator;
        typedef ctrue_type    has_trivial_destructor;
        typedef ctrue_type    is_POD_type;
    };

    template <>
    struct ctype_traits<signed char*> {
        typedef ctrue_type    has_trivial_default_constructor;
        typedef ctrue_type    has_trivial_copy_constructor;
        typedef ctrue_type    has_trivial_assignment_operator;
        typedef ctrue_type    has_trivial_destructor;
        typedef ctrue_type    is_POD_type;
    };

    template <>
    struct ctype_traits<unsigned char*> {
        typedef ctrue_type    has_trivial_default_constructor;
        typedef ctrue_type    has_trivial_copy_constructor;
        typedef ctrue_type    has_trivial_assignment_operator;
        typedef ctrue_type    has_trivial_destructor;
        typedef ctrue_type    is_POD_type;
    };
}// namespace chen
#endif //CSTL_SOURCE_CTYPE_TRAITS_H
