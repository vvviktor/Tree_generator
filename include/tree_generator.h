#pragma once

#include <algorithm>
#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <set>
#include <vector>

#include "dom.h"

namespace tree_generator {

struct Comp {
    Comp(const Vertice& ancor);
    const Vertice& r;
    bool operator()(const Vertice& lhs, const Vertice& rhs) const;
};

class NextSelector {
   public:
    virtual int Next(const std::vector<Vertice>& sorted, int first,
                     int last, const Vertice& r) const = 0;

    virtual ~NextSelector() = default;
};

class SelectNearest final : public NextSelector {
   public:
    SelectNearest() = default;

    int Next(const std::vector<Vertice>& sorted, int first, int last,
             const Vertice& r) const override;
};

class ZoneSelect final : public NextSelector {
   public:
    ZoneSelect(double zone_denom);

    int Next(const std::vector<Vertice>& sorted, int first, int last,
             const Vertice& r) const override;

   private:
    double zone_denom_ = 1;
};

class SelectRandom final : public NextSelector {
   public:
    SelectRandom();

    int Next(const std::vector<Vertice>& sorted, int first, int last,
             const Vertice& r) const override;

   private:
    mutable std::mt19937 g_;
};

class TreeGenerator {
   public:
    TreeGenerator(int n, int max_x, int max_y);

    void PrintV(const std::vector<Vertice>& v) const;
    std::vector<Vertice> GenVertices();
    std::shared_ptr<MultNode> BuildAnySelectBin(
        const std::vector<Vertice>& v, int root,
        std::shared_ptr<NextSelector> next_selector);
    std::shared_ptr<MultNode> BuildBinTree(const std::vector<Vertice>& v);

   private:
    int n_, max_x_, max_y_;

    std::shared_ptr<MultNode> DFS_AnySelect(
        std::vector<Vertice>& sorted, int first, int last,
        const Vertice& r, std::shared_ptr<NextSelector> next_selector);
    std::shared_ptr<MultNode> DFS(const std::vector<Vertice>& sorted,
                                  const std::vector<int>& l_ch,
                                  const std::vector<int>& r_ch, int r);
    void SortByAngle(std::vector<Vertice>& sorted, int first, int last,
                     const Vertice& r);
};

int FindNearestIdx(const std::vector<Vertice>& sorted, int first, int last,
                   const Vertice& r);

}  // namespace tree_generator