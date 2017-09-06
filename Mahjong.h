#ifndef Mahjong_h__
#define Mahjong_h__
#include <set>

namespace MJ
{
  void add_hand_card(int hand_datas[4][10], int card);
  void remove_hand_card(int hand_datas[4][10], int card);
  bool check_good_hands(int hand_datas[4][10]);
  void check_wait(int hand_datas[4][10], std::set<int>& results);

};

#endif // Mahjong_h__
