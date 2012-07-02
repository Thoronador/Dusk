#ifndef NO_OGRE_IN_LANDSCAPE
  //avoids using Ogre headers which we don't need for converter
  #define NO_OGRE_IN_LANDSCAPE
#endif
#include "Converter.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdint.h>
#include "../Engine/Landscape.h"

const uint32_t cACTI = 0x49544341; //"ACTI" = 41 43 54 49
const uint32_t cALCH = 0x48434C41; //"ALCH" = 41 4C 43 48
const uint32_t cAPPA = 0x41505041; //"APPA" = 41 50 50 41
const uint32_t cARMO = 0x4F4D5241; //"ARMO" = 41 52 4D 4F
const uint32_t cBODY = 0x59444F42; //"BODY" = 42 4F 44 59
const uint32_t cBOOK = 0x4B4F4F42; //"BOOK" = 42 4F 4F 4B
const uint32_t cBSGN = 0x4E475342; //"BSGN" = 42 53 47 4E
const uint32_t cCELL = 0x4C4C4543; //"CELL" = 43 45 4C 4C
const uint32_t cCLAS = 0x53414C43; //"CLAS" = 43 4C 41 53
const uint32_t cCLOT = 0x544F4C43; //"CLOT" = 43 4C 4F 54
const uint32_t cCONT = 0x544E4F43; //"CONT" = 43 4F 4E 54
const uint32_t cCREA = 0x41455243; //"CREA" = 43 52 45 41
const uint32_t cDATA = 0x41544144; //"DATA" = 44 41 54 41
const uint32_t cDIAL = 0x4C414944; //"DIAL" = 44 49 41 4C
const uint32_t cDOOR = 0x524F4F44; //"DOOR" = 44 4F 4F 52
const uint32_t cENCH = 0x48434E45; //"ENCH" = 45 4E 43 48
const uint32_t cFACT = 0x54434146; //"FACT" = 46 41 43 54
const uint32_t cGLOB = 0x424F4C47; //"GLOB" = 47 4C 4F 42
const uint32_t cGMST = 0x54534D47; //"GMST" = 47 4D 53 54
const uint32_t cINFO = 0x4F464E49; //"INFO" = 49 4E 46 4F
const uint32_t cINGR = 0x52474E49; //"INGR" = 49 4E 47 52
const uint32_t cINTV = 0x56544E49; //"INTV" = 49 4E 54 56
const uint32_t cLAND = 0x444E414C; //"LAND" = 4C 41 4E 44
const uint32_t cLEVC = 0x4356454C; //"LEVC" = 4C 45 56 43
const uint32_t cLEVI = 0x4956454C; //"LEVI" = 4C 45 56 49
const uint32_t cLIGH = 0x4847494C; //"LIGH" = 4C 49 47 48
const uint32_t cLOCK = 0x4B434F4C; //"LOCK" = 4C 4F 43 4B
const uint32_t cLTEX = 0x5845544C; //"LTEX" = 4C 54 45 58
const uint32_t cMGEF = 0x4645474D; //"MGEF" = 4D 47 45 46
const uint32_t cMISC = 0x4353494D; //"MISC" = 4D 49 53 43
const uint32_t cNPC_ = 0x5F43504E; //"NPC_" = 4E 50 43 5F
const uint32_t cPGRD = 0x44524750; //"PGRD" = 50 47 52 44
const uint32_t cPROB = 0x424F5250; //"PROB" = 50 52 4F 42
const uint32_t cRACE = 0x45434152; //"RACE" = 52 41 43 45
const uint32_t cREGN = 0x4E474552; //"REGN" = 52 45 47 4E
const uint32_t cREPA = 0x41504552; //"REPA" = 52 45 50 41
const uint32_t cSCPT = 0x54504353; //"SCPT" = 53 43 50 54
const uint32_t cSKIL = 0x4C494B53; //"SKIL" = 53 4B 49 4C
const uint32_t cSNDG = 0x47444E53; //"SNDG" = 53 4E 44 47
const uint32_t cSOUN = 0x4E554F53; //"SOUN" = 53 4F 55 4E
const uint32_t cSPEL = 0x4C455053; //"SPEL" = 53 50 45 4C
const uint32_t cSSCR = 0x52435353; //"SSCR" = 53 53 43 52
const uint32_t cSTAT = 0x54415453; //"STAT" = 53 54 41 54
const uint32_t cVCLR = 0x524C4356; //"VCLR" = 56 43 4C 52
const uint32_t cVHGT = 0x54474856; //"VHGT" = 56 48 47 54
const uint32_t cVNML = 0x4C4D4E56; //"VNML" = 56 4E 4D 4C
const uint32_t cVTEX = 0x58455456; //"VTEX" = 56 54 45 58
const uint32_t cWEAP = 0x50414557; //"WEAP" = 57 45 41 50
const uint32_t cWNAM = 0x4D414E57; //"WNAM" = 57 4E 41 4D


