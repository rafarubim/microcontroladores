#pragma once

enum States {
  MENU,
  ADD_RECORD,
  SHOW_RECORDS,
  SCREAM_JUMP,
  SHOOTING_STARS,
  MAZE_CRAWLER
};

void machineSetup();

void machineLoop();

void changeState(States state);
