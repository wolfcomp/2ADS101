#include <algorithm>
#include <iostream>
#include <ostream>
#include <Windows.h>

#include "Graph.h"
#include "Node.h"

template <class T>
void print_tree(Node<T> tree, const int i)
{
    std::cout << tree.get_value() << std::endl;
    auto children = tree.get_children();
    Node<T>* last;
    if (children.empty())
    {
        last = nullptr;
    }
    else
    {
        last = children[children.size() - 1];
    }
    for (auto it = children.begin(); it != children.end(); ++it)
    {
        for (auto k = 0; k < i; k++)
        {
            std::cout << u8"│ ";
        }
        std::cout << (last != *it ? u8"├─" : u8"└─");
        print_tree(**it, i + 1);
    }
}

template <class T>
std::vector<std::vector<T>> print_graph(Graph<T>& graph)
{
    auto output = std::vector<std::vector<T>>();
    auto vertices = graph.get_vertices();
    for (auto it = vertices.begin(); it != vertices.end(); ++it)
    {
        auto vertex = *it;
        auto edges = vertex->get_edges();
        for (auto k = 0; k < edges.size(); k++)
        {
            auto edge = edges[k];
            output.push_back({ edge->get_from()->get_value(), edge->get_to()->get_value() });
        }
    }
    return output;
}

template <class T>
std::vector<T> fastest_path(Node<T> tree, const T value, std::vector<T>& traced)
{
    auto output = std::vector<T>();
    if (tree.get_value() == value)
    {
        output.push_back(tree.get_value());
        return output;
    }
    auto children = tree.get_children();
    for (auto it = children.begin(); it != children.end(); ++it)
    {
        auto child = *it;
        if (std::find(traced.begin(), traced.end(), child->get_value()) != traced.end())
        {
            continue;
        }
        traced.push_back(child->get_value());
        auto path = fastest_path(*child, value, traced);
        if (!path.empty() && std::find(path.begin(), path.end(), value) != path.end())
        {
            output.push_back(tree.get_value());
            output.insert(output.end(), path.begin(), path.end());
            return output;
        }
    }
    if (output.empty())
    {
        if (tree.get_parent() != nullptr && std::find(traced.begin(), traced.end(), tree.get_value()) == traced.end())
        {
            traced.push_back(tree.get_value());
            auto path = fastest_path(*tree.get_parent(), value, traced);
            if (!path.empty())
            {
                output.push_back(tree.get_value());
                output.insert(output.end(), path.begin(), path.end());
            }
        }
    }
    return output;
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    setvbuf(stdout, nullptr, _IOFBF, 1000);
    std::cout.imbue(std::locale(""));

#pragma region Tree
    auto tree = Node<int>(0);
    tree.add_child(new Node<int>(1));
    tree.add_child(new Node<int>(2));
    tree.add_child(new Node<int>(3));
    auto child = tree[0];
    child->add_child(new Node<int>(4));
    child->add_child(new Node<int>(5));
    child = (*child)[0];
    child->add_child(new Node<int>(6));
    child->add_child(new Node<int>(7));
    tree[1]->add_child(new Node<int>(8));
    tree[1]->add_child(new Node<int>(9));
    tree[2]->add_child(new Node<int>(10));
    tree[2]->operator[](0)->add_child(new Node<int>(11));
    print_tree(tree, 0);

    auto traced = std::vector<int>();
    auto path = fastest_path(*tree[2]->operator[](0), 3, traced);

    std::cout << std::endl;
    std::cout << std::endl;

    child->get_parent()->remove_child(child);

    print_tree(tree, 0);
#pragma endregion functions

#pragma region Graph
    auto graph = Graph<int>();
    auto vertex0 = new Vertex<int>(0);
    auto vertex1 = new Vertex<int>(1);
    auto vertex2 = new Vertex<int>(2);
    auto vertex3 = new Vertex<int>(3);
    auto vertex4 = new Vertex<int>(4);
    auto vertex5 = new Vertex<int>(5);
    auto vertex6 = new Vertex<int>(6);
    vertex0->add_edge(vertex1);
    vertex0->add_edge(vertex2);
    vertex1->add_edge(vertex3);
    vertex2->add_edge(vertex3);
    vertex3->add_edge(vertex4);
    vertex3->add_edge(vertex5);
    vertex5->add_edge(vertex6);
    vertex6->add_edge(vertex3);
    graph.add_vertex(vertex0);
    graph.add_vertex(vertex1);
    graph.add_vertex(vertex2);
    graph.add_vertex(vertex3);
    graph.add_vertex(vertex4);
    graph.add_vertex(vertex5);
    graph.add_vertex(vertex6);

    const auto list = print_graph(graph);

    std::cout << "[" << std::endl;

    for (auto it = list.begin(); it != list.end(); ++it)
    {
        std::cout << "  [" << (*it)[0] << ", " << (*it)[1] << "]," << std::endl;
    }


    std::cout << "]";

    graph.remove_all_vertices();
#pragma endregion functions

    return 0;
}