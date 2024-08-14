#ifndef IndexTree_H
#define IndexTree_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#pragma GCC optimize("O3")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,tune=native")

using namespace std;

struct Node {
  bool is_leaf;
  int degree;
  int size;
  pair<int, int> *item;
  Node **children;
  Node *parent;

  Node(int _degree);
};

class Index {
  Node *root;
  int degree;

public:
  Index(const int &num_rows, const vector<int> &key, const vector<int> &value);
  Node *getroot();
  int find_index(pair<int, int> *arr, const pair<int, int> &data,
                 const int &len);
  Node *IndexTreeSearch(Node *node, const pair<int, int> &key);
  Node **child_insert(Node **child_item, Node *child, const int &len, int idx);
  Node *child_item_insert(Node *node, const pair<int, int> &data, Node *child);
  pair<int, int> *item_insert(pair<int, int> *arr, const pair<int, int> &data,
                              const int &len);
  void InsertPar(Node *far, Node *children, const pair<int, int> &data);
  void insert(const pair<int, int> &data);
  int range_search(const int &start, const int &end);
  void range_query(const vector<pair<int, int>> &query_keys);
  void key_query(const vector<int> &query_keys);
  void clear(Node *cursor);
  void clear_index();
  void bpt_print();
  void print(Node *cursor);
};

#endif