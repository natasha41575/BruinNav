#ifndef MYMAP_H
#define MYMAP_H

// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.

/* You will create a class template MyMap which works much like the C++ STL map and
 * which must use a binary search tree as its data structure. This class template will hold
 * associations between an arbitrary type of key (e.g., a string containing an attraction
 * name like “Barney’s Beanery”) with an arbitrary type of value (e.g., a latitude/longitude
 * where that attraction is located).
 */

#include <iostream>
#include <string>
using namespace std;

template<typename KeyType, typename ValueType>
class MyMap {
private:
    struct Node {
        Node(const KeyType &key, const ValueType &value) : v(value), k(key){}
        KeyType k;
        ValueType v;
        Node* left;
        Node* right;
    };
    Node* m_root;
    int m_size;
    
    void associate(Node*& root, const KeyType& key, const ValueType& value) {
        if (root == nullptr) {
            root = new Node(key, value);
            root->left = nullptr;
            root->right = nullptr;
            m_size++;
        }
        else if (root->k == key) {
            root->v = value;
        }
        else if (root->k < key) {
            associate(root->left, key, value);
        }
        else {
            associate(root->right, key, value);
        }
    }
    
    const ValueType* find(Node* root, const KeyType& key) const {
        if (root == nullptr) {
            return nullptr;
        }
        else if (root->k == key) {
            return &(root->v);
        }
        else if (root->k < key) {
            return find(root->left, key);
        }
        else {
            return find(root->right, key);
        }
    }
    
    void clear(Node*& root) {
        if (root == nullptr) {
            return;
        }
        clear(root->right);
        clear(root->left);
        delete root;
    }

public:
    MyMap() {
        m_root = nullptr;
        m_size = 0;
    }
    ~MyMap() {
        clear();
    }
    void clear() {
        clear(m_root);
        m_size = 0;
    }
    int size() const {
        return m_size;
    }
    
    //	The	associate	method	associates	one	item	(key)	with	another	(value).
    //	If	no	association	currently	exists	with	that	key,	this	method	inserts
    //	a	new	association	into	the	tree	with	that	key/value	pair.		If	there	is
    //	already	an	association	with	that	key	in	the	tree,	then	the	item
    //	associated	with	that	key	is	replaced	by	the	second	parameter	(value).
    //	Thus,	the	tree	contains	no	duplicate	keys.
    void associate(const KeyType& key, const ValueType& value) {
        associate(m_root, key, value);
    }
    
    // for a map that can't be modified, return a pointer to const ValueType
    //	If	no	association	exists	with	the	given	key,	return	nullptr;	otherwise,
    //	return	a	pointer	to	the	value	associated	with	that	key.		This	pointer	can	be
    //	used	to	examine	that	value,	and	if	the	map	is	allowed	to	be	modified,	to
    //	modify	that	value	directly	within	the	map (the	second	overload	enables
    //	this).		Using	a	little	C++	magic,	we	have	implemented	it	in	terms	of	the
    //	first	overload,	which	you	must	implement.
    const ValueType* find(const KeyType& key) const {
        if (m_size == 0) {
            return nullptr;
        }
        else {
            return find(m_root, key);
        }
    }
    
    // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key) {
        return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
    }
    
    // C++11 syntax for preventing copying and assignment
    MyMap(const MyMap&) = delete;
    MyMap& operator=(const MyMap&) = delete;
};

#endif //MYMAP_H
