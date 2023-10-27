#include <algorithm>
#include <functional>
#include <iostream>
#include <ostream>
#include <Windows.h>

#include "Graph.h"
#include "Node.h"

const char8_t* line = u8"│ ";
const char8_t* corner = u8"└─";
const char8_t* tee = u8"├─";

template <class T>
void print_tree(Node<T> tree, const int i, const bool end = false)
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
            std::cout << (end ? "  " : reinterpret_cast<const char*>(line));
        }
        std::cout << (last != *it ? reinterpret_cast<const char*>(tee) : reinterpret_cast<const char*>(corner));
        print_tree(**it, i + 1, last == *it);
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
        for (auto k = 0; k < edges.size(); k++)  // NOLINT(clang-diagnostic-sign-compare)
        {
            auto edge = edges[k];
            output.push_back({ edge->get_from()->get_value(), edge->get_to()->get_value() });
        }
    }
    return output;
}

/**
 * \brief Finds the shortest path between two nodes in a tree.
 * \tparam T The type of the value in the nodes.
 * \param tree The tree to search with starting node.
 * \param value The value of the node to search for.
 * \param traced The nodes that have already been visited.
 * \return
 */
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
        if (auto path = fastest_path(*child, value, traced); !path.empty() && std::find(path.begin(), path.end(), value) != path.end())
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
            if (auto path = fastest_path(*tree.get_parent(), value, traced); !path.empty())
            {
                output.push_back(tree.get_value());
                output.insert(output.end(), path.begin(), path.end());
            }
        }
    }
    return output;
}

/**
 * \brief Finds the shortest path between two vertexes in a graph.
 * \tparam T The type of the value in the vertexes.
 * \param graph The graph to search with starting vertex.
 * \param value The value of the vertex to search for.
 * \param visited The vertexes that have already been visited.
 * \param f The function to compare the paths with. Returns bool with input of <see cref="std::vector<Vertex<T>*>"/> and <see cref="std::vector<Vertex<T>*>"/>.
 * \return
 */
template <class T, typename Func>
std::vector<Vertex<T>*> fastest_path(Vertex<T>* graph, const T value, std::vector<Vertex<T>*> visited, Func f)
{
    auto paths = std::vector<std::vector<Vertex<T>*>>();
    auto edges = graph->get_edges();
    Vertex<T>* vertex;
    for (auto it = edges.begin(); it != edges.end(); ++it)
    {
        auto edge = *it;
        vertex = edge->get_to();
        if (std::find(visited.begin(), visited.end(), vertex) != visited.end())
            continue;

        visited.push_back(vertex);
        if (vertex->get_value() == value)
        {
            break;
        }
        if (auto path = fastest_path(vertex, value, visited, f); !path.empty())
        {
            paths.push_back(path);
        }
    }
    auto shortest = std::vector<Vertex<T>*>();
    for (auto it = paths.begin(); it != paths.end(); ++it)
    {
        auto path = *it;
        if (shortest.empty() || f(path, shortest))
        {
            shortest = path;
        }
    }
    if (std::find(shortest.begin(), shortest.end(), vertex) == shortest.end())
        shortest.push_back(vertex);
    return shortest;
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    setvbuf(stdout, nullptr, _IOFBF, 1000);
    std::cout.imbue(std::locale(""));

#pragma region Tree
    // Create a tree with 12 random inserted nodes.
    auto tree = Node(0);
    tree.add_child(new Node(1));
    tree.add_child(new Node(2));
    tree.add_child(new Node(3));
    auto child = tree[0];
    child->add_child(new Node(4));
    child->add_child(new Node(5));
    child = (*child)[0];
    child->add_child(new Node(6));
    child->add_child(new Node(7));
    tree[1]->add_child(new Node(8));
    tree[1]->add_child(new Node(9));
    tree[2]->add_child(new Node(10));
    tree[2]->operator[](0)->add_child(new Node(11));

    // Print the tree to console.
    print_tree(tree, 0);

    std::cout << std::endl;
    std::cout << std::endl;

    // Find the shortest path between two nodes.
    auto traced = std::vector<int>();
    const auto path = fastest_path(*tree[2]->operator[](0), 7, traced);

    // Print the path to console.
    for (auto i = 0; i < path.size(); i++)
    {
        std::cout << path[i] << (i != path.size() - 1 ? " -> " : "");
    }

    std::cout << std::endl;
    std::cout << std::endl;

    // Remove a node from the tree.
    child->get_parent()->remove_child(child);

    // Print the new tree to console.
    print_tree(tree, 0);

    std::cout << std::endl;
    std::cout << std::endl;
#pragma endregion functions

#pragma region Graph
    // Create a graph with 7 vertexes and 8 random edges.
    auto graph = Graph<int>();
    auto vertex0 = new Vertex(0);
    auto vertex1 = new Vertex(1);
    auto vertex2 = new Vertex(2);
    auto vertex3 = new Vertex(3);
    auto vertex4 = new Vertex(4);
    auto vertex5 = new Vertex(5);
    auto vertex6 = new Vertex(6);
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

    // Print the graph adjacency list to console.
    const auto list = print_graph(graph);

    std::cout << "[" << std::endl;

    for (const auto& it : list)
    {
        std::cout << "  [" << it[0] << ", " << it[1] << "]," << std::endl;
    }


    std::cout << "]";

    std::cout << std::endl;
    std::cout << std::endl;

    // Find the shortest path between two vertexes.
    auto visited2 = std::vector<Vertex<int>*>();
    visited2.push_back(vertex1);

    const auto path2 = fastest_path(vertex1, vertex4->get_value(), visited2, [](const std::vector<Vertex<int>*>& a, const std::vector<Vertex<int>*>& b) { return a.size() < b.size(); });

    // Print the path to console.
    std::cout << "Path: " << vertex1->get_value() << " -> ";

    for (auto i = 0; i < path2.size(); i++)  // NOLINT(clang-diagnostic-sign-compare)
    {
        std::cout << path2[i]->get_value() << (i != path2.size() - 1 ? " -> " : "");  // NOLINT(clang-diagnostic-sign-compare)
    }
#pragma endregion functions

    return 0;
}