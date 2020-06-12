////////////////////////////////////////////////////////////////////////////////
// Module Name:  skip_list.h/hpp
// Authors:      Leonid Dworzanski, Sergey Shershakov
// Version:      2.0.0
// Date:         28.10.2018
//
// This is a part of the course "Algorithms and Data Structures"
// provided by  the School of Software Engineering of the Faculty
// of Computer Science at the Higher School of Economics.
////////////////////////////////////////////////////////////////////////////////

// !!! DO NOT include skip_list.h here, 'cause it leads to circular refs. !!!

#include <cstdlib>
#include <vector>

//==============================================================================
// class NodeSkipList
//==============================================================================

template <class Value, class Key, int numLevels>
void NodeSkipList<Value, Key, numLevels>::clear(void)
{
    for (int i = 0; i < numLevels; ++i)
        Base::nextJump[i] = 0;

    Base::levelHighest = -1;
}

//------------------------------------------------------------------------------

template <class Value, class Key, int numLevels>
NodeSkipList<Value, Key, numLevels>::NodeSkipList(void)
{
    clear();
}

//------------------------------------------------------------------------------

template <class Value, class Key, int numLevels>
NodeSkipList<Value, Key, numLevels>::NodeSkipList(const Key& tkey)
{
    clear();

    Base::Base::key = tkey;
}

//------------------------------------------------------------------------------

template <class Value, class Key, int numLevels>
NodeSkipList<Value, Key, numLevels>::NodeSkipList(const Key& tkey, const Value& val)
{
    clear();

    Base::Base::key = tkey;
    Base::Base::value = val;
}


//==============================================================================
// class SkipList
//==============================================================================

template <class Value, class Key, int numLevels>
SkipList<Value, Key, numLevels>::SkipList(double probability)
{
    _probability = probability;

    // Lets use m_pPreHead as a final sentinel element
    for (int i = 0; i < numLevels; ++i)
        Base::_preHead->nextJump[i] = Base::_preHead;

    Base::_preHead->levelHighest = numLevels - 1;
    srand(time(0));
}

template <class Value, class Key, int numLevels>
void SkipList<Value, Key, numLevels>::insert(const Value &val, const Key &key)
{
    std::vector<Node*> nearestNods;
    Node* bufNode = Base::_preHead;
    for (int iter = bufNode->levelHighest ; iter > -1; iter--)
    {
        while (bufNode->nextJump[iter] != Base::_preHead && bufNode->nextJump[iter]->key <= key)
        {
            bufNode = bufNode->nextJump[iter];
        }
        nearestNods.insert(nearestNods.begin(), bufNode);
    }

    while (bufNode->next != Base::_preHead && bufNode->next->key <= key)
    {
        bufNode = bufNode->next;
    }

    Node* newNode = new Node(key, val);
    newNode->next = bufNode->next;
    bufNode->next = newNode;

    double randDouble = (rand() * 1.0 / RAND_MAX);
    int layerCount = 0;
    while (randDouble < _probability && layerCount < nearestNods.size()){
        newNode->nextJump[layerCount] = nearestNods[layerCount]->nextJump[layerCount];
        nearestNods[layerCount]->nextJump[layerCount] = newNode;
        layerCount++;
        randDouble = (rand() * 1.0 / RAND_MAX);
    }
    newNode->levelHighest = layerCount - 1;
    nearestNods.clear();
}

template <class Value, class Key, int numLevels>
void SkipList<Value, Key, numLevels>::removeNext(SkipList<Value, Key, numLevels>::Node *nodeBefore)
{
    if(nodeBefore == nullptr || nodeBefore->next == Base::_preHead)
    {
        throw std::invalid_argument("You can't delete preHead");
    }

    Node* nearestNode = Base::_preHead;

    for (int iter = nearestNode->levelHighest ; iter > -1; iter--)
    {
        while (nearestNode->nextJump[iter] != Base::_preHead && nearestNode->nextJump[iter]->key < nodeBefore->next->key)
        {
            nearestNode = nearestNode->nextJump[iter];
        }
        Node* nearestBufNode = nearestNode;
        while (nearestBufNode->nextJump[iter] != Base::_preHead && nearestBufNode->nextJump[iter]->key == nodeBefore->next->key)
        {

            if (nearestBufNode->nextJump[iter] == nodeBefore->next && nearestBufNode->nextJump[iter] != Base::_preHead)
            {
                nearestBufNode->nextJump[iter] = nearestBufNode->next->nextJump[iter];
            }
            nearestBufNode = nearestBufNode->nextJump[iter];
        }
    }

    while (nearestNode->next->key < nodeBefore->next->key && nearestNode->next != Base::_preHead)
    {
        nearestNode = nearestNode->next;
    }

    while (nearestNode->next->key == nodeBefore->next->key && nearestNode->next != Base::_preHead)
    {
        if (nearestNode->next == nodeBefore->next)
        {
            Node *bufNode = nodeBefore->next;
            nearestNode->next = nodeBefore->next->next;
            delete bufNode;
            return;
        }
        nearestNode = nearestNode->next;
    }

    throw std::invalid_argument("No such node was found");

}

template<class Value, class Key, int numLevels>
NodeSkipList<Value,Key, numLevels>* SkipList<Value, Key, numLevels>::findLastLessThan(const Key &key) const
{
    Node* nearestNode = Base::_preHead;

    if(Base::_preHead->next == Base::_preHead || Base::_preHead->next->key >= key)
    {
        return Base::_preHead;
    }

    for (int iter = nearestNode->levelHighest ; iter > -1; iter--)
    {
        while (nearestNode->nextJump[iter] != Base::_preHead && nearestNode->nextJump[iter]->key < key)
        {
            nearestNode = nearestNode->nextJump[iter];
        }
    }

    while (nearestNode->next->key < key && nearestNode != Base::_preHead)
    {
        nearestNode = nearestNode->next;
    }

    return nearestNode;
}

template<class Value, class Key, int numLevels>
NodeSkipList<Value,Key, numLevels>* SkipList<Value, Key, numLevels>::findFirst(const Key &key) const
{
    Node* nearestNode = Base::_preHead;

    for (int iter = nearestNode->levelHighest ; iter > -1; iter--)
    {
        while (nearestNode->nextJump[iter] != Base::_preHead && nearestNode->nextJump[iter]->key < key)
        {
            nearestNode = nearestNode->nextJump[iter];
        }

        if(nearestNode->nextJump[iter]->key == key)
        {
            return nearestNode->nextJump[iter];
        }
    }

    while (nearestNode->next->key < key && nearestNode->next != Base::_preHead){
        nearestNode = nearestNode->next;
    }

    if(nearestNode->next->key == key)
    {
        return nearestNode -> next;
    } else{
        return nullptr;
    }
}

template<class Value, class Key, int numLevels>
SkipList<Value, Key, numLevels>::~SkipList()
{
    // All deleting in ~OrderedList
}
