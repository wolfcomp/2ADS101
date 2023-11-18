#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <ostream>
#include <queue>
#include <Windows.h>

#include "Graph.h"
#include "Node.h"
#include "PerlinNoise.h"

const char8_t* line = u8"│ ";
const char8_t* corner = u8"└─";
const char8_t* tee = u8"├─";
const char8_t* microseconds = u8" µs";

class vector3d
{
    float x;
    float y;
    float z;

public:
    vector3d(float x, float y, float z) : x(x), y(y), z(z)
    {

    }

    vector3d() : x(0), y(0), z(0)
    {

    }

    [[nodiscard]] float length() const
    {
        return sqrtf(x * x + y * y + z * z);
    }

    [[nodiscard]] float distance(vector3d other) const
    {
        return sqrtf(powf(x - other.x, 2) + powf(y - other.y, 2) + powf(z - other.z, 2));
    }

    [[nodiscard]] bool connected(vector3d other) const
    {
        const float distance_x = abs(x - other.x);
        const float distance_z = abs(z - other.z);
        const float manhattan_distance = distance_x + distance_z;
        return (distance_x < 2 && distance_x > 0 || (distance_z < 2 && distance_z > 0)) && manhattan_distance < 3;
    }

    [[nodiscard]] float heuristic(vector3d other) const
    {
        return distance(other);
    }

    void print(bool x = true, bool y = true, bool z = true) const
    {
        std::cout << "(";
        bool printed = false;
        if (x)
        {
            std::cout << this->x;
            printed = true;
        }
        if (printed)
        {
            std::cout << ", ";
            printed = false;
        }
        if (y)
        {
            std::cout << this->y;
            printed = true;
        }
        if (printed)
            std::cout << ", ";
        if (z)
            std::cout << this->z;
        std::cout << ")";
    }
};

#pragma region Print
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
void print_graph(Graph<T>& graph)
{
    auto output = std::vector<std::vector<T>>();
    auto vertices = graph.get_vertices();
    std::cout << "[" << std::endl;
    for (auto it = vertices.begin(); it != vertices.end(); ++it)
    {
        auto vertex = *it;
        auto edges = vertex->get_edges();
        for (auto k = 0; k < edges.size(); k++)  // NOLINT(clang-diagnostic-sign-compare)
        {
            auto edge = edges[k];
            std::cout << "  [" << edge->get_from()->get_value() << ", " << edge->get_to()->get_value() << ", " << edge->get_distance() << "]," << std::endl;
        }
    }
    std::cout << "]" << std::endl;
}
#pragma endregion functions

#pragma region Path finding
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

template <class T>
struct VertexTotalDistance
{
    Vertex<T>* vertex;
    float distance;

    bool operator>(const VertexTotalDistance& other) const
    {
        return distance > other.distance;
    }

    bool operator<(const VertexTotalDistance& other) const
    {
        return distance < other.distance;
    }

    bool operator==(const VertexTotalDistance& other) const
    {
        return vertex == other.vertex;
    }

    bool operator!=(const VertexTotalDistance& other) const
    {
        return vertex != other.vertex;
    }
};

/**
 * \brief Finds the shortest path between two vertexes in a graph.
 * \param start The starting vertex.
 * \param goal The goal vertex.
 * \return A tuple with the goal vertex and the path to it.
 */
std::tuple<VertexTotalDistance<int>, Node<VertexTotalDistance<int>>*> dijkstra_internal(Graph<int>* graph, Vertex<int>* start, Vertex<int>* goal)
{
    auto visited = std::vector<VertexTotalDistance<int>>();
    auto process = std::priority_queue<VertexTotalDistance<int>, std::vector<VertexTotalDistance<int>>, std::greater<>>();
    auto nodes = std::map<Vertex<int>*, Node<VertexTotalDistance<int>>*>();
    const auto node = new Node<VertexTotalDistance<int>>({ start, 0 });
    process.push(node->get_value());
    visited.push_back(node->get_value());
    nodes.insert({ start, node });
    while (!process.empty())
    {
        const auto [vertex, distance] = process.top();
        process.pop();
        if (vertex == goal)
        {
            return { { vertex, distance }, node };
        }
        for (auto edges = vertex->get_edges(); auto edge : edges)
        {
            const auto to = edge->get_to();
            const auto from = edge->get_from();
            const auto computed_distance = edge->get_distance() + distance;
            const auto vertex_distance = VertexTotalDistance{ to, computed_distance };
            if (std::find_if(visited.begin(), visited.end(), [&vertex_distance](const VertexTotalDistance<int> v)->bool {return v == vertex_distance; }) != visited.end())
                continue;
            visited.push_back(vertex_distance);
            process.push(vertex_distance);
            const auto new_node = new Node<VertexTotalDistance<int>>({ to, computed_distance });
            nodes[from]->add_child(new_node);
            nodes.insert({ to, new_node });
        }
    }
}

