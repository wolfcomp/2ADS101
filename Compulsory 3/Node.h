#pragma once
#include <vector>

template <class T>
class Node
{
public:
    /**
     * \brief Default constructor for Node with value 0
     */
    Node();
    /**
     * \brief Constructor for Node with value
     * \param value Value of the Node
     */
    explicit Node(T value);
    /**
     * \brief Destructor for Node
     */
    ~Node();
    /**
     * \brief Assignment operator for Node
     * \param other Node to assign to
     * \return Assigned Node
     */
    Node& operator=(const Node& other);
    /**
     * \brief Move assignment operator for Node
     * \param other Node to assign to
     */
    Node(const Node& other);
    /**
     * \brief Move assignment operator for Node
     * \param other Node to assign to
     * \return Assigned Node
     */
    Node& operator=(Node&& other);
    /**
     * \brief Move assignment operator for Node
     * \param other Node to assign to
     */
    Node(Node&& other);
    /**
     * \brief Adds child to the Node
     * \param child Child to add to Node
     */
    void add_child(Node* child);
    /**
     * \brief Removes child from the Node
     * \param child Child to remove from Node
     */
    void remove_child(Node* child);
    /**
     * \brief Removes all children from the Node and child Nodes
     */
    void remove_all_children();
    /**
     * \brief Sets parent of the Node
     * \param parent Parent to set for Node
     */
    void set_parent(Node* parent);
    /**
     * \brief Gets the parent of the Node
     * \return Parent Node
     */
    Node* get_parent() const;
    /**
     * \brief Gets the children's of the Node
     * \return Children's of the Node
     */
    std::vector<Node*> get_children() const;
    /**
     * \brief Gets the value of the Node
     * \return Value of the Node
     */
    T get_value() const;
    /**
     * \brief Gets a child of the Node
     * \param index Index of the child to retrieve
     * \return Node in the children list
     */
    Node* operator[](int index) const;
private:
    Node* parent;
    T value;
    std::vector<Node*> children;
};

template <class T>
Node<T>::Node()
{
    this->parent = nullptr;
    this->value = T();
    this->children = std::vector<Node*>();
}

template <class T>
Node<T>::Node(T value)
{
    this->parent = nullptr;
    this->value = value;
    this->children = std::vector<Node*>();
}

template <class T>
Node<T>::~Node()
{
    this->children.clear();
    this->parent = nullptr;
    this->value = T();
}

template <class T>
Node<T>& Node<T>::operator=(const Node& other)
{
    this->parent = other.parent;
    this->value = other.value;
    this->children = other.children;
    return *this;
}

template <class T>
Node<T>::Node(const Node& other)
{
    this->parent = other.parent;
    this->value = other.value;
    this->children = other.children;
}

template <class T>
Node<T>& Node<T>::operator=(Node&& other)
{
    this->parent = other.parent;
    this->value = other.value;
    this->children = other.children;
    return *this;
}

template <class T>
Node<T>::Node(Node&& other)
{
    this->parent = other.parent;
    this->value = other.value;
    this->children = other.children;
}

template <class T>
void Node<T>::add_child(Node* child)
{
    this->children.push_back(child);
    child->set_parent(this);
}

template <class T>
std::vector<Node<T>*> Node<T>::get_children() const
{
    return this->children;
}

template <class T>
Node<T>* Node<T>::get_parent() const
{
    return this->parent;
}

template <class T>
T Node<T>::get_value() const
{
    return this->value;
}

template <class T>
void Node<T>::remove_all_children()
{
    for (const auto& child : this->children)
    {
        child->remove_all_children();
        child->set_parent(nullptr);
        delete child;
    }
    this->children.clear();
}

template <class T>
void Node<T>::remove_child(Node* child)
{
    for (auto it = this->children.begin(); it != this->children.end(); ++it)
    {
        if (*it == child)
        {
            // child->remove_all_children();

            this->children.erase(it);
            for (const auto& grandchild : child->get_children())
            {
                this->add_child(grandchild);
            }
            child->set_parent(nullptr);
            delete child;
            break;
        }
    }
}

template <class T>
void Node<T>::set_parent(Node* parent)
{
    this->parent = parent;
}

template <class T>
Node<T>* Node<T>::operator[](int index) const
{
    return this->children[index];
}