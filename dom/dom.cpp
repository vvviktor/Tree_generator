#include "dom.h"

TreeNode::TreeNode(const Vertice& vert, std::unique_ptr<TreeNode> l,
                   std::unique_ptr<TreeNode> r)
    : u(vert), left(std::move(l)), right(std::move(r)) {}

TernNode::TernNode(const Vertice& vert, std::unique_ptr<TernNode> l,
                   std::unique_ptr<TernNode> m,
                   std::unique_ptr<TernNode> r)
    : u(vert),
      left(std::move(l)),
      mid(std::move(m)),
      right(std::move(r)) {}

RenderSettings& RenderSettings::SetMaxX(double x) {
    max_x = x;
    return *this;
}

RenderSettings& RenderSettings::SetMaxY(double y) {
    max_y = y;
    return *this;
}

RenderSettings& RenderSettings::SetNodeColor(svg::Color color) {
    node_color = color;
    return *this;
}

RenderSettings& RenderSettings::SetEdgeColor(svg::Color color) {
    edge_color = color;
    return *this;
}

RenderSettings& RenderSettings::SetOutlineColor(svg::Color color) {
    outline_color = color;
    return *this;
}

RenderSettings& RenderSettings::SetNodeRadius(double r) {
    node_radius = r;
    return *this;
}

RenderSettings& RenderSettings::SetEdgeWidth(double w) {
    edge_width = w;
    return *this;
}

RenderSettings& RenderSettings::SetLineCap(svg::StrokeLineCap l_c) {
    line_cap = l_c;
    return *this;
}

RenderSettings& RenderSettings::SetLineJoin(svg::StrokeLineJoin l_j) {
    line_join = l_j;
    return *this;
}