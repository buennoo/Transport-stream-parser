#include "tsCommon.h"
#include "tsTransportStream.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <iomanip>

//=============================================================================================================================================================================

int main(int argc, char *argv[ ], char *envp[ ])
{
  // TODO - open file
  std::ifstream file;
  file.open("example_new.ts", std::ios::binary);

  // TODO - check if file if opened
  if(!file){
    std::perror("File opening failed");
    return EXIT_FAILURE;
  }

  xTS_PacketHeader    TS_PacketHeader;
  int32_t TS_PacketId = 0;

  // 188 bajtow - tyle ma caly pakiet
  char *TS_PacketBuffer = new char[188];
  uint8_t *Header = new uint8_t[4];

  while(!file.eof() )
  {
    // TODO - read from file
    file.read(TS_PacketBuffer, 188);

    for(int i = 0; i < 4; i++){
      Header[i] = (uint8_t)TS_PacketBuffer[i];
    }

    //reset pol naglowka pakietu strumienia transportowego
    TS_PacketHeader.Reset();
    TS_PacketHeader.Parse(Header);

    printf("%010d ", TS_PacketId);
    TS_PacketHeader.Print();
    printf("\n");

    if(TS_PacketId == 20){
      break;
    }
    //nastepny pakiet (odczyt co 188 bajtow)
    TS_PacketId++;
  }
  
  // TODO - close file
  file.close();

  delete [] TS_PacketBuffer;

  return EXIT_SUCCESS;
}

//=============================================================================================================================================================================
