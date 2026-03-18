#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <numbers>
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
    void SetMinSpanAngleDeg(double deg);

   private:
    int n_, max_x_, max_y_;
    static constexpr double EPS = 1e-9, PI = 3.14159265358979323846,
                            DEG_TO_RAD = PI / 180.;
    double min_span_cos_ = 1., min_span_cos_sq_ = 1.;

    std::shared_ptr<MultNode> DFS_AnySelect(
        std::vector<Vertice>& sorted, int first, int last,
        const Vertice& r, std::shared_ptr<NextSelector> next_selector);
    std::shared_ptr<MultNode> DFS(const std::vector<Vertice>& sorted,
                                  const std::vector<int>& l_ch,
                                  const std::vector<int>& r_ch, int r);
    static void SortByAngle(std::vector<Vertice>& sorted, int first,
                            int last, const Vertice& r);
    static double ToRadians(double deg);
    bool AXB_IsEqOrGreaterMinSpan(const Vertice& a, const Vertice& x,
                                  const Vertice& b);
};

int FindNearestIdx(const std::vector<Vertice>& sorted, int first, int last,
                   const Vertice& r);

}  // namespace tree_generator