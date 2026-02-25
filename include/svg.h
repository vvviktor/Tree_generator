#pragma once

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace svg {

struct Rgb {
    Rgb() = default;
    Rgb(uint8_t red, uint8_t green, uint8_t blue);

    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
};

struct Rgba {
    Rgba() = default;
    Rgba(uint8_t red, uint8_t green, uint8_t blue, double opacity);

    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
    double opacity = 1.0;
};

std::ostream& operator<<(std::ostream& out, Rgb color);

std::ostream& operator<<(std::ostream& out, Rgba color);

using Color = std::variant<std::monostate, std::string, Rgb, Rgba>;

// Объявив в заголовочном файле константу со спецификатором inline,
// мы сделаем так, что она будет одной на все единицы трансляции,
// которые подключают этот заголовок.
// В противном случае каждая единица трансляции будет использовать свою
// копию этой константы
inline const Color NoneColor{"none"};

struct ColorDataPrinter {
    std::ostream& out;

    void operator()(std::monostate) const;
    void operator()(const std::string& color) const;
    void operator()(Rgb color) const;
    void operator()(Rgba color) const;
};

std::ostream& operator<<(std::ostream& out, Color color);

enum class StrokeLineCap {
    BUTT,
    ROUND,
    SQUARE,
};

enum class StrokeLineJoin {
    ARCS,
    BEVEL,
    MITER,
    MITER_CLIP,
    ROUND,
};

std::ostream& operator<<(std::ostream& out, StrokeLineCap line_cap);

std::ostream& operator<<(std::ostream& out, StrokeLineJoin line_join);

template <typename Owner>
class PathProps {
   public:
    Owner& SetFillColor(Color color);
    Owner& SetStrokeColor(Color color);
    Owner& SetStrokeWidth(double width);
    Owner& SetStrokeLineCap(StrokeLineCap line_cap);
    Owner& SetStrokeLineJoin(StrokeLineJoin line_join);

   protected:
    ~PathProps() = default;

    void RenderAttrs(std::ostream& out) const;

   private:
    std::optional<Color> fill_color_;
    std::optional<Color> stroke_color_;
    std::optional<double> stroke_width_;
    std::optional<StrokeLineCap> stroke_line_cap_;
    std::optional<StrokeLineJoin> stroke_line_join_;

    Owner& AsOwner();
};

template <typename Owner>
Owner& PathProps<Owner>::SetFillColor(Color color) {
    fill_color_ = std::move(color);
    return AsOwner();
}

template <typename Owner>
Owner& PathProps<Owner>::SetStrokeColor(Color color) {
    stroke_color_ = std::move(color);
    return AsOwner();
}

template <typename Owner>
Owner& PathProps<Owner>::SetStrokeWidth(double width) {
    stroke_width_ = width;
    return AsOwner();
}

template <typename Owner>
Owner& PathProps<Owner>::SetStrokeLineCap(StrokeLineCap line_cap) {
    stroke_line_cap_ = line_cap;
    return AsOwner();
}

template <typename Owner>
Owner& PathProps<Owner>::SetStrokeLineJoin(StrokeLineJoin line_join) {
    stroke_line_join_ = line_join;
    return AsOwner();
}

template <typename Owner>
void PathProps<Owner>::RenderAttrs(std::ostream& out) const {
    using namespace std::literals;

    if (fill_color_) {
        out << "fill=\""sv << *fill_color_ << "\" "sv;
    }
    if (stroke_color_) {
        out << "stroke=\""sv << *stroke_color_ << "\" "sv;
    }
    if (stroke_width_) {
        out << "stroke-width=\""sv << *stroke_width_ << "\" "sv;
    }
    if (stroke_line_cap_) {
        out << "stroke-linecap=\""sv << *stroke_line_cap_ << "\" "sv;
    }
    if (stroke_line_join_) {
        out << "stroke-linejoin=\""sv << *stroke_line_join_ << "\" "sv;
    }
}

template <typename Owner>
Owner& PathProps<Owner>::AsOwner() {
    // static_cast безопасно преобразует *this к Owner&,
    // если класс Owner — наследник PathProps
    return static_cast<Owner&>(*this);
}

enum class AnimationFill {
    FREEZE,
    REMOVE,
};

std::ostream& operator<<(std::ostream& out, AnimationFill fill);

template <typename Owner>
class AnimationProps {
   public:
    Owner& SetFrom(double from);
    Owner& SetTo(double to);
    Owner& SetDur(double dur);
    Owner& SetBegin(double begin);
    Owner& SetEnd(double end);
    Owner& SetFill(AnimationFill fill);

   protected:
    ~AnimationProps() = default;
    void RenderAttrs(std::ostream& out) const;

   private:
    std::optional<double> from_, to_, dur_, begin_, end_;
    std::optional<AnimationFill> fill_;

    Owner& AsOwner();
};

template <typename Owner>
Owner& AnimationProps<Owner>::SetFrom(double from) {
    from_ = from;
    return AsOwner();
}

template <typename Owner>
Owner& AnimationProps<Owner>::SetTo(double to) {
    to_ = to;
    return AsOwner();
}

template <typename Owner>
Owner& AnimationProps<Owner>::SetDur(double dur) {
    dur_ = dur;
    return AsOwner();
}

template <typename Owner>
Owner& AnimationProps<Owner>::SetBegin(double begin) {
    begin_ = begin;
    return AsOwner();
}

template <typename Owner>
Owner& AnimationProps<Owner>::SetEnd(double end) {
    end_ = end;
    return AsOwner();
}

template <typename Owner>
Owner& AnimationProps<Owner>::SetFill(AnimationFill fill) {
    fill_ = fill;
    return AsOwner();
}

template <typename Owner>
void AnimationProps<Owner>::RenderAttrs(std::ostream& out) const {
    using namespace std::literals;

    if (from_) {
        out << "from=\""sv << *from_ << "\" "sv;
    }
    if (to_) {
        out << "to=\""sv << *to_ << "\" "sv;
    }
    if (dur_) {
        out << "dur=\""sv << *dur_ << "\" "sv;
    }
    if (begin_) {
        out << "begin=\""sv << *begin_ << "\" "sv;
    }
    if (end_) {
        out << "end=\""sv << *end_ << "\" "sv;
    }
    if (fill_) {
        out << "fill=\""sv << *fill_ << "\" "sv;
    }
}

template <typename Owner>
Owner& AnimationProps<Owner>::AsOwner() {
    return static_cast<Owner&>(*this);
}

struct Point {
    Point() = default;
    Point(double x, double y);

    double x = 0;
    double y = 0;
};

/*
 * Вспомогательная структура, хранящая контекст для вывода SVG-документа с
 * отступами. Хранит ссылку на поток вывода, текущее значение и шаг отступа
 * при выводе элемента
 */
struct RenderContext {
    RenderContext(std::ostream& out);
    RenderContext(std::ostream& out, int indent_step, int indent);

    RenderContext Indented() const;
    void RenderIndent() const;

    std::ostream& out;
    int indent_step = 0;
    int indent = 0;
};

/*
 * Абстрактный базовый класс Object служит для унифицированного хранения
 * конкретных тегов SVG-документа
 * Реализует паттерн "Шаблонный метод" для вывода содержимого тега
 */
class Object {
   public:
    void Render(const RenderContext& context) const;

    virtual ~Object() = default;

   private:
    virtual void RenderObject(const RenderContext& context) const = 0;
};

class ObjectContainer {
   public:
    template <class T>
    void Add(T obj);

    virtual void AddPtr(std::unique_ptr<Object>&&) = 0;

    virtual ~ObjectContainer() = default;
};

template <class T>
void ObjectContainer::Add(T obj) {
    AddPtr(std::make_unique<T>(std::move(obj)));
}

/*
 * Класс Circle моделирует элемент <circle> для отображения круга
 * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/circle
 */
class Circle final : public Object, public PathProps<Circle> {
   public:
    Circle& SetCenter(Point center);
    Circle& SetRadius(double radius);

   private:
    Point center_;
    double radius_ = 1.0;

    void RenderObject(const RenderContext& context) const override;
};

/*
 * Класс Polyline моделирует элемент <polyline> для отображения ломаных
 * линий https://developer.mozilla.org/en-US/docs/Web/SVG/Element/polyline
 */
class Polyline final : public Object, public PathProps<Polyline> {
   public:
    // Добавляет очередную вершину к ломаной линии
    Polyline& AddPoint(Point point);

   private:
    std::vector<Point> points_;

    void RenderObject(const RenderContext& context) const override;
};

class Line : public Object, public PathProps<Line> {
   public:
    Line& SetA(Point a);
    Line& SetB(Point b);

   protected:
    void RenderHeader(const RenderContext& context) const;
    void RenderCloseTag(const RenderContext& context) const;

   private:
    Point a_, b_;

    void RenderObject(const RenderContext& context) const override;
};

/*
 * Класс Text моделирует элемент <text> для отображения текста
 * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/text
 */
class Text final : public Object, public PathProps<Text> {
   public:
    // Задаёт координаты опорной точки (атрибуты x и y)
    Text& SetPosition(Point pos);

    // Задаёт смещение относительно опорной точки (атрибуты dx, dy)
    Text& SetOffset(Point offset);

    // Задаёт размеры шрифта (атрибут font-size)
    Text& SetFontSize(uint32_t size);

    // Задаёт название шрифта (атрибут font-family)
    Text& SetFontFamily(std::string font_family);

    // Задаёт толщину шрифта (атрибут font-weight)
    Text& SetFontWeight(std::string font_weight);

    // Задаёт текстовое содержимое объекта (отображается внутри тега text)
    Text& SetData(std::string data);

   private:
    Point pos_;
    Point offset_;
    uint32_t font_size_ = 1;
    std::string font_family_;
    std::string font_weight_;
    std::string data_;

    void RenderObject(const RenderContext& context) const override;
};

class Document final : public ObjectContainer {
   public:
    Document() = default;

    // Добавляет в svg-документ объект-наследник svg::Object
    void AddPtr(std::unique_ptr<Object>&& obj) override;

    // Выводит в ostream svg-представление документа
    void Render(std::ostream& out) const;

    Document& SetWidth(int w);
    Document& SetHeight(int h);

   private:
    std::vector<std::unique_ptr<Object>> doc_data_;
    int width_ = -1, height_ = -1;
};

}  // namespace svg