// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.
#ifndef MYMAP_H
#define MYMAP_H

#include <queue>

using namespace std;

template<typename KeyType, typename ValueType>
class MyMap
{
public:
    MyMap();
    ~MyMap();
    void clear();
    int size() const;
    void associate(const KeyType& key, const ValueType& value);
    
	// for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	// for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}
    
    // C++11 syntax for preventing copying and assignment
    MyMap(const MyMap&) = delete;
    MyMap& operator=(const MyMap&) = delete;
    
private:
    struct Node
    {
        KeyType key;
        ValueType value;
        Node* left = nullptr;
        Node* right = nullptr;
    };
    
    Node* m_head;
};

template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::MyMap()
{
    m_head = nullptr;
}

template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::~MyMap()
{
    clear();
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::clear()
{
    if (m_head == nullptr)
        return;
    queue<Node*> data;
    data.push(m_head);
    while (!data.empty()) //looping through all nodes in the tree, and delete each visited one
    {
        Node* tmp = data.front();
        data.pop();
        if (tmp->left != nullptr)
            data.push(tmp->left);
        if (tmp->right != nullptr)
            data.push(tmp->right);
        delete tmp;
    }
}

template<typename KeyType, typename ValueType>
int MyMap<KeyType, ValueType>::size() const
{
    if (m_head == nullptr)
        return 0;
    int count = 0;
    queue<Node*> data;
    data.push(m_head);
    while (!data.empty()) //looping through all nodes in the tree, and return the count
    {
        Node* tmp = data.front();
        data.pop();
        count++;
        if (tmp->left != nullptr)
            data.push(tmp->left);
        if (tmp->right != nullptr)
            data.push(tmp->right);
    }
    return count;
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::associate(const KeyType &key, const ValueType &value)
{
    if (m_head == nullptr) //if tree is empty, add new node and assign m_head point to it
    {
		Node* n = new Node;
        n->key = key;
        n->value = value;
		m_head = n;
    }
    else //if there are elements in tree already, find the correct position and insert
    {
        Node* current = m_head;
        while (current != nullptr)
        {
            if (key < current->key)
            {
                if (current->left == nullptr)
                {
					Node* n;
                    n = new Node;
                    n->key = key;
                    n->value = value;
					current->left = n;
                    return;

                }
                current = current->left;
            }
            else if (key > current->key)
            {
                if (current->right == nullptr)
                {
					Node* n;
                    n = new Node;
                    n->key = key;
                    n->value = value;
					current->right = n;
                    return;
                    
                }
                current = current->right;
            }
            else if (key == current->key)
            {
                current->value = value;
                return;
            }
        }

    }
}

template<typename KeyType, typename ValueType>
const ValueType* MyMap<KeyType, ValueType>::find(const KeyType &key) const
{
    Node* current = m_head;
    while (current != nullptr)
    {
        if (key == current->key)
            return &(current->value);
        if (key < current->key)
            current = current->left;
        else
            current = current->right;
    }
    return nullptr;
}

#endif
