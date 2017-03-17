#include "GoBoard.h"
#include "Message.h"
#include "Point.h"
#include "Pattern.h"
#include "Semeai.h"

// IsCapturableAtari�֐��p
game_info_t capturable_game;
// CheckOiotoshi�֐��p
game_info_t oiotoshi_game;
// CheckLibertyState�֐��p
game_info_t liberty_game;
// IsSelfAtariCapture�֐��p
game_info_t capture_game;

/////////////////////////
//  1��Ŏ��邩�m�F  //
/////////////////////////
bool
IsCapturableAtari( game_info_t *game, int pos, int color, int opponent_pos )
{
  string_t *string;
  int *string_id;
  int other = FLIP_COLOR(color);
  int neighbor;
  int id;
  int libs;

  if (!IsLegal(game, pos, color)) {
    return false;
  }

  // �ǖʂ��R�s�[
  CopyGame(&capturable_game, game);
  // �Ƃ肠�����΂�u��
  PutStone(&capturable_game, pos, color);

  string = capturable_game.string;
  string_id = capturable_game.string_id;
  id = string_id[opponent_pos];

  // ���͂Ɏ��Ԃ���΂�����Έ��S
  neighbor = string[id].neighbor[0];
  while (neighbor != NEIGHBOR_END) {
    if (string[neighbor].libs == 1) {
      return false;
    }
    neighbor = string[id].neighbor[neighbor];
  }

  if (!IsLegal(&capturable_game, string[string_id[opponent_pos]].lib[0], other)) {
    return true;
  }
  // ���������Ń_���ɑł�
  PutStone(&capturable_game, string[string_id[opponent_pos]].lib[0], other);

  libs = string[string_id[opponent_pos]].libs;

  // �����Ă��ċz�_��1�Ȃ�ߊl�\�Ɣ���
  if (libs == 1) {
    return true;
  } else {
    return false;
  }
}


////////////////////////////////
//  �I�C�I�g�V���ǂ������m�F  //
////////////////////////////////
// �Ԃ�l��int��bool�̈Ⴂ������IsCapturableAtari�֐��Ɠ���
int
CheckOiotoshi( game_info_t *game, int pos, int color, int opponent_pos )
{
  string_t *string;
  int *string_id;
  int other = FLIP_COLOR(color);
  int neighbor;
  int id, num = -1;

  if (!IsLegal(game, pos, color)) {
    return -1;
  }

  CopyGame(&oiotoshi_game, game);
  PutStone(&oiotoshi_game, pos, color);
  string = oiotoshi_game.string;
  string_id = oiotoshi_game.string_id;
  id = string_id[opponent_pos];

  neighbor = string[id].neighbor[0];
  while (neighbor != NEIGHBOR_END) {
    if (string[neighbor].libs == 1) {
      return -1;
    }
    neighbor = string[id].neighbor[neighbor];
  }

  if (!IsLegal(&oiotoshi_game, string[string_id[opponent_pos]].lib[0], other)) {
    return -1;
  }
  PutStone(&oiotoshi_game, string[string_id[opponent_pos]].lib[0], other);

  if (string[string_id[opponent_pos]].libs == 1) {
    num = string[string_id[opponent_pos]].size;
  }

  return num;
}


//////////////////////////////////////////////
//  �΂������ɕߊl�ł������Ȍ�������߂�  //
//////////////////////////////////////////////
int
CapturableCandidate( game_info_t *game, int id )
{
  string_t *string = game->string;
  int neighbor = string[id].neighbor[0];
  bool flag = false;
  int capturable_pos = -1;

  // �אڂ���ċz�_��1�̓G�A��1�����̎�, ����Ԃ�
  while (neighbor != NEIGHBOR_END) {
    if (string[neighbor].libs == 1) {
      if (string[neighbor].size >= 2) {
	return -1;
      } else {
	if (flag) {
	  return -1;
	}
	capturable_pos = string[neighbor].lib[0];
	flag = true;
      }
    }
    neighbor = string[id].neighbor[neighbor];
  }

  return capturable_pos;
}


