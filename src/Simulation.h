#ifndef _SIMULATION_H_
#define _SIMULATION_H_

#include <random>

#include "GoBoard.h"
#include "UctSearch.h"

// �΋ǂ̃V�~�����[�V����(�m������)
void Simulation( game_info_t *game, int color, std::mt19937_64 *mt );

int SimulationGenmove(game_info_t *game, int color);

#endif
