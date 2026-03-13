#include <fstream>
#include <iostream>

#include "dom.h"
#include "svg.h"
#include "tree_generator.h"
#include "tree_renderer.h"

int main() {
    std::ofstream a_full_zone_output("a_full_zone.svg");
    std::ofstream a_narrow_zone_output("a_narrow_zone.svg");
    std::ofstream a_random_tree_output("a_random_tree.svg");
    TreeGenerator tree_gen(500, 1500, 1100);
    RenderSettings settings;
    settings.SetMaxX(1500).SetMaxY(1100);
    auto v = tree_gen.GenVertices();
    auto full_zone_bin = tree_gen.BuildZonedBin(v, 60);
    auto narrow_zone_bin = tree_gen.BuildZonedBin(v, 60, 5);
    auto random_tree = tree_gen.BuildRandomBin(v, 60);
    TreeRenderer tree_rend(settings);
    tree_rend.RenderAnimatedBFS(full_zone_bin).Render(a_full_zone_output);
    tree_rend.RenderAnimatedBFS(narrow_zone_bin)
        .Render(a_narrow_zone_output);
    tree_rend.RenderAnimatedBFS(random_tree).Render(a_random_tree_output);
}