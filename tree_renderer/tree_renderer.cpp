#include "tree_renderer.h"

TreeRenderer::TreeRenderer(const RenderSettings& render_settings)
    : render_settings_(render_settings) {}

svg::Document TreeRenderer::ToSvg(const std::unique_ptr<TreeNode>& root) {
    svg::Document doc;
    DFS_Render(doc, root, nullptr);
    return doc;
}

void TreeRenderer::DFS_Render(svg::Document& doc,
                              const std::unique_ptr<TreeNode>& root,
                              const std::unique_ptr<TreeNode>& parent) {
    if (!root) {
        return;
    }
    svg::Point this_node(root->u.x, render_settings_.max_y - root->u.y +
                                        render_settings_.padding);
    svg::Circle node;
    node.SetCenter(this_node)
        .SetRadius(render_settings_.node_radius)
        .SetFillColor(render_settings_.node_color);
    doc.Add(node);
    if (parent) {
        svg::Point parent_node(parent->u.x, render_settings_.max_y -
                                                parent->u.y +
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
    DFS_Render(doc, root->left, root);
    DFS_Render(doc, root->right, root);
}