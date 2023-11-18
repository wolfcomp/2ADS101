#pragma once
#include <vector>

template <class T> class Edge;
template <class T> class Graph;

template <class T>
class Vertex
{
    T info;
    std::vector<Edge<T>*> edges;

    void add_edge(Edge<T>* edge)
    {
        this->edges.push_back(edge);
    }

public:
    /**
     * \brief Creates a new vertex with default value.
     */
    Vertex();

    /**
     * \brief Creates a new vertex with the given value.
     * \param value The value of the vertex.
     */
    explicit Vertex(T value);

    /**
     * \brief Destroys the vertex and all edges connected to it.
     */
    ~Vertex();

    /**
     * \brief Copy constructor.
     * \param other The vertex to copy.
     */
    Vertex(const Vertex& other);

    /**
     * \brief Copy assignment operator.
     * \param other The vertex to copy.
     * \return The copied vertex.
     */
    Vertex& operator=(const Vertex& other);

    /**
     * \brief Move constructor.
     * \param other The vertex to move.
     */
    Vertex(Vertex&& other) noexcept;

    /**
     * \brief Move assignment operator.
     * \param other The vertex to move.
     * \return The moved vertex.
     */
    Vertex& operator=(Vertex&& other) noexcept;

    /**
     * \brief Adds an edge to the vertex.
     * \param to The vertex to connect to.
     */
    void add_edge(Vertex* to, float distance);

    /**
     * \brief Removes an edge to the vertex.
     * \param to The vertex to disconnect from.
     */
    void remove_edge(Vertex* to);

    /**
     * \brief Removes an edge to the vertex.
     * \param edge The edge to disconnect from.
     */
    void remove_edge(Edge<T>* edge);

    /**
     * \brief Removes all edges from the vertex.
     */
    void remove_all_edges();

    /**
     * \brief Gets all edges from the vertex.
     * \return A vector of edges.
     */
    [[nodiscard]] std::vector<Edge<T>*> get_edges() const;

    /**
     * \brief Gets the value of the vertex.
     * \return The value of the vertex.
     */
    [[nodiscard]] T get_value() const;

    /**
     * \brief Gets the amount of edges connected to the vertex.
     * \return The amount of edges connected to the vertex.
     */
    [[nodiscard]] int get_size() const;

    /**
     * \brief Checks if the vertex has any edges.
     * \return True if the vertex has no edges, false otherwise.
     */
    [[nodiscard]] bool is_empty() const;
};

template <class T>
class Edge
{
public:
    /**
     * \brief Creates a new edge with default values.
     */
    Edge();

    /**
     * \brief Creates a new edge with the given values.
     * \param from The vertex to connect from.
     * \param to The vertex to connect to.
     */
    Edge(Vertex<T>* from, Vertex<T>* to, float distance);

    /**
     * \brief Destroys the edge.
     */
    ~Edge();

    /**
     * \brief Copy constructor.
     * \param other The edge to copy.
     */
    Edge(const Edge& other);

    /**
     * \brief Copy assignment operator.
     * \param other The edge to copy.
     * \return The copied edge.
     */
    Edge& operator=(const Edge& other);

    /**
     * \brief Move constructor.
     * \param other The edge to move.
     */
    Edge(const Edge&& other) noexcept;

    /**
     * \brief Move assignment operator.
     * \param other The edge to move.
     * \return The moved edge.
     */
    Edge& operator=(Edge&& other) noexcept;

    /**
     * \brief Sets the vertex to connect from.
     * \param from The vertex to connect from.
     */
    void set_from(Vertex<T>* from);

    /**
     * \brief Sets the vertex to connect to.
     * \param to The vertex to connect to.
     */
    void set_to(Vertex<T>* to);

    /**
     * \brief Sets the distance between the two vertexes.
     * \param distance The distance between the two vertexes.
     */
    void set_distance(float distance);

    /**
     * \brief Flips the edge.
     */
    void flip();

    /**
     * \brief Gets the vertex to connect from.
     * \return The vertex to connect from.
     */
    [[nodiscard]] Vertex<T>* get_from() const;

    /**
     * \brief Gets the vertex to connect to.
     * \return The vertex to connect to.
     */
    [[nodiscard]] Vertex<T>* get_to() const;

    /**
     * \brief Gets the distance between the two vertexes.
     * \return The distance between the two vertexes.
     */
    [[nodiscard]] float get_distance() const;

private:
    Vertex<T>* from;
    Vertex<T>* to;
    float distance;
};

template <class T>
class Graph
{
    std::vector<Vertex<T>*> vertices;
public:
    /**
     * \brief Creates a new graph.
     */
    Graph();

    /**
     * \brief Destroys the graph and all vertices and edges.
     */
    ~Graph();

    /**
     * \brief Copy constructor.
     * \param other The graph to copy.
     */
    Graph(const Graph& other);

    /**
     * \brief Copy assignment operator.
     * \param other The graph to copy.
     * \return The copied graph.
     */
    Graph& operator=(const Graph& other);

    /**
     * \brief Move constructor.
     * \param other The graph to move.
     */
    Graph(const Graph&& other) noexcept;

    /**
     * \brief Move assignment operator.
     * \param other The graph to move.
     * \return The moved graph.
     */
    Graph& operator=(Graph&& other) noexcept;

