#include <fstream>
#include <iostream>

#include "dom.h"
#include "svg.h"
#include "tree_generator.h"
#include "tree_renderer.h"

int main() {
    std::ofstream a_full_zone_output("a_short_route_tree.svg");
    std::ofstream a_narrow_zone_output("a_narrow_zone_tree.svg");
    std::ofstream a_random_tree_output("a_random_tree.svg");
    tree_generator::TreeGenerator tree_gen(50, 1500, 1100);
    tree_gen.SetMinSpanAngleDeg(30);
    RenderSettings settings;
    settings.SetMaxX(1500).SetMaxY(1100);
    auto v = tree_gen.GenVertices();
    auto nearest_selector =
        std::make_shared<tree_generator::SelectNearest>();
    auto random_selector =
        std::make_shared<tree_generator::SelectRandom>();
    auto zoned_selector =
        std::make_shared<tree_generator::ZoneSelect>(1.01);
    auto greedy_short_route_bin =
        tree_gen.BuildAnySelectBin(v, 6, nearest_selector);
    auto narrow_zone_bin =
        tree_gen.BuildAnySelectBin(v, 6, zoned_selector);
    auto random_bin_tree =
        tree_gen.BuildAnySelectBin(v, 6, random_selector);
    TreeRenderer tree_rend(settings);
    tree_rend.RenderAnimatedBFS(greedy_short_route_bin)
        .Render(a_full_zone_output);
    tree_rend.RenderAnimatedBFS(narrow_zone_bin)
        .Render(a_narrow_zone_output);
    tree_rend.RenderAnimatedBFS(random_bin_tree)
        .Render(a_random_tree_output);
}