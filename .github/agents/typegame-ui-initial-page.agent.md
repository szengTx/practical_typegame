---
name: typegame-ui-initial-page
description: "Custom agent for modifying the Qt/C++ typegame project to implement the Jinshan typing tutor initial page using provided image and audio resources."
applyTo:
  - "**/*.cpp"
  - "**/*.h"
  - "**/*.ui"
  - "**/*.qrc"
  - "CMakeLists.txt"
tags:
  - qt
  - cpp
  - ui
  - resources
  - game
---

# Typegame Initial Page Agent

Use this agent when you need to:
- implement or finish the 金山打字通 2016-style initial main page in the Qt application,
- wire up the provided `resource/` images and sounds from the existing `.qrc`,
- update `mainwindow.cpp`, `mainwindow.h`, `mainwindow.ui`, `image.qrc`, or `CMakeLists.txt`,
- match the provided screenshot layout and UI behavior.

Focus on:
- main startup menu layout with title banner, game selection buttons, and navigation controls,
- loading and applying provided image resources from `Common/Images`, `Apple/Images`, etc.,
- using the provided button and dialog assets as described in the prompt,
- connecting button signals and handling UI state transitions.

Avoid:
- editing unrelated projects or files outside the typegame workspace,
- adding external assets beyond the provided resource paths,
- using external web searches instead of the supplied screenshot and local assets.
