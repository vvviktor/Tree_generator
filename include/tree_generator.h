#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <random>
#include <set>
#include <vector>
#include <limits>

#include "dom.h"

class TreeGenerator {
   public:
    explicit TreeGenerator(int n, int max_x, int max_y);

    void PrintV(const std::vector<Vertice>& v) const;
    void PrintDFS_I(std::unique_ptr<TreeNode>& root) const;
    void PrintDFS_P(std::unique_ptr<TreeNode>& root) const;
    std::unique_ptr<TreeNode> GenerateRandom();
    std::vector<Vertice> GenVertices();
    std::unique_ptr<MultNode> BuildBinFrom(const std::vector<Vertice>& v, int root);
    std::unique_ptr<TreeNode> BuildBinTree(const std::vector<Vertice>& v);
    std::unique_ptr<TernNode> BuildTernTree(const std::vector<Vertice>& v, int mid_size);

   private:
    int n_, max_x_, max_y_;
    
    std::unique_ptr<MultNode> DFS_From(std::vector<Vertice>& sorted, int first, int last, int o_x, int o_y);
    std::unique_ptr<TreeNode> DFS(const std::vector<Vertice>& sorted,
                                  const std::vector<int>& l_ch,
                                  const std::vector<int>& r_ch, int r);
    std::unique_ptr<TernNode> DFS_Tern(const std::vector<Vertice>& sorted,
                                       const std::vector<int>& l_ch,
                                       const std::vector<int>& m_ch,
                                       const std::vector<int>& r_ch,
                                       int r);

      void SortByAngle(std::vector<Vertice>& sorted, int first, int last, int o_x, int o_y);
      int GetQuadrant(int x, int y);
};