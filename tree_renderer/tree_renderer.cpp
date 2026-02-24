#include "tree_renderer.h"

TreeRenderer::TreeRenderer(const RenderSettings& render_settings)
    : render_settings_(render_settings) {}

svg::Document TreeRenderer::Render_Bin(
    const std::unique_ptr<TreeNode>& root) {
    svg::Document doc;
    DFS_Bin_Render(doc, root, nullptr);
    return doc;
}

svg::Document TreeRenderer::Render_Tern(
    const std::unique_ptr<TernNode>& root) {
    svg::Document doc;
    DFS_Tern_Render(doc, root, nullptr);
    return doc;
}

svg::Document TreeRenderer::RenderOmni(
    const std::unique_ptr<MultNode>& root) {
    svg::Document doc;
    DFS_Omni(doc, root, nullptr);
    return doc;
}

void TreeRenderer::DFS_Omni(svg::Document& doc,
                            const std::unique_ptr<MultNode>& root,
                            const std::unique_ptr<MultNode>& parent) {
    if (!root) {
        return;
    }
    for (const auto& ch : root->ch) {
        DFS_Omni(doc, ch, root);
    }
    svg::Point this_node(
        root->u.x + render_settings_.padding,
        render_settings_.max_y - root->u.y + render_settings_.padding);
    svg::Circle node;
    node.SetCenter(this_node).SetRadius(render_settings_.node_radius);
    if (parent) {
        svg::Point parent_node(parent->u.x + render_settings_.padding,
                               render_settings_.max_y - parent->u.y +
                                   render_settings_.padding);
        svg::Polyline edge;
        edge.AddPoint(this_node)
            .AddPoint(parent_node)
            .SetStrokeColor(render_settings_.edge_color)
            .SetStrokeWidth(render_settings_.edge_width)
            .SetStrokeLineCap(render_settings_.line_cap)
            .SetStrokeLineJoin(render_settings_.line_join);
        doc.Add(edge);
        node.SetFillColor(render_settings_.node_color);
    } else {
        node.SetFillColor(svg::Rgb(255, 0, 0));
    }
    doc.Add(node);
}

void TreeRenderer::DFS_Bin_Render(
    svg::Document& doc, const std::unique_ptr<TreeNode>& root,
    const std::unique_ptr<TreeNode>& parent) {
    if (!root) {
        return;
    }
    DFS_Bin_Render(doc, root->left, root);
    DFS_Bin_Render(doc, root->right, root);
    svg::Point this_node(
        root->u.x + render_settings_.padding,
        render_settings_.max_y - root->u.y + render_settings_.padding);
    if (parent) {
        svg::Point parent_node(parent->u.x + render_settings_.padding,
                               render_settings_.max_y - parent->u.y +
                                   render_settings_.padding);
        svg::Polyline edge;
        edge.AddPoint(this_node)
            .AddPoint(parent_node)
            .SetStrokeColor(render_settings_.edge_color)
            .SetStrokeWidth(render_settings_.edge_width)
            .SetStrokeLineCap(render_settings_.line_cap)
            .SetStrokeLineJoin(render_settings_.line_join);
        doc.Add(edge);
    }
    svg::Circle node;
    node.SetCenter(this_node)
        .SetRadius(render_settings_.node_radius)
        .SetFillColor(render_settings_.node_color);
    doc.Add(node);
}

void TreeRenderer::DFS_Tern_Render(
    svg::Document& doc, const std::unique_ptr<TernNode>& root,
    const std::unique_ptr<TernNode>& parent) {
    if (!root) {
        return;
    }
    DFS_Tern_Render(doc, root->left, root);
    DFS_Tern_Render(doc, root->mid, root);
    DFS_Tern_Render(doc, root->right, root);
    svg::Point this_node(
        root->u.x + render_settings_.padding,
        render_settings_.max_y - root->u.y + render_settings_.padding);
    if (parent) {
        svg::Point parent_node(parent->u.x + render_settings_.padding,
                               render_settings_.max_y - parent->u.y +
                                   render_settings_.padding);
        svg::Polyline edge;
        edge.AddPoint(this_node)
            .AddPoint(parent_node)
            .SetStrokeColor(render_settings_.edge_color)
            .SetStrokeWidth(render_settings_.edge_width)
            .SetStrokeLineCap(render_settings_.line_cap)
            .SetStrokeLineJoin(render_settings_.line_join);
        doc.Add(edge);
    }
    svg::Circle node;
    node.SetCenter(this_node)
        .SetRadius(render_settings_.node_radius)
        .SetFillColor(render_settings_.node_color);
    doc.Add(node);
}