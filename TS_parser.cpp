#include "tsCommon.h"
#include "tsTransportStream.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <iomanip>

//=============================================================================================================================================================================

int main(int argc, char *argv[ ], char *envp[ ])
{
  // open file
  std::ifstream file;
  file.open("example_new.ts", std::ios::binary);

  // check if file if opened
  if(!file){
    std::perror("File opening failed");
    return EXIT_FAILURE;
  }

  xTS_PacketHeader    TS_PacketHeader;
  int32_t TS_PacketId = 0;

  // 188 bytes - whole packet
  char *TS_PacketBuffer = new char[188];
  uint8_t *Header = new uint8_t[4];

  // Adaptation field
  xTS_AdaptationField TS_AdaptationField;

  // PES 
  xPES_Assembler PES_Assembler;

  while(!file.eof() )
  {
    // read from file
    file.read(TS_PacketBuffer, 188);

    for(int i = 0; i < 4; i++){
      Header[i] = (uint8_t)TS_PacketBuffer[i];
    }

    // reset packet header fields
    TS_PacketHeader.Reset();
    TS_PacketHeader.Parse(Header);

    // printf("%010d ", TS_PacketId);
    // TS_PacketHeader.Print();

    // AF FIELD
    TS_AdaptationField.Reset();
    // if(TS_PacketHeader.hasAdaptationField()){
    //   // TS_AdaptationField.Reset();
    //   // TS_AdaptationField.Parse((uint8_t*)TS_PacketBuffer, TS_PacketHeader.hasAdaptationField());
    //   // printf("\t");
    //   // TS_AdaptationField.Print();
    // }

    // PES Header
    // only for audio
    if(TS_PacketHeader.getSyncByte() == 'G' && TS_PacketHeader.getPIDentifier() == 136){
      if(TS_PacketHeader.hasAdaptationField()) {
        TS_AdaptationField.Parse((uint8_t*)TS_PacketBuffer, TS_PacketHeader.hasAdaptationField());
      }

      // printf("%010d ", TS_PacketId);
      // TS_PacketHeader.Print();

      // if(TS_PacketHeader.hasAdaptationField()){
      //   TS_AdaptationField.Print();
      // }

      //PES Assembler
      // verify the PID
      if(TS_PacketHeader.getPIDentifier() == 136){
        xPES_Assembler::eResult Result = PES_Assembler.AbsorbPacket((uint8_t*)TS_PacketBuffer, &TS_PacketHeader, &TS_AdaptationField);
      
      //PESH
      uint32_t startAdd = 188 - (TS_AdaptationField.getAdaptationFieldLength() + 8);
      
      // if(TS_PacketHeader.hasPayload()){
      //   // no AF, Payload only
      //   PES_PacketHeader.Parse((uint8_t*)TS_PacketBuffer);
      // }

      switch(Result){
        // case xPES_Assembler::eResult::StreamPackedLost : printf("PcktLost "); break;
        // case xPES_Assembler::eResult::AssemblingStarted : printf("Started "); PES_Assembler.PrintPESH(); break;
        // // case xPES_Assembler::eResult::AssemblingStarted : printf("Started "); break;
        // case xPES_Assembler::eResult::AssemblingContinue: printf("Continue "); break;
        // case xPES_Assembler::eResult::AssemblingFinished: printf("Finished "); printf("PES: Len=%d", PES_Assembler.getNumPacketBytes()); break;
        // // case xPES_Assembler::eResult::AssemblingFinished: printf("Finished "); break;
        case xPES_Assembler::eResult::StreamPackedLost : break;
        case xPES_Assembler::eResult::AssemblingStarted : break;
        case xPES_Assembler::eResult::AssemblingContinue: break;
        case xPES_Assembler::eResult::AssemblingFinished: break;
        default: break;
      }
      printf("\n");
      }
    }

    // if(TS_PacketId == 20){
    //   break;
    // }
    
    // next packet (read every 188 bytes)
    TS_PacketId++;
  }

  // close file
  file.close();

  delete [] TS_PacketBuffer;

  return EXIT_SUCCESS;
}


/*
PES
>= 6 bytes total

PES HEADER
- packet_start_code_prefix (3 bajty) 0x00001
- stream_id (1 bajt)
- PES_packet_length (2 bajty)
--> if length = 0 then the length can vary
*/

//=============================================================================================================================================================================
