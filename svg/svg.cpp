#include "svg.h"

namespace svg {

using namespace std::literals;

Rgb::Rgb(uint8_t red, uint8_t green, uint8_t blue)
    : red(red), green(green), blue(blue) {}

Rgba::Rgba(uint8_t red, uint8_t green, uint8_t blue, double opacity)
    : red(red), green(green), blue(blue), opacity(opacity) {}

std::ostream& operator<<(std::ostream& out, Rgb color) {
    out << "rgb("sv << (unsigned short)color.red << ","sv
        << (unsigned short)color.green << ","sv
        << (unsigned short)color.blue << ")"sv;
    return out;
}

std::ostream& operator<<(std::ostream& out, Rgba color) {
    out << "rgba("sv << (unsigned short)color.red << ","sv
        << (unsigned short)color.green << ","sv
        << (unsigned short)color.blue << ","sv << color.opacity << ")"sv;
    return out;
}

std::ostream& operator<<(std::ostream& out, Color color) {
    std::visit(ColorDataPrinter{out}, color);
    return out;
}

std::ostream& operator<<(std::ostream& out, StrokeLineCap line_cap) {
    switch (line_cap) {
        case StrokeLineCap::BUTT:
            out << "butt"sv;
            break;
        case StrokeLineCap::ROUND:
            out << "round"sv;
            break;
        case StrokeLineCap::SQUARE:
            out << "square"sv;
            break;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, StrokeLineJoin line_join) {
    switch (line_join) {
        case StrokeLineJoin::ARCS:
            out << "arcs"sv;
            break;
        case StrokeLineJoin::BEVEL:
            out << "bevel"sv;
            break;
        case StrokeLineJoin::MITER:
            out << "miter"sv;
            break;
        case StrokeLineJoin::MITER_CLIP:
            out << "miter-clip"sv;
            break;
        case StrokeLineJoin::ROUND:
            out << "round"sv;
            break;
    }
    return out;
}

void ColorDataPrinter::operator()(std::monostate) const {
    out << "none"sv;
}

void ColorDataPrinter::operator()(const std::string& color) const {
    out << color;
}

void ColorDataPrinter::operator()(Rgb color) const {
    out << color;
}

void ColorDataPrinter::operator()(Rgba color) const {
    out << color;
}

std::ostream& operator<<(std::ostream& out, AnimationFill fill) {
    switch (fill) {
        case AnimationFill::FREEZE:
            out << "freeze"sv;
            break;
        case AnimationFill::REMOVE:
            out << "remove"sv;
            break;
    }
    return out;
}

Point::Point(double x, double y) : x(x), y(y) {}

RenderContext::RenderContext(std::ostream& out) : out(out) {}

RenderContext::RenderContext(std::ostream& out, int indent_step,
                             int indent)
    : out(out), indent_step(indent_step), indent(indent) {}

RenderContext RenderContext::Indented() const {
    return {out, indent_step, indent + indent_step};
}

void RenderContext::RenderIndent() const {
    for (int i = 0; i < indent; ++i) {
        out.put(' ');
    }
}

void Object::Render(const RenderContext& context) const {
    RenderObject(context);
}

AttribAnimation::AttribAnimation(std::string attr_name)
    : AnimationProps(std::move(attr_name), AnimationFill::FREEZE) {}

void AttribAnimation::RenderObject(const RenderContext& context) const {
    context.RenderIndent();
    auto& out = context.out;
    out << "<animate "sv;
    RenderAnimationAttrs(out);
    out << "/>"sv;
    out << "\n"sv;
}

// ---------- Circle ------------------

Circle& Circle::SetCenter(Point center) {
    center_ = center;
    return *this;
}

Circle& Circle::SetRadius(double radius) {
    radius_ = radius;
    return *this;
}

void Circle::RenderHeader(std::ostream& out) const {
    out << "<circle cx=\""sv << center_.x << "\" cy=\""sv << center_.y
        << "\" "sv << "r=\""sv << radius_ << "\" "sv;
    RenderAttrs(out);
    out << ">"sv;
}

void Circle::RenderCloseTag(std::ostream& out) const {
    out << "</circle>"sv;
}

void Circle::RenderObject(const RenderContext& context) const {
    auto& out = context.out;
    context.RenderIndent();
    RenderHeader(out);
    RenderCloseTag(out);
    out << "\n"sv;
}

FadeInCircle::FadeInCircle() : opacity_fade_("opacity"s) {
    opacity_fade_.SetFrom(0.).SetTo(1.);
}

FadeInCircle& FadeInCircle::SetDur(double dur) {
    opacity_fade_.SetDur(dur);
    return *this;
}

FadeInCircle& FadeInCircle::SetBegin(double begin) {
    opacity_fade_.SetBegin(begin);
    return *this;
}

void FadeInCircle::RenderObject(const RenderContext& context) const {
    auto& out = context.out;
    context.RenderIndent();
    RenderHeader(out);
    out << "\n"sv;
    RenderContext inner = context.Indented();
    opacity_fade_.Render(inner);
    context.RenderIndent();
    RenderCloseTag(out);
    out << "\n"sv;
}

Polyline& Polyline::AddPoint(svg::Point point) {
    points_.push_back(point);
    return *this;
}

void Polyline::RenderObject(const svg::RenderContext& context) const {
    auto& out = context.out;
    context.RenderIndent();
    out << "<polyline points=\""sv;
    if (!points_.empty()) {
        for (auto it = points_.begin(); it != points_.end(); ++it) {
            out << it->x << ","sv << it->y;
            if (!(next(it, 1) == points_.end())) {
                out << " "sv;
            }
        }
    }
    out << "\" "sv;
    RenderAttrs(out);
    out << "/>"sv;
    out << "\n"sv;
}

Line& Line::SetA(Point a) {
    a_ = a;
    return *this;
}

Line& Line::SetB(Point b) {
    b_ = b;
    return *this;
}

void Line::RenderHeader(std::ostream& out, Point a, Point b) const {
    using namespace std::literals;
    out << "<line x1=\""sv << a.x << "\" y1=\""sv << a.y << "\" x2=\""sv
        << b.x << "\" y2=\""sv << b.y << "\" "sv;
    RenderAttrs(out);
    out << ">"sv;
}

void Line::RenderCloseTag(std::ostream& out) const {
    out << "</line>";
}

void Line::RenderObject(const RenderContext& context) const {
    auto& out = context.out;
    context.RenderIndent();
    RenderHeader(out, a_, b_);
    RenderCloseTag(out);
    out << "\n"sv;
}

GrowingLine::GrowingLine()
    : move_x2_("x2"s), move_y2_("y2"s), make_visible_("opacity"s) {
    SetOpacity(0.);
    make_visible_.SetFrom(0.).SetTo(1.).SetDur(0.01);
}

GrowingLine& GrowingLine::SetA(Point a) {
    a_ = a;
    b_ = a;
    move_x2_.SetFrom(a_.x);
    move_y2_.SetFrom(a_.y);
    return *this;
}

GrowingLine& GrowingLine::SetB(Point b) {
    move_x2_.SetTo(b.x);
    move_y2_.SetTo(b.y);
    return *this;
}

GrowingLine& GrowingLine::SetDur(double dur) {
    move_x2_.SetDur(dur);
    move_y2_.SetDur(dur);
    return *this;
}

GrowingLine& GrowingLine::SetBegin(double begin) {
    make_visible_.SetBegin(begin);
    move_x2_.SetBegin(begin);
    move_y2_.SetBegin(begin);
    return *this;
}

void GrowingLine::RenderObject(const RenderContext& context) const {
    auto& out = context.out;
    context.RenderIndent();
    RenderHeader(out, a_, a_);
    out << "\n"sv;
    RenderContext inner = context.Indented();
    make_visible_.Render(inner);
    move_x2_.Render(inner);
    move_y2_.Render(inner);
    context.RenderIndent();
    RenderCloseTag(out);
    out << "\n"sv;
}

Text& Text::SetPosition(svg::Point pos) {
    pos_ = pos;
    return *this;
}

Text& Text::SetOffset(svg::Point offset) {
    offset_ = offset;
    return *this;
}

Text& Text::SetFontSize(uint32_t size) {
    font_size_ = size;
    return *this;
}

Text& Text::SetFontFamily(std::string font_family) {
    font_family_ = std::move(font_family);
    return *this;
}

Text& Text::SetFontWeight(std::string font_weight) {
    font_weight_ = std::move(font_weight);
    return *this;
}

Text& Text::SetData(std::string data) {
    std::string_view raw_data(std::move(data));
    if (!raw_data.empty()) {
        for (const char c : raw_data) {
            switch (c) {
                case '\"':
                    data_ += "&quot;"s;
                    break;
                case '\'':
                    data_ += "&apos;"s;
                    break;
                case '<':
                    data_ += "&lt;"s;
                    break;
                case '>':
                    data_ += "&gt;"s;
                    break;
                case '&':
                    data_ += "&amp;"s;
                    break;
                default:
                    data_ += c;
            }
        }
    }
    return *this;
}

void Text::RenderObject(const svg::RenderContext& context) const {
    auto& out = context.out;
    context.RenderIndent();
    out << "<text x=\""sv << pos_.x << "\" y=\""sv << pos_.y << "\" "sv;
    out << "dx=\""sv << offset_.x << "\" dy=\""sv << offset_.y << "\" "sv;
    out << "font-size=\""sv << font_size_ << "\" "sv;
    if (!font_family_.empty()) {
        out << "font-family=\""sv << font_family_ << "\" "sv;
    }
    if (!font_weight_.empty()) {
        out << "font-weight=\""sv << font_weight_ << "\" "sv;
    }
    RenderAttrs(out);
    out << ">"sv;
    out << data_ << "</text>"sv;
    out << "\n"sv;
}

void Document::AddPtr(std::unique_ptr<Object>&& obj) {
    doc_data_.push_back(std::move(obj));
}

void Document::Render(std::ostream& out) const {
    RenderContext ctx(out, 2, 2);
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"sv << std::endl;
    out << "<svg"sv;
    if (width_ != -1 && height_ != -1) {
        out << " width=\""sv << width_ << "\" height=\""sv << height_
            << "\" viewBox=\"0 0 "sv << width_ << " "sv << height_
            << "\""sv;
    }
    out << " xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">"sv
        << std::endl;
    ctx.RenderIndent();
    out << "<rect width=\"100%\" height=\"100%\" fill=\""sv
        << background_color_ << "\"/>";
    out << "\n"sv;
    if (!doc_data_.empty()) {
        for (const auto& obj_ptr : doc_data_) {
            obj_ptr->Render(ctx);
        }
    }
    out << "</svg>"sv;
    out << "\n"sv;
}

Document& Document::SetWidth(int w) {
    width_ = w;
    return *this;
}

Document& Document::SetHeight(int h) {
    height_ = h;
    return *this;
}

Document& Document::SetBackgroundColor(Color color) {
    background_color_ = color;
    return *this;
}

}  // namespace svg