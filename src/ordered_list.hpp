////////////////////////////////////////////////////////////////////////////////
// Module Name:  ordered_list.h/hpp
// Authors:      Leonid Dworzanski, Sergey Shershakov
// Version:      2.0.0
// Date:         28.10.2018
//
// This is a part of the course "Algorithms and Data Structures"
// provided by  the School of Software Engineering of the Faculty
// of Computer Science at the Higher School of Economics.
////////////////////////////////////////////////////////////////////////////////

// !!! DO NOT include ordered_list.h here, 'cause it leads to circular refs. !!!


//=============================================================================
// class NodeSkipList
//=============================================================================


template <class Value, class Key, class Node>
OrderedList<Value, Key, Node>::OrderedList()
{
    _preHead = new Node;
    _preHead->next = _preHead;
}

//-----------------------------------------------------------------------------


    // TODO: !!! Implement destructor correctly !!!
template <class Value, class Key, class Node>
OrderedList<Value, Key, Node>::~OrderedList()
{
    Node* iterNode = _preHead->next;
    if(iterNode != _preHead)
    {
        Node *bufNode;
        while (iterNode->next != _preHead)
        {
            bufNode = iterNode;
            iterNode = iterNode->next;
            delete bufNode;
        }
        delete iterNode;
        delete _preHead;
    } else{
        delete _preHead;
    }
}

//-----------------------------------------------------------------------------


template <class Value, class Key, class Node >
void OrderedList<Value,Key,Node>::insert(const Value& val, const Key& tkey)
{
    // ищем "последний" элемент в цепочке
    Node* run = _preHead;
    while (run->next != _preHead)
        run = run->next;


    Node* tmp = new Node;
    tmp->next = run->next;
    tmp->key = tkey;
    tmp->value = val;

    run->next = tmp;
}

//-----------------------------------------------------------------------------

template <class Value, class Key, class Node >
void OrderedList<Value,Key,Node>::removeNext(Node* nodeBefore)
{
    if (nodeBefore == nullptr
        || nodeBefore->next == nullptr
        || nodeBefore->next == _preHead)
    {
        return;
    }

    Node* tmp = nodeBefore->next;
    nodeBefore->next = tmp->next;
    delete tmp;
}

//-----------------------------------------------------------------------------


template <class Value, class Key, class Node >
Node* OrderedList<Value,Key,Node>::findLastLessThan(const Key& key) const
{
    Node* run = _preHead;
    while(run->next != _preHead && run->next->key < key)
        run = run->next;

    return run;
}

//-----------------------------------------------------------------------------

template <class Value, class Key, class Node >
Node* OrderedList<Value,Key,Node>::findFirst(const Key& key) const
{
    Node* run = _preHead;
    while (run->next != _preHead)
    {
        run = run->next;
        if (run->key == key)
            return run;
    }

    return nullptr;
}

//-----------------------------------------------------------------------------

// Returns the sentinel node
template <class Value, class Key, class Node>
Node* OrderedList<Value, Key, Node>::getPreHead() const
{
    return _preHead;
}

//-----------------------------------------------------------------------------

