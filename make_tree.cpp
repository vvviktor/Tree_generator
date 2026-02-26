#include <fstream>
#include <iostream>

#include "dom.h"
#include "svg.h"
#include "tree_generator.h"
#include "tree_renderer.h"

int main() {
    /* std::ofstream bin_output("bin_tree.svg");
    std::ofstream tern_output("tern_tree.svg");
    std::ofstream tern_output_ext("tern_tree_ext.svg"); */
    std::ofstream omni_output("omni_tree.svg");
    TreeGenerator tree_gen(140, 1500, 1100);
    RenderSettings settings;
    settings.SetMaxX(1500).SetMaxY(1100);
    auto v = tree_gen.GenVertices();
    auto arbitary_bin = tree_gen.BuildBinFrom(v, 60);
    /* auto bin_t = tree_gen.BuildBinTree(v);
    auto tern_t = tree_gen.BuildTernTree(v, 1);
    auto tern_t_ext = tree_gen.BuildTernTree(v, 2); */
    TreeRenderer tree_rend(settings);
    tree_rend.RenderOmni(arbitary_bin).Render(omni_output);
    /* tree_rend.Render_Bin(bin_t).Render(bin_output);
    tree_rend.Render_Tern(tern_t).Render(tern_output);
    tree_rend.Render_Tern(tern_t_ext).Render(tern_output_ext); */

    /* svg::Point a(50, 50), b(150, 150);
    svg::Document doc;
    svg::AnimatedLine line;
    line.SetA(a).SetB(b).SetStrokeColor(svg::Rgb(0,255,0)).SetStrokeWidth(3).SetDur() */
}