//converting "integer strings" to real C++ string
std::string IntTo4Char(const uint32_t value);
//helper function for message on wrong records
void UnexpectedRecord(const uint32_t expected, const uint32_t unexpected);

//"dispatcher"
bool ProcessNextRecord(std::ifstream& in_File, const int FileSize);

//routines for reading/ skipping different records
bool ReadLAND(std::ifstream& in_File);
bool SkipRecord(std::ifstream& in_File);

bool ScanESP(const std::string& FileName, const std::string& DuskFileName)
{
  std::ifstream input;
  char Buffer[4];
  long int FileSize = 0;

  input.open(FileName.c_str(), std::ios::in | std::ios::binary);
  if (!input)
  {
    std::cout << "Error: could not open file \""<<FileName<<"\".\n";
    return false;
  }

  //set file pointer to end
  input.seekg(0, std::ios::end);
  FileSize = input.tellg();
  //return file pointer to start of file
  input.seekg(0, std::ios::beg);

  //read the header
  //TES3
  input.read(Buffer, 4);
  if (Buffer[0]!='T' || Buffer[1]!='E' || Buffer[2]!='S' || Buffer[3]!='3')
  {
    std::cout << "Error: File \""<<FileName<<"\" is not a valid .esp/.esm file.\n";
    input.close();
    return false;
  }
  int32_t Size, Header_One, Flags;
  uint32_t SubSize;
  input.read((char*) &Size, 4);
  input.read((char*) &Header_One, 4);
  input.read((char*) &Flags, 4);
  if (!input.good())
  {
    std::cout << "Error while reading from \""<<FileName<<"\".\n";
    input.close();
    return false;
  }

  //HEDR
  input.read(Buffer, 4);
  if (Buffer[0]!='H' || Buffer[1]!='E' || Buffer[2]!='D' || Buffer[3]!='R')
  {
    std::cout << "Error: File \""<<FileName<<"\" is not a valid .esp/.esm file."
              << "Record HEDR is not present.\n";
    input.close();
    return false;
  }

  /*HEDR (300bytes)
    4 bytes, float Version (1.2)
    4 bytes, long Unknown (1)
    32 Bytes, Company/ Author Name string
    256 Bytes, ESM file description
    4 bytes, long NumRecords (48227)
  */
  SubSize = 0;
  input.read((char*) &SubSize, 4);//size of HEDR record
  if (SubSize!=300)
  {
    std::cout << "Error: record HEDR of file \""<<FileName<<"\" has wrong size."
              << " Actual size: "<<SubSize<<" bytes. Must-have size: 300 bytes.\n";
    input.close();
    return false;
  }

  float Version = 0.0f;
  int32_t Unknown, NumRecords;
  input.read((char*) &Version, 4);//version of file
  input.read((char*) &Unknown, 4);//unknown value

  char CName[32];
  char Description[256];
  memset(CName, '\0', 32);
  memset(Description, '\0', 256);
  input.read(CName, 32);//company name
  input.read(Description, 256);//file description
  input.read((char*) &NumRecords, 4);//total number of records

  if (!input.good())
  {
    std::cout << "Error while reading HEDR data from file \""<<FileName<<"\".\n";
    input.close();
    return false;
  }
  std::cout << "Info for file \""<<FileName<<"\":"
            << "\n Version: "<<Version
            << "\n Unknown value: "<<Unknown
            << "\n Company Name: "<<CName
            << "\n Description: "<<Description
            << "\n Number of records: "<< NumRecords<< "\n\n";

  std::cout << "Master files:\n";
  do
  {
    memset(Buffer, '\0', 4);
    input.read(Buffer, 4);
    if (Buffer[0]=='M' && Buffer[1]=='A' && Buffer[2]=='S' && Buffer[3]=='T')
    {
      //read the data
      SubSize = 0;
      input.read((char*) &SubSize, 4);
      if (SubSize<=256)
      {
        memset(Description, '\0', 256);
        input.read(Description, SubSize);
        std::cout << " File: "<<Description<<"\n";
      }
      else
      {
        //skip the data
        input.seekg(SubSize, std::ios::cur);
        std::cout << " Skipping one master file due to long ("<<SubSize
                  << " bytes) file name.\n";
      }
      //reading DATA record
      memset(Buffer, '\0', 4);
      input.read(Buffer, 4);
      if (Buffer[0]!='D' || Buffer[1]!='A' || Buffer[2]!='T' || Buffer[3]!='A')
      {
        std::cout << "Error while reading file \""<<FileName<<"\": DATA "
                  << "record expected, but got: \""<<Buffer[0]<<Buffer[1]
                  <<Buffer[2]<<Buffer[3]<<"\".\n";
        input.close();
        return false;
      }
      //reading size of DATA record
      input.read((char*) &SubSize, 4);
      if (SubSize!=8)
      {
        std::cout << "Error: Record DATA has invalid size ("<<SubSize
                  <<" bytes). Should be 8 bytes.\n";
        input.close();
        return false;
      }
      //skip 8 bytes of DATA
      input.seekg(8, std::ios::cur);
    }//read master file info
    else
    {
      std::cout <<"End of Master File list.\n\n First record is \""<<Buffer[0]
                <<Buffer[1]<<Buffer[2]<<Buffer[3]<<"\".\n";
    }

  } while //(RecordType == Master / Data);
    (Buffer[0]=='D' && Buffer[1]=='A' && Buffer[2]=='T' && Buffer[3]=='A');

  //seek four bytes into the direction if the beginning to land before the
  // name of the next record
  input.seekg(-4, std::ios::cur);

  bool Go_on_processing = input.good();
  unsigned int Processed_Records = 0;
  //now read all the records
  while (Go_on_processing)
  {
    Go_on_processing = ProcessNextRecord(input, FileSize);
    Processed_Records++;
    if (input.tellg()>=FileSize)
    {
      Go_on_processing = false;
    }
  }

  std::cout << "Number of processed and/or skipped records (including possible failures): "
            <<Processed_Records<<"\n"
            << "Current file position: "<<input.tellg()<< " bytes.\n";
  //end
  input.close();

  //save data and return
  return Dusk::Landscape::getSingleton().saveToFile(DuskFileName);
}

