#include <fstream>
#include <iostream>

#include "dom.h"
#include "svg.h"
#include "tree_generator.h"
#include "tree_renderer.h"

int main() {
    std::ofstream a_restricted_output("a_restricted_tree.svg");
    std::ofstream a_free_output("a_free_tree.svg");
    tree_generator::TreeGenerator tree_gen(1500, 1600, 1200);
    RenderSettings settings;
    settings.SetMaxX(1600).SetMaxY(1200);
    auto nearest_selector =
        std::make_shared<tree_generator::SelectNearest>();
    tree_gen.SetMinSpanAngleDeg(95);
    auto restricted_bin = tree_gen.BuildAnySelectBin(nearest_selector);
    tree_gen.SetMinSpanAngleDeg(0);
    auto free_bin = tree_gen.BuildAnySelectBin(nearest_selector);
    TreeRenderer tree_rend(settings);
    tree_rend.RenderAnimatedBFS(restricted_bin)
        .Render(a_restricted_output);
    tree_rend.RenderAnimatedBFS(free_bin).Render(a_free_output);
}