#ifndef _SIMULATION_H_
#define _SIMULATION_H_

#include <random>

#include "GoBoard.h"
#include "UctSearch.h"

class LGR;
class LGRContext;

// �΋ǂ̃V�~�����[�V����(�m������)
void Simulation( game_info_t *game, int color, std::mt19937_64 *mt, LGR& lgrf, LGRContext& ctx );

#endif
