#include <fstream>
#include <iostream>

#include "svg.h"
#include "dom.h"
#include "tree_generator.h"
#include "tree_renderer.h"

int main() {
    std::ofstream svg_tree("tree.svg");
    TreeGenerator tree_gen(60, 1500, 1200);
    RenderSettings settings;
    settings.SetMaxX(1200).SetMaxY(1200);
    TreeRenderer tree_rend(settings);
    tree_rend.ToSvg(tree_gen.GenerateRandom()).Render(svg_tree);
}