bool ProcessNextRecord(std::ifstream& in_File, const int FileSize)
{
  uint32_t RecordName; //normally should be 4 char, but char is not eligible for switch
  RecordName = 0;

  bool Success = false;

  //read record name
  in_File.read((char*) &RecordName, 4);
  switch(RecordName)
  {
    case cACTI:
    case cALCH:
    case cAPPA:
    case cARMO:
    case cBODY:
    case cBOOK:
    case cBSGN:
    case cCELL:
    case cCLAS:
    case cCLOT:
    case cCONT:
    case cCREA:
    case cDIAL:
    case cDOOR:
    case cENCH:
    case cFACT:
    case cGLOB:
    case cGMST:
    case cINFO:
    case cINGR:
         Success = SkipRecord(in_File); break;
    case cLAND:
         Success = ReadLAND(in_File); break;
    case cLEVC:
    case cLEVI:
    case cLIGH:
    case cLOCK:
    case cLTEX:
    case cMGEF:
    case cMISC:
    case cNPC_:
    case cPGRD:
    case cPROB:
    case cRACE:
    case cREGN:
    case cREPA:
    case cSCPT:
    case cSKIL:
    case cSNDG:
    case cSOUN:
    case cSPEL:
    case cSSCR:
    case cSTAT:
    case cWEAP:
         Success = SkipRecord(in_File); break;
    default:
         std::cout << "ProcessRecords: ERROR: unknown record type found: \""
                   <<IntTo4Char(RecordName)<<"\".\n"
                   << "Current file position: "<<in_File.tellg()<< " bytes.\n";
         Success = false;
  }//swi
  return Success;
}//ProcessNextRecord

