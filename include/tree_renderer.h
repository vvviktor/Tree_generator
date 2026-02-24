#pragma once

#include <memory>

#include "dom.h"
#include "svg.h"

class TreeRenderer {
   public:
    TreeRenderer() = default;
    TreeRenderer(const RenderSettings& render_settings);

    svg::Document Render_Bin(const std::unique_ptr<TreeNode>& root);
    svg::Document Render_Tern(const std::unique_ptr<TernNode>& root);
    svg::Document RenderOmni(const std::unique_ptr<MultNode>& root);

   private:
    RenderSettings render_settings_;

    void DFS_Omni(svg::Document& doc,
                  const std::unique_ptr<MultNode>& root,
                  const std::unique_ptr<MultNode>& parent);
    void DFS_Bin_Render(svg::Document& doc,
                        const std::unique_ptr<TreeNode>& root,
                        const std::unique_ptr<TreeNode>& parent);
    void DFS_Tern_Render(svg::Document& doc,
                         const std::unique_ptr<TernNode>& root,
                         const std::unique_ptr<TernNode>& parent);

    void ApplyDocSize(svg::Document& doc);
};
