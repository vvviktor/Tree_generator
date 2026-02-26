#pragma once

#include <algorithm>
#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <set>
#include <vector>

#include "dom.h"

class TreeGenerator {
   public:
    TreeGenerator(int n, int max_x, int max_y);

    void PrintV(const std::vector<Vertice>& v) const;
    std::vector<Vertice> GenVertices();
    std::shared_ptr<MultNode> BuildBinFrom(const std::vector<Vertice>& v,
                                           int root);
    std::shared_ptr<MultNode> BuildBinTree(const std::vector<Vertice>& v);

   private:
    int n_, max_x_, max_y_;

    std::shared_ptr<MultNode> DFS_From(std::vector<Vertice>& sorted,
                                       int first, int last,
                                       const Vertice& r);
    std::shared_ptr<MultNode> DFS(const std::vector<Vertice>& sorted,
                                  const std::vector<int>& l_ch,
                                  const std::vector<int>& r_ch, int r);
    void SortByAngle(std::vector<Vertice>& sorted, int first, int last,
                     const Vertice& r);
    int FindNearestIdx(const std::vector<Vertice>& sorted, int first,
                       int last, const Vertice& r);
};