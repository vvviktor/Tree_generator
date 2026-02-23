#include <fstream>
#include <iostream>

#include "dom.h"
#include "svg.h"
#include "tree_generator.h"
#include "tree_renderer.h"

int main() {
    std::ofstream bin_output("bin_tree.svg");
    std::ofstream tern_output("tern_tree.svg");
    TreeGenerator tree_gen(60, 1500, 1100);
    RenderSettings settings;
    settings.SetMaxX(1500).SetMaxY(1100);
    auto v = tree_gen.GenVertices();
    auto bin_t = tree_gen.BuildBinTree(v);
    auto tern_t = tree_gen.BuildTernTree(v);
    TreeRenderer tree_rend(settings);
    tree_rend.Render_Bin(bin_t).Render(bin_output);
    tree_rend.Render_Tern(tern_t).Render(tern_output);
}