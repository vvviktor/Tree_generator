#include "tree_renderer.h"

#include <queue>

TreeRenderer::TreeRenderer(const RenderSettings& render_settings)
    : render_settings_(render_settings) {}

svg::Document TreeRenderer::Render_Bin(
    const std::unique_ptr<TreeNode>& root) {
    svg::Document doc;
    ApplyDocBackgroundProps(doc);
    DFS_Bin_Render(doc, root, nullptr);
    return doc;
}

svg::Document TreeRenderer::Render_Tern(
    const std::unique_ptr<TernNode>& root) {
    svg::Document doc;
    ApplyDocBackgroundProps(doc);
    DFS_Tern_Render(doc, root, nullptr);
    return doc;
}

svg::Document TreeRenderer::RenderOmni(
    const std::shared_ptr<MultNode>& root) {
    svg::Document doc;
    ApplyDocBackgroundProps(doc);
    DFS_Omni(doc, root, nullptr);
    return doc;
}

svg::Document TreeRenderer::RenderAnimatedBFS(
    std::shared_ptr<MultNode> root) {
    svg::Document doc;
    ApplyDocBackgroundProps(doc);
    double time = 0.;
    std::queue<std::shared_ptr<MultNode>> q;
    q.push(root);
    while (!q.empty()) {
        int layer = q.size();
        while (layer--) {
            std::shared_ptr<MultNode> curr = q.front();
            svg::Point curr_node(curr->u.x + render_settings_.padding,
                                 render_settings_.max_y - curr->u.y +
                                     render_settings_.padding);
            for (std::shared_ptr<MultNode> v : curr->ch) {
                if (!v) {
                    continue;
                }
                svg::Point child_node(v->u.x + render_settings_.padding,
                                      render_settings_.max_y - v->u.y +
                                          render_settings_.padding);
                svg::AnimatedLine edge;
                edge.SetA(curr_node)
                    .SetB(child_node)
                    .SetStrokeWidth(render_settings_.edge_width)
                    .SetStrokeColor(render_settings_.edge_color)
                    .SetStrokeLineCap(render_settings_.line_cap)
                    .SetDur(render_settings_.edge_drawing_dur)
                    .SetBegin(time)
                    .SetFill(svg::AnimationFill::FREEZE);
                doc.Add(edge);
                q.push(std::move(v));
                svg::Circle node;
                node.SetCenter(child_node)
                    .SetRadius(render_settings_.node_radius)
                    .SetFillColor(render_settings_.node_color);
                doc.Add(node);
            }
            q.pop();
        }
        time += render_settings_.edge_drawing_dur + 0.1;
    }
    svg::Point root_node(
        root->u.x + render_settings_.padding,
        render_settings_.max_y - root->u.y + render_settings_.padding);
    svg::Circle node;
    node.SetCenter(root_node)
        .SetRadius(render_settings_.node_radius)
        .SetFillColor(svg::Rgb(255, 0, 0));
    doc.Add(node);
    return doc;
}

void TreeRenderer::DFS_Omni(svg::Document& doc,
                            const std::shared_ptr<MultNode>& root,
                            const std::shared_ptr<MultNode>& parent) {
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
        svg::Line edge;
        edge.SetB(this_node)
            .SetA(parent_node)
            .SetStrokeColor(render_settings_.edge_color)
            .SetStrokeWidth(render_settings_.edge_width)
            .SetStrokeLineCap(render_settings_.line_cap);
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

void TreeRenderer::ApplyDocBackgroundProps(svg::Document& doc) {
    doc.SetWidth(render_settings_.max_x + render_settings_.padding * 2)
        .SetHeight(render_settings_.max_y + render_settings_.padding * 2)
        .SetBackgroundColor(render_settings_.background_color);
}