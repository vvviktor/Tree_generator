#include <fstream>
#include <iostream>

#include "dom.h"
#include "svg.h"
#include "tree_generator.h"
#include "tree_renderer.h"

int main() {
    std::ofstream a_bin_output("animated_bin_tree.svg");
    std::ofstream a_output("animated_tree.svg");
    std::ofstream bin_output("bin_tree.svg");
    std::ofstream omni_output("omni_tree.svg");
    TreeGenerator tree_gen(500, 1500, 1100);
    RenderSettings settings;
    settings.SetMaxX(1500).SetMaxY(1100);
    auto v = tree_gen.GenVertices();
    auto arbitary_bin = tree_gen.BuildBinFrom(v, 60);
    auto bin_t = tree_gen.BuildBinTree(v);
    TreeRenderer tree_rend(settings);
    tree_rend.RenderOmni(bin_t).Render(bin_output);
    tree_rend.RenderOmni(arbitary_bin).Render(omni_output);
    tree_rend.RenderAnimatedBFS(bin_t).Render(a_bin_output);
    tree_rend.RenderAnimatedBFS(arbitary_bin).Render(a_output);
}