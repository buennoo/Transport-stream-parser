#include "tsTransportStream.h"
#include <iostream>

//=============================================================================================================================================================================
// xTS_PacketHeader
//=============================================================================================================================================================================


/// @brief Reset - reset all TS packet header fields
void xTS_PacketHeader::Reset()
{
  //write me
  m_SB = 0;
  m_E = 0;
  m_S = 0;
  m_T = 0;
  m_PID = 0;
  m_TSC = 0;
  m_AFC = 0;
  m_CC = 0;
}

/**
  @brief Parse all TS packet header fields
  @param Input is pointer to buffer containing TS packet
  @return Number of parsed bytes (4 on success, -1 on failure) 
 */
int32_t xTS_PacketHeader::Parse(const uint8_t* Input)
{
  //i`m empty
  if(Input == nullptr){
    return -1;
  }

  // polaczenie 4 pierwszych bajtow do zmiennej (32-bit)
  //uint32_t *HP = (uint32_t*)Input;

  // zamiana kolejnosci bajtow - big endian
  // najbardziej znaczacy na poczatku, najmniej znaczacy na koncu
  int32_t Head = xSwapBytes32(*reinterpret_cast<const uint32_t*>(Input));

  m_SB = (Head & 0xff000000) >> 24;
  m_E = (Head & 0x800000) >> 23;
  m_S = (Head & 0x400000) >> 22;
  m_T = (Head & 0x200000) >> 21;
  m_PID = (Head & 0x1fff00) >> 8;
  m_TSC = (Head & 0xc0) >> 6;
  m_AFC = (Head & 0x30) >> 4;
  m_CC = Head & 0xf;

  return 4; // Number of parsed bytes
}

/// @brief Print all TS packet header fields
void xTS_PacketHeader::Print() const
{
  //print sth
  std::cout << "SB=" << (int)m_SB <<
    " E=" << (int)m_E <<
    " S=" << (int)m_S << 
    " m_T=" << (int)m_T << 
    " PID=" << (int)m_PID << 
    " TSC=" << (int)m_TSC << 
    " AFC=" << (int)m_AFC <<
    " CC=" << (int)m_CC << std::endl;
}

//=============================================================================================================================================================================