std::string IntTo4Char(const uint32_t value)
{
  std::string result = "1234";

  result[0] = char(value&255);
  result[1] = char((value>>8)&255);
  result[2] = char((value>>16)&255);
  result[3] = char(value>>24);
  return result;
}

void UnexpectedRecord(const uint32_t expected, const uint32_t unexpected)
{
  std::cout << "Error: expected record name \""<<IntTo4Char(expected)
            <<"\" was not found. Instead, \""<<IntTo4Char(unexpected)
            <<"\" was found.\n";
  return;
}

bool ReadLAND(std::ifstream& in_File)
{
  uint32_t Size;
  int32_t HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

  /*Landscape:
    INTV (8 bytes)
        long CellX }
        long CellY } The cell coordinates of the cell.
    DATA (4 bytes)
        long Unknown (default of 0x09)
    VNML (12675 bytes)
        struct {
          signed byte X
          signed byte Y
          signed byte Z
        } normals[65][65];
            A RGB color map 65x65 pixels in size representing the land normal vectors.
            The signed value of the 'color' represents the vector's component. Blue
            is vertical (Z), Red the X direction and Green the Y direction. Note that
            the y-direction of the data is from the bottom up.
    VHGT (4232 bytes)
        float HeightOffset
            A height offset for the entire cell. Decreasing this value will shift the
            entire cell land down.
        byte Unknown1 (0x00)
        signed byte  HeightData[65][65]
            Contains the height data for the cell in the form of a 65x65 pixel array. The
            height data is not absolute values but uses differences between adjacent pixels.
            Thus a pixel value of 0 means it has the same height as the last pixel. Note that
            the y-direction of the data is from the bottom up.
        short Unknown2 (0x0000)
    WNAM (81 bytes)
        byte Data[9][9] (Unknown byte data.)
    VCLR (12675 bytes) optional
        Vertex color array, looks like another RBG image 65x65 pixels in size.
    VTEX (512 bytes) optional
        A 16x16 array of short texture indices (from a LTEX record I think). */

  uint32_t SubRecName, SubLength;
  int32_t CellX, CellY;
  SubRecName = SubLength = CellX = CellY = 0;

  if (Size<4232/*size of VHGT*/+8/*Header*/)
  { //skip the rest of the data, because it's too small to contain height data
    in_File.seekg(Size, std::ios::cur);
    return in_File.good();
  }

  //read INTV
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cINTV)
  {
    UnexpectedRecord(cINTV, SubRecName);
    return false;
  }
  //INTV's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=8)
  {
    std::cout <<"Error: sub record INTV of LAND has invalid length ("<<SubLength
              <<" bytes). Should be 8 bytes.\n";
    return false;
  }
  //read cell offsets
  in_File.read((char*) &CellX, 4);
  in_File.read((char*) &CellY, 4);

  //read DATA
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cDATA)
  {
    UnexpectedRecord(cDATA, SubRecName);
    return false;
  }
  //DATA's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=4)
  {
    std::cout <<"Error: sub record DATA of LAND has invalid length ("<<SubLength
              <<" bytes). Should be 4 bytes.\n";
    return false;
  }
  in_File.seekg(4, std::ios::cur); //skip Unknown

  //read VNML
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cVNML)
  {
    UnexpectedRecord(cVNML, SubRecName);
    return false;
  }
  //VNML's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=65*65*3)
  {
    std::cout <<"Error: sub record VNML of LAND has invalid length ("<<SubLength
              <<" bytes). Should be 12.675 bytes.\n";
    return false;
  }
  in_File.seekg(65*65*3, std::ios::cur); //skip normal vectors

  //read VHGT
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cVHGT)
  {
    UnexpectedRecord(cVHGT, SubRecName);
    return false;
  }
  //VHGT's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=4232)
  {
    std::cout <<"Error: sub record VHGT of LAND has invalid length ("<<SubLength
              <<" bytes). Should be 4.232 bytes.\n";
    return false;
  }

  std::cout << "Debug: sub-record VHGT found for cell "<<CellX<<","<<CellY<<".\n";

  //data vars
  char MW_Height[65][65];
  float HeightOffset = 0.0f;
  memset(MW_Height, '\0', 65*65);
  //read height offset
  in_File.read((char*) &HeightOffset, 4);
  //skip Unknown1
  in_File.seekg(1, std::ios::cur);
  //read height data
  in_File.read(&MW_Height[0][0], 65*65);
  std::cout << "Debug: read operation finished. "<< in_File.gcount() <<" of "<<65*65<<" bytes were read.\n";
  //skip Unknown2
  in_File.seekg(2, std::ios::cur);

  //read WNAM
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cWNAM)
  {
    UnexpectedRecord(cWNAM, SubRecName);
    return false;
  }
  //WNAM's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=81)
  {
    std::cout <<"Error: sub record WNAM of LAND has invalid length ("<<SubLength
              <<" bytes). Should be 81 bytes.\n";
    return false;
  }
  in_File.seekg(81, std::ios::cur); //skip WNAM

  //data var
  char MW_Colour[65][65][3];
  memset(MW_Colour, 255, 65*65*3);

  //read optional VCLR
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName==cVCLR)
  {
    //VCLR's length
    in_File.read((char*) &SubLength, 4);
    if (SubLength!=65*65*3)
    {
      std::cout <<"Error: sub record VCLR of LAND has invalid length ("<<SubLength
                <<" bytes). Should be 12.675 bytes.\n";
      return false;
    }
    in_File.read(&MW_Colour[0][0][0], 65*65*3);
    //read next (optional) record
    in_File.read((char*) &SubRecName, 4);
  }

  //read VTEX
  if (SubRecName==cVTEX)
  {
    //VTEX's length
    in_File.read((char*) &SubLength, 4);
    if (SubLength!=512)
    {
      std::cout <<"Error: sub record VTEX of LAND has invalid length ("<<SubLength
                <<" bytes). Should be 512 bytes.\n";
      return false;
    }
    in_File.seekg(512, std::ios::cur); //skip texture data
    //read next record name
    in_File.read((char*) &SubRecName, 4);
  }//if

  //seek 4 bytes towards beginning to land before next record name
  in_File.seekg(-4, std::ios::cur);

  /*++++
    ++
    ++ now do the actual conversion stuff
    ++
    ++++ */
  //conversion of data to Dusk's format

  std::cout << "Debug: Starting conversion to Dusk's format.\n";

  Dusk::LandscapeRecord* DuskLand;
  unsigned int i,j;

  //get new record from Landscape singleton
  DuskLand = Dusk::Landscape::getSingleton().createRecord();

  for (i=0; i<65; i++)
  {
    for (j=0; j<65; j++)
    {
      HeightOffset = HeightOffset + MW_Height[i][j];
      DuskLand->Height[i][j] = HeightOffset;

      DuskLand->Colour[i][j][0] = MW_Colour[i][j][0];
      DuskLand->Colour[i][j][1] = MW_Colour[i][j][1];
      DuskLand->Colour[i][j][2] = MW_Colour[i][j][2];
    }//for j
  }//for i
  DuskLand->setStride(Dusk::LandscapeRecord::cDefaultStride);
  DuskLand->moveTo(Dusk::LandscapeRecord::cDefaultStride * 64 * CellX,
                   Dusk::LandscapeRecord::cDefaultStride * 64 * CellY);
  DuskLand->setLoadedState(true);
  std::cout << "Debug: conversion finished.\n";

  return in_File.good();
}//ReadLAND

bool SkipRecord(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
  Size = 0;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

  /* data does not really matter here */

  in_File.seekg(Size, std::ios::cur);
  return in_File.good();
}//SkipRecord
