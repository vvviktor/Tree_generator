#pragma once

#include <memory>

#include "dom.h"
#include "svg.h"

class TreeRenderer {
   public:
    TreeRenderer() = default;
    TreeRenderer(const RenderSettings& render_settings);

    svg::Document ToSvg(const std::unique_ptr<TreeNode>& root);

   private:
    RenderSettings render_settings_;

    void DFS_Render(svg::Document& doc,
                    const std::unique_ptr<TreeNode>& root,
                    const std::unique_ptr<TreeNode>& parent);
};
