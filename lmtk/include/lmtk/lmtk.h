#pragma once

#include <optional>
#include <variant>

#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>

#include "input_state.h"
#include "resource_sink.h"
#include <lmgl/lmgl.h>
#include <lmlib/geometry.h>
#include <lmlib/variant_visitor.h>
#include <lmpl/lmpl.h>
#include <range/v3/algorithm/remove.hpp>

namespace lmtk
{
struct key_down_event
{
    const struct input_state &input_state;
    lmpl::key_code key;
};
struct key_up_event
{
    const struct input_state &input_state;
    lmpl::key_code key;
};
struct mouse_button_down_event
{
    const struct input_state &input_state;
    lmpl::mouse_button button;
    lm::point2i pos;
};
struct mouse_button_up_event
{
    const struct input_state &input_state;
    lmpl::mouse_button button;
    lm::point2i pos;
};
struct mouse_move_event
{
    const struct input_state &input_state;
    lm::point2i pos;
    lm::size2i delta;
};
using input_event = std::variant<
  key_down_event,
  key_up_event,
  mouse_button_down_event,
  mouse_button_up_event,
  mouse_move_event>;

/// Create input event from platform window event, updating the input state.
std::optional<input_event>
  create_input_event(const lmpl::window_message &msg, input_state &input_state);
} // namespace lmtk
