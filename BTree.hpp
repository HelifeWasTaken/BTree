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

#pragma once

#ifdef _MSVC_LANG
static_assert(defined(_MSVC_LANG) && _MSVC_LANG >= 201703L && "BtreeImplementation requires C++17 or higher (std::optional)");
#else
static_assert(__cplusplus >= 201703L, "BtreeImplementation requires C++17 or higher (std::optional)");
#endif

#include <optional>
#include <memory>
#include <functional>

#ifndef BTREE_NAMESPACE
#define BTREE_NAMESPACE btree
#endif

namespace BTREE_NAMESPACE {

template<typename T>
class BtreeNode;

template<typename T>
using BtreeValue = std::unique_ptr<BtreeNode<T>>;

template<typename T>
using BtreeCmp = std::function<bool(const T& toInsert, const T& cNode)>;

template<typename T>
using BtreeSearchResult = std::optional<std::reference_wrapper<T>>;

template<typename T>
using BtreeSearchResultConst = const std::optional<const std::reference_wrapper<const T>>;

template<typename T>
class BtreeNode {
private:
    BtreeValue<T>  _left = nullptr;
    BtreeValue<T> _right = nullptr;
    T _value;
    bool _visited = false;

public:
    BtreeNode(T &&value)
        : _value(std::move(value))
    {}

    // The value passed as parameter in reference will be moved
    BtreeNode(T &value)
        : _value(value)
    {}

    static BtreeValue<T> makeNode(T &value)
    { return std::make_unique<BtreeNode<T>>(value); }
    static BtreeValue<T> makeNode(T &&value)
    { return std::make_unique<BtreeNode<T>>(std::move(value)); }

    // Returns the value in the object
    T &get() { return _value; }
    // Returns the value in the object (const)
    const T &get() const { return _value; }

    // Returns the left element of the BinaryTree
    BtreeValue<T>& left() { return _left; }
    // Returns the left element of the BinaryTree (const)
    const BtreeValue<T>& left() const { return _left; }

    // Returns the Right element of the BinaryTree
    BtreeValue<T>& right() { return _right; }
    // Returns the Right element of the BinaryTree (const)
    const BtreeValue<T>& right() const { return _right; }

    // Takes the value as parameter
    // If cmp returns false Left is choosen otherwise right is choosen
    void insert(T &value, const BtreeCmp<T>& cmp, const bool& allow_duplicates) {
        if (!allow_duplicates && (value == _value)) {
            return;
        } else if (cmp(value, _value)) {
            if (_left.get()) _left->insert(value, cmp, allow_duplicates);
            else _left = makeNode(value);
        } else {
            if (_right.get()) _right->insert(value, cmp, allow_duplicates);
            else _right = makeNode(value);
        }
    }

    // Takes the value as parameter
    // If cmp returns false Left is choosen otherwise right is choosen
    // Is used to move the value
    void insert(T &&value, const BtreeCmp<T>& cmp, const bool& allow_duplicates) {
        if (!allow_duplicates && (value == _value)) {
            return;
        } else if (cmp(value, _value)) {
            if (_left.get()) _left->insert(value, cmp, allow_duplicates);
            else _left = makeNode(value);
        } else {
            if (_right.get()) _right->insert(value, cmp, allow_duplicates);
            else _right = makeNode(value);
        }
    }

    // Takes a value to research as parameter and the comparator
    // If cmp returns false Left is choosen otherwise right is choosen
    BtreeSearchResult<T> search(const T& value, const BtreeCmp<T>& cmp) {
        if (_value == value) {
            return { _value };
        } else if (cmp(value, _value)) {
            if (_left.get()) return _left->search(value, cmp);
            else return std::nullopt;
        } else {
            if (_right.get()) return _right->search(value, cmp);
            else return std::nullopt;
        }
    }

    // See search method (Same as const)
    BtreeSearchResultConst<T> search(const T& value, const BtreeCmp<T>& cmp) const {
        if (_value == value) {
            return { _value };
        } else if (cmp(value, _value)) {
            if (_left.get()) return _left->search(value, cmp);
            else return std::nullopt;
        } else {
            if (_right.get()) return _right->search(value, cmp);
            else return std::nullopt;
        }
    }

    // Tells wheter this node has been visited
    const bool& isVisited() const { return _visited; }

    // Triggers visited to false
    // Does not set unvisited on all subNodes by default
    void unvisit(const bool& unvisitSubNodes=false) {
        _visited = false;
        if (unvisitSubNodes) {
            if (_left.get()) _left->unvisit(unvisitSubNodes);
            if (_right.get()) _right->unvisit(unvisitSubNodes);
        }
    }

    // Triggers visited to true
    void visit() { _visited = true; }
};

template<typename T>
class Btree {
private:
    BtreeValue<T> _root = nullptr;
    BtreeCmp<T> _cmp;
    bool _allow_duplicates;

public:
    Btree(const BtreeCmp<T>& cmp, const bool &allow_duplicates=true)
        : _cmp(cmp)
        , _allow_duplicates(allow_duplicates)
    {}

    Btree& insert(T& value) {
        if (!_root.get()) _root = BtreeNode<T>::makeNode(value);
        else _root->insert(value, _cmp, _allow_duplicates);
        return *this;
    }

    Btree& insert(T&& value) {
        if (!_root.get()) _root = BtreeNode<T>::makeNode(std::move(value));
        else _root->insert(value, _cmp, _allow_duplicates);
        return *this;
    }

    BtreeSearchResult<T> search(const T& value) {
        return _root.get() ? _root->search(value, _cmp) : std::nullopt;
    }

    BtreeSearchResultConst<T> search(const T& value) const {
        return _root.get() ? _root->search(value, _cmp) : std::nullopt;
    }

    BtreeValue<T>& root() { return _root; }
    const BtreeValue<T>& root() const { return _root; }

    Btree& unvisitNodes() {
        _root->unvisit(true); return *this;
    }
};
}
