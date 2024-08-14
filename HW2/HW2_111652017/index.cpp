#include <algorithm>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#pragma GCC optimize("O3")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,tune=native")
#include "index.h"
using namespace std;

const int DEGREE = 100;
const int INF = 2147483647;

inline int min(int a, int b) { return a < b ? a : b; }
Node::Node(int _degree) { // Constructor
  this->is_leaf = false;
  this->degree = _degree;
  this->size = 0;

  pair<int, int> *_item = new pair<int, int>[degree - 1];
  for (int i = 0; i < degree - 1; i++) {
    _item[i] = make_pair(0, 0);
  }
  this->item = _item;
  Node **_children = new Node *[degree];
  for (int i = 0; i < degree; i++) {
    _children[i] = NULL;
  }
  this->children = _children;
  this->parent = NULL;
}

Index::Index(const int &num_rows, const vector<int> &key,
             const vector<int> &value) { // ok Constructor
  this->root = NULL;
  for (int i = 0; i < num_rows; i++) {
    insert(make_pair(key[i], value[i]));
  }
}

Node *Index::getroot() { return this->root; }

int Index::find_index(pair<int, int> *arr, const pair<int, int> &data,
                      const int &len) {
  int idx = upper_bound(arr, arr + len, data) - arr;
  return idx;
}
Node *Index::IndexTreeSearch(Node *node, const pair<int, int> &key) {
  if (node == NULL)
    return NULL;
  else {
    Node *cursor = node;       // cursor finding key
    while (!cursor->is_leaf) { // until cusor pointer arrive leaf
      int k = find_index(cursor->item, key, cursor->size);
      cursor = cursor->children[k];
    }
    return cursor;
  }
}
Node **Index::child_insert(Node **child_item, Node *child, const int &len,
                           int idx) {
  for (int i = len; i > idx; i--)
    child_item[i] = child_item[i - 1];
  child_item[idx] = child;
  return child_item;
}
Node *Index::child_item_insert(Node *node, const pair<int, int> &data,
                               Node *child) {
  int itidx = find_index(node->item, data, node->size);
  for (int i = node->size; i > itidx; i--) {
    node->item[i] = node->item[i - 1];
    node->children[i + 1] = node->children[i];
  }
  node->item[itidx] = data;
  node->children[itidx + 1] = child;
  return node;
}
pair<int, int> *Index::item_insert(pair<int, int> *arr,
                                   const pair<int, int> &data, const int &len) {
  int idx = find_index(arr, data, len);
  for (int i = len; i > idx; i--) {
    arr[i] = arr[i - 1];
  }
  arr[idx] = data;
  return arr;
}
void Index::InsertPar(Node *par, Node *children, const pair<int, int> &data) {
  Node *now = par;
  if (now->size < this->degree - 1) {
    now = child_item_insert(now, data, children);
    now->size++;
  } else {
    Node *Newnode = new Node(this->degree);
    Newnode->parent = now->parent;

    pair<int, int> *new_item = new pair<int, int>[now->size + 1];
    for (int i = 0; i < now->size; i++)
      new_item[i] = now->item[i];
    new_item = item_insert(new_item, data, now->size);
    Node **new_child = new Node *[now->size + 2];
    for (int i = 0; i < now->size + 1; i++)
      new_child[i] = now->children[i];
    new_child[now->size + 1] = NULL;
    new_child = child_insert(new_child, children, now->size + 1,
                             find_index(new_item, data, now->size + 1));
    now->size = (this->degree) >> 1;
    if ((this->degree & 1) == 0)
      Newnode->size = (this->degree >> 1) - 1;
    else {
      Newnode->size = this->degree >> 1;
    }

    for (int i = 0; i < now->size; i++) {
      now->item[i] = new_item[i];
      now->children[i] = new_child[i];
    }
    now->children[now->size] = new_child[now->size];

    for (int i = 0; i < Newnode->size; i++) {
      Newnode->item[i] = new_item[now->size + i + 1];
      Newnode->children[i] = new_child[now->size + i + 1];
      Newnode->children[i]->parent = Newnode;
    }
    Newnode->children[Newnode->size] = new_child[now->size + Newnode->size + 1];
    Newnode->children[Newnode->size]->parent = Newnode;

    pair<int, int> paritem = new_item[this->degree >> 1];
    delete[] new_item;
    delete[] new_child;

    if (now->parent == NULL) {
      Node *new_father = new Node(this->degree);
      now->parent = new_father;
      Newnode->parent = new_father;
      new_father->item[0] = paritem;
      new_father->size++;
      new_father->children[0] = now;
      new_father->children[1] = Newnode;
      this->root = new_father;
    }

    else {
      InsertPar(now->parent, Newnode, paritem);
    }
  }
}
void Index::insert(const pair<int, int> &data) {
  if (this->root == NULL) {
    this->degree = DEGREE;
    this->root = new Node(this->degree);
    this->root->is_leaf = true;
    this->root->item[0] = data;
    this->root->size = 1;
  } else {
    Node *cursor = this->root;
    cursor = IndexTreeSearch(cursor, data);
    if ((cursor->size + 1) < cursor->degree) {
      cursor->item = item_insert(cursor->item, data, cursor->size);
      cursor->size++;
      cursor->children[cursor->size] = cursor->children[cursor->size - 1];
      cursor->children[cursor->size - 1] = NULL;
    } else {
      Node *Newnode = new Node(this->degree);
      Newnode->is_leaf = true;
      Newnode->parent = cursor->parent;
      pair<int, int> *new_item = new pair<int, int>[cursor->size + 1];
      for (int i = 0; i < cursor->size; i++)
        new_item[i] = cursor->item[i];
      new_item = item_insert(new_item, data, cursor->size);
      cursor->size = (this->degree) >> 1;
      if ((this->degree & 1) == 0)
        Newnode->size = this->degree >> 1;
      else {
        Newnode->size = (this->degree >> 1) + 1;
      }
      for (int i = 0; i < cursor->size; i++)
        cursor->item[i] = new_item[i];
      for (int i = 0; i < Newnode->size; i++)
        Newnode->item[i] = new_item[cursor->size + i];
      cursor->children[cursor->size] = Newnode;
      Newnode->children[Newnode->size] = cursor->children[this->degree - 1];
      cursor->children[this->degree - 1] = NULL;
      delete[] new_item;
      pair<int, int> paritem = Newnode->item[0];
      if (cursor->parent == NULL) {
        Node *Newparent = new Node(this->degree);
        cursor->parent = Newparent;
        Newnode->parent = Newparent;
        Newparent->item[0] = paritem;
        Newparent->size++;
        Newparent->children[0] = cursor;
        Newparent->children[1] = Newnode;
        this->root = Newparent;
      } else {
        InsertPar(cursor->parent, Newnode, paritem);
      }
    }
  }
}
int Index::range_search(const int &start, const int &end) {
  int index = 0;
  Node *temp = IndexTreeSearch(this->root, make_pair(start, INF));
  int k =
      lower_bound(temp->item, temp->item + temp->size, make_pair(start, -1)) -
      temp->item;
  int min_value = INF;
  pair<int, int> now = temp->item[k];
  if (k == temp->size) {
    k = 0;
    temp = temp->children[temp->size];
  }
  for (int i = k; i < temp->size; i++) {
    now = temp->item[i];
    if (now.first <= end)
      min_value = min(min_value, temp->item[i].second);
    else
      break;
  }
  while (now.first <= end) {
    temp = temp->children[temp->size];
    if (!temp)
      break;
    for (int i = 0; i < temp->size; i++) {
      now = temp->item[i];
      if (now.first <= end)
        min_value = min(min_value, now.second);
      else
        break;
    }
  }
  return (min_value != INF) ? min_value : -1;
}
void Index::range_query(const vector<pair<int, int>> &query_keys) { // ok
  ofstream outfile("range_query_out.txt");
  for (const pair<int, int> &key : query_keys) {
    outfile << range_search(key.first, key.second) << '\n';
  }
  outfile.close();
}
void Index::key_query(const vector<int> &query_keys) { // ok
  // cout<<"YES\n";
  ofstream outfile("key_query_out.txt");
  for (const int &key : query_keys) {
    Node *temp = IndexTreeSearch(this->root, make_pair(key, INF));
    int k =
        lower_bound(temp->item, temp->item + temp->size, make_pair(key, -1)) -
        temp->item;
    if (temp->item[k].first == key) {
      outfile << temp->item[k].second << "\n";
    } else
      outfile << -1 << "\n";
  }
  outfile.close();
}
void Index::clear(Node *cursor) {
  if (cursor != nullptr) {
    if (!cursor->is_leaf) {
      for (int i = 0; i <= cursor->size; i++) {
        clear(cursor->children[i]);
      }
    }
    delete[] cursor->item;
    delete[] cursor->children;
    delete cursor;
  }
}
void Index::clear_index() { // Destructor
  clear(this->root);
}
void Index::bpt_print() { print(this->root); }
void Index::print(Node *cursor) {
  // // You must NOT edit this function.
  // if (cursor != NULL) {
  //     for (int i = 0; i < cursor->size; ++i) {
  //         std::cout << "item:" << cursor->item[i] << " v: " << cursor->v[i]
  //         << " ";
  //     }
  //     std::cout << "\n";

  //     if (!cursor->is_leaf) {
  //         for (int i = 0; i < cursor->size + 1; ++i) {
  //             print(cursor->children[i]);
  //         }
  //     }
  // }
}