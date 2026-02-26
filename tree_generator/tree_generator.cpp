#include "tree_generator.h"

TreeGenerator::TreeGenerator(int n, int max_x, int max_y)
    : n_(n), max_x_(max_x), max_y_(max_y) {}

void TreeGenerator::PrintV(const std::vector<Vertice>& v) const {
    bool is_first = true;
    std::cout << '[';
    for (auto& v : v) {
        if (is_first) {
            is_first = false;
        } else {
            std::cout << ',';
        }
        std::cout << '[' << v.x << ',' << v.y << ']';
    }
    std::cout << "]\n";
}

std::unique_ptr<TreeNode> TreeGenerator::GenerateRandom() {
    return BuildBinTree(GenVertices());
}

std::vector<Vertice> TreeGenerator::GenVertices() {
    std::random_device r;
    std::mt19937 g(r());
    std::uniform_int_distribution<int> distr_x(0, max_x_),
        distr_y(0, max_y_);
    std::vector<Vertice> v(n_);
    std::set<Vertice> seen;
    for (auto& vert : v) {
        int x = distr_x(g), y = distr_y(g);
        while (seen.find({x, y}) != seen.end()) {
            x = distr_x(g);
            y = distr_y(g);
        }
        seen.insert({x, y});
        vert.x = x;
        vert.y = y;
    }
    return v;
}

std::shared_ptr<MultNode> TreeGenerator::BuildBinFrom(
    const std::vector<Vertice>& v, int root) {
    std::vector<Vertice> sorted = v;
    int n = sorted.size();
    Vertice vroot = sorted[root];
    Vertice azimuth = -vroot;  // negative (must be out of border) coords
                               // for correct first partition!

    SortByAngle(sorted, 0, n - 1, azimuth);
    int root_idx = -1;
    for (int i = 0; i < n; ++i) {
        if (sorted[i] == vroot) {
            root_idx = i;
            break;
        }
    }
    std::shared_ptr<MultNode> root_node =
        std::make_shared<MultNode>(vroot);
    root_node->ch.push_back(DFS_From(sorted, 0, root_idx - 1, vroot));
    root_node->ch.push_back(DFS_From(sorted, root_idx + 1, n - 1, vroot));
    return root_node;
}

std::unique_ptr<TreeNode> TreeGenerator::BuildBinTree(
    const std::vector<Vertice>& v) {
    std::vector<Vertice> sorted = v;
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

std::unique_ptr<TernNode> TreeGenerator::BuildTernTree(
    const std::vector<Vertice>& v, int mid_size) {
    std::vector<Vertice> sorted = v;
    std::sort(sorted.begin(), sorted.end());
    std::vector<int> parent(n_, -1), l_ch(n_, -1), m_ch(n_, -1),
        r_ch(n_, -1), size(n_, 1);
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
            if (m_ch[last] == -1) {
                m_ch[last] = i;
            } else if (m_ch[last] != -1 && size[m_ch[last]] < mid_size) {
                parent[m_ch[last]] = i;  // отцепляем среднего сына last
                size[last] -=
                    size[m_ch[last]];  // уменьшаем размер last на размер
                                       // ее среднего сына
                l_ch[i] = m_ch[last];  // вешаем среднего сына last под i
                size[i] += size[m_ch[last]];  // увеличиваем размер i на
                                              // размер среднего сына last
                m_ch[last] = i;  // вешаем i средним сыном last
            } else if (r_ch[last] != -1) {
                parent[r_ch[last]] = i;  // отцепляем правого сына last
                size[last] -=
                    size[r_ch[last]];  // уменьшаем размер last на размер
                                       // ее правого сына
                l_ch[i] = r_ch[last];  // вешаем правого сына last под i
                size[i] += size[r_ch[last]];  // увеличиваем размер i на
                                              // размер правого сына last
                r_ch[last] = i;               // вешаем i правым сыном last
            } else {
                r_ch[last] = i;
            }
            size[last] += size[i];  // увеличиваем размер last на размер i
            parent[i] = last;
        }
    }
    return DFS_Tern(sorted, l_ch, m_ch, r_ch, root);
}

std::unique_ptr<TreeNode> TreeGenerator::DFS(
    const std::vector<Vertice>& sorted, const std::vector<int>& l_ch,
    const std::vector<int>& r_ch, int r) {
    if (r == -1) {
        return nullptr;
    }
    std::unique_ptr<TreeNode> curr =
        std::make_unique<TreeNode>(sorted[r], nullptr, nullptr);
    curr->left = DFS(sorted, l_ch, r_ch, l_ch[r]);
    curr->right = DFS(sorted, l_ch, r_ch, r_ch[r]);
    return curr;
}

std::unique_ptr<TernNode> TreeGenerator::DFS_Tern(
    const std::vector<Vertice>& sorted, const std::vector<int>& l_ch,
    const std::vector<int>& m_ch, const std::vector<int>& r_ch, int r) {
    if (r == -1) {
        return nullptr;
    }
    std::unique_ptr<TernNode> curr =
        std::make_unique<TernNode>(sorted[r], nullptr, nullptr, nullptr);
    curr->left = DFS_Tern(sorted, l_ch, m_ch, r_ch, l_ch[r]);
    curr->mid = DFS_Tern(sorted, l_ch, m_ch, r_ch, m_ch[r]);
    curr->right = DFS_Tern(sorted, l_ch, m_ch, r_ch, r_ch[r]);
    return curr;
}

std::shared_ptr<MultNode> TreeGenerator::DFS_From(
    std::vector<Vertice>& sorted, int first, int last, const Vertice& r) {
    if (first > last) {
        return nullptr;
    }
    SortByAngle(sorted, first, last, r);
    int v = FindNearestIdx(sorted, first, last, r);
    Vertice vv = sorted[v];
    std::shared_ptr<MultNode> curr = std::make_shared<MultNode>(vv);
    curr->ch.push_back(DFS_From(sorted, first, v - 1, vv));
    curr->ch.push_back(DFS_From(sorted, v + 1, last, vv));
    return curr;
}

void TreeGenerator::SortByAngle(std::vector<Vertice>& sorted, int first,
                                int last, const Vertice& r) {
    auto comp = [this, &r](const Vertice& lhs,
                           const Vertice& rhs) -> bool {
        long long xd1 = 1ll * lhs.x - r.x, yd1 = 1ll * lhs.y - r.y,
                  xd2 = 1LL * rhs.x - r.x, yd2 = 1ll * rhs.y - r.y;
        long long cp = (xd1 * yd2 - yd1 * xd2);
        long long sq_d1 = 1ll * xd1 * xd1 + 1ll * yd1 * yd1,
                  sq_d2 = 1ll * xd2 * xd2 + 1LL * yd2 * yd2;
        return cp != 0 ? cp > 0 : sq_d1 < sq_d2;
    };
    std::sort(sorted.begin() + first, sorted.begin() + last + 1, comp);
}

int TreeGenerator::FindNearestIdx(const std::vector<Vertice>& sorted,
                                  int first, int last, const Vertice& r) {
    long long min_d = std::numeric_limits<long long>::max();
    int idx = -1;
    for (int i = first; i <= last; ++i) {
        if (sorted[i] == r) {
            continue;
        }
        long long xd = sorted[i].x - r.x, yd = sorted[i].y - r.y;
        long long sq_d = xd * xd + yd * yd;
        if (sq_d < min_d) {
            min_d = sq_d;
            idx = i;
        }
    }
    return idx;
}