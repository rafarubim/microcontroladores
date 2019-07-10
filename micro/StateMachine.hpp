#pragma once

enum States {
  MENU,
  SCREAM_JUMP,
  SHOOTING_STARS,
  MAZE_CRAWLER
};

void machineSetup();

void machineLoop();

void changeState(States state);
