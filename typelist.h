//
// Created by Overcastan on 17.12.2020.
//

#ifndef METHGRAPH_TYPELIST_H
#define METHGRAPH_TYPELIST_H

/**
 * \brief Pretty common TypeList
 * TypeList with some additional structures for proper work with visitor pattern
 */

struct Nulltype {
};

template<typename ...Args>
struct TypeList {
    using Head = Nulltype;
    using Tail = Nulltype;
};

template<typename H, typename ...T>
struct TypeList<H, T...> {
    using Head = H;
    using Tail = TypeList<T...>;
};

template<typename H>
struct TypeList<H> {
    using Head = H;
    using Tail = Nulltype;
};

//...................................................................//

template<size_t ind, typename T>
struct TypeAt {
    using res = Nulltype;
};

template<size_t ind, typename ...T>
struct TypeAt<ind, TypeList<T...>> {
    using res = typename TypeAt<ind - 1, typename TypeList<T...>::Tail>::res;
};

template<size_t ind>
struct TypeAt<ind, Nulltype> {
    using res = Nulltype;
};

template<typename ...T>
struct TypeAt<0, TypeList<T...>> {
    using res = typename TypeList<T...>::Head;
};

//...................................................................//

template<typename T>
struct Length {
    static constexpr int res = 0;
};

template<typename ...Args>
struct Length<TypeList<Args...>> {
    static constexpr int res = 1 + Length<typename TypeList<Args...>::Tail>::res;
};

template<>
struct Length<TypeList<>> {
    static constexpr int res = 0;
};

template<>
struct Length<Nulltype> {
    static constexpr int res = 0;
};


//...................................................................//

/**
 * Struct Merge is used for edges creation in visitor pattern
 * @tparam V
 * @tparam TL
 */

template<typename V, typename TL>
struct Merge;

template<typename V, typename ...Args>
struct Merge<V, TypeList<Args...>> {
    using res = TypeList<V, Args...>;
};
template<typename V>
struct Merge<V, Nulltype> {
    using res = TypeList<V>;
};

//...................................................................//

template<typename T, typename TL>
struct IndexOf;

template<typename T, typename ...Args>
struct IndexOf<T, TypeList<Args...>> {
    static constexpr int res = 1 + IndexOf<T, typename TypeList<Args...>::Tail>::res;
};

template<typename T, typename ...Args>
struct IndexOf<T, TypeList<T, Args...>> {
    static constexpr int res = 0;
};

template<typename T>
struct IndexOf<T, Nulltype> {
    static constexpr int res = 0;
};

//...................................................................//

#endif //METHGRAPH_TYPELIST_H
