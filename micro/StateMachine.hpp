#pragma once

enum States {
  MENU,
  SCREAM_JUMP,
  SHOOTING_STARS,
  MAZE_CRAWLER,
  ADD_RECORD,
  SHOW_RECORDS
};

void machineSetup();

void machineLoop();

void changeState(States state);

void setupState(States state);
