#pragma once

#include <memory>

#include "dom.h"
#include "svg.h"

class TreeRenderer {
   public:
    TreeRenderer() = default;
    TreeRenderer(const RenderSettings& render_settings);

    svg::Document RenderOmni(std::shared_ptr<MultNode> root);
    svg::Document RenderAnimatedBFS(std::shared_ptr<MultNode> root);

   private:
    RenderSettings render_settings_;

    void DFS_Omni(svg::Document& doc, std::shared_ptr<MultNode> root,
                  std::shared_ptr<MultNode> parent);
    void ApplyDocBackgroundProps(svg::Document& doc);
};
