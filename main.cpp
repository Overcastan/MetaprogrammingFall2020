#include <iostream>

#include "typelist.h"
#include "Graph.h"
#include "Visitor_pattern.h"

/**
 * \example
 */
struct Iterator {
    static void exec(int &val) {
        std::cout << "Do something in int " << val << " vertex" << "\n";
    }

    static void exec(float &val) {
        std::cout << "Do something in float " << val << " vertex" << "\n";
    }

    static void exec(std::string &val) {
        std::cout << "Do something in string " << val << " vertex" << "\n";
    }
};

void IntToFloatEdge(int a, float b) {
    std::cout << "Edge from int " << a << " to float " << b << "\n";
}

void IntToStringEdge(int a, std::string c) {
    std::cout << "Edge from int " << a << " to string " << c << "\n";
}

void FloatToString(float b, std::string c) {
    std::cout << "Edge from float " << b << " to string " << c << "\n";
}

int main() {
    Graph<TypeList<int, float, std::string>,
            TypeList<
                    TypeList<Nulltype, int, int>,
                    TypeList<int, Nulltype, int>,
                    TypeList<int, int, Nulltype>>> graph;

    graph.SetVertexVal<0>(4);
    graph.SetVertexVal<1>(5);
    graph.SetVertexVal<2>("I'm a string vertex!");

    graph.iterate(Iterator());

    Visitor<int, float, std::string> visitor;
    visitor.SetEdgeVal<int, float>(IntToFloatEdge);
    visitor.SetEdgeVal<int, std::string>(IntToStringEdge);
    visitor.SetEdgeVal<float, std::string>(FloatToString);

    visitor.exec(5, 7.0f);
    visitor.exec(10, std::string("ITS Edge"));
    visitor.exec(13.0f, std::string("FTS Edge"));

    return 0;
}