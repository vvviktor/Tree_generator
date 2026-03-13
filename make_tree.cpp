#include <fstream>
#include <iostream>

#include "dom.h"
#include "svg.h"
#include "tree_generator.h"
#include "tree_renderer.h"

int main() {
    std::ofstream a_bin_output("animated_bin_tree.svg");
    std::ofstream a_full_zone_output("a_full_zone.svg");
    std::ofstream a_narrow_zone_output("a_narrow_zone.svg");
    std::ofstream bin_output("bin_tree.svg");
    std::ofstream omni_output("omni_tree.svg");
    TreeGenerator tree_gen(500, 1500, 1100);
    RenderSettings settings;
    settings.SetMaxX(1500).SetMaxY(1100);
    auto v = tree_gen.GenVertices();
    auto full_zone_bin = tree_gen.BuildZonedBin(v, 60);
    auto narrow_zone_bin = tree_gen.BuildZonedBin(v, 60, 5);
    auto bin_t = tree_gen.BuildBinTree(v);
    TreeRenderer tree_rend(settings);
    tree_rend.RenderOmni(bin_t).Render(bin_output);
    tree_rend.RenderAnimatedBFS(bin_t).Render(a_bin_output);
    tree_rend.RenderAnimatedBFS(full_zone_bin).Render(a_full_zone_output);
    tree_rend.RenderAnimatedBFS(narrow_zone_bin)
        .Render(a_narrow_zone_output);
}