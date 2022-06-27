/**
 * BTree.hpp
 * Copyright (c) 2022 Mattis Dalleau <mattis.dalleau@epitech.eu>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <optional>

#ifndef BTREE_NAMESPACE
#define BTREE_NAMESPACE btree
#endif

namespace BTREE_NAMESPACE {

template<typename T>
class BtreeNode {
public:
    using BtreeValue = std::optional<BtreeNode<T>>;
    using BtreeNoValue = std::nullopt;
    using BtreeCmp = std::function<bool(const T& toInsert, const T& cNode)>;

private:
    BtreeValue<T> _left = BtreeNoValue;
    BtreeValue<T> _right = BtreeNoValue;
    T _value;
    bool _visited = false;

public:
    BtreeNode(T &&value)
        : _value(std::move(value))
    {}

    // The value passed as parameter in reference will be moved
    BtreeNode(T &value)
        : _value(std::move(value))
    {}

    // Returns the value in the object
    T &get() { return _value; }
    // Returns the value in the object (const)
    const T &get() const { return _value; }

    // Returns the left element of the BinaryTree
    BtreeValue& left() { return _left; }
    // Returns the left element of the BinaryTree (const)
    const BtreeValue& left() const { return _left; }

    // Returns the Right element of the BinaryTree
    BtreeValue& right() { return _right; }
    // Returns the Right element of the BinaryTree (const)
    const BtreeValue& right() const { return _right; }

    // Takes the value as parameter
    // If cmp returns false Left is choosen otherwise right is choosen
    void insert(T &value, const BtreeCmp& cmp) {
        if (cmp(value, _value)) {
            if (_left) _left.insert(value, cmp);
            else _left = { BtreeNode<T>(value) };
        } else {
            if (_right) _right.insert(value, cmp);
            else _right = { BtreeNode<T>(value) };
        }
    }

    // Takes a value to research as parameter and the comparator
    // If cmp returns false Left is choosen otherwise right is choosen
    std::optional<T&> search(const T& value, const BtreeCmp& cmp) {
        if (_value == value) return { _value };
        if (cmp(value, _value)) {
            if (_left) return _left.search(value, cmp);
            else return std::nullopt;
        } else {
            if (_right) return _right.search(value, cmp);
            else return std::nullopt;
        }
    }

    // See search method (Same as const)
    const std::optional<const T&> search(const T& value, const BtreeCmp& cmp) const {
        if (_value == value) return { _value };
        if (cmp(value, _value)) {
            if (_left) return _left.search(value, cmp);
            else return std::nullopt;
        } else {
            if (_right) return _right.search(value, cmp);
            else return std::nullopt;
        }
    }

    // Tells wheter this node has been visited
    const bool& isVisited const { return _visited; }

    // Triggers visited to false
    // Does not set unvisited on all subNodes by default
    void unvisit(const bool& unvisitSubNodes=false) {
        _visited = false;
        if (unvisitSubNodes) {
            if (_left) _left.unvisit(unvisitSubNodes);
            if (_right) _right.unvisit(unvisitSubNodes);
        }
    }

    // Triggers visited to true
    void visit() { _visited = true; }
};

template<typename T>
class Btree {
private:
    BtreeNode<T>::BtreeValue _root;
    BtreeNode<T>::BtreeCmp _cmp;

public:
    Btree(const BtreeNode<T>::BtreeCmp& cmp)
        : _cmp(cmp)
    {}

    Btree(const BtreeNode<T>::BtreeCmp& cmp, T& value)
        : _root(value)
        , _cmp(cmp)
    {}

    Btree(const BtreeNode<T>::BtreeCmp& cmp, T&& value)
        : _root(value)
        , _cmp(cmp)
    {}

    Btree& insert(T& value) {
        if (_root) _root = { BtreeNode(value) };
        else _root.insert(value, cmp);
        return *this;
    }

    std::optional<T&> search(const T& value) {
        return _root ? _root.search(value, _cmp) : std::nullopt;
    }

    const std::optional<const T&> search(const T& value) const {
        return _root ? _root.search(value, _cmp) : std::nullopt;
    }

    BtreeNode<T>::BtreeValue& root() { return _root; }
    const BtreeNode<T>::BtreeValue& root() const { return _root; }

    Btree& unvisitNodes() { _root.unvisit(true); }
};
}
