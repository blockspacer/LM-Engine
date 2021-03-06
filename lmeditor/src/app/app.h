#pragma once

#include <filesystem>
#include <set>
#include <tbb/flow_graph.h>
#include <tbb/task_scheduler_init.h>

#include <lmeditor/map_editor.h>
#include <lmlib/application.h>

#include "resources.h"
#include <lmeditor/inspector.h>
#include <lmeditor/map_selector.h>
#include <lmeditor/simulation.h>
#include <lmeditor/trimesh.h>
#include <lmhuv.h>
#include <lmlib/realtime_clock.h>
#include <lmtk/app_flow_graph.h>
#include <lmtk/rect_border.h>
#include <lmtk/text_layout.h>

namespace lmeditor
{
class editor_app;

struct state_handle_args
{
    lmtk::input_event const &input_event;
    lmtk::resource_sink &resource_sink;
    editor_app &app;
};

struct gui_state
{
    explicit gui_state(editor_app &app);
    void add_to_frame(editor_app &app, lmgl::iframe *frame);
    bool handle_input_event(state_handle_args const &args);
    void move_resources(editor_app &app);
};
struct modal_state
{
    bool handle_input_event(state_handle_args const &args);
    std::unique_ptr<lmtk::iwidget> modal;
    std::function<
      bool(editor_app &, lmtk::iwidget *, lmtk::input_event const &)>
      input_handler{[](auto &, auto, auto &) { return false; }};
    void add_to_frame(editor_app &app, lmgl::iframe *frame);
    void move_resources(editor_app &app);
};
struct player_state
{
    entt::registry registry;
    psimulation simulation;
    lmhuv::pvisual_view visual_view;

    lm::realtime_clock clock;

    bool handle_input_event(state_handle_args const &args);

    void update_simulation(lmtk::input_state const &input_state);

    void move_resources(editor_app &app);
    void add_to_frame(editor_app &app, lmgl::iframe *frame);
};

class editor_app
{
  public:
    friend class gui_state;
    friend class modal_state;
    friend class player_state;

    using state_variant_type =
      std::variant<gui_state, modal_state, player_state>;

    editor_app_resources resources;
    lmtk::app_flow_graph flow_graph;

    state_variant_type state;
    std::vector<itool_panel *> panel_order_horizontal;
    std::vector<itool_panel *> visible_panels;
    std::map<
      lmtk::iwidget *,
      std::function<bool(editor_app &, lmtk::input_event const &)>>
      input_handlers;
    std::filesystem::path map_file_project_relative_path{};

    player_state create_player_state();
    modal_state create_simulation_select_state();

    bool map_editor_handle(lmtk::input_event const &input_event);
    bool inspector_handle(lmtk::input_event const &input_event);
    bool entity_list_handle(lmtk::input_event const &input_event);

    void focus_tool_panel(itool_panel *tool_panel);
    void toggle_tool_panel(itool_panel *tool_panel);

    map_selector create_map_selector();

    void sync_entity_list();

  protected:
    bool on_input_event(lmtk::input_event const &variant);
    bool on_new_frame(lmgl::iframe *frame);
    void on_quit();

  public:
    editor_app(const std::filesystem::path &project_dir);
    editor_app(editor_app const &) = delete;
    void main() { flow_graph.enter(); }
    ~editor_app();
    void fit_inspector();
    void refit_visible_panels();
};
} // namespace lmeditor
