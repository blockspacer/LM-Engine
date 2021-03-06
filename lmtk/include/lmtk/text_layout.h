#pragma once

#include <any>

#include <lmgl/frame.h>
#include <lmlib/reference.h>

#include "font.h"
#include "lmtk.h"
#include "resource_sink.h"

namespace lm
{
class iframe;
}

namespace lmtk
{
struct text_layout_init;

enum class text_alignment
{
    left,
    center
};

class text_layout
{
  public:
    static lmgl::material create_material(lmgl::irenderer &renderer);

    explicit text_layout(text_layout_init const &init);
    text_layout &render(lmgl::iframe *frame);

    void set_text(
      lmgl::irenderer &renderer,
      ifont const *font,
      std::string const &text,
      resource_sink &sink);

    void recreate(
      lmgl::irenderer &renderer,
      ifont const *font,
      std::string const &text);

    lm::size2i get_size();
    void set_position(lm::point2i const &pos) { position = pos; }

    lmgl::material material;
    lm::point2i position;
    std::array<float, 3> colour;

    lmgl::buffer vposbuffer;
    lmgl::buffer tcoordbuffer;
    lmgl::buffer ubuffer;
    lmgl::buffer ibuffer;
    lmgl::geometry geometry_;

    text_alignment alignment;
    float pixel_width{0.f};
    unsigned max_glyph_height;
    void move_text_resources(resource_sink &sink, lmgl::irenderer *renderer);
    void move_resources(lmgl::irenderer *renderer, resource_sink &sink);
};

struct text_layout_init
{
    lmgl::irenderer &renderer;
    lmgl::material material;
    ifont const *font;
    std::array<float, 3> colour{0.f, 0.f, 0.f};
    lm::point2i position{0, 0};
    std::string const &text;
    text_alignment const alignment{text_alignment::left};
};

struct text_layout_factory
{
    lmgl::irenderer &renderer;
    lmgl::material material;
    ifont const *font;
    std::array<float, 3> colour{0.f, 0.f, 0.f};
    lm::point2i position{0, 0};

    text_layout create(std::string const &text)
    {
        return text_layout{
          text_layout_init{
            .renderer = renderer,
            .material = material,
            .font = font,
            .colour = colour,
            .position = position,
            .text = text,
          },
        };
    }
};
} // namespace lmtk