////////////////////////////////////
//  �����ɕ߂܂�肩�ǂ����𔻒�  //
////////////////////////////////////
bool
IsDeadlyExtension( game_info_t *game, int color, int id )
{
  game_info_t search_game;
  int other = FLIP_COLOR(color);
  int pos = game->string[id].lib[0];
  bool flag = false;

  if (nb4_empty[Pat3(game->pat, pos)] == 0 &&
      IsSuicide(game, game->string, other, pos)) {
    return true;
  }

  CopyGame(&search_game, game);
  PutStone(&search_game, pos, other);

  if (search_game.string[search_game.string_id[pos]].libs == 1) {
    flag = true;
  }


  return flag;
}


////////////////////////////////////
//  �אڂ���G�A�����邩�𔻒�  //
////////////////////////////////////
bool
IsCapturableNeighborNone(game_info_t *game, int id)
{
  string_t *string = game->string;
  int neighbor = string[id].neighbor[0];

  while (neighbor != NEIGHBOR_END) {
    if (string[neighbor].libs == 1) {
      return false;
    }
    neighbor = string[id].neighbor[neighbor];
  }

  return true;
}


/////////////////////////////////
//  ���ȃA�^���ɂȂ�g��������  //
/////////////////////////////////
bool
IsSelfAtariCapture( game_info_t *game, int pos, int color, int id )
{
  string_t *string;
  int string_pos = game->string[id].origin;
  int *string_id;

  if (!IsLegal(game, pos, color)) {
    return false;
  }

  CopyGame(&capture_game, game);
  PutStone(&capture_game, pos, color);

  string = capture_game.string;
  string_id = capture_game.string_id;

  if (string[string_id[string_pos]].libs == 1) {
    return true;
  } else {
    return false;
  }
}

////////////////////////////////////////
//  �ċz�_���ǂ̂悤�ɕω����邩���m�F  //
////////////////////////////////////////
int
CheckLibertyState( game_info_t *game, int pos, int color, int id )
{
  string_t *string;
  int string_pos = game->string[id].origin;
  int *string_id;
  int libs = game->string[id].libs;
  int new_libs;

  if (!IsLegal(game, pos, color)) {
    return L_DECREASE;
  }

  CopyGame(&liberty_game, game);
  PutStone(&liberty_game, pos, color);

  string = liberty_game.string;
  string_id = liberty_game.string_id;

  new_libs = string[string_id[string_pos]].libs;

  if (new_libs > libs + 1) {
    return L_INCREASE;
  } else if (new_libs > libs) {
    return L_EVEN;
  } else {
    return L_DECREASE;
  }
}


///////////////////////////////////////////////
//  1��Ŏ��邩�𔻒�(�V�~�����[�V�����p)  //
///////////////////////////////////////////////
bool
IsCapturableAtariForSimulation( game_info_t *game, int pos, int color, int id )
{
  char *board = game->board;
  string_t *string = game->string;
  int *string_id = game->string_id;
  int other = FLIP_COLOR(color);
  int lib;
  bool neighbor = false;
  int index_distance;
  int pat3;
  int empty;
  int connect_libs = 0;
  int tmp_id;

  lib = string[id].lib[0];

  if (lib == pos) {
    lib = string[id].lib[lib];
  }

  index_distance = lib - pos;

  // �l�߂���Ƃ͋t�̃_���̎��͂̋�_���𒲂ׂ�
  pat3 = Pat3(game->pat, lib);
  empty = nb4_empty[pat3];

  // �t�̃_���̎��͂̋�_����3�Ȃ���Ȃ��̂�false
  if (empty == 3) {
    return false;
  }

  if (index_distance ==           1) neighbor = true;
  if (index_distance ==          -1) neighbor = true;
  if (index_distance ==  board_size) neighbor = true;
  if (index_distance == -board_size) neighbor = true;

  // �_�����ׂ荇���Ă��鎞��
  // �_��������Ă��鎞�̕���
  if (( neighbor && empty >= 3) ||
      (!neighbor && empty >= 2)) {
    return false;
  }

  // �אڂ���A��lib�ȊO�Ɏ��ċz�_�̍��v����
  // 2�ȏ�Ȃ疳������1��Ŏ���A�^���ł͂Ȃ��̂�false

  // ��̊m�F
  if (board[NORTH(lib)] == other && 
      string_id[NORTH(lib)] != id) {
    tmp_id = string_id[NORTH(lib)];
    if (string[tmp_id].libs > 2) {
      return false;
    } else {
      connect_libs += string[tmp_id].libs - 1;
    }
  } 

  // ���̊m�F
  if (board[WEST(lib)] == other && 
      string_id[WEST(lib)] != id) {
    tmp_id = string_id[WEST(lib)];
    if (string[tmp_id].libs > 2) {
      return false;
    } else {
      connect_libs += string[tmp_id].libs - 1;
    }
  }

  // �E�̊m�F
  if (board[EAST(lib)] == other && 
      string_id[EAST(lib)] != id) {
    tmp_id = string_id[EAST(lib)];
    if (string[tmp_id].libs > 2) {
      return false;
    } else {
      connect_libs += string[tmp_id].libs - 1;
    }
  }

  // ���̊m�F
  if (board[SOUTH(lib)] == other && 
      string_id[SOUTH(lib)] != id) {
    tmp_id = string_id[SOUTH(lib)];
    if (string[tmp_id].libs > 2) {
      return false;
    } else {
      connect_libs += string[tmp_id].libs - 1;
    }
  }

  // �_���ɑł��Ă�������ċz�_����1�ȉ��Ȃ�
  // 1��Ŏ���A�^��
  if (( neighbor && connect_libs < 2) ||
      (!neighbor && connect_libs < 1)) {
    return true;
  } else {
    return false;
  }
}


