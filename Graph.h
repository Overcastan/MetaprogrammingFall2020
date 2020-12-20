//
// Created by Overcastan on 17.12.2020.
//

#ifndef METHGRAPH_GRAPH_H
#define METHGRAPH_GRAPH_H

#include "typelist.h"


/**
 *\brief VertexValues is a simple storage class that contains values on each vertex
 * @tparam TL
 */
template<typename TL>
struct VertexValues;

/**
 * \brief EdgeValues is a storage class that contains values on edges for each vertex
 * @tparam TL
 */
template<typename TL>
struct EdgeValues;

template<int ind>
struct VertexValGetter;

template<int ind>
struct VertexValSetter;

template<int ind1, int ind2>
struct EdgeValGetter;

template<int ind1, int ind2>
struct EdgeValSetter;

/**
 * Struct for visiting every vertex
 * @tparam ind
 * @tparam size
 */
template<int ind, int size>
struct VertexIterator;

template<typename V, typename E>
class Graph {
};

/**
 * \brief Struct Graph is needed for create and contain vertexes and edges. It includes Getter and Setter methods for
 * values in vertexes and edges. Also it contain iterate method, that goes in every vertex and do some custom action.
 *
 * @tparam V
 * @tparam E
 */
template<typename ...V, typename ...E>
struct Graph<TypeList<V...>, TypeList<E...>> {
    using Vertexes =  TypeList<V...>;
    using Edges =  TypeList<E...>;
    static constexpr int size = Length<Vertexes>::res;

    VertexValues<Vertexes> vertexes;
    EdgeValues<Edges> edges;

    Graph() : vertexes(), edges() {}

    template<int ind>
    decltype(auto) GetVertexVal() {
        return VertexValGetter<ind>::Get(vertexes);
    }

    template<int ind, typename T>
    void SetVertexVal(T &&obj) {
        VertexValSetter<ind>::Set(vertexes, std::forward<T>(obj));
    }

    template<int ind1, int ind2>
    decltype(auto) GetEdgeVal() {
        return EdgeValGetter<ind1, ind2>::Get(edges);
    }

    template<int ind1, int ind2, typename T>
    void SetEdgeVal(T &&obj) {
        EdgeValSetter<ind1, ind2>::Set(edges, std::forward<T>(obj));
    }

    /// Iterate method accepts the Iterator class, that must be written by user for its own purposes.
    /// Iterator must contain exec method for simply doing something with vertex.
    template<typename Iterator>
    void iterate(Iterator i) {
        VertexIterator<0, size>::iterate(this, i);
    }

};

//...................................................................//

template<typename ...Args>
struct VertexValues<TypeList<Args...>> {
    using VertexType = typename TypeList<Args...>::Head;
    using Tail = typename TypeList<Args...>::Tail;

    VertexType VertexValue;
    VertexValues<Tail> TailVertexValues;

    VertexValues() : VertexValue() {}
};

template<>
struct VertexValues<Nulltype> {
};

//...................................................................//

template<int ind>
struct VertexValGetter {
    template<typename ValuesStructType>
    static decltype(auto) Get(ValuesStructType &storage) {
        return VertexValGetter<ind - 1>::Get(storage.TailVertexValues);
    }
};

template<>
struct VertexValGetter<0> {
    template<typename ValuesStructType>
    static typename ValuesStructType::VertexType &Get(ValuesStructType &storage) {
        return storage.VertexValue;
    }
};

//...................................................................//

template<int ind>
struct VertexValSetter {
    template<typename T, typename ValuesStructType>
    static void Set(ValuesStructType &storage, T &&obj) {
        VertexValSetter<ind - 1>::Set(storage.TailVertexValues, std::forward<T>(obj));
    }
};

template<>
struct VertexValSetter<0> {
    template<typename T, typename ValuesStructType>
    static void Set(ValuesStructType &storage, T &&obj) {
        storage.VertexValue = typename ValuesStructType::VertexType(obj);
    }
};

//...................................................................//

template<typename ...E>
struct EdgeValues<TypeList<E...>> {
    EdgeValues() = default;

    VertexValues<typename TypeList<E...>::Head> EdgeValue;
    EdgeValues<typename TypeList<E...>::Tail> TailEdgeValues;
};

template<>
struct EdgeValues<TypeList<>> {
};

template<>
struct EdgeValues<Nulltype> {
};

//...................................................................//

template<int ind1, int ind2>
struct EdgeValGetter {
    template<typename ValuesStructType>
    static decltype(auto) Get(ValuesStructType &storage) {
        return EdgeValGetter<ind1 - 1, ind2>::Get(storage.TailEdgeValues);
    }
};

template<int ind2>
struct EdgeValGetter<0, ind2> {
    template<typename ValuesStructType>
    static decltype(auto) Get(ValuesStructType &storage) {
        return VertexValGetter<ind2>::Get(storage.EdgeValue);
    }
};

//...................................................................//

template<int ind1, int ind2>
struct EdgeValSetter {
    template<typename T, typename ValuesStructType>
    static void Set(ValuesStructType &storage, T &&obj) {
        EdgeValSetter<ind1 - 1, ind2>::Set(storage.TailEdgeValues, std::forward<T>(obj));
    }
};

template<int ind2>
struct EdgeValSetter<0, ind2> {
    template<typename T, typename ValuesStructType>
    static void Set(ValuesStructType &storage, T &&obj) {
        VertexValSetter<ind2>::Set(storage.EdgeValue, std::forward<T>(obj));
    }
};

//...................................................................//

/**
 * VertexIterator recursively visit every vertex and do some action with value in it.
 * @tparam ind
 * @tparam size
 */
template<int ind, int size>
struct VertexIterator {
    template<typename Graph, typename Iterator>
    static void iterate(Graph *g, Iterator i) {
        i.exec(g->template GetVertexVal<ind>());
        VertexIterator<ind + 1, size>::iterate(g, i);
    }
};

template<int size>
struct VertexIterator<size, size> {
    template<typename Graph, typename Iterator>
    static void iterate(Graph *g, Iterator i) {
    }
};
//...................................................................//

#endif //METHGRAPH_GRAPH_H
