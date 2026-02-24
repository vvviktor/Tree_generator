#pragma once

#include <memory>
#include <tuple>

#include "svg.h"

struct Vertice {
    int x = 0, y = 0, i = 0;

    constexpr bool operator<(const Vertice& rhs) const {
        return std::tie(x, y) < std::tie(rhs.x, rhs.y);
    }

    constexpr bool operator==(const Vertice& rhs) const {
        return std::tie(x, y) == std::tie(rhs.x, rhs.y);
    }
};

struct MultNode {
    MultNode(const Vertice& vert);

    Vertice u;
    std::vector<std::unique_ptr<MultNode>> ch;
};

struct TreeNode {
    TreeNode(const Vertice& vert, std::unique_ptr<TreeNode> l,
             std::unique_ptr<TreeNode> r);
    Vertice u;
    std::unique_ptr<TreeNode> left = nullptr, right = nullptr;
};

struct TernNode {
    TernNode(const Vertice& vert, std::unique_ptr<TernNode> l,
             std::unique_ptr<TernNode> m, std::unique_ptr<TernNode> r);
    Vertice u;
    std::unique_ptr<TernNode> left = nullptr, mid = nullptr,
                              right = nullptr;
};

struct RenderSettings {
    RenderSettings& SetMaxX(double x);
    RenderSettings& SetMaxY(double y);
    RenderSettings& SetNodeColor(svg::Color color);
    RenderSettings& SetEdgeColor(svg::Color color);
    RenderSettings& SetOutlineColor(svg::Color color);
    RenderSettings& SetNodeRadius(double r);
    RenderSettings& SetEdgeWidth(double w);
    RenderSettings& SetLineCap(svg::StrokeLineCap l_c);
    RenderSettings& SetLineJoin(svg::StrokeLineJoin l_j);

    double max_x = .0, max_y = .0;
    svg::Color node_color = svg::Rgb(255, 128, 0),
               edge_color = svg::Rgb(0, 153, 0),
               outline_color = svg::Rgb(96, 96, 96);
    double node_radius = 8., edge_width = 4.;
    svg::StrokeLineCap line_cap = svg::StrokeLineCap::ROUND;
    svg::StrokeLineJoin line_join = svg::StrokeLineJoin::ROUND;
    double padding = 30.;
};