bool
IsSelfAtariCaptureForSimulation( game_info_t *game, int pos, int color, int lib )
{
  char *board = game->board;
  string_t *string = game->string;
  int *string_id = game->string_id;
  int other = FLIP_COLOR(color);
  int id;
  int size = 0;

  if (lib != pos || 
      nb4_empty[Pat3(game->pat, pos)] != 0) {
    return false;
  }

  if (board[NORTH(pos)] == color) {
    id = string_id[NORTH(pos)];
    if (string[id].libs > 1) {
      return false;
    }
  } else if (board[NORTH(pos)] == other) {
    id = string_id[NORTH(pos)];
    size += string[id].size;
    if (size > 1) {
      return false;
    }
  }

  if (board[WEST(pos)] == color) {
    id = string_id[WEST(pos)];
    if (string[id].libs > 1) {
      return false;
    }
  } else if (board[WEST(pos)] == other) {
    id = string_id[WEST(pos)];
    size += string[id].size;
    if (size > 1) {
      return false;
    }
  }

  if (board[EAST(pos)] == color) {
    id = string_id[EAST(pos)];
    if (string[id].libs > 1) {
      return false;
    }
  } else if (board[EAST(pos)] == other) {
    id = string_id[EAST(pos)];
    size += string[id].size;
    if (size > 1) {
      return false;
    }
  }

  if (board[SOUTH(pos)] == color) {
    id = string_id[SOUTH(pos)];
    if (string[id].libs > 1) {
      return false;
    }
  } else if (board[SOUTH(pos)] == other) {
    id = string_id[SOUTH(pos)];
    size += string[id].size;
    if (size > 1) {
      return false;
    }
  }

  return true;
}

bool
IsSelfAtari( game_info_t *game, int color, int pos )
{
  char *board = game->board;
  string_t *string = game->string;
  int *string_id = game->string_id;
  int other = FLIP_COLOR(color);
  int already[4] = { 0 };
  int already_num = 0;
  int lib, count = 0, libs = 0;
  int lib_candidate[10];
  int i;
  int id;
  bool checked;

  // �㉺���E����_�Ȃ�ċz�_�̌��ɓ����
  if (board[NORTH(pos)] == S_EMPTY) lib_candidate[libs++] = NORTH(pos);
  if (board[WEST(pos)] == S_EMPTY) lib_candidate[libs++] = WEST(pos);
  if (board[EAST(pos)] == S_EMPTY) lib_candidate[libs++] = EAST(pos);
  if (board[SOUTH(pos)] == S_EMPTY) lib_candidate[libs++] = SOUTH(pos);

  //  ��_
  if (libs >= 2) return false;

  // ��𒲂ׂ�
  if (board[NORTH(pos)] == color) {
    id = string_id[NORTH(pos)];
    if (string[id].libs > 2) return false;
    lib = string[id].lib[0];
    count = 0;
    while (lib != LIBERTY_END) {
      if (lib != pos) {
	checked = false;
	for (i = 0; i < libs; i++) {
	  if (lib_candidate[i] == lib) {
	    checked = true;
	    break;
	  }
	}
	if (!checked) {
	  lib_candidate[libs + count] = lib;
	  count++;
	}
      }
      lib = string[id].lib[lib];
    }
    libs += count;
    already[already_num++] = id;
    if (libs >= 2) return false;
  } else if (board[NORTH(pos)] == other &&
	     string[string_id[NORTH(pos)]].libs == 1) {
    return false;
  }

  // ���𒲂ׂ�
  if (board[WEST(pos)] == color) {
    id = string_id[WEST(pos)];
    if (already[0] != id) {
      if (string[id].libs > 2) return false;
      lib = string[id].lib[0];
      count = 0;
      while (lib != LIBERTY_END) {
	if (lib != pos) {
	  checked = false;
	  for (i = 0; i < libs; i++) {
	    if (lib_candidate[i] == lib) {
	      checked = true;
	      break;
	    }
	  }
	  if (!checked) {
	    lib_candidate[libs + count] = lib;
	    count++;
	  }
	}
	lib = string[id].lib[lib];
      }
      libs += count;
      already[already_num++] = id;
      if (libs >= 2) return false;
    }
  } else if (board[WEST(pos)] == other &&
	     string[string_id[WEST(pos)]].libs == 1) {
    return false;
  }

  // �E�𒲂ׂ�
  if (board[EAST(pos)] == color) {
    id = string_id[EAST(pos)];
    if (already[0] != id && already[1] != id) {
      if (string[id].libs > 2) return false;
      lib = string[id].lib[0];
      count = 0;
      while (lib != LIBERTY_END) {
	if (lib != pos) {
	  checked = false;
	  for (i = 0; i < libs; i++) {
	    if (lib_candidate[i] == lib) {
	      checked = true;
	      break;
	    }
	  }
	  if (!checked) {
	    lib_candidate[libs + count] = lib;
	    count++;
	  }
	}
	lib = string[id].lib[lib];
      }
      libs += count;
      already[already_num++] = id;
      if (libs >= 2) return false;
    }
  } else if (board[EAST(pos)] == other &&
	     string[string_id[EAST(pos)]].libs == 1) {
    return false;
  }


  // ���𒲂ׂ�
  if (board[SOUTH(pos)] == color) {
    id = string_id[SOUTH(pos)];
    if (already[0] != id && already[1] != id && already[2] != id) {
      if (string[id].libs > 2) return false;
      lib = string[id].lib[0];
      count = 0;
      while (lib != LIBERTY_END) {
	if (lib != pos) {
	  checked = false;
	  for (i = 0; i < libs; i++) {
	    if (lib_candidate[i] == lib) {
	      checked = true;
	      break;
	    }
	  }
	  if (!checked) {
	    lib_candidate[libs + count] = lib;
	    count++;
	  }
	}
	lib = string[id].lib[lib];
      }
      libs += count;
      already[already_num++] = id;
      if (libs >= 2) return false;
    }
  } else if (board[SOUTH(pos)] == other &&
	     string[string_id[SOUTH(pos)]].libs == 1) {
    return false;
  }

  return true;
}


bool 
IsAlreadyCaptured( game_info_t *game, int color, int id, int player_id[], int player_ids )
{
  string_t *string = game->string;
  int *string_id = game->string_id;
  int lib1, lib2;
  bool checked;
  int neighbor4[4];
  int i, j;

  if (string[id].libs == 1) {
    return true;
  } else if (string[id].libs == 2) {
    lib1 = string[id].lib[0];
    lib2 = string[id].lib[lib1];

    GetNeighbor4(neighbor4, lib1);
    checked = false;
    for (i = 0; i < 4; i++) {
      for (j = 0; j < player_ids; j++) {
	if (player_id[j] == string_id[neighbor4[i]]) {
	  checked = true;
	  player_id[j] = 0;
	}
      }
    }
    if (checked == false) return false;

    GetNeighbor4(neighbor4, lib2);
    checked = false;
    for (i = 0; i < 4; i++) {
      for (j = 0; j < player_ids; j++) {
	if (player_id[j] == string_id[neighbor4[i]]) {
	  checked = true;
	  player_id[j] = 0;
	}
      }
    }
    if (checked == false) return false;

    for (i = 0; i < player_ids; i++) {
      if (player_id[i] != 0) {
	return false;
      }
    }

    return true;
  } else {
    return false;
  }
}
