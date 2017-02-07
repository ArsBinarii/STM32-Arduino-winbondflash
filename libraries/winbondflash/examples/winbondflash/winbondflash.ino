#include <SPI.h>
//#include <Metro.h>
#include "winbondflash.h"

winbondFlashSPI mem;

SPIClass spiToUse(2); // int represents SPI port number
//#define SS  PA4
#define SS  PB12

void setup()
{
  Serial1.begin(9600);
  Serial1.println("Init Chip...");
  if(mem.begin(_W25Q64,spiToUse,SS))
    Serial1.println("OK");
  else
  {
    Serial1.println("FAILED");
    while(1);
  }
}

void loop()
{
  if(Serial1.available() > 0)
  {
    switch(Serial1.read())
    {
    case 'R':
      {
        //R0,100
        Serial1.print(("Read "));
        int addr = Serial1.parseInt();
        int len = Serial1.parseInt();
        Serial1.print(("addr=0x"));
        Serial1.print(addr>>8,HEX);
        Serial1.print(addr,HEX);
        Serial1.print((",len=0x"));
        Serial1.print(len>>8,HEX);
        Serial1.print(len,HEX);
        Serial1.println((":"));
        uint8_t *buf = new uint8_t[len];
        while(mem.busy());
        mem.read(addr,buf,len);
        for(int i = 0;i < len; i++)
        {
          Serial1.print((char)buf[i]);
        }
        Serial1.println();
        Serial1.println(("OK"));
        delete [] buf;
      }
      break;
    case 'W':
      {
        //W0,test string[LF]
        //
        Serial1.print(("Write "));
        int addr = Serial1.parseInt();
        Serial1.print(("0x"));
        Serial1.print(addr>>8,HEX);
        Serial1.print(addr,HEX);
        Serial1.read();
        uint8_t buf[256];
        uint8_t len = Serial1.readBytesUntil('\n',(char*)buf,256);
        Serial1.print((",0x"));
        Serial1.print(len>>8,HEX);
        Serial1.print(len,HEX);
        Serial1.print((": "));
        while(mem.busy());
        mem.WE();
        mem.writePage(addr,buf);
        Serial1.println(("OK"));
      }
      break;
    case 'E':
      while(Serial1.available() < 1);
      char x = Serial1.read();
      //Serial1.print(x);
      switch(x)
      {
        int addr;
      case 'S':
        Serial1.print(("Erase Sector "));
        addr = Serial1.parseInt();
        Serial1.print(("addr=0x"));
        Serial1.print(addr>>8,HEX);
        Serial1.print(addr,HEX);
        Serial1.print((": "));
        mem.WE();
        mem.eraseSector(addr);
        Serial1.println("OK");
        break;
      case 'b':
        Serial1.print(("Erase 32k Block "));
        addr = Serial1.parseInt();
        Serial1.print(("addr=0x"));
        Serial1.print(addr>>8,HEX);
        Serial1.print(addr,HEX);
        Serial1.print((": "));
        mem.WE();
        mem.erase32kBlock(addr);
        Serial1.println("OK");
        break;
      case 'B':
        Serial1.print(("Erase 64k Block "));
        addr = Serial1.parseInt();
        Serial1.print(("addr=0x"));
        Serial1.print(addr>>8,HEX);
        Serial1.print(addr,HEX);
        Serial1.print((": "));
        mem.WE();
        mem.erase64kBlock(addr);
        Serial1.println("OK");
        break;
      case 'A':
        while(mem.busy());
        mem.WE();
        mem.eraseAll();
        long ss = millis();
        while(mem.busy())
        {
          Serial1.print(millis()-ss);
          delay(1000);
        }
        break;
      }

      /*
      if(Serial1.read() == 'S')
       {
       //ES0
       Serial1.print(("Erase Sector "));
       int addr = Serial1.parseInt();
       Serial1.print(("addr=0x"));
       Serial1.print(addr>>8,HEX);
       Serial1.print(addr,HEX);
       Serial1.print((": "));
       mem.WE();
       mem.eraseSector(addr);
       Serial1.println("OK");
       }
       break;
       */
    } 
  }
}


