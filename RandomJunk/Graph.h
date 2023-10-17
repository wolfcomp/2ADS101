#pragma once
#include <vector>

template <class T> class Edge;
template <class T> class Graph;

template <class T>
class Vertex
{
    T info;
    std::vector<Edge<T>*> edges;
    void add_edge(Edge<T>* edge);

public:
    Vertex()
    {
        this->info = T();
        this->edges = std::vector<Edge<T>*>();
    }
    explicit Vertex(T value)
    {
        this->info = value;
        this->edges = std::vector<Edge<T>*>();
    }
    ~Vertex()
    {
        this->remove_all_edges();
    }
    Vertex(const Vertex& other)
    {
        this->info = other.info;
        this->edges = other.edges;
    }
    Vertex& operator=(const Vertex& other)
    {
        if (this != &other)
        {
            this->info = other.info;
            this->edges = other.edges;
        }
        return *this;
    }
    Vertex(Vertex&& other) noexcept
    {
        this->info = other.info;
        this->edges = other.edges;
    }
    Vertex& operator=(Vertex&& other) noexcept
    {
        if (this != &other)
        {
            this->info = other.info;
            this->edges = other.edges;
        }
        return *this;
    }
    void add_edge(Vertex* to);
    void remove_edge(Vertex* to);
    void remove_edge(Edge<T>* edge);
    void remove_all_edges();
    std::vector<Edge<T>*> get_edges() const;
    T get_value() const;
    int get_size() const
    {
        return this->edges.size();
    }
    bool is_empty() const
    {
        return this->edges.empty();
    }
};

template <class T>
class Edge
{
public:
    Edge()
    {
        this->from = nullptr;
        this->to = nullptr;
    }
    Edge(Vertex<T>* from, Vertex<T>* to)
    {
        this->from = from;
        this->to = to;
    }
    ~Edge()
    {
        this->from = nullptr;
        this->to = nullptr;
    }
    Edge(const Edge& other)
    {
        this->from = other.from;
        this->to = other.to;
    }
    Edge& operator=(const Edge& other)
    {
        if (this != &other)
        {
            this->from = other.from;
            this->to = other.to;
        }
        return *this;
    }
    Edge(const Edge&& other) noexcept
    {
        this->from = other.from;
        this->to = other.to;
    }
    Edge& operator=(Edge&& other) noexcept
    {
        if (this != &other)
        {
            this->from = other.from;
            this->to = other.to;
        }
        return *this;
    }
    void set_from(Vertex<T>* from);
    void set_to(Vertex<T>* to);
    void flip()
    {
        auto temp = this->from;
        this->from = this->to;
        this->to = temp;
    }
    Vertex<T>* get_from() const;
    Vertex<T>* get_to() const;
private:
    Vertex<T>* from;
    Vertex<T>* to;
};

template <class T>
class Graph
{
    std::vector<Vertex<T>*> vertices;
public:
    Graph()
    {
        this->vertices = std::vector<Vertex<T>*>();
    }
    ~Graph()
    {
        this->remove_all_vertices();
    }
    Graph(const Graph& other)
    {
        this->vertices = other.vertices;
    }
    Graph& operator=(const Graph& other)
    {
        if (this != &other)
        {
            this->vertices = other.vertices;
        }
        return *this;
    }
    Graph(const Graph&& other) noexcept
    {
        this->vertices = other.vertices;
    }
    Graph& operator=(Graph&& other) noexcept
    {
        if (this != &other)
        {
            this->vertices = other.vertices;
        }
        return *this;
    }
    void add_vertex(Vertex<T>* vertex);
    void remove_vertex(Vertex<T>* vertex);
    void remove_all_vertices();
    std::vector<Vertex<T>*> get_vertices() const;
    Vertex<T>* operator[](int index) const;
    bool is_empty() const
    {
        return this->vertices.empty();
    }
    int size() const
    {
        return this->vertices.size();
    }
};

template <class T>
void Vertex<T>::add_edge(Edge<T>* edge)
{
    this->edges.push_back(edge);
}

template <class T>
void Vertex<T>::add_edge(Vertex* to)
{
    auto edge = new Edge<T>(this, to);
    auto edge2 = new Edge<T>(to, this);
    this->edges.push_back(edge);
    to->add_edge(edge2);
}

template <class T>
std::vector<Edge<T>*> Vertex<T>::get_edges() const
{
    return this->edges;
}

template <class T>
T Vertex<T>::get_value() const
{
    return this->info;
}

template <class T>
void Vertex<T>::remove_all_edges()
{
    for (const auto& edge : this->edges)
    {
        edge->get_to()->remove_edge(edge);
        delete edge;
    }
    this->edges.clear();
}

template <class T>
void Vertex<T>::remove_edge(Edge<T>* edge)
{
    if (this->edges.empty())
        return;
    for (auto it = this->edges.begin(); it != this->edges.end(); ++it)
    {
        if (*it == edge)
        {
            this->edges.erase(it);
            break;
        }
    }
}

template <class T>
void Vertex<T>::remove_edge(Vertex* to)
{
    for (auto it = this->edges.begin(); it != this->edges.end(); ++it)
    {
        if ((*it)->get_to() == to)
        {
            this->edges.erase(it);
            break;
        }
    }
}

template <class T>
Vertex<T>* Edge<T>::get_from() const
{
    return this->from;
}

template <class T>
Vertex<T>* Edge<T>::get_to() const
{
    return this->to;
}

template <class T>
void Edge<T>::set_from(Vertex<T>* from)
{
    this->from = from;
}

template <class T>
void Edge<T>::set_to(Vertex<T>* to)
{
    this->to = to;
}

template <class T>
void Graph<T>::add_vertex(Vertex<T>* vertex)
{
    this->vertices.push_back(vertex);
}

template <class T>
std::vector<Vertex<T>*> Graph<T>::get_vertices() const
{
    return this->vertices;
}

template <class T>
Vertex<T>* Graph<T>::operator[](int index) const
{
    return this->vertices[index];
}

template <class T>
void Graph<T>::remove_all_vertices()
{
    for (const auto& vertex : this->vertices)
    {
        if (vertex != nullptr)
            delete vertex;
    }
    this->vertices.clear();
}

template <class T>
void Graph<T>::remove_vertex(Vertex<T>* vertex)
{
    for (auto it = this->vertices.begin(); it != this->vertices.end(); ++it)
    {
        if (*it == vertex)
        {
            this->vertices.erase(it);
            delete vertex;
            break;
        }
    }
}
