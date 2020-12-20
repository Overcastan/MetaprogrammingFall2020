//
// Created by Overcastan on 19.12.2020.
//

#ifndef METHGRAPH_VISITOR_PATTERN_H
#define METHGRAPH_VISITOR_PATTERN_H

#include "typelist.h"
#include "Graph.h"
#include "functor.h"

template<int size, int row, typename TL>
struct BuildEdgeColumn;

template<int size, int row, int column, typename TL>
struct BuildEdgeRow;
/**
 * \brief The Visitor creates a graph with various elements at the vertices, and writes to the edges a functor from the types
 * of vertices that this edge connects, so for each pair of types (vertices), its own function will be called.
 *
 * @tparam V
 */
template<typename ...V>
struct Visitor {
    using vertexes = TypeList<V...>;
    static constexpr int size = Length<vertexes>::res;
    using edges = typename BuildEdgeColumn<size, 0, vertexes>::res;
    Graph<vertexes, edges> graph;

    Visitor() : graph() {}

    /// SetEdgeVal method sets a custom user's function to the edge with specified types.
    template<typename RowT, typename ColT>
    void SetEdgeVal(const functor<void(RowT, ColT)> &func) {
        constexpr int row = IndexOf<RowT, vertexes>::res;
        constexpr int column = IndexOf<ColT, vertexes>::res;
        graph.template SetEdgeVal<row, column>(func);
    }

    /// exec method simply executes a function on this edge
    template<typename RowT, typename ColT>
    void exec(const RowT &rowVal, const ColT &colVal) {
        constexpr int row = IndexOf<RowT, vertexes>::res;
        constexpr int column = IndexOf<ColT, vertexes>::res;
        graph.template GetEdgeVal<row, column>()(rowVal, colVal);
    }

};

//...................................................................//

/**
 * \brief BuildEdge functions build adjacency matrix for each vertex with a functor type elements
 * @tparam V
 * @tparam size
 * @tparam row
 */

/// row is a number of element in creating matrix Column
template<typename ...V, int size, int row>
struct BuildEdgeColumn<size, row, TypeList<V...>> {
    using res = typename Merge<typename BuildEdgeRow<size, row, 0, TypeList<V...>>::res,
            typename BuildEdgeColumn<size, row + 1, TypeList<V...>>::res>::res;
};

template<typename ...V, int size>
struct BuildEdgeColumn<size, size, TypeList<V...>> {
    using res = Nulltype;
};

//...................................................................//

// row is a number of element in creating matrix Column, column is a number of element in creating matrix Row
template<typename ...V, int size, int row, int column>
struct BuildEdgeRow<size, row, column, TypeList<V...>> {
    using RowType = typename TypeAt<row, TypeList<V...>>::res;
    using ColumnType = typename TypeAt<column, TypeList<V...>>::res;
    using res = typename Merge<functor<void(RowType, ColumnType)>,
            typename BuildEdgeRow<size, row, column + 1, TypeList<V...>>::res>::res;
};

template<typename ...V, int size, int row>
struct BuildEdgeRow<size, row, size, TypeList<V...>> {
    using res = Nulltype;
};

#endif //METHGRAPH_VISITOR_PATTERN_H
