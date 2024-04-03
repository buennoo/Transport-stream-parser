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

  // zamiana kolejnosci bajtow - najbardziej znaczacy bajt pierwszy big endian
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

//=============================================================================================================================================================================
// xTS_AdaptationField
//=============================================================================================================================================================================

// @brief Reset - reset all TS packet header fields
void xTS_AdaptationField::Reset(){
      // reset mandatory
      m_AdaptationFieldLength = 0;
      m_DC = 0;
      m_RA = 0;
      m_SP = 0;
      m_SF = 0;
      m_TP = 0;
      m_EX = 0;
      // reset optional fields
      m_PR = 0;
      m_OR = 0;
  }
  /**
  @brief Parse adaptation field
  @param PacketBuffer is pointer to buffer containing TS packet
  @param AdaptationFieldControl is value of Adaptation Field Control field of
  corresponding TS packet header
  @return Number of parsed bytes (length of AF or -1 on failure)
  */
  int32_t xTS_AdaptationField::Parse(const uint8_t* PacketBuffer, uint8_t AdaptationFieldControl){
    //i`m empty
    if(!AdaptationFieldControl){
      return -1;
    }
    else{
      if(PacketBuffer == nullptr){
        return -1;
      }
    }

    uint8_t *AF = new uint8_t[2];
    for(int i = 0; i < 2; i++){
      AF[i] = (uint8_t)PacketBuffer[i+4];
    }

    //parsing
    //AF to wydzielona wczesniej czesc PacketBuffera
    uint16_t AField = xSwapBytes16(*(reinterpret_cast<const uint16_t*>(AF)));

    // parsing the mandatory fields
    // buffer 
    m_AdaptationFieldLength = AF[0];
    m_DC = (AField & 0x80) >> 7;
    m_RA = (AField & 0x40) >> 6;
    m_SP = (AField & 0x04) >> 5;

    m_PR = (AField & 0x10) >> 4;
    m_OR = (AField & 0x08) >> 3;   

    m_SF = (AField & 0x10) >> 2;
    m_TP = (AField & 0x02) >> 1;
    m_EX = AField & 0x01;
  }

  /// @brief Print all TS packet header fields
  void xTS_AdaptationField::Print() const{
    //print sth
    std::cout << "AF: L=" << (int)m_AdaptationFieldLength <<
      " DC=" << (int)m_DC <<
      " RA=" << (int)m_RA << 
      " SP=" << (int)m_SP << 
      " PR=" << (int)m_PR << 
      " OR=" << (int)m_OR << 
      " SF=" << (int)m_SF <<
      " TP=" << (int)m_TP <<
      " EX=" << (int)m_EX << std::endl;
  }
