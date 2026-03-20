#include "tree_generator.h"

namespace tree_generator {

Comp::Comp(const Vertice& azimuth) : az(azimuth) {}

bool Comp::operator()(const Vertice& lhs, const Vertice& rhs) const {
    long long xd1 = 1ll * lhs.x - az.x, yd1 = 1ll * lhs.y - az.y,
              xd2 = 1LL * rhs.x - az.x, yd2 = 1ll * rhs.y - az.y;
    long long cp = (xd1 * yd2 - yd1 * xd2);
    long long sq_d1 = 1ll * xd1 * xd1 + 1ll * yd1 * yd1,
              sq_d2 = 1ll * xd2 * xd2 + 1LL * yd2 * yd2;
    return cp != 0 ? cp > 0 : sq_d1 < sq_d2;
}

int SelectNearest::Next(const std::vector<Vertice>& sorted, int first,
                        int last, const Vertice& azimuth) const {
    return FindNearestIdx(sorted, first, last, azimuth);
}

ZoneSelect::ZoneSelect(double zone_denom) : zone_denom_(zone_denom) {}

int ZoneSelect::Next(const std::vector<Vertice>& sorted, int first,
                     int last, const Vertice& azimuth) const {
    int sz = last - first, mid = first + (sz >> 1),
        left =
            mid - static_cast<int>(std::floor(sz / (zone_denom_ * 2.0))),
        right =
            left +
            static_cast<int>(std::floor(
                sz / zone_denom_));  // Find next node zone borders. If
                                     // zone_denom == 1 (default value),
                                     // then left == first, right == last.
    left =
        std::lower_bound(sorted.begin() + first, sorted.begin() + last + 1,
                         *(sorted.begin() + left), Comp(azimuth)) -
        sorted.begin();
    return FindNearestIdx(sorted, left, right, azimuth);
}

SelectRandom::SelectRandom() : g_(std::random_device{}()) {}

int SelectRandom::Next(const std::vector<Vertice>& sorted, int first,
                       int last, const Vertice& azimuth) const {
    std::uniform_int_distribution<int> distr(first, last);
    int candidate = distr(g_);
    int v =
        std::lower_bound(sorted.begin() + first, sorted.begin() + last + 1,
                         sorted[candidate], Comp(azimuth)) -
        sorted.begin();
    return v;
}

TreeGenerator::TreeGenerator(int n, int max_x, int max_y)
    : n_(n), max_x_(max_x), max_y_(max_y), v_(n) {
    GenVertices();
}

void TreeGenerator::PrintV() const {
    bool is_first = true;
    std::cout << '[';
    for (auto& v : v_) {
        if (is_first) {
            is_first = false;
        } else {
            std::cout << ',';
        }
        std::cout << '[' << v.x << ',' << v.y << ']';
    }
    std::cout << "]\n";
}

std::shared_ptr<MultNode> TreeGenerator::BuildAnySelectBin(
    std::shared_ptr<NextSelector> next_selector) {
    std::vector<Vertice> sorted = v_;
    int n = sorted.size();
    return DFS_AnySelect(sorted, 0, n - 1, start_azimuth_, next_selector);
}

std::shared_ptr<MultNode> TreeGenerator::BuildBinTree() {
    std::vector<Vertice> sorted = v_;
    std::sort(sorted.begin(), sorted.end());
    std::vector<int> parent(n_, -1), l_ch(n_, -1), r_ch(n_, -1);
    int last = 0, root = 0;
    for (int i = 1; i < n_; ++i) {
        last = i - 1;
        while (last != root && sorted[last].y <= sorted[i].y) {
            last = parent[last];
        }
        if (sorted[last].y <= sorted[i].y) {
            parent[root] = i;
            l_ch[i] = root;
            root = i;
        } else {
            if (r_ch[last] != -1) {
                parent[r_ch[last]] = i;
                l_ch[i] = r_ch[last];
            }
            r_ch[last] = i;
            parent[i] = last;
        }
    }
    return DFS(sorted, l_ch, r_ch, root);
}

void TreeGenerator::SetMinSpanAngleDeg(double deg) {
    min_span_cos_ = std::cos(ToRadians(deg));
    min_span_cos_sq_ = min_span_cos_ * min_span_cos_;
}

void TreeGenerator::GenVertices() {
    std::random_device r;
    std::mt19937 g(r());
    std::uniform_int_distribution<int> distr_x(0, max_x_),
        distr_y(0, max_y_);
    std::set<Vertice> seen;
    for (auto& vert : v_) {
        int x = distr_x(g), y = distr_y(g);
        while (seen.count({x, y}) != 0) {
            x = distr_x(g);
            y = distr_y(g);
        }
        seen.insert({x, y});
        vert.x = x;
        vert.y = y;
    }
}

int TreeGenerator::FindRootID(const std::vector<Vertice>& sorted) {
    Vertice lookup = v_[root_id_];
    for (int i = 0; i < n_; ++i) {
        if (sorted[i] == lookup) {
            return i;
        }
    }
    return -1;
}

std::shared_ptr<MultNode> TreeGenerator::DFS(
    const std::vector<Vertice>& sorted, const std::vector<int>& l_ch,
    const std::vector<int>& r_ch, int r) {
    if (r == -1) {
        return nullptr;
    }
    std::shared_ptr<MultNode> curr = std::make_shared<MultNode>(sorted[r]);
    curr->ch.push_back(
        DFS(sorted, l_ch, r_ch, l_ch[r]));  // left node first
    curr->ch.push_back(DFS(sorted, l_ch, r_ch, r_ch[r]));
    return curr;
}

std::shared_ptr<MultNode> TreeGenerator::DFS_AnySelect(
    std::vector<Vertice>& sorted, int first, int last,
    const Vertice& azimuth, std::shared_ptr<NextSelector> next_selector) {
    if (first > last) {
        return nullptr;
    }
    SortByAngle(sorted, first, last, azimuth);
    int v = azimuth == start_azimuth_
                ? FindRootID(sorted)
                : next_selector->Next(sorted, first, last, azimuth);
    Vertice vv = sorted[v], l_most = sorted[first];
    std::shared_ptr<MultNode> curr = std::make_shared<MultNode>(vv),
                              l_ch = nullptr, r_ch = nullptr;
    if (v - 1 - first >= 0 &&
        last - v - 1 >= 0) {  // check both subtrees not empty
        auto next_sort = sorted;
        SortByAngle(next_sort, first, v - 1, vv);
        SortByAngle(next_sort, v + 1, last, vv);
        Vertice
            a = sorted[next_selector->Next(next_sort, first, v - 1, vv)],
            b = sorted[next_selector->Next(next_sort, v + 1, last, vv)];
        if (!AXB_IsEqOrGreaterMinSpan(a, vv, b)) {
            curr->u = l_most;  // if span angle is less then min span
                               // angle, then don't split at this node, set
                               // l_ch leftmost point in current sector
            l_ch = DFS_AnySelect(sorted, first + 1, last, l_most,
                                 next_selector);
            curr->ch.push_back(l_ch);
            return curr;
        }
    }
    l_ch = DFS_AnySelect(sorted, first, v - 1, vv, next_selector);
    r_ch = DFS_AnySelect(sorted, v + 1, last, vv, next_selector);
    curr->ch.push_back(l_ch);
    curr->ch.push_back(r_ch);
    return curr;
}

void TreeGenerator::SortByAngle(std::vector<Vertice>& sorted, int first,
                                int last, const Vertice& r) {
    if (first > last) {
        return;
    }
    std::sort(sorted.begin() + first, sorted.begin() + last + 1, Comp(r));
}

double TreeGenerator::ToRadians(double deg) {
    return deg * DEG_TO_RAD;
}

bool TreeGenerator::AXB_IsEqOrGreaterMinSpan(const Vertice& a,
                                             const Vertice& x,
                                             const Vertice& b) {
    double XAxd = 1. * a.x - x.x, XAyd = 1. * a.y - x.y,
           XBxd = 1. * b.x - x.x, XByd = 1. * b.y - x.y;
    double dot = XAxd * XBxd + XAyd * XByd,
           XAsql = XAxd * XAxd + XAyd * XAyd,
           XBsql = XBxd * XBxd + XByd * XByd;
    if (min_span_cos_ + EPS > 0) {
        if (dot < -EPS) {
            return true;
        }
        return dot * dot <= min_span_cos_sq_ * XAsql * XBsql + EPS;
    }
    if (dot >= -EPS) {
        return false;
    }
    return dot * dot + EPS >= min_span_cos_sq_ * XAsql * XBsql;
}

int FindNearestIdx(const std::vector<Vertice>& sorted, int first, int last,
                   const Vertice& azimuth) {
    long long min_d = std::numeric_limits<long long>::max();
    int idx = -1;
    for (int i = first; i <= last; ++i) {
        if (sorted[i] == azimuth) {
            continue;
        }
        long long xd = sorted[i].x - azimuth.x,
                  yd = sorted[i].y - azimuth.y;
        long long sq_d = xd * xd + yd * yd;
        if (sq_d < min_d) {
            min_d = sq_d;
            idx = i;
        }
    }
    return idx;
}

}  // namespace tree_generator