std::tuple<VertexTotalDistance<vector3d>, Node<VertexTotalDistance<vector3d>>*> dijkstra_internal(Graph<vector3d>* graph, Vertex<vector3d>* start, Vertex<vector3d>* goal)
{
    auto visited = std::vector<VertexTotalDistance<vector3d>>();
    auto process = std::priority_queue<VertexTotalDistance<vector3d>, std::vector<VertexTotalDistance<vector3d>>, std::greater<>>();
    auto nodes = std::map<Vertex<vector3d>*, Node<VertexTotalDistance<vector3d>>*>();
    const auto node = new Node<VertexTotalDistance<vector3d>>({ start, 0 });
    process.push(node->get_value());
    visited.push_back(node->get_value());
    nodes.insert({ start, node });
    while (!process.empty())
    {
        const auto [vertex, distance] = process.top();
        process.pop();
        if (vertex == goal)
        {
            return { { vertex, distance }, node };
        }
        for (auto vertexes = graph->get_vertices(); const auto to : vertexes)
        {
            if (!vertex->get_value().connected(to->get_value()))
                continue;
            const auto computed_distance = vertex->get_value().distance(to->get_value()) + distance;
            const auto edges = vertex->get_edges();
            if(std::ranges::find_if(edges.begin(), edges.end(), [&to](const Edge<vector3d>* e){return e->get_to() == to || e->get_from() == to;}) == edges.end())
                vertex->add_edge(to, computed_distance);
            const auto vertex_distance = VertexTotalDistance{ to, computed_distance };
            if (std::ranges::find_if(visited, [&vertex_distance](const VertexTotalDistance<vector3d> v)->bool {return v == vertex_distance; }) != visited.end())
                continue;
            visited.push_back(vertex_distance);
            process.push(vertex_distance);
            const auto new_node = new Node<VertexTotalDistance<vector3d>>({ to, computed_distance });
            nodes[vertex]->add_child(new_node);
            nodes.insert({ to, new_node });
        }
    }
}

template <class T>
std::vector<VertexTotalDistance<T>> dijkstra(Graph<T>* graph, Vertex<T>* start, Vertex<T>* goal)
{
    std::tuple<VertexTotalDistance<T>, Node<VertexTotalDistance<T>>*> tuple = dijkstra_internal(graph, start, goal);
    auto [vertex, node] = tuple;
    auto visited = std::vector<VertexTotalDistance<T>>();
    auto path = fastest_path(*node, vertex, visited);
    node->remove_all_children();
    delete node;
    return path;
}

std::tuple<VertexTotalDistance<vector3d>, Node<VertexTotalDistance<vector3d>>*> astar_internal(Graph<vector3d>* graph, Vertex<vector3d>* start, Vertex<vector3d>* goal)
{
    auto visited = std::vector<VertexTotalDistance<vector3d>>();
    auto process = std::priority_queue<VertexTotalDistance<vector3d>, std::vector<VertexTotalDistance<vector3d>>, std::greater<>>();
    auto nodes = std::map<Vertex<vector3d>*, Node<VertexTotalDistance<vector3d>>*>();
    const auto node = new Node<VertexTotalDistance<vector3d>>({ start, 0 });
    process.push(node->get_value());
    visited.push_back(node->get_value());
    nodes.insert({ start, node });
    while (!process.empty())
    {
        const auto [vertex, distance] = process.top();
        process.pop();
        if (vertex == goal)
        {
            return { { vertex, distance }, node };
        }
        for (auto vertexes = graph->get_vertices(); const auto to : vertexes)
        {
            if (!vertex->get_value().connected(to->get_value()))
                continue;
            const auto heuristic = to->get_value().heuristic(goal->get_value());
            const auto computed_distance = vertex->get_value().distance(to->get_value()) + heuristic;
            const auto edges = vertex->get_edges();
            if(std::ranges::find_if(edges.begin(), edges.end(), [&to](const Edge<vector3d>* e){return e->get_to() == to || e->get_from() == to;}) == edges.end())
                vertex->add_edge(to, computed_distance);
            const auto vertex_distance = VertexTotalDistance{ to, computed_distance };
            if (std::ranges::find_if(visited, [&vertex_distance](const VertexTotalDistance<vector3d> v)->bool {return v == vertex_distance; }) != visited.end())
                continue;
            visited.push_back(vertex_distance);
            process.push(vertex_distance);
            const auto new_node = new Node<VertexTotalDistance<vector3d>>({ to, computed_distance });
            nodes[vertex]->add_child(new_node);
            nodes.insert({ to, new_node });
        }
    }
}

std::vector<VertexTotalDistance<vector3d>> astar(Graph<vector3d>* graph, Vertex<vector3d>* start, Vertex<vector3d>* goal)
{
    std::tuple<VertexTotalDistance<vector3d>, Node<VertexTotalDistance<vector3d>>*> tuple = astar_internal(graph, start, goal);
    auto [vertex, node] = tuple;
    auto visited = std::vector<VertexTotalDistance<vector3d>>();
    auto path = fastest_path(*node, vertex, visited);
    node->remove_all_children();
    delete node;
    return path;
}
#pragma endregion functions

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
    const auto graph = new Graph<int>();
    const auto vertex0 = new Vertex<int>(0);
    const auto vertex1 = new Vertex<int>(1);
    const auto vertex2 = new Vertex<int>(2);
    const auto vertex3 = new Vertex<int>(3);
    const auto vertex4 = new Vertex<int>(4);
    const auto vertex5 = new Vertex<int>(5);
    const auto vertex6 = new Vertex<int>(6);
    const auto vertex7 = new Vertex<int>(7);
    const auto vertex8 = new Vertex<int>(8);

    vertex0->add_edge(vertex1, 4);
    vertex0->add_edge(vertex7, 8);
    vertex1->add_edge(vertex2, 8);
    vertex1->add_edge(vertex7, 11);
    vertex2->add_edge(vertex3, 7);
    vertex2->add_edge(vertex5, 4);
    vertex2->add_edge(vertex8, 2);
    vertex3->add_edge(vertex4, 9);
    vertex3->add_edge(vertex5, 14);
    vertex4->add_edge(vertex5, 10);
    vertex5->add_edge(vertex6, 2);
    vertex6->add_edge(vertex7, 1);
    vertex6->add_edge(vertex8, 6);
    vertex7->add_edge(vertex8, 7);

    graph->add_vertex(vertex0);
    graph->add_vertex(vertex1);
    graph->add_vertex(vertex2);
    graph->add_vertex(vertex3);
    graph->add_vertex(vertex4);
    graph->add_vertex(vertex5);
    graph->add_vertex(vertex6);
    graph->add_vertex(vertex7);
    graph->add_vertex(vertex8);

    // Print the graph adjacency list to console.
    print_graph(*graph);

    std::cout << std::endl;
    std::cout << std::endl;

    // Find the shortest path between two vertexes.

    const auto path2 = dijkstra(graph, vertex1, vertex4);
    std::cout << "Path: ";

    for (auto it = path2.begin(); it != path2.end(); ++it)
    {
        std::cout << it->vertex->get_value() << (it != path2.end() - 1 ? " -> " : "");
    }

    std::cout << std::endl;
    std::cout << std::endl;

    delete graph;

    constexpr siv::PerlinNoise::seed_type seed = 123456u;
    const siv::PerlinNoise perlin(seed);
    const auto graph_3d = new Graph<vector3d>();

    for (int x = 1; x <= 100; x++)
    {
        for (int z = 1; z <= 100; z++)
        {
            auto value = static_cast<float>(perlin.octave2D_01(x * 0.03, z * 0.03, 4) * 10);
            value = static_cast<float>(perlin.octave3D_01(x * 0.03, value, z * 0.03, 4) * 10);
            graph_3d->add_vertex(new Vertex<vector3d>({ static_cast<float>(x), value, static_cast<float>(z) }));
        }
    }

    const auto vertices_3d = graph_3d->get_vertices();

    const auto vertex1_3d = vertices_3d[0];
    const auto vertex2_3d = vertices_3d[vertices_3d.size() - 1];

    auto start = std::chrono::high_resolution_clock::now();
    const auto path3 = dijkstra(graph_3d, vertex1_3d, vertex2_3d);
    auto end = std::chrono::high_resolution_clock::now();
    const auto dihkstra_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "Path: ";

    for (auto it = path3.begin(); it != path3.end(); ++it)
    {
        it->vertex->get_value().print(true, false);
        std::cout << (it != path3.end() - 1 ? " -> " : "");
    }

    std::cout << std::endl;
    std::cout << std::endl;
    int dijkstra_length = 0;

    for(const auto vertex_3d : vertices_3d)
    {
        if(!vertex_3d->is_empty())
            dijkstra_length++;
        for(const auto edge_3d : vertex_3d->get_edges())
            vertex_3d->remove_edge(edge_3d);
    }

    start = std::chrono::high_resolution_clock::now();
    const auto path4 = astar(graph_3d, vertex1_3d, vertex2_3d);
    end = std::chrono::high_resolution_clock::now();
    const auto astar_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "Path: ";

    for (auto it = path4.begin(); it != path4.end(); ++it)
    {
        it->vertex->get_value().print(true, false);
        std::cout << (it != path4.end() - 1 ? " -> " : "");
    }

    std::cout << std::endl;
    std::cout << std::endl;

    int astar_length = 0;
    for(const auto vertex_3d : vertices_3d)
    {
        if(!vertex_3d->is_empty())
            astar_length++;
        for(const auto edge_3d : vertex_3d->get_edges())
            vertex_3d->remove_edge(edge_3d);
    }

    std::cout << "Graph legnth: " << vertices_3d.size() << std::endl;
    std::cout << "Dijkstra length: " << dijkstra_length << std::endl;
    std::cout << "A* length: " << astar_length << std::endl;

    std::cout << "Dijkstra time: " << dihkstra_time << reinterpret_cast<const char*>(microseconds) << std::endl;
    std::cout << "A* time: " << astar_time << reinterpret_cast<const char*>(microseconds) << std::endl;
#pragma endregion functions

    return 0;
}