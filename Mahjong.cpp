#include "Mahjong.h"
#include <cstdint>
#include <stdio.h>

#if 0
#include "ace/Basic_Types.h"
#include "ace/Log_Msg.h"
#endif

typedef uint16_t ACE_UINT16;
typedef uint32_t ACE_UINT32;

#ifndef HIWORD
#define HIWORD(_dw)     ((ACE_UINT16)((((ACE_UINT32)(_dw)) >> 16) & 0xffff))
#endif
#ifndef LOWORD
#define LOWORD(_dw)     ((ACE_UINT16)(((ACE_UINT32)(_dw)) & 0xffff))
#endif
#ifndef MAKELONG
#define MAKELONG(a, b)  ((ACE_UINT32)(((ACE_UINT16)(((ACE_UINT32)(a)) & 0xffff)) | ((ACE_UINT32)((ACE_UINT16)(((ACE_UINT32)(b)) & 0xffff))) << 16))
#endif

namespace MJ
{
  int g_cards_[] =
  {
    0x00010001, 0x00010001, 0x00010001, 0x00010001,
    0x00010002, 0x00010002, 0x00010002, 0x00010002,
    0x00010003, 0x00010003, 0x00010003, 0x00010003,
    0x00010004, 0x00010004, 0x00010004, 0x00010004,
    0x00010005, 0x00010005, 0x00010005, 0x00010005,
    0x00010006, 0x00010006, 0x00010006, 0x00010006,
    0x00010007, 0x00010007, 0x00010007, 0x00010007,
    0x00010008, 0x00010008, 0x00010008, 0x00010008,
    0x00010009, 0x00010009, 0x00010009, 0x00010009,

    0x00020001, 0x00020001, 0x00020001, 0x00020001,
    0x00020002, 0x00020002, 0x00020002, 0x00020002,
    0x00020003, 0x00020003, 0x00020003, 0x00020003,
    0x00020004, 0x00020004, 0x00020004, 0x00020004,
    0x00020005, 0x00020005, 0x00020005, 0x00020005,
    0x00020006, 0x00020006, 0x00020006, 0x00020006,
    0x00020007, 0x00020007, 0x00020007, 0x00020007,
    0x00020008, 0x00020008, 0x00020008, 0x00020008,
    0x00020009, 0x00020009, 0x00020009, 0x00020009,

    0x00030001, 0x00030001, 0x00030001, 0x00030001,
    0x00030002, 0x00030002, 0x00030002, 0x00030002,
    0x00030003, 0x00030003, 0x00030003, 0x00030003,
    0x00030004, 0x00030004, 0x00030004, 0x00030004,
    0x00030005, 0x00030005, 0x00030005, 0x00030005,
    0x00030006, 0x00030006, 0x00030006, 0x00030006,
    0x00030007, 0x00030007, 0x00030007, 0x00030007,
    0x00030008, 0x00030008, 0x00030008, 0x00030008,
    0x00030009, 0x00030009, 0x00030009, 0x00030009,

    0x00040001, 0x00040001, 0x00040001, 0x00040001,
  };




  bool analyze(int kind_cards[10], bool is_honor)
  {
    if (kind_cards[0] == 0)
    {
      return true;
    }
    int j;
    for (j = 1; j < 10; j++)
    {
      if (kind_cards[j] != 0)
      {
        break;
      }
    }
    bool result;
    if (kind_cards[j] >= 3)
    {
      kind_cards[j] -= 3;
      kind_cards[0] -= 3;
      result = analyze(kind_cards, is_honor);
      kind_cards[j] += 3;
      kind_cards[0] += 3;
      return result;
    }

    if (!is_honor && j < 8
      && kind_cards[j + 1] > 0
      && kind_cards[j + 2] > 0)
    {
      kind_cards[j] --;
      kind_cards[j + 1] --;
      kind_cards[j + 2] --;
      kind_cards[0] -= 3;
      result = analyze(kind_cards, is_honor);
      kind_cards[j] ++;
      kind_cards[j + 1] ++;
      kind_cards[j + 2] ++;
      kind_cards[0] += 3;
      return result;
    }


    return false;
  }


  bool check_win(int hand_cards[4][10], int& c)
  {
    int pair_pos = -1;
    int remainder;
    bool pair_existed = false;
    for (int i = 0; i < 4; i++)
    {
      remainder = hand_cards[i][0] % 3;
      if (remainder == 1)
      {
        return false;
      }
      if (remainder == 2)
      {
        if (pair_existed)
        {
          return false;
        }
        pair_pos = i;
        pair_existed = true;
      }
    } // set pair pos

    for (int i = 0; i < 4; i++)
    {
      if (i != pair_pos)
      {
        if (!analyze(hand_cards[i], i == 3))
        {
          return false;
        }
      }
    }
    bool success = false;
    for (int j = 1; j < 10; j++)
    {
      if (hand_cards[pair_pos][j] >= 2)
      {
        hand_cards[pair_pos][j] -= 2;
        hand_cards[pair_pos][0] -= 2;
        if (analyze(hand_cards[pair_pos], pair_pos == 3))
        {
          c = MAKELONG(pair_pos, j + 1);
          success = true;
        }
        hand_cards[pair_pos][j] += 2;
        hand_cards[pair_pos][0] += 2;
        if (success)
          break;
      }
    }

    return success;
  }


  bool test_win(int hand_cards[4][10])
  {
    int pair_pos = -1;
    int remainder;
    bool pair_existed = false;
    for (int i = 0; i < 3; i++)
    {
      remainder = hand_cards[i][0] % 3;
      if (remainder == 1)
      {
        return false;
      }
      if (remainder == 2)
      {
        if (pair_existed)
        {
          return false;
        }
        pair_pos = i;
        pair_existed = true;
      }
    } // set pair pos

    for (int i = 0; i < 3; i++)
    {
      if (i != pair_pos)
      {
        if (!analyze(hand_cards[i], i == 3))
        {
          return false;
        }
      }
    }
    bool success = false;
    for (int j = 1; j < 10; j++)
    {
      if (hand_cards[pair_pos][j] >= 2)
      {
        hand_cards[pair_pos][j] -= 2;
        hand_cards[pair_pos][0] -= 2;
        if (analyze(hand_cards[pair_pos], pair_pos == 3))
        {
          success = true;
        }
        hand_cards[pair_pos][j] += 2;
        hand_cards[pair_pos][0] += 2;
        if (success)
          break;
      }
    }

    return success;
  }

  void add_hand_card(int hand_datas[4][10], int card)
  {
    int t = HIWORD(card) - 1;
    int v = LOWORD(card);
    hand_datas[t][0]++;
    hand_datas[t][v]++;
  }

  void remove_hand_card(int hand_datas[4][10], int card)
  {
    int t = HIWORD(card) - 1;
    int v = LOWORD(card);
    hand_datas[t][0]--;
    hand_datas[t][v]--;
  }
  bool is_wait(int hand_datas[4][10], int c)
  {
    bool retval = false;
    add_hand_card(hand_datas, c);
    retval = test_win(hand_datas);
    remove_hand_card(hand_datas, c);
    return retval;
  }

  void check_wait_no_zhong(int hand_datas[4][10], std::set<int>& results)
  {
    for (int i = 0; i < 3; i++)
    {
      for (int j = 0; j < 9; j++)
      {
        int c = MAKELONG(j + 1, i + 1);

        if (is_wait(hand_datas, c))
        {
          results.insert(c);
        }
      }
    }
  }

  void check_wait_one_zhong(int hand_datas[4][10], std::set<int>& results)
  {

    for (int i = 0; i < 3; i++)
    {
      for (int j = 0; j < 9; j++)
      {
        int c = MAKELONG(j + 1, i + 1);

        add_hand_card(hand_datas, c);
        check_wait_no_zhong(hand_datas, results);
        remove_hand_card(hand_datas, c);
      }
    }
  }

  void check_wait_two_zhong(int hand_datas[4][10], std::set<int>& results)
  {

    for (int i = 0; i < 3; i++)
    {
      for (int j = 0; j < 9; j++)
      {
        int c = MAKELONG(j + 1, i + 1);

        add_hand_card(hand_datas, c);
        check_wait_one_zhong(hand_datas, results);
        remove_hand_card(hand_datas, c);
      }
    }
  }


  void check_wait_three_zhong(int hand_datas[4][10], std::set<int>& results)
  {

    for (int i = 0; i < 3; i++)
    {
      for (int j = 0; j < 9; j++)
      {
        int c = MAKELONG(j + 1, i + 1);

        add_hand_card(hand_datas, c);
        check_wait_two_zhong(hand_datas, results);
        remove_hand_card(hand_datas, c);
      }
    }
  }

  bool check_good_hands(int hand_datas[4][10])
  {
    bool is_good = false;
    do
    {
      if (hand_datas[3][0] > 4)
      {
          printf ("more than 4 hong zhong \n");
          break;
      }
      for (int i = 0; i < 3; i++)
      {
        for (int j = 1; j < 10; j++)
        {
          if (hand_datas[i][j] >= 4)
          {
              printf ("more than 4 0x%08x \n", MAKELONG (j,i+1));
          }
        }
      }
      is_good = true;
    } while (0);
    return is_good;
  }

  void check_wait(int hand_datas[4][10], std::set<int>& results)
  {
    if (hand_datas[3][0] == 0)
    {
      check_wait_no_zhong(hand_datas, results);
    }
    else  if (hand_datas[3][0] == 1)
    {
      check_wait_one_zhong(hand_datas, results);
    }
    else  if (hand_datas[3][0] == 2)
    {
      check_wait_two_zhong(hand_datas, results);
    }
    else  if (hand_datas[3][0] == 3)
    {
      check_wait_three_zhong(hand_datas, results);
    }
    else  if (hand_datas[3][0] == 4)
    {
        //      ACE_DEBUG((LM_DEBUG,
        //                  ACE_TEXT("(%P|%T) win as 4 hong zhong\n")));
        printf ("win as 4 hong zhong.\n");
    }
  }

};
