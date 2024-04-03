#pragma once
#include "tsCommon.h"
#include <string>

/*
MPEG-TS packet:
`        3                   2                   1                   0  `
`      1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0  `
`     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ `
`   0 |                             Header                            | `
`     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ `
`   4 |                  Adaptation field + Payload                   | `
`     |                                                               | `
` 184 |                                                               | `
`     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ `


MPEG-TS packet header:
`        3                   2                   1                   0  `
`      1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0  `
`     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ `
`   0 |       SB      |E|S|T|           PID           |TSC|AFC|   CC  | `
`     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ `

Sync byte                    (SB ) :  8 bits
Transport error indicator    (E  ) :  1 bit
Payload unit start indicator (S  ) :  1 bit
Transport priority           (T  ) :  1 bit
Packet Identifier            (PID) : 13 bits
Transport scrambling control (TSC) :  2 bits
Adaptation field control     (AFC) :  2 bits
Continuity counter           (CC ) :  4 bits
*/


//=============================================================================================================================================================================

class xTS
{
public:
  static constexpr uint32_t TS_PacketLength  = 188;
  static constexpr uint32_t TS_HeaderLength  = 4;

  static constexpr uint32_t PES_HeaderLength = 6;

  static constexpr uint32_t BaseClockFrequency_Hz         =    90000; //Hz
  static constexpr uint32_t ExtendedClockFrequency_Hz     = 27000000; //Hz
  static constexpr uint32_t BaseClockFrequency_kHz        =       90; //kHz
  static constexpr uint32_t ExtendedClockFrequency_kHz    =    27000; //kHz
  static constexpr uint32_t BaseToExtendedClockMultiplier =      300;
};

//=============================================================================================================================================================================

class xTS_PacketHeader
{

public:
  enum class ePID : uint16_t
  {
    PAT  = 0x0000,
    CAT  = 0x0001,
    TSDT = 0x0002,
    IPMT = 0x0003,
    NIT  = 0x0010, //DVB specific PID
    SDT  = 0x0011, //DVB specific PID
    NuLL = 0x1FFF,
  };

protected:
  //TODO - header fields, e.g.:
  uint8_t  m_SB;
  uint8_t  m_E;
  uint8_t  m_S;
  uint8_t  m_T;
  uint32_t  m_PID;
  uint8_t  m_TSC;
  uint8_t  m_AFC;
  uint8_t  m_CC;

public:
  void     Reset();
  int32_t  Parse(const uint8_t* Input);
  void     Print() const;

public:
  //TODO - direct acces to header field value, e.g.:
  uint8_t  getSyncByte() const { return m_SB; } 
  uint8_t  getTransportError() const { return m_E; }
  uint8_t  getPUStartIndicator() const { return m_S; } 
  uint8_t  getTransportPriority() const { return m_T; }
  uint8_t  getPIDentifier() const { return m_PID; }
  uint8_t  getTSControl() const { return m_TSC; }
  uint8_t  getAFControl() const { return m_AFC; }
  uint8_t  getContinuityCounter() const { return m_CC; } 

public:
  //TODO - derrived informations
  bool     hasAdaptationField() const { 
  // 10 – adaptation field only, no payload 
    return (m_AFC & 0x2) != 0; 
  }
  bool     hasPayload() const {
  // 01 – no adaptation field, payload only 
     return (m_AFC & 0x1) != 0;
  }

  //11 - kiedy oba pola sa czyli i AF i Payload
  
};

//=============================================================================================================================================================================

//=============================================================================================================================================================================
// xTS_AdaptationField
//=============================================================================================================================================================================

class xTS_AdaptationField
{
  protected:
    //setup
    uint8_t m_AdaptationFieldControl;
    //mandatory fields
    uint8_t m_AdaptationFieldLength;
    uint8_t  m_DC;
    uint8_t  m_RA;
    uint8_t  m_SP;
    uint8_t  m_SF;
    uint8_t  m_TP;
    uint8_t  m_EX;
    //optional fields - PCR
    uint8_t  m_PR;
    uint8_t m_OR;

  public:
    void Reset();
    int32_t Parse(const uint8_t* PacketBuffer, uint8_t AdaptationFieldControl);
    void Print() const;
  public:
    //mandatory fields
    uint8_t getAdaptationFieldLength() const { return m_AdaptationFieldLength; }
    uint8_t getDiscontinuityIndicator() const { return m_DC; }
    uint8_t getRandomAccessIndicator() const { return m_RA; }
    uint8_t getElementaryStreamPriorityIndicator() const { return m_SP; }
    uint8_t getSplicingPointFlag() const { return m_SF; }
    uint8_t getTranspotPrivateDataFlag() const { return m_TP; }
    uint8_t getAdaptationFieldExtensionFlag() const { return m_EX; }

    // optional fields
    int8_t getProgramClockReferenceFlag() const { return m_PR; }
    int8_t getOriginalProgramClockReferenceFlag() const { return m_OR; }     
    //derived values
    uint32_t getNumBytes () const {
      //TO-DO
    }
};