    /**
     * \brief Adds a vertex to the graph.
     * \param vertex The vertex to add.
     */
    void add_vertex(Vertex<T>* vertex);

    /**
     * \brief Removes a vertex from the graph.
     * \param vertex The vertex to remove.
     */
    void remove_vertex(Vertex<T>* vertex);

    /**
     * \brief Removes all vertices from the graph.
     */
    void remove_all_vertices();

    /**
     * \brief Gets all vertices from the graph.
     * \return A vector of vertices.
     */
    [[nodiscard]] std::vector<Vertex<T>*> get_vertices() const;

    /**
     * \brief Gets a vertex from the graph.
     * \param index The index of the vertex to get.
     * \return The vertex at the given index.
     */
    Vertex<T>* operator[](int index) const;

    /**
     * \brief Checks if the graph has any vertices.
     * \return True if the graph has no vertices, false otherwise.
     */
    [[nodiscard]] bool is_empty() const;

    /**
     * \brief Gets the amount of vertices in the graph.
     * \return The amount of vertices in the graph.
     */
    [[nodiscard]] int size() const;
};

template <class T>
Vertex<T>::Vertex()
{
    this->info = T();
    this->edges = std::vector<Edge<T>*>();
}

template <class T>
Vertex<T>::Vertex(T value)
{
    this->info = value;
    this->edges = std::vector<Edge<T>*>();
}

template <class T>
Vertex<T>::~Vertex()
{
    this->remove_all_edges();
}

template <class T>
Vertex<T>::Vertex(const Vertex& other)
{
    this->info = other.info;
    this->edges = other.edges;
}

template <class T>
Vertex<T>& Vertex<T>::operator=(const Vertex& other)
{
    if (this != &other)
    {
        this->info = other.info;
        this->edges = other.edges;
    }
    return *this;
}

template <class T>
Vertex<T>::Vertex(Vertex&& other) noexcept
{
    this->info = other.info;
    this->edges = other.edges;
}

template <class T>
Vertex<T>& Vertex<T>::operator=(Vertex&& other) noexcept
{
    if (this != &other)
    {
        this->info = other.info;
        this->edges = other.edges;
    }
    return *this;
}

template <class T>
void Vertex<T>::add_edge(Vertex* to, float distance)
{
    auto edge = new Edge<T>(this, to, distance);
    auto edge2 = new Edge<T>(to, this, distance);
    this->edges.push_back(edge);
    to->add_edge(edge2);
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
int Vertex<T>::get_size() const
{
    return this->edges.size();
}

template <class T>
bool Vertex<T>::is_empty() const
{
    return this->edges.empty();
}

template <class T>
Edge<T>::Edge()
{
    this->from = nullptr;
    this->to = nullptr;
    this->distance = 0;
}

template <class T>
Edge<T>::Edge(Vertex<T>* from, Vertex<T>* to, float distance)
{
    this->from = from;
    this->to = to;
    this->distance = distance;
}

template <class T>
Edge<T>::~Edge()
{
    this->from = nullptr;
    this->to = nullptr;
    this->distance = 0;
}

template <class T>
Edge<T>::Edge(const Edge& other)
{
    this->from = other.from;
    this->to = other.to;
    this->distance = other.distance;
}

template <class T>
Edge<T>& Edge<T>::operator=(const Edge& other)
{
    if (this != &other)
    {
        this->from = other.from;
        this->to = other.to;
        this->distance = other.distance;
    }
    return *this;
}

template <class T>
Edge<T>::Edge(const Edge&& other) noexcept
{
    this->from = other.from;
    this->to = other.to;
    this->distance = other.distance;
}

template <class T>
Edge<T>& Edge<T>::operator=(Edge&& other) noexcept
{
    if (this != &other)
    {
        this->from = other.from;
        this->to = other.to;
        this->distance = other.distance;
    }
    return *this;
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
void Edge<T>::set_distance(float distance)
{
    this->distance = distance;
}

template <class T>
void Edge<T>::flip()
{
    auto temp = this->from;
    this->from = this->to;
    this->to = temp;
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
float Edge<T>::get_distance() const
{
    return this->distance;
}

template <class T>
Graph<T>::Graph()
{
    this->vertices = std::vector<Vertex<T>*>();
}

template <class T>
Graph<T>::~Graph()
{
    this->remove_all_vertices();
}

template <class T>
Graph<T>::Graph(const Graph& other)
{
    this->vertices = other.vertices;
}

template <class T>
Graph<T>& Graph<T>::operator=(const Graph& other)
{
    if (this != &other)
    {
        this->vertices = other.vertices;
    }
    return *this;
}

template <class T>
Graph<T>::Graph(const Graph&& other) noexcept
{
    this->vertices = other.vertices;
}

template <class T>
Graph<T>& Graph<T>::operator=(Graph&& other) noexcept
{
    if (this != &other)
    {
        this->vertices = other.vertices;
    }
    return *this;
}

template <class T>
void Graph<T>::add_vertex(Vertex<T>* vertex)
{
    this->vertices.push_back(vertex);
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
bool Graph<T>::is_empty() const
{
    return this->vertices.empty();
}

template <class T>
int Graph<T>::size() const
{
    return this->vertices.size();
}
