#include "tree_renderer.h"

#include <queue>

TreeRenderer::TreeRenderer(const RenderSettings& render_settings)
    : render_settings_(render_settings) {}

svg::Document TreeRenderer::RenderOmni(std::shared_ptr<MultNode> root) {
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
        time += render_settings_.edge_drawing_dur +
                render_settings_.edge_drawing_interval;
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
                            std::shared_ptr<MultNode> root,
                            std::shared_ptr<MultNode> parent) {
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

void TreeRenderer::ApplyDocBackgroundProps(svg::Document& doc) {
    doc.SetWidth(render_settings_.max_x + render_settings_.padding * 2)
        .SetHeight(render_settings_.max_y + render_settings_.padding * 2)
        .SetBackgroundColor(render_settings_.background_color);
}