// -*- C++ -*-
//===========================================================================
/* Time-stamp: <2017-04-29 15:51:41 by root> */

/* @file main.cpp
 * $Id: main.cpp 2017-03-24 09:41:08 root Exp $
 *
 * @author:Stone Jiang<jiangtao@tao-studio.net>
 */
//===========================================================================


#include "main.h"

#include "ace/OS_main.h"
#include "ace/Log_Msg.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/istreamwrapper.h"
#include <fstream>
#include <set>
#include "Mahjong.h"

#define MAKELONG(a, b) ((ACE_UINT32)(((ACE_UINT16)(a)) | ((ACE_UINT32)((ACE_UINT16)(b))) << 16))

using namespace MJ;


int ACE_TMAIN(int, ACE_TCHAR*[])
{

  
  int hand_datas[4][10] = { 0 };

#if 1

  std::ifstream ifs("input.json");
  rapidjson::IStreamWrapper isw(ifs);
  rapidjson::Document doc;
  if (doc.ParseStream(isw).HasParseError())
  {
    ACE_DEBUG((LM_DEBUG,
      ACE_TEXT("(%P|%T) bad input json\n")));
    return -1;
  }
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  doc.Accept(writer);
  ACE_DEBUG((LM_DEBUG,
    ACE_TEXT("\n(%P|%T) %s\n"), buffer.GetString()));
  if (!doc.IsArray())
  {
    ACE_DEBUG((LM_DEBUG,
      ACE_TEXT("(%P|%T) bad input json\n")));
    return -1;
  }
  if (doc.Size() > 13)
  {
    ACE_DEBUG((LM_DEBUG,
      ACE_TEXT("(%P|%T) bad cards size = %d\n"), doc.Size()));
    return -1;
  }
  for (rapidjson::SizeType i = 0; i < doc.Size(); i++)
  {
    int t = doc[i]["t"].GetInt();
    int v = doc[i]["v"].GetInt();
    int c = MAKELONG(v, t);
    add_hand_card(hand_datas,c);
  }

  if (!check_good_hands(hand_datas))
  {
    ACE_DEBUG((LM_DEBUG,
      ACE_TEXT("(%P|%T) bad input json\n")));
    return -1;
  }


#else
  rapidjson::Document doc;
  rapidjson::Document::AllocatorType& a = doc.GetAllocator();
  doc.SetArray();
  rapidjson::Value the_card;
  the_card.SetObject();
  the_card.AddMember("t",1,a);
  the_card.AddMember("v", 1, a);
  doc.PushBack(the_card, a);
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  doc.Accept(writer);
  ACE_DEBUG((LM_DEBUG,
    ACE_TEXT("(%P|%T) %s"),buffer.GetString()));

#endif
#if 0
//  cards[index++] = 0x00010001;  // 1 tong 2 wan 3 su
  cards[index++] = 0x00040001;
  cards[index++] = 0x00040001;

  cards[index++] = 0x00040001;  // 1 tong 2 wan 3 su
  cards[index++] = 0x00010006;
  cards[index++] = 0x00010005;

  cards[index++] = 0x00010007;  // 1 tong 2 wan 3 su
  cards[index++] = 0x00010008;
  cards[index++] = 0x00010009;


  cards[index++] = 0x00030007;  // 1 tong 2 wan 3 su
  cards[index++] = 0x00030008;
  cards[index++] = 0x00020009;

  cards[index++] = 0x00020009;
  cards[index++] = 0x00020009;

  for (int i = 0; i < 13; i++)
  {
    add_hand_card(hand_datas,cards[i]);
  }
#endif

  std::set<int> results;

  check_wait(hand_datas, results);


  if (results.size() > 0)
  {
    for (auto c : results)
    {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%T) waitting on 0x%08x\n"), c));
    }
   
  }
  else
  {
    ACE_DEBUG((LM_DEBUG,
      ACE_TEXT("(%P|%T) lose\n")));
  }

  return 0;
}
