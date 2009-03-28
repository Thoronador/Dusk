#ifndef NO_OGRE_IN_LANDSCAPE
  //avoids using Ogre headers which we don't need for converter
  #define NO_OGRE_IN_LANDSCAPE
#endif
#include "Converter.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include "../Engine/Landscape.h"

const long int cAADT = 0x54444141; //"AADT" = 41 41 44 54
const long int cACTI = 0x49544341; //"ACTI" = 41 43 54 49
const long int cALCH = 0x48434C41; //"ALCH" = 41 4C 43 48
const long int cALDT = 0x54444C41; //"ALDT" = 41 4C 44 54
const long int cANAM = 0x4D414E41; //"ANAM" = 41 4E 41 4D
const long int cAODT = 0x54444F41; //"AODT" = 41 4F 44 54
const long int cAPPA = 0x41505041; //"APPA" = 41 50 50 41
const long int cARMO = 0x4F4D5241; //"ARMO" = 41 52 4D 4F
const long int cASND = 0x444E5341; //"ASND" = 41 53 4E 44
const long int cAVFX = 0x58465641; //"AVFX" = 41 56 46 58
const long int cBKDT = 0x54444B42; //"BKDT" = 42 4B 44 54
const long int cBNAM = 0x4D414E42; //"BNAM" = 42 4E 41 4D
const long int cBODY = 0x59444F42; //"BODY" = 42 4F 44 59
const long int cBOOK = 0x4B4F4F42; //"BOOK" = 42 4F 4F 4B
const long int cBSGN = 0x4E475342; //"BSGN" = 42 53 47 4E
const long int cBSND = 0x444E5342; //"BSND" = 42 53 4E 44
const long int cBYDT = 0x54445942; //"BYDT" = 42 59 44 54
const long int cBVFX = 0x58465642; //"BVFX" = 42 56 46 58
const long int cCELL = 0x4C4C4543; //"CELL" = 43 45 4C 4C
const long int cCLAS = 0x53414C43; //"CLAS" = 43 4C 41 53
const long int cCLDT = 0x54444C43; //"CLDT" = 43 4C 44 54
const long int cCLOT = 0x544F4C43; //"CLOT" = 43 4C 4F 54
const long int cCNAM = 0x4D414E43; //"CNAM" = 43 4E 41 4D
const long int cCNDT = 0x54444E43; //"CNDT" = 43 4E 44 54
const long int cCONT = 0x544E4F43; //"CONT" = 43 4F 4E 54
const long int cCREA = 0x41455243; //"CREA" = 43 52 45 41
const long int cCSND = 0x444E5343; //"CSND" = 43 53 4E 44
const long int cCTDT = 0x54445443; //"CTDT" = 43 54 44 54
const long int cCVFX = 0x58465643; //"CVFX" = 43 56 46 58
const long int cDATA = 0x41544144; //"DATA" = 44 41 54 41
const long int cDESC = 0x43534544; //"DESC" = 44 45 53 43
const long int cDIAL = 0x4C414944; //"DIAL" = 44 49 41 4C
const long int cDOOR = 0x524F4F44; //"DOOR" = 44 4F 4F 52
const long int cENAM = 0x4D414E45; //"ENAM" = 45 4E 41 4D
const long int cENCH = 0x48434E45; //"ENCH" = 45 4E 43 48
const long int cENDT = 0x54444E45; //"ENDT" = 45 4E 44 54
const long int cFACT = 0x54434146; //"FACT" = 46 41 43 54
const long int cFADT = 0x54444146; //"FADT" = 46 41 44 54
const long int cFLAG = 0x47414C46; //"FLAG" = 46 4C 41 47
const long int cFLTV = 0x56544C46; //"FLTV" = 46 4C 54 56
const long int cFNAM = 0x4D414E46; //"FNAM" = 46 4E 41 4D
const long int cGLOB = 0x424F4C47; //"GLOB" = 47 4C 4F 42
const long int cGMST = 0x54534D47; //"GMST" = 47 4D 53 54
const long int cHSND = 0x444E5348; //"HSND" = 48 53 4E 44
const long int cHVFX = 0x58465648; //"HVFX" = 48 56 46 58
const long int cINDX = 0x58444E49; //"INDX" = 49 4E 44 58
const long int cINFO = 0x4F464E49; //"INFO" = 49 4E 46 4F
const long int cINGR = 0x52474E49; //"INGR" = 49 4E 47 52
const long int cINTV = 0x56544E49; //"INTV" = 49 4E 54 56
const long int cIRDT = 0x54445249; //"IRDT" = 49 52 44 54
const long int cITEX = 0x58455449; //"ITEX" = 49 54 45 58
const long int cLAND = 0x444E414C; //"LAND" = 4C 41 4E 44
const long int cLEVC = 0x4356454C; //"LEVC" = 4C 45 56 43
const long int cLEVI = 0x4956454C; //"LEVI" = 4C 45 56 49
const long int cLHDT = 0x5444484C; //"LHDT" = 4C 48 44 54
const long int cLIGH = 0x4847494C; //"LIGH" = 4C 49 47 48
const long int cLKDT = 0x54444B4C; //"LKDT" = 4C 4B 44 54
const long int cLOCK = 0x4B434F4C; //"LOCK" = 4C 4F 43 4B
const long int cLTEX = 0x5845544C; //"LTEX" = 4C 54 45 58
const long int cMCDT = 0x5444434D; //"MCDT" = 4D 43 44 54
const long int cMEDT = 0x5444454D; //"MEDT" = 4D 45 44 54
const long int cMGEF = 0x4645474D; //"MGEF" = 4D 47 45 46
const long int cMISC = 0x4353494D; //"MISC" = 4D 49 53 43
const long int cMODL = 0x4C444F4D; //"MODL" = 4D 4F 44 4C
const long int cNAME = 0x454D414E; //"NAME" = 4E 41 4D 45
const long int cNPCO = 0x4F43504E; //"NPCO" = 4E 50 43 4F
const long int cNPCS = 0x5343504E; //"NPCS" = 4E 50 43 53
const long int cNPC_ = 0x5F43504E; //"NPC_" = 4E 50 43 5F
const long int cPBDT = 0x54444250; //"PBDT" = 50 42 44 54
const long int cPGRC = 0x43524750; //"PGRC" = 50 47 52 43
const long int cPGRD = 0x44524750; //"PGRD" = 50 47 52 44
const long int cPGRP = 0x50524750; //"PGRP" = 50 47 52 50
const long int cPROB = 0x424F5250; //"PROB" = 50 52 4F 42
const long int cPTEX = 0x58455450; //"PTEX" = 50 54 45 58
const long int cRACE = 0x45434152; //"RACE" = 52 41 43 45
const long int cRADT = 0x54444152; //"RADT" = 52 41 44 54
const long int cREGN = 0x4E474552; //"REGN" = 52 45 47 4E
const long int cREPA = 0x41504552; //"REPA" = 52 45 50 41
const long int cRIDT = 0x54444952; //"RIDT" = 52 49 44 54
const long int cRNAM = 0x4D414E52; //"RNAM" = 52 4E 41 4D
const long int cSCDT = 0x54444353; //"SCDT" = 53 43 44 54
const long int cSCHD = 0x44484353; //"SCHD" = 53 43 48 44
const long int cSCPT = 0x54504353; //"SCPT" = 53 43 50 54
const long int cSCRI = 0x49524353; //"SCRI" = 53 43 52 49
const long int cSCTX = 0x58544353; //"SCTX" = 53 43 54 58
const long int cSCVR = 0x52564353; //"SCVR" = 53 43 56 52
const long int cSKDT = 0x54444B53; //"SKDT" = 53 4B 44 54
const long int cSKIL = 0x4C494B53; //"SKIL" = 53 4B 49 4C
const long int cSNAM = 0x4D414E53; //"SNAM" = 53 4E 41 4D
const long int cSNDG = 0x47444E53; //"SNDG" = 53 4E 44 47
const long int cSOUN = 0x4E554F53; //"SOUN" = 53 4F 55 4E
const long int cSPDT = 0x54445053; //"SPDT" = 53 50 44 54
const long int cSPEL = 0x4C455053; //"SPEL" = 53 50 45 4C
const long int cSSCR = 0x52435353; //"SSCR" = 53 53 43 52
const long int cSTAT = 0x54415453; //"STAT" = 53 54 41 54
const long int cSTRV = 0x56525453; //"STRV" = 53 54 52 56
const long int cTEXT = 0x54584554; //"TEXT" = 54 45 58 54
const long int cVCLR = 0x524C4356; //"VCLR" = 56 43 4C 52
const long int cVHGT = 0x54474856; //"VHGT" = 56 48 47 54
const long int cVNML = 0x4C4D4E56; //"VNML" = 56 4E 4D 4C
const long int cVTEX = 0x58455456; //"VTEX" = 56 54 45 58
const long int cWEAP = 0x50414557; //"WEAP" = 57 45 41 50
const long int cWEAT = 0x54414557; //"WEAT" = 57 45 41 54
const long int cWPDT = 0x54445057; //"WPDT" = 57 50 44 54
const long int cWNAM = 0x4D414E57; //"WNAM" = 57 4E 41 4D


//converting "integer strings" to real C++ string
std::string IntTo4Char(const long int value);
//helper function for message on wrong records
void UnexpectedRecord(const long int expected, const long int unexpected);

//"dispatcher"
bool ProcessNextRecord(std::ifstream& in_File, const int FileSize);

//routines for reading/ skipping different records
/*bool ReadACTI(std::ifstream& in_File, const long int FileSize);
bool ReadALCH(std::ifstream& in_File, const long int FileSize);
bool ReadAPPA(std::ifstream& in_File, const long int FileSize);
bool ReadARMO(std::ifstream& in_File, const long int FileSize);
bool ReadBODY(std::ifstream& in_File);
bool ReadBOOK(std::ifstream& in_File);
bool ReadCLAS(std::ifstream& in_File, const long int FileSize);
bool ReadCLOT(std::ifstream& in_File, const long int FileSize);
bool ReadCONT(std::ifstream& in_File, const long int FileSize);
bool ReadCREA(std::ifstream& in_File);
bool ReadDOOR(std::ifstream& in_File);
bool ReadENCH(std::ifstream& in_File, const long int FileSize);
bool ReadFACT(std::ifstream& in_File);
bool ReadGLOB(std::ifstream& in_File);
bool ReadGMST(std::ifstream& in_File, const long int FileSize);
bool ReadINGR(std::ifstream& in_File, const long int FileSize);*/
bool ReadLAND(std::ifstream& in_File);
/*bool ReadLIGH(std::ifstream& in_File, const long int FileSize);
bool ReadLOCK(std::ifstream& in_File, const long int FileSize);
bool ReadLTEX(std::ifstream& in_File);
bool ReadMGEF(std::ifstream& in_File);
bool ReadMISC(std::ifstream& in_File);
bool ReadPGRD(std::ifstream& in_File);
bool ReadPROB(std::ifstream& in_File, const long int FileSize);
bool ReadRACE(std::ifstream& in_File);
bool ReadREGN(std::ifstream& in_File);
bool ReadREPA(std::ifstream& in_File, const long int FileSize);
bool ReadSCPT(std::ifstream& in_File);
bool ReadSKIL(std::ifstream& in_File);
bool ReadSNDG(std::ifstream& in_File);
bool ReadSOUN(std::ifstream& in_File);
bool ReadSPEL(std::ifstream& in_File);
bool ReadSSCR(std::ifstream& in_File);
bool ReadSTAT(std::ifstream& in_File);
bool ReadWEAP(std::ifstream& in_File, const long int FileSize);*/
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
  long int Size, Header_One, Flags, SubSize;
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
  long int Unknown, NumRecords;
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
  return Dusk::Landscape::GetSingleton().SaveToFile(DuskFileName);
}

bool ProcessNextRecord(std::ifstream& in_File, const int FileSize)
{
  long int RecordName; //normally should be 4 char, but char is not eligible for switch
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

std::string IntTo4Char(const long int value)
{
  std::string result = "1234";

  result[0] = char(value&255);
  result[1] = char((value>>8)&255);
  result[2] = char((value>>16)&255);
  result[3] = char(value>>24);
  return result;
}

void UnexpectedRecord(const long int expected, const long int unexpected)
{
  std::cout << "Error: expected record name \""<<IntTo4Char(expected)
            <<"\" was not found. Instead, \""<<IntTo4Char(unexpected)
            <<"\" was found.\n";
  return;
}

/*bool ReadACTI(std::ifstream& in_File, const long int FileSize)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);*/

  /*Activators:
    NAME = Item ID, required
    MODL = Model Name, required
    FNAM = Item Name, required
    SCRI = Script Name (optional) */

  /*long int SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip activator ID

  //read MODL
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cMODL)
  {
    UnexpectedRecord(cMODL, SubRecName);
    return false;
  }
  //MODL's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur);//skip model path

  //read FNAM
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return false;
  }
  //FNAM's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur);//skip item name

  if (in_File.tellg()>=FileSize)
  {
    return in_File.good();
  }

  //read optional SCRI
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName==cSCRI)
  {
    //SCRI's length
    in_File.read((char*) &SubLength, 4);
    in_File.seekg(SubLength, std::ios::cur); //skip script ID
  }
  else
  {
    //seek four bytes towards beginning to land before the name of the next record
    in_File.seekg(-4, std::ios::cur);
  }
  return in_File.good();
}//ReadACTI
*/
/*
bool ReadALCH(std::ifstream& in_File, const long int FileSize)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);*/

  /*Alchemy Potions:
    NAME = Item ID, required
    MODL = Model Name, required
    FNAM = Item Name, required
    ALDT = Alchemy Data (12 bytes), required
        float  Weight
        long   Value
        long   AutoCalc
    ENAM = Enchantment (24 bytes) 1 to 8 per record
        short EffectID
        byte  SkillID		for skill related effects, -1/0 otherwise
        byte  AttributeID	for attribute related effects, -1/0 otherwise
        long  Unknown1
        long  Unknown2
        long  Duration
        long  Magnitude
        long  Unknown4
    TEXT = Inventory Icon
    SCRI = Script Name (optional) */

  /*long int SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip potion ID

  //read MODL
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cMODL)
  {
    UnexpectedRecord(cMODL, SubRecName);
    return false;
  }
  //MODL's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur);//skip potion's model path

  //read FNAM
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return false;
  }
  //FNAM's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur);//skip potion's "real" name

  //read ALDT
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cALDT)
  {
    UnexpectedRecord(cALDT, SubRecName);
    return false;
  }
  //ALDT's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=12)
  {
    std::cout <<"Error: sub record ALDT of ALCH has invalid length ("<<SubLength
              <<" bytes). Should be 12 bytes.\n";
    return false;
  }
  in_File.seekg(12, std::ios::cur);//skip data of potion

  //read multiple ENAM
  do
  {
    //read ENAM
    in_File.read((char*) &SubRecName, 4);
    if (SubRecName == cENAM)
    {
      //ENAM's length
      in_File.read((char*) &SubLength, 4);
      if (SubLength!=24)
      {
        std::cout <<"Error: sub record ENAM of ALCH has invalid length ("<<SubLength
                  <<" bytes). Should be 24 bytes.\n";
        return false;
      }
      in_File.seekg(24, std::ios::cur); //skip potion enchantment
    }
  } while (SubRecName == cENAM);

  //read TEXT (record name was already read in previous do-while-loop)
  if (SubRecName!=cTEXT)
  {
    UnexpectedRecord(cTEXT, SubRecName);
    return false;
  }
  //TEXT's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip potion icon path

  if (in_File.tellg()>=FileSize)
  { //there's no optional part, if end of file is already reached
    return in_File.good();
  }

  //read optional SCRI
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName==cSCRI)
  {
    //SCRI's length
    in_File.read((char*) &SubLength, 4);
    in_File.seekg(SubLength, std::ios::cur); //skip potion script ID
  }
  else
  {
    //seek four bytes towards beginning to land before next record name
    in_File.seekg(-4, std::ios::cur);
  }
  return in_File.good();
}//ReadALCH
*/
/*
bool ReadAPPA(std::ifstream& in_File, const long int FileSize)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);*/

  /*Alchemy Apparatus:
    NAME = Item ID, required
    MODL = Model Name, required
    FNAM = Item Name, required
    AADT = Alchemy Data (16 bytes), required
        long    Type (0=Mortar and Pestle,1=Albemic,2=Calcinator,3=Retort)
        float	Quality
        float	Weight
        long	Value
    ITEX = Inventory Icon
    SCRI = Script Name (optional) */

  /*long int SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip apparatus ID

  //read MODL
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cMODL)
  {
    UnexpectedRecord(cMODL, SubRecName);
    return false;
  }
  //MODL's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur);//skip model path

  //read FNAM
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return false;
  }
  //FNAM's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur);//skip apparatus name

  //read AADT
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cAADT)
  {
    UnexpectedRecord(cAADT, SubRecName);
    return false;
  }
  //AADT's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=16)
  {
    std::cout <<"Error: sub record AADT of APPA has invalid length ("<<SubLength
              <<" bytes). Should be 16 bytes.\n";
    return false;
  }
  in_File.seekg(16, std::ios::cur);//skip data of apparatus

  //read ITEX
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cITEX)
  {
    UnexpectedRecord(cITEX, SubRecName);
    return false;
  }
  //ITEX's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip apparatus icon path

  if (in_File.tellg()>=FileSize)
  { //there's no optional part, if end of file is already reached
    return in_File.good();
  }

  //read optional SCRI
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName==cSCRI)
  {
    //SCRI's length
    in_File.read((char*) &SubLength, 4);
    in_File.seekg(SubLength, std::ios::cur); //skip apparatus script ID
  }
  else
  {
    //seek four bytes towards beginning to land before next record name
    in_File.seekg(-4, std::ios::cur);
  }
  return in_File.good();
}//ReadAPPA
*/
/*
bool ReadARMO(std::ifstream& in_File, const long int FileSize)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);*/

  /*Armor pieces:
    NAME = Item ID, required
    MODL = Model Filename, required
    FNAM = Item Name, required
    AODT = Armour Data, required (24 bytes)
        long  Type (0 = Helmet,1 = Cuirass,2 = L. Pauldron,3 = R. Pauldron,
                    4 = Greaves,5 = Boots,6 = L. Gauntlet,7 = R. Gauntlet,
                    8 = Shield,9 = L. Bracer,10 = R. Bracer)
        float Weight
        long  Value
        long  Health
        long  EnchantPts
        long  Armour
    ITEX = Icon Filename, required
    INDX = Body Part Index (1 byte)
        (0=Head,1=Hair,2=Neck,3=Cuirass,4=Groin,5=Skirt,6=Right Hand,
         7=Left Hand,8=Right Wrist,9=Left Wrist,10=Shield,11=Right Forearm,
         12=Left Forearm,13=Right Upper Arm,14=Left Upper Arm,15=Right Foot,
         16=Left Foot,17=Right Ankle,18=Left Ankle,19=Right Knee,20=Left Knee,
         21=Right Upper Leg,22=Left Upper Leg,23=Right Pauldron,24=Left Pauldron,
         25=Weapon,26=Tail)
    BNAM = Male Part Name
    CNAM = Female Body Part Name (0 occurences)
        INDX and BNAM/CNAM are grouped together.  INDX first followed
        by an optional BNAM (no BNAM indicates a NULL field for that index).
        Up to 7 pairs allowed.
    SCRI = Script Name (optional)
    ENAM = Enchantment Name (optional) */

  /*long int SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip armour ID

  //read MODL
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cMODL)
  {
    UnexpectedRecord(cMODL, SubRecName);
    return false;
  }
  //MODL's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur);//skip MODL path

  //read FNAM
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return false;
  }
  //FNAM's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur);//armour's "real" name

  //read AODT
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cAODT)
  {
    UnexpectedRecord(cAODT, SubRecName);
    return false;
  }
  //AODT's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=24)
  {
    std::cout <<"Error: sub record AODT of ARMO has invalid length ("<<SubLength
              <<" bytes). Should be 24 bytes.\n";
    return false;
  }
  in_File.seekg(24, std::ios::cur);//skip armour data

  //read ITEX
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cITEX)
  {
    UnexpectedRecord(cITEX, SubRecName);
    return false;
  }
  //ITEX's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur);//skip path to armour icon texture


  //still need to implement INDX/BNAM-pairs here ****

  SubRecName = 0;
  do
  {
    if (in_File.tellg()>=FileSize)
    { //avoid reading non-existing, optional sub-records
      return in_File.good();
    }//if

    //read INDX
    in_File.read((char*) &SubRecName, 4);
    if (SubRecName==cINDX)
    {
      //INDX's length
      in_File.read((char*) &SubLength, 4);
      if (SubLength!=1)
      {
        std::cout <<"Error: sub record INDX of ARMO has invalid length ("<<SubLength
                  <<" bytes). Should be one byte only.\n";
        return false;
      }
      in_File.seekg(1, std::ios::cur);//skip body part index

      if (in_File.tellg()>=FileSize)
      { //avoid reading non-existing, optional sub-records; BNAM must not follow
        return in_File.good();
      }//if

      //read BNAM
      in_File.read((char*) &SubRecName, 4);
      if (SubRecName==cBNAM)
      {
        //BNAM's length
        in_File.read((char*) &SubLength, 4);
        in_File.seekg(SubLength, std::ios::cur);//skip body part Name
      }
      else
      { //land before next INDX
        in_File.seekg(-4, std::ios::cur);
      }
    }//if INDX read
    else
    {
      //seek four bytes towards beginning to land before next (sub-)record name
      in_File.seekg(-4, std::ios::cur);
      //set SubRecName to zero to exit loop
      SubRecName = 0;
    }
  } while (SubRecName == cBNAM or SubRecName == cINDX);

  //still need to implement INDX/BNAM-pairs above here ****

  if (in_File.tellg()>=FileSize)
  { //avoid reading non-existing, optional sub-records
    return in_File.good();
  }

  //read optional SCRI
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName==cSCRI)
  {
    //SCRI's length
    in_File.read((char*) &SubLength, 4);
    in_File.seekg(SubLength, std::ios::cur); //skip armour script name
    if (in_File.tellg()>=FileSize)
    { //avoid reading non-existing, optional sub-records
      return in_File.good();
    }
    //read next sub-record name
    in_File.read((char*) &SubRecName, 4);
  }
  if (SubRecName==cENAM)
  {
    //ENAM's length
    in_File.read((char*) &SubLength, 4);
    in_File.seekg(SubLength, std::ios::cur); //skip armour script name
  }
  else
  {
    //seek four bytes towards beginning to land before next record name
    in_File.seekg(-4, std::ios::cur);
  }
  return in_File.good();
}//ReadARMO
*/
/*
bool ReadBODY(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);*/

  /*
  NAME = Body Part ID
  MODL = Path to Mesh
  FNAM = Race ID
  BYDT = Body part data (4 bytes)
  */

  /*long int SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip name

  //read MODL
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cMODL)
  {
    UnexpectedRecord(cMODL, SubRecName);
    return false;
  }
  //MODL's length
  in_File.read((char*) &SubLength, 4);
  //skip MODL
  in_File.seekg(SubLength, std::ios::cur);

  //read Race Name(?) FNAM
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return false;
  }
  //FNAM's length
  in_File.read((char*) &SubLength, 4);
  //skip FNAM
  in_File.seekg(SubLength, std::ios::cur);

  //read Body part data BYDT
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cBYDT)
  {
    UnexpectedRecord(cBYDT, SubRecName);
    return false;
  }
  //BYDT's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength != 4)
  {
    std::cout << "Error: Sub record BYDT has invalid length ("<<SubLength
              << " bytes), should be 4 bytes.\n";
    return false;
  }
  //skip BYDT
  in_File.seekg(4, std::ios::cur);
  return in_File.good();
}//ReadBODY
*/
/*
bool ReadBOOK(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);*/

  /*Books:
    NAME = Item ID, required
    MODL = Model Name, required
    FNAM = Item Name, required
    BKDT = Book Data (20 bytes), required
        float  Weight
        long   Value
        long   Scroll	(1 is scroll, 0 not)
        long   SkillID	(-1 is no skill)
        long   EnchantPts
    ITEX = Inventory Icon
    SCRI = Script Name (optional)
    TEXT = Book text */

  /*long int SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip book ID

  //read MODL
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cMODL)
  {
    UnexpectedRecord(cMODL, SubRecName);
    return false;
  }
  //MODL's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip model path

  //read FNAM
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return false;
  }
  //FNAM's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip book's name

  //read BKDT
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cBKDT)
  {
    UnexpectedRecord(cBKDT, SubRecName);
    return false;
  }
  //BKDT's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip book data

  //read ITEX
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cITEX)
  {
    UnexpectedRecord(cITEX, SubRecName);
    return false;
  }
  //ITEX's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip inventory icon

  //read SCRI (optional)
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName==cSCRI)
  {
    //SCRI's length
    in_File.read((char*) &SubLength, 4);
    in_File.seekg(SubLength, std::ios::cur); //skip script name
    //read next record name
    in_File.read((char*) &SubRecName, 4);
  }

  //read TEXT
  if (SubRecName!=cTEXT)
  {
    UnexpectedRecord(cTEXT, SubRecName);
    return false;
  }
  //TEXT's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip text of book

  return in_File.good();
}//ReadBOOK
*/
/*
bool ReadCLAS(std::ifstream& in_File, const long int FileSize)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);*/

  /*Class definition
    NAME = Class ID string
    FNAM = Class name string
    CLDT = Class Data (60 bytes)
        long AttributeID1
        long AttributeID2
        long Specialization (0 = Combat,1 = Magic,2 = Stealth)
        long MinorID1
        long MajorID1
        long MinorID2
        long MajorID2
        long MinorID3
        long MajorID3
        long MinorID4
        long MajorID4
        long MinorID5
        long MajorID5
        long Flags (0x0001 = Playable)
        long AutoCalcFlags (0x00001 = Weapon,0x00002 = Armor,0x00004 = Clothing
                            0x00008 = Books,0x00010 = Ingrediant,0x00020 = Picks
                            0x00040 = Probes,0x00080 = Lights,0x00100 = Apparatus
                            0x00200 = Repair,0x00400 = Misc,0x00800 = Spells
                            0x01000 = Magic Items,0x02000 = Potions,0x04000 = Training
                            0x08000 = Spellmaking,0x10000 = Enchanting,0x20000 = Repair Item)
    DESC = Description string (optional for non-playable classes) */

  /*long int SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip class ID

  //read FNAM
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return false;
  }
  //FNAM's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip class name

  //read CLDT
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cCLDT)
  {
    UnexpectedRecord(cCLDT, SubRecName);
    return false;
  }
  //CLDT's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=60)
  {
    std::cout <<"Error: sub record CLDT of CLAS has invalid length ("<<SubLength
              <<" bytes). Should be 60 bytes.\n";
    return false;
  }
  in_File.seekg(60, std::ios::cur); //skip class data

  if (in_File.tellg()>=FileSize)
  {
    return in_File.good();
  }

  //read optional DESC
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName==cDESC)
  {
    //DESC's length
    in_File.read((char*) &SubLength, 4);
    in_File.seekg(SubLength, std::ios::cur); //skip class description string
  }
  else
  { //seek four bytes towards beginning to land before next record name
    in_File.seekg(-4, std::ios::cur);
  }
  return in_File.good();
}//ReadCLAS
*/
/*
bool ReadCLOT(std::ifstream& in_File, const long int FileSize)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);*/

  /*Clothing:
    NAME = Item ID, required
    MODL = Model Name, required
    FNAM = Item Name, required
    CTDT = Clothing Data (12 bytes), required
        long  Type (0=Pants,1=Shoes,2=Shirt,3=Belt,4=Robe,5=Right Glove,
                    6=Left Glove,7=Skirt,8=Ring,9=Amulet)
        float Weight
        short Value
        short EnchantPts
    ITEX = Inventory Icon
    INDX = Body Part Index (1 byte)
    BNAM = Male Body Part Name
    CNAM = Female Body Part Name
        INDX and BNAM/CNAM are grouped together.  INDX first followed
        by an optional BNAM (no BNAM indicates a NULL field for that index).
        Up to 7 pairs allowed.
    ENAM = Enchantment Name (optional)
    SCRI = Script Name (optional) */

  /*long int SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip clothing name

  //read MODL
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cMODL)
  {
    UnexpectedRecord(cMODL, SubRecName);
    return false;
  }
  //MODL's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur);//skip path to clothing model

  //read FNAM
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return false;
  }
  //FNAM's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur);//skip clothing name

  //read CTDT
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cCTDT)
  {
    UnexpectedRecord(cCTDT, SubRecName);
    return false;
  }
  //CTDT's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=12)
  {
    std::cout <<"Error: sub record CTDT of CLOT has invalid length ("<<SubLength
              <<" bytes). Should be 12 bytes.\n";
    return false;
  }
  in_File.seekg(12, std::ios::cur);//skip clothing data

  //read ITEX
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cITEX)
  {
    UnexpectedRecord(cITEX, SubRecName);
    return false;
  }
  //ITEX's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur);//skip path to clothing icon texture

  // ************************************************* INDX/ BNAM

  SubRecName = 0;
  do
  {
    if (in_File.tellg()>=FileSize)
    { //avoid reading non-existing, optional sub-records
      return in_File.good();
    }//if

    //read INDX
    in_File.read((char*) &SubRecName, 4);
    if (SubRecName==cINDX)
    {
      //INDX's length
      in_File.read((char*) &SubLength, 4);
      if (SubLength!=1)
      {
        std::cout <<"Error: sub record INDX of CLOT has invalid length ("<<SubLength
                  <<" bytes). Should be one byte only.\n";
        return false;
      }
      in_File.seekg(1, std::ios::cur);//skip body part index

      if (in_File.tellg()>=FileSize)
      { //avoid reading non-existing, optional sub-records; BNAM must not follow
        return in_File.good();
      }//if

      //read BNAM
      in_File.read((char*) &SubRecName, 4);
      if (SubRecName==cBNAM)
      {
        //BNAM's length
        in_File.read((char*) &SubLength, 4);
        in_File.seekg(SubLength, std::ios::cur);//skip body part name
      }
      else
      { //land before next INDX
        in_File.seekg(-4, std::ios::cur);
      }
    }//if INDX read
    else
    {
      //seek four bytes towards beginning to land before next (sub-)record name
      in_File.seekg(-4, std::ios::cur);
      //set SubRecName to zero to exit loop
      SubRecName = 0;
    }
  } while (SubRecName == cBNAM or SubRecName == cINDX);

  // ************************************************* INDX/ BNAM

  if (in_File.tellg()>=FileSize)
  { //avoid reading non-existing, optional sub-records
    return in_File.good();
  }

  //read optional SCRI
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName==cSCRI)
  {
    //SCRI's length
    in_File.read((char*) &SubLength, 4);
    in_File.seekg(SubLength, std::ios::cur); //skip clothing script name
    if (in_File.tellg()>=FileSize)
    { //avoid reading non-existing, optional sub-records
      return in_File.good();
    }
    //read next sub-record name
    in_File.read((char*) &SubRecName, 4);
  }
  //optional ENAM
  if (SubRecName==cENAM)
  {
    //ENAM's length
    in_File.read((char*) &SubLength, 4);
    in_File.seekg(SubLength, std::ios::cur); //skip clothing enchantment name
  }
  else
  {
    //seek four bytes towards beginning to land before next record name
    in_File.seekg(-4, std::ios::cur);
  }
  return in_File.good();
}//ReadCLOT
*/
/*
bool ReadCONT(std::ifstream& in_File, const long int FileSize)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);*/

  /*Container:
    NAME = ID
    MODL = NIF Model
    FNAM = Container name
    CNDT = Container data (4 bytes)
        float Weight
    FLAG = Container flags (4 bytes, bit-field)
        (0x0001=Organic;0x0002=Respawns, organic only;0x0008=Default, unknown)
    SCRI = script name (optional)
    NPCO = An item record (36 bytes, 0+ times)
        long	Count	  Number of the item
        char	Name[32]  The ID of the item */

  /*long int SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip name

  //read MODL
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cMODL)
  {
    UnexpectedRecord(cMODL, SubRecName);
    return false;
  }
  //MODL's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip model

  //read FNAM
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return false;
  }
  //FNAM's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip container name

  //read CNDT
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cCNDT)
  {
    UnexpectedRecord(cCNDT, SubRecName);
    return false;
  }
  //CNDT's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=4)
  {
    std::cout <<"Error: sub record CNDT of CONT has invalid length ("<<SubLength
              <<" bytes). Should be 4 bytes.\n";
    return false;
  }
  in_File.seekg(4, std::ios::cur); //skip Weight

  //read FLAG
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cFLAG)
  {
    UnexpectedRecord(cFLAG, SubRecName);
    return false;
  }
  //FLAG's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=4)
  {
    std::cout <<"Error: sub record FLAG of CONT has invalid length ("<<SubLength
              <<" bytes). Should be 4 bytes.\n";
    return false;
  }
  in_File.seekg(4, std::ios::cur); //skip flags

  //read optional SCRI
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName==cSCRI)
  {
    //SCRI's length
    in_File.read((char*) &SubLength, 4);
    in_File.seekg(SubLength, std::ios::cur); //skip container script
  }
  else
  { //seek four bytes towards beginning to land before next record name
    in_File.seekg(-4, std::ios::cur);
  }

  //"init" SubRecName to be not cNPCO
  SubRecName = 0;
  //read multiple NPCO sub-records
  do
  {
    if (in_File.tellg()<FileSize)
    {
      //read NPCO
      in_File.read((char*) &SubRecName, 4);
      if (SubRecName==cNPCO)
      {
        //NPCO's length
        in_File.read((char*) &SubLength, 4);
        if (SubLength!=36)
        {
          std::cout <<"Error: sub record FLAG of CONT has invalid length ("
                    <<SubLength<<" bytes). Should be 36 bytes.\n";
          return false;
        }
        in_File.seekg(36, std::ios::cur); //skip count and name
      }//if
    }//if
    else
    { SubRecName = 0; }
  } while (SubRecName == cNPCO);
  if (SubRecName!=0)
  {
    //seek 4 bytes towards beginning to land before next record name
    in_File.seekg(-4, std::ios::cur);
  }

  return in_File.good();
}//ReadCONT
*/
/*
bool ReadDOOR(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);*/

  /*Door:
    NAME = door ID
    FNAM = door name
    MODL = NIF model filename
    SCRI = Script (optional)
    SNAM = Sound name open (optional)
    ANAM = Sound name close (optional)
  */

  /*long int SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read NAME, MODL, FNAM
  bool go_on_with_door = true;
  unsigned short int RecordsDone = 0;
  do
  {
    //read NAME/ MODL/ FNAM
    in_File.read((char*) &SubRecName, 4);
    switch(SubRecName)
    {
      case cNAME:
      case cMODL:
      case cFNAM:
      case cANAM: //  }
      case cSCRI: //  }  optional part
      case cSNAM: //  }
           //NAME's/ MODL's/ FNAM's/ SCRI's/ SNAM's/ ANAM's length
           in_File.read((char*) &SubLength, 4);
           //skip ID/ model path/ door name
           in_File.seekg(SubLength, std::ios::cur);
           RecordsDone++;
           break;
      default:
           //unknown record
           go_on_with_door = false;
    }//swi
  } while (go_on_with_door and RecordsDone<=6);

  //seek 4 bytes towards beginning to land before next record name
  in_File.seekg(-4, std::ios::cur);

  if (RecordsDone<3)
  {
    std::cout << "Error: too few sub-records of door were read.\n Next unknown"
              << " sub-record is \""<<IntTo4Char(SubRecName)<<"\".\n";
    return false;
  }//if

  return in_File.good();
}//ReadDOOR
*/
/*
bool ReadENCH(std::ifstream& in_File, const long int FileSize)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);*/

  /*Enchantment:
    NAME = ID string
    ENDT = Enchant Data (16 bytes)
        long Type(0=Cast Once,1=Cast Strikes,2=Cast when Used,3=Constant Effect)
        long EnchantCost
        long Charge
        long AutoCalc
    ENAM = Single enchantment data (24 bytes), can occur multiple times
        short EffectID
        byte  SkillID     (-1 if NA)
        byte  AttributeID (-1 if NA)
        long  RangeType (0 = Self,1 = Touch,2 = Target)
        long  Area
        long  Duration
        long  MagMin
        long  MagMax */

  /*long int SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip enchantment ID

  //read ENDT
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //ENDT's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=16)
  {
    std::cout <<"Error: sub record ENDT of ENCH has invalid length ("
             <<SubLength<<" bytes). Should be 16 bytes.\n";
    return false;
  }
  in_File.seekg(16, std::ios::cur); //skip enchantment data

  //read multiple ENAMs
  do
  {
    if (in_File.tellg()>=FileSize)
    {
      return in_File.good();
    }
    //read ENAM
    in_File.read((char*) &SubRecName, 4);
    if (SubRecName==cENAM)
    {
      //ENAM's length
      in_File.read((char*) &SubLength, 4);
      if (SubLength!=24)
      {
        std::cout <<"Error: sub record ENAM of ENCH has invalid length ("
                 <<SubLength<<" bytes). Should be 24 bytes.\n";
        return false;
      }
      in_File.seekg(24, std::ios::cur); //skip enchantment data2
    }
    else
    { //seek four bytes towards beginning to land before next record
      in_File.seekg(-4, std::ios::cur);
    }
  } while (SubRecName == cENAM);

  return in_File.good();
}//ReadENCH
*/
/*
bool ReadFACT(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);*/

  /*Faction:
    NAME = Faction ID string
    FNAM = Faction name string
    RNAM = Rank Name (32 bytes)
        Occurs 10 times for each rank in order
    FADT = Faction data (240 bytes)
        long AttributeID1
        long AttributeID2
        RankData[10]
            long Attribute1
            long Attribute2
            long FirstSkill
            long SecondSkill
            long Faction
        long SkillID[6]
        long Unknown1 (-1)?
        long Flags (1 = Hidden from Player)
    ANAM = Faction name string
    INTV = Faction reaction value (4 bytes, long)
        The ANAM/INTV occur in pairs for each faction with
        a reaction adjustment (usually -4 to +4) */

  /*long int SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip name

  //read FNAM
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return false;
  }
  //FNAM's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip name

  //read flexible number of rank names (doesn't need to be the same all the time)
  do
  {
    //read RNAM
    in_File.read((char*) &SubRecName, 4);
    if (SubRecName==cRNAM)
    {
      //RNAM's length
      in_File.read((char*) &SubLength, 4);
      in_File.seekg(SubLength, std::ios::cur); //skip name
    }
  } while (SubRecName == cRNAM);*/

  /* //seek 4 bytes towards beginning to land before next record name
  in_File.seekg(-4, std::ios::cur);

  //read FADT
  in_File.read((char*) &SubRecName, 4);*/

  //last SubRecord read in do-while-loop is already the next (FADT)

  /*if (SubRecName!=cFADT)
  {
    UnexpectedRecord(cFADT, SubRecName);
    return false;
  }
  //FADT's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip faction data

  //alignment data
  do
  {
    //read ANAM
    in_File.read((char*) &SubRecName, 4);
    if (SubRecName==cANAM)
    {
      //ANAM's length
      in_File.read((char*) &SubLength, 4);
      in_File.seekg(SubLength, std::ios::cur); //skip name

      //read INTV
      in_File.read((char*) &SubRecName, 4);
      if (SubRecName!=cINTV)
      {
        UnexpectedRecord(cINTV, SubRecName);
        return false;
      }
      //INTV's length
      in_File.read((char*) &SubLength, 4);
      if (SubLength!=4)
      {
        std::cout <<"Error: sub record INTV of FACT has invalid length ("
                  <<SubLength<<" bytes). Should be four bytes.\n";
        return false;
      }
      in_File.seekg(4, std::ios::cur); //skip integer value
    }
  } while (SubRecName == cINTV);
  //seek 4 bytes towards beginning to land before next record name
  in_File.seekg(-4, std::ios::cur);
  return in_File.good();
}//ReadFACT
*/
/*
bool ReadGLOB(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);*/

  /*Global variable:
    NAME = Global ID
    FNAM = Type of global (1 byte)
        's' = short
        'l' = long
        'f' = float
    FLTV = Float data (4 bytes) */
  /*long int SubRecName, SubLength;
  SubRecName = SubLength = 0;
  char TypeChar = '\0';

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip name

  //read FNAM
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return false;
  }
  //FNAM's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=1)
  {
    std::cout <<"Error: sub record FNAM of GLOB has invalid length ("<<SubLength
              <<" bytes). Should be one byte.\n";
    return false;
  }
  in_File.read(&TypeChar, 1);
  if (TypeChar != 'f' && TypeChar != 'l' && TypeChar != 's')
  {
    std::cout <<"Error: sub record FNAM of GLOB has invalid value ("<<TypeChar
              <<"). Should be 'f', 'l' or 's'.\n";
    return false;
  }

  //read FLTV
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cFLTV)
  {
    UnexpectedRecord(cFLTV, SubRecName);
    return false;
  }
  //FLTV's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=4)
  {
    std::cout <<"Error: sub record FLTV of GLOB has invalid length ("<<SubLength
              <<" bytes). Should be four bytes.\n";
    return false;
  }
  in_File.seekg(4, std::ios::cur); //skip value

  return in_File.good();
}//ReadGLOB
*/
/*
bool ReadGMST(std::ifstream& in_File, const long int FileSize)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);*/

  /*GMST: Game Setting
    NAME = Setting ID string
    STRV = String value/ INTV = Integer value (4 btes)/FLTV = Float value (4 bytes)
  */
  /*long int SubRecName, SubLength;
  SubRecName = SubLength = 0;
  char NameFirstChar = '\0';

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  in_File.read(&NameFirstChar, 1);
  in_File.seekg(SubLength-1, std::ios::cur); //skip rest of name

  //read value record based on first character of setting name
  switch(NameFirstChar)
  {
    case 'f':
         //read FLTV
         in_File.read((char*) &SubRecName, 4);
         if (SubRecName!=cFLTV)
         {
           UnexpectedRecord(cFLTV, SubRecName);
           return false;
         }
         //FLTV's length
         in_File.read((char*) &SubLength, 4);
         if (SubLength != 4)
         {
           std::cout << "Error: sub record FLTV has invalid length ("<<SubLength
                     << " bytes). Should be 4 bytes.\n";
           return false;
         }
         //skip FLTV
         in_File.seekg(4, std::ios::cur);
         break;
    case 'i':
         //read INTV
         in_File.read((char*) &SubRecName, 4);
         if (SubRecName!=cINTV)
         {
           UnexpectedRecord(cINTV, SubRecName);
           return false;
         }
         //INTV's length
         in_File.read((char*) &SubLength, 4);
         if (SubLength != 4)
         {
           std::cout << "Error: sub record INTV has invalid length ("<<SubLength
                     << " bytes). Should be 4 bytes.\n";
           return false;
         }
         //skip INTV
         in_File.seekg(4, std::ios::cur);
         break;
    default: //'s'
         // if string is empty, STRV sub-record may not be present at all
         if (in_File.tellg()>=FileSize)
         {
           return in_File.good();
         }
         //read STRV
         in_File.read((char*) &SubRecName, 4);
         if (SubRecName==cSTRV)
         {
           //STRV's length
           in_File.read((char*) &SubLength, 4);
           in_File.seekg(SubLength, std::ios::cur);//skip STRV
         }
         else
         { //seek four bytes towards beginning to be before next record name
           in_File.seekg(-4, std::ios::cur);
         }
         break;
  }//switch
  return in_File.good();
}//ReadGMST
*/
/*
bool ReadINGR(std::ifstream& in_File, const long int FileSize)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);*/

  /*Ingredients:
    NAME = Item ID, required
    MODL = Model Name, required
    FNAM = Item Name, required
    IRDT = Ingredient Data (56 bytes), required
        float  Weight
        long   Value
        long   EffectID[4]	0 or -1 means no effect
        long   SkillID[4]	only for Skill related effects, 0 or -1 otherwise
        long   AttributeID[4]  only for Attribute related effects, 0 or -1 otherwise
    ITEX = Inventory Icon
    SCRI = Script Name (optional) */

  /*long int SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip ID of ingredient

  //read MODL
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cMODL)
  {
    UnexpectedRecord(cMODL, SubRecName);
    return false;
  }
  //MODL's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip model path

  //read FNAM
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return false;
  }
  //FNAM's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur);//skip "real" name of ingred.

  //read IRDT
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cIRDT)
  {
    UnexpectedRecord(cIRDT, SubRecName);
    return false;
  }
  //IRDT's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=56)
  {
    std::cout <<"Error: sub record IRDT of INGR has invalid length ("<<SubLength
              <<" bytes). Should be 56 bytes.\n";
    return false;
  }
  in_File.seekg(56, std::ios::cur);//skip ingrediant data

  //read ITEX
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cITEX)
  {
    UnexpectedRecord(cITEX, SubRecName);
    return false;
  }
  //ITEX's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip icon texture path

  if (in_File.tellg()>=FileSize)
  {
    return in_File.good();
  }

  //read optional SCRI
  in_File.read((char*) &SubLength, 4);
  if (SubRecName==cSCRI)
  {
    //SCRI's length
    in_File.read((char*) &SubLength, 4);
    in_File.seekg(SubLength, std::ios::cur); //skip ingredient's script name
  }
  else
  { //search four bytes towards beginning to land before name of next record
    in_File.seekg(-4, std::ios::cur);
  }
  return in_File.good();
}//ReadINGR
*/

bool ReadLAND(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
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

  long int SubRecName, SubLength;
  long int CellX, CellY;
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
  DuskLand = Dusk::Landscape::GetSingleton().CreateRecord();

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
  DuskLand->Stride = Dusk::cDefaultStride;
  DuskLand->OffsetX = Dusk::cDefaultStride * 64 * CellX;
  DuskLand->OffsetY = Dusk::cDefaultStride * 64 * CellY;
  DuskLand->SetLoadedState(true);
  std::cout << "Debug: conversion finished.\n";

  return in_File.good();
}//ReadLAND

/*
bool ReadLIGH(std::ifstream& in_File, const long int FileSize)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);*/

  /*Lights:
    NAME = ID string
    MODL = NIF model name
    FNAM = Item name (optional)
    ITEX = Inventory icon (optional)
    LHDT = Light data (24 bytes)
        float Weight
        long  Value
        long  Time
        long  Radius
        byte  Red	}
        byte  Green	}
        byte  Blue	} long ColorRef?
        byte  Null	}
        long  Flags (0x0001 = Dynamic,0x0002 = Can Carry,0x0004 = Negative,
                     0x0008 = Flicker,0x0010 = Fire,0x0020 = Off Default,
                     0x0040 = Flicker Slow,0x0080 = Pulse,0x0100 = Pulse Slow)
    SCRI = Script name (optional)
    SNAM = Sound name (optional?) */

  /*long int SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip light's ID

  //read MODL
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cMODL)
  {
    UnexpectedRecord(cMODL, SubRecName);
    return false;
  }
  //MODL's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip light's model path

  //read optional FNAM sub-record
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName==cFNAM)
  {
    //FNAM's length
    in_File.read((char*) &SubLength, 4);
    in_File.seekg(SubLength, std::ios::cur); //skip light's "real" name
    //read next (optional) sub-record name
    in_File.read((char*) &SubRecName, 4);
  }

  //skip ITEX
  if (SubRecName==cITEX)
  {
    //ITEX's length
    in_File.read((char*) &SubLength, 4);
    in_File.seekg(SubLength, std::ios::cur); //skip light's inventory icon path
    //read next sub-record name
    in_File.read((char*) &SubRecName, 4);
  }

  //read LHDT (already read before)
  if (SubRecName!=cLHDT)
  {
    UnexpectedRecord(cLHDT, SubRecName);
    return false;
  }
  //LHDT's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=24)
  {
    std::cout <<"Error: sub record LHDT of LIGH has invalid length ("<<SubLength
              <<" bytes). Should be 24 bytes.\n";
    return false;
  }
  in_File.seekg(24, std::ios::cur); //skip light data

  if (in_File.tellg()>=FileSize)
  { //avoid to read non-present optional records
    return in_File.good();
  }

  //read optional SCRI
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName==cSCRI)
  {
    //SCRI's length
    in_File.read((char*) &SubLength, 4);
    in_File.seekg(SubLength, std::ios::cur); //skip light's script ID
    if (in_File.tellg()>=FileSize)
    {
      return in_File.good();
    }//if
    //read next (optional) record
    in_File.read((char*) &SubLength, 4);
  }

  //read optional SNAM
  if (SubRecName==cSNAM)
  {
    //SNAM's length
    in_File.read((char*) &SubLength, 4);
    in_File.seekg(SubLength, std::ios::cur); //skip light's sound name
  }
  else
  { //search four bytes towards beginning to land before name of next record
    in_File.seekg(-4, std::ios::cur);
  }
  return in_File.good();
}//ReadLIGH
*/
/*
bool ReadLOCK(std::ifstream& in_File, const long int FileSize)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);*/

  /*Lockpicks:
    NAME = Item ID, required
	MODL = Model Name, required
	FNAM = Item Name, required
	LKDT = Lock Data (16 bytes), required
		float	Weight
		long	Value
		float	Quality
		long 	Uses
	ITEX = Inventory Icon
	SCRI = Script Name (optional) */

  /*long int SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip lockpick's ID

  //read MODL
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cMODL)
  {
    UnexpectedRecord(cMODL, SubRecName);
    return false;
  }
  //MODL's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip lockpick's model path

  //read FNAM
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return false;
  }
  //FNAM's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip lockpick's real name

  //read LKDT
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cLKDT)
  {
    UnexpectedRecord(cLKDT, SubRecName);
    return false;
  }
  //LKDT's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=16)
  {
    std::cout <<"Error: sub record LKDT of LOCK has invalid length ("<<SubLength
              <<" bytes). Should be 16 bytes.\n";
    return false;
  }
  in_File.seekg(16, std::ios::cur); //skip lockpick data

  //read ITEX
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cITEX)
  {
    UnexpectedRecord(cITEX, SubRecName);
    return false;
  }
  //ITEX's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip lockpick's icon texture path

  if (in_File.tellg()>=FileSize)
  {
    return in_File.good();
  }

  //read optional SCRI
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName==cSCRI)
  {
    //SCRI's length
    in_File.read((char*) &SubLength, 4);
    in_File.seekg(SubLength, std::ios::cur); //skip lockpick's script name
  }
  else
  {
    //seek four bytes towards beginning to land before the name of the next record
    in_File.seekg(-4, std::ios::cur);
  }
  return in_File.good();
}//ReadLOCK
*/
/*
bool ReadLTEX(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);*/

  /*Land texture(?): (no documentation known)
    NAME = texture name/ID (?)
    INTV = texture index (?)
    DATA = path to texture */

  /*long int SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip name

  //read INTV
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cINTV)
  {
    UnexpectedRecord(cINTV, SubRecName);
    return false;
  }
  //INTV's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=4)
  {
    std::cout <<"Error: sub record INTV of LTEX has invalid length ("<<SubLength
              <<" bytes). Should be 4 bytes.\n";
    return false;
  }
  in_File.seekg(SubLength, std::ios::cur); //skip integer value

  //read DATA
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cDATA)
  {
    UnexpectedRecord(cDATA, SubRecName);
    return false;
  }
  //DATA's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip texture path

  return in_File.good();
}//Read LTEX
*/
/*
bool ReadMGEF(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);*/

  /*Magic effects:
    INDX = The Effect ID (4 bytes, long)
    MEDT = Effect Data (36 bytes)
        long  SpellSchool (0 = Alteration,1 = Conjuration,2 = Destruction,
                           3 = Illusion,4 = Mysticism,5 = Restoration)
        float BaseCost
        long  Flags (0x0200 = Spellmaking,0x0400 = Enchanting,0x0800 = Negative)
        long  Red
        long  Blue
        long  Green
        float SpeedX
        float SizeX
        float SizeCap
    ITEX = Effect Icon string
    PTEX = Particle texture string
    CVFX = Casting visual string (optional?)
    BVFX = Bolt visual string (optional?)
    HVFX = Hit visual string (optional?)
    AVFX = Area visual string (optional?)
    DESC = Description text (optional?
    CSND = Cast sound (optional)
    BSND = Bolt sound (optional)
    HSND = Hit sound (optional)
    ASND = Area sound (optional)
      Note (Thoronador):
        sequence of the (at least) last four optional sub records can vary.*/

  /*long int SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read INDX
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cINDX)
  {
    UnexpectedRecord(cINDX, SubRecName);
    return false;
  }
  //INDX's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=4)
  {
    std::cout << "Error: sub record INDX of MGEF has invalid length ("
              <<SubLength<< " bytes). Should be 4 bytes.\n";
    return false;
  }
  in_File.seekg(4, std::ios::cur); //skip the index

  //read MEDT
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cMEDT)
  {
    UnexpectedRecord(cMEDT, SubRecName);
    return false;
  }
  //MEDT's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=36)
  {
    std::cout << "Error: sub record MEDT of MGEF has invalid length ("
              <<SubLength<< " bytes). Should be 36 bytes.\n";
    return false;
  }
  in_File.seekg(36, std::ios::cur); //skip the data

  //read ITEX
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cITEX)
  {
    UnexpectedRecord(cITEX, SubRecName);
    return false;
  }
  //ITEX's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip tex name

  //read PTEX
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cPTEX)
  {
    UnexpectedRecord(cPTEX, SubRecName);
    return false;
  }
  //PTEX's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip tex name

  //read optional CVFX
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName==cCVFX)
  {
    //CVFX's length
    in_File.read((char*) &SubLength, 4);
    in_File.seekg(SubLength, std::ios::cur); //skip effect string
    //read next optional header
    in_File.read((char*) &SubRecName, 4);
  }
  //read optional BVFX
  if (SubRecName==cBVFX)
  {
    //BVFX's length
    in_File.read((char*) &SubLength, 4);
    in_File.seekg(SubLength, std::ios::cur); //skip effect string
    //read next optional header
    in_File.read((char*) &SubRecName, 4);
  }
  //read optional HVFX
  if (SubRecName==cHVFX)
  {
    //HVFX's length
    in_File.read((char*) &SubLength, 4);
    in_File.seekg(SubLength, std::ios::cur); //skip effect string
    //read next optional header
    in_File.read((char*) &SubRecName, 4);
  }
  //read optional AVFX
  if (SubRecName==cAVFX)
  {
    //AVFX's length
    in_File.read((char*) &SubLength, 4);
    in_File.seekg(SubLength, std::ios::cur); //skip effect string
    //read next header (DESC?)
    in_File.read((char*) &SubRecName, 4);
  }

  //read optional DESC
  if (SubRecName==cDESC)
  {
    //DESC's length
    in_File.read((char*) &SubLength, 4);
    in_File.seekg(SubLength, std::ios::cur); //skip effect description
    //read next optional header
    in_File.read((char*) &SubRecName, 4);
  }

  //effect sounds, all optional
  bool go_on_with_sounds = true;
  do
  {
    switch(SubRecName)
    {
      case cCSND:
           //CSND's length
           in_File.read((char*) &SubLength, 4);
           in_File.seekg(SubLength, std::ios::cur); //skip sound string
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cBSND:
           //BSND's length
           in_File.read((char*) &SubLength, 4);
           in_File.seekg(SubLength, std::ios::cur); //skip sound string
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cHSND:
           //HSND's length
           in_File.read((char*) &SubLength, 4);
           in_File.seekg(SubLength, std::ios::cur); //skip sound string
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cASND:
           //ASND's length
           in_File.read((char*) &SubLength, 4);
           in_File.seekg(SubLength, std::ios::cur); //skip sound string
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      default:
           //unknown sub record, thus stop here, it's beginning of new record
           go_on_with_sounds = false;
    }//switch
  } while (go_on_with_sounds);

  //seek four bytes into the direction if the beginning to land before the
  // name of the next record
  in_File.seekg(-4, std::ios::cur);
  return in_File.good();
}//ReadMGEF
*/
/*
bool ReadMISC(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);*/

  /*Misc. Items:
    Misc Items
    NAME = item ID, required
    MODL = model filename, required
    FNAM = item name
    MCDT = Data, 12 bytes binary, required
        float Weight
        long  Value
        long  Unknown
    ITEX = Inventory icon filename
    ENAM = Enchantment ID string??? (optional)
    SCRI = script ID string (optional)
  */

  /*long int SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip region ID

  //read MODL
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cMODL)
  {
    UnexpectedRecord(cMODL, SubRecName);
    return false;
  }
  //MODL's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip model path

  //read FNAM
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return false;
  }
  //FNAM's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip region name

  //read MCDT
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cMCDT)
  {
    UnexpectedRecord(cMCDT, SubRecName);
    return false;
  }
  //MCDT's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=12)
  {
    std::cout << "Error: sub record MCDT of MISC has invalid length ("
              <<SubLength<< " bytes). Should be 12 bytes.\n";
    return false;
  }
  in_File.seekg(12, std::ios::cur); //skip misc. data

  //read ITEX
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cITEX)
  {
    UnexpectedRecord(cITEX, SubRecName);
    return false;
  }
  //ITEX's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip icon texture path

  //read optional parts
  bool go_on_with_misc = true;
  unsigned short int RecordsDone = 0;
  do
  {
    //read ENAM/ SCRI
    in_File.read((char*) &SubRecName, 4);
    switch(SubRecName)
    {
      case cENAM:
      case cSCRI:
           //ENAM's/ SCRI's length
           in_File.read((char*) &SubLength, 4);
           in_File.seekg(SubLength, std::ios::cur); //skip script/ enchantment
           RecordsDone++;
           break;
      default:
           //unknown record, stop
           go_on_with_misc = false;
           break;
    }//swi
  } while (go_on_with_misc and RecordsDone<=2);
  //seek four bytes towards beginning to land before the name of the next record
  in_File.seekg(-4, std::ios::cur);
  return in_File.good();
}//ReadMISC
*/
/*
bool ReadPGRD(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);*/

  /*Path Grid:
    DATA = (unknown data, length seems to be 12 bytes, always)
    NAME = name of cell the grid belongs to?
    PGRP = unknown data (path grid points?)
    PGRC = unknown data (length: ?) */

  /*long int SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read DATA
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cDATA)
  {
    UnexpectedRecord(cDATA, SubRecName);
    return false;
  }
  //DATA's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip data

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip cell name

  //read PGRP
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cPGRP)
  {
    UnexpectedRecord(cPGRP, SubRecName);
    return false;
  }
  //PGRP's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip grid points(?)

  //read PGRC
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cPGRC)
  {
    UnexpectedRecord(cPGRC, SubRecName);
    return false;
  }
  //PGRC's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip whatever this is

  return in_File.good();
}//ReadPGRD
*/
/*
bool ReadPROB(std::ifstream& in_File, const long int FileSize)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);*/

  /*Probe Item:
    NAME = Item ID, required
    MODL = Model Name, required
    FNAM = Item Name, required
    PBDT = Probe Data (16 bytes), required
        float	Weight
        long	Value
        float	Quality
        long 	Uses
    ITEX = Inventory Icon
    SCRI = Script Name (optional) */

  /*long int SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip ID of probe

  //read MODL
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cMODL)
  {
    UnexpectedRecord(cMODL, SubRecName);
    return false;
  }
  //MODL's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip model path

  //read FNAM
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return false;
  }
  //FNAM's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur);//skip "real" name of probe item

  //read PBDT
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cPBDT)
  {
    UnexpectedRecord(cPBDT, SubRecName);
    return false;
  }
  //PBDT's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=16)
  {
    std::cout <<"Error: sub record PBDT of PROB has invalid length ("<<SubLength
              <<" bytes). Should be 16 bytes.\n";
    return false;
  }
  in_File.seekg(16, std::ios::cur);//skip probe data

  //read ITEX
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cITEX)
  {
    UnexpectedRecord(cITEX, SubRecName);
    return false;
  }
  //ITEX's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip icon texture path

  if (in_File.tellg()>=FileSize)
  {
    return in_File.good();
  }

  //read optional SCRI
  in_File.read((char*) &SubLength, 4);
  if (SubRecName==cSCRI)
  {
    //SCRI's length
    in_File.read((char*) &SubLength, 4);
    in_File.seekg(SubLength, std::ios::cur); //skip probe item's script name
  }
  else
  { //search four bytes towards beginning to land before name of next record
    in_File.seekg(-4, std::ios::cur);
  }
  return in_File.good();
}//ReadPROB
*/
/*
bool ReadRACE(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);*/
  /*Race Definition:
    NAME = Race ID string
    FNAM = Race name string
    RADT = Race data (140 bytes)
        SkillBonuses[7]
            long SkillID
            long Bonus
        long Strength[2]     (Male/Female)
        long Intelligence[2]
        long Willpower[2]
        long Agility[2]
        long Speed[2]
        long Endurance[2]
        long Personality[2]
        long Luck[2]
        float Height[2]
        float Weight[2]
        long Flags (1 = Playable,2 = Beast Race)
    NPCS = Special power/ability name string (32 bytes), multiple
    DESC = Race description */

  /*long int SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip race ID

  //read FNAM
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return false;
  }
  //FNAM's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip race name

  //read RADT
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cRADT)
  {
    UnexpectedRecord(cRADT, SubRecName);
    return false;
  }
  //RADT's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=140)
  {
    std::cout << "Error: sub record RADT of RACE has invalid length ("
              <<SubLength<< " bytes). Should be 140 bytes.\n";
    return false;
  }//if
  in_File.seekg(140, std::ios::cur); //skip race data

  //read multiple NPCS
  do
  {
    //read NPCS
    in_File.read((char*) &SubRecName, 4);
    if (SubRecName==cNPCS)
    {
      //NPCS's length
      in_File.read((char*) &SubLength, 4);
      if (SubLength!=32)
      {
        std::cout << "Error: sub record NPCS of RACE has invalid length ("
                  <<SubLength<< " bytes). Should be 32 bytes.\n";
        return false;
      }//if
      in_File.seekg(32, std::ios::cur); //skip spell ID
    }
  } while (SubRecName == cNPCS);

  //read DESC (record name was already read in loop above)
  if (SubRecName!=cDESC)
  {
    UnexpectedRecord(cDESC, SubRecName);
    return false;
  }
  //DESC's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur);//skip race description

  return in_File.good();
}//ReadRACE
*/
/*
bool ReadREGN(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);*/
  /*Regions:
    NAME = Region ID string
    FNAM = Region name string
    WEAT = Weather Data (8 bytes, or 10 in Bloodmoon)
        byte Clear
        byte Cloudy
        byte Foggy
        byte Overcast
        byte Rain
        byte Thunder
        byte Ash
        byte Blight
    BNAM = Sleep creature string (note: optional)
    CNAM = Map Color (4 bytes, COLORREF)
        byte Red
        byte Green
        byte Blue
        byte Null
    SNAM = Sound Record
        byte SoundName[32] (lots of extra junk beyond string?)
        byte Chance
        Multiple records with the order determining the sound priority */

  /*long int SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip region ID

  //read FNAM
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return false;
  }
  //FNAM's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip region name

  //read WEAT
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cWEAT)
  {
    UnexpectedRecord(cWEAT, SubRecName);
    return false;
  }
  //WEAT's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=8 && SubLength!=10)
  {
    std::cout << "Error: sub record WEAT of REGN has invalid length ("
              <<SubLength<< " bytes). Should be 8 or 10 bytes.\n";
    return false;
  }//if
  in_File.seekg(SubLength, std::ios::cur); //skip region weather

  //read BNAM (optional)
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName==cBNAM)
  {
    //BNAM's length
    in_File.read((char*) &SubLength, 4);
    in_File.seekg(SubLength, std::ios::cur); //skip creature name
    //read next record name
    in_File.read((char*) &SubRecName, 4);
  }

  //read CNAM
  if (SubRecName!=cCNAM)
  {
    UnexpectedRecord(cCNAM, SubRecName);
    return false;
  }
  //CNAM's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=4)
  {
    std::cout << "Error: sub record CNAM of REGN has invalid length ("
              <<SubLength<< " bytes). Should be four bytes.\n";
    return false;
  }//if
  in_File.seekg(4, std::ios::cur); //skip colour values

  //read multiple SNAM
  in_File.read((char*) &SubRecName, 4);
  do
  {
    if (SubRecName==cSNAM)
    {
      //SNAM's length
      in_File.read((char*) &SubLength, 4);
      in_File.seekg(SubLength, std::ios::cur); //skip sound name and chance
      //read next record name
      in_File.read((char*) &SubRecName, 4);
    }
  } while (SubRecName==cSNAM);

  //seek four bytes into the direction if the beginning to land before the
  // name of the next record
  in_File.seekg(-4, std::ios::cur);
  return in_File.good();
}//ReadREGN
*/
/*
bool ReadREPA(std::ifstream& in_File, const long int FileSize)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);*/

  /*Repair Item:
    NAME = Item ID, required
    MODL = Model Name, required
    FNAM = Item Name, required
    RIDT = Repair Data (16 bytes), required
        float	Weight
        long	Value
        long	Uses
        float	Quality
    ITEX = Inventory Icon
    SCRI = Script Name (optional) */

  /*long int SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip repair item ID

  //read MODL
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cMODL)
  {
    UnexpectedRecord(cMODL, SubRecName);
    return false;
  }
  //MODL's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip model path

  //read FNAM
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return false;
  }
  //FNAM's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip name

  //read RIDT
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cRIDT)
  {
    UnexpectedRecord(cRIDT, SubRecName);
    return false;
  }
  //RIDT's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=16)
  {
    std::cout << "Error: sub record RIDT of REPA has invalid length ("
              <<SubLength<< " bytes). Should be 16 bytes.\n";
    return false;
  }//if
  in_File.seekg(16, std::ios::cur); //skip item data

  //read ITEX
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cITEX)
  {
    UnexpectedRecord(cITEX, SubRecName);
    return false;
  }
  //ITEX's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip icon texture path

  if (in_File.tellg()>=FileSize)
  {
    return in_File.good();
  }

  //read optional SCRI
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName==cSCRI)
  {
    //SCRI's length
    in_File.read((char*) &SubLength, 4);
    in_File.seekg(SubLength, std::ios::cur); //skip script name
  }
  else
  {
    //seek four bytes towards beginning to land before the name of next record
    in_File.seekg(-4, std::ios::cur);
  }
  return in_File.good();
}//ReadREPA
*/
/*
bool ReadSCPT(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);*/

  /*Script:
    SCHD = Script Header (52 bytes)
        char Name[32]
        long NumShorts
        long NumLongs
        long NumFloats
        long ScriptDataSize
        long LocalVarSize
    SCVR = List of all the local script variables seperated by '\0' NULL characters.
    SCDT = The compiled script data
    SCTX = Script text
      Note (thoronador): SCVR may not be present at all, if there are no local
          vars. Moreover, SCVR, SCDT and SCTX can occur in any order.*/

  /*long int SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read SCHD
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cSCHD)
  {
    UnexpectedRecord(cSCHD, SubRecName);
    return false;
  }
  //SCHD's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=52)
  {
    std::cout <<"Error: sub record SCHD of SCPT has invalid length ("<<SubLength
              <<" bytes). Should be 52 bytes.\n";
    return false;
  }
  in_File.seekg(52, std::ios::cur); //skip script header


  //optional records/ records with unknown sequence
  //read script sub records
  bool go_on_with_script = true;

  in_File.read((char*) &SubRecName, 4);
  do
  {
    switch(SubRecName)
    {
      case cSCVR:
      case cSCDT:
      case cSCTX:
           //SCVR's/SCDT's/ SCTX's length
           in_File.read((char*) &SubLength, 4);
           //skip script variables/ compiled script/ script text
           in_File.seekg(SubLength, std::ios::cur);
           //read next record name
           in_File.read((char*) &SubRecName, 4);
           break;
      default: //unknown record header, finish
           go_on_with_script = false;
    }//switch
  } while (go_on_with_script);
  //seek four bytes into the direction if the beginning to land before the
  // name of the next record
  in_File.seekg(-4, std::ios::cur);
  return in_File.good();
}//ReadSCPT
*/
/*
bool ReadSKIL(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);*/

  /*Skills:
    INDX = Skill ID (4 bytes, long)
        The Skill ID (0 to 26) since skills are hardcoded in the game
    SKDT = Skill Data (24 bytes)
        long Attribute
        long Specialization (0 = Combat,1 = Magic,2 = Stealth)
        float UseValue[4] (The use types for each skill are hard-coded.)
    DESC = Skill description string */

  /*long int SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read INDX
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cINDX)
  {
    UnexpectedRecord(cINDX, SubRecName);
    return false;
  }
  //INDX's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=4)
  {
    std::cout << "Error: sub record INDX of SKIL has invalid length ("
              <<SubLength<< " bytes). Should be four bytes.\n";
    return false;
  }
  in_File.seekg(4, std::ios::cur);//skip the skill index

  //read SKDT
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cSKDT)
  {
    UnexpectedRecord(cSKDT, SubRecName);
    return false;
  }
  //SKDT's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=24)
  {
    std::cout << "Error: sub record SKDT of SKIL has invalid length ("
              <<SubLength<< " bytes). Should be 24 bytes.\n";
    return false;
  }
  in_File.seekg(24, std::ios::cur);//skip the skill index

  //read DESC
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cDESC)
  {
    UnexpectedRecord(cDESC, SubRecName);
    return false;
  }
  //DESC's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip the skill description

  return in_File.good();
}//ReadSKIL
*/
/*
bool ReadSNDG(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);*/

  /*Sound Generator:
    NAME = Name? (DEFAULT0001, ALIT0001, etc...)
    DATA = Sound Type Data (4 bytes, long)
        (0 = Left Foot,1 = Right Foot,2 = Swim Left,3 = Swim Right,4 = Moan,
         5 = Roar,6 = Scream,7 = Land)
    CNAM = Creature name (optional)
    SNAM = Sound ID string */

  /*long int SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip the name

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
    std::cout << "Error: sub record DATA of SNDG has invalid length ("
              <<SubLength<< " bytes). Should be four bytes.\n";
    return false;
  }
  in_File.seekg(4, std::ios::cur);//skip the sound gen data

  //read optional CNAM
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName==cCNAM)
  {
    //CNAM's length
    in_File.read((char*) &SubLength, 4);
    in_File.seekg(SubLength, std::ios::cur); //skip creature name
    //read next record name
    in_File.read((char*) &SubRecName, 4);
  }

  //read SNAM (record name already read above
  if (SubRecName!=cSNAM)
  {
    UnexpectedRecord(cSNAM, SubRecName);
    return false;
  }
  //SNAM's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip sound ID

  return in_File.good();
}//ReadSNDG
*/
/*
bool ReadSOUN(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);*/

  /*Sound:
    NAME = Sound ID
    FNAM = Sound Filename (relative to Sounds\)
    DATA = Sound Data (3 bytes)
        byte Volume (0=0.00, 255=1.00)
        byte MinRange
        byte MaxRange*/

  /*long int SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip the name

  //read FNAM
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return false;
  }
  //FNAM's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip file name

  //read DATA
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cDATA)
  {
    UnexpectedRecord(cDATA, SubRecName);
    return false;
  }
  //DATA's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=3)
  {
    std::cout << "Error: sub record DATA of SOUN has invalid length ("
              <<SubLength<< " bytes). Should be 3 bytes.\n";
    return false;
  }
  in_File.seekg(3, std::ios::cur); //skip data

  return in_File.good();
}//ReadSOUN
*/
/*
bool ReadSPEL(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);*/

  /*Spell:
    NAME = Spell ID
    FNAM = Spell Name
    SPDT = Spell Data (12 bytes)
        long Type (0 = Spell,1 = Ability,2 = Blight,3 = Disease,4 = Curse,5 = Power)
        long SpellCost
        long Flags (0x0001 = AutoCalc,0x0002 = PC Start,0x0004 = Always Succeeds)
    ENAM = Enchantment data (24 bytes, 0 to 8) */

  /*long int SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur);//skip ID

  //read FNAM
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return false;
  }
  //FNAM's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur);//skip spell name

  //read SPDT
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cSPDT)
  {
    UnexpectedRecord(cSPDT, SubRecName);
    return false;
  }
  //SPDT's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=12)
  {
    std::cout << "Error: sub record SPDT of SPEL has invalid length ("
              <<SubLength<< " bytes). Should be 12 bytes.\n";
    return false;
  }
  in_File.seekg(12, std::ios::cur);//skip spell data

  //read multiple ENAM records
  do
  {
    in_File.read((char*) &SubRecName, 4);
    if (SubRecName==cENAM)
    {
      //ENAM's length
      in_File.read((char*) &SubLength, 4);
      if (SubLength!=24)
      {
        std::cout << "Error: sub record ENAM of SPEL has invalid length ("
                  <<SubLength<< " bytes). Should be 24 bytes.\n";
        return false;
      }
      in_File.seekg(24, std::ios::cur);//skip enchantment data
    }
  } while (SubRecName == cENAM);
  //seek four bytes towards beginning to land before the name of the next record
  in_File.seekg(-4, std::ios::cur);
  return in_File.good();
}//ReadSPEL
*/
/*
bool ReadSSCR(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);*/

  /*Start Script(?): (no documentation known)
    DATA = ? (a sequence of digits)
    NAME = ID string*/

  /*long int SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read DATA
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cDATA)
  {
    UnexpectedRecord(cDATA, SubRecName);
    return false;
  }
  //DATA's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur);//skip data

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  //skip NAME
  in_File.seekg(SubLength, std::ios::cur);

  return in_File.good();
}//ReadSSCR
*/
/*
bool ReadSTAT(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);*/

  /*Static:
    NAME = ID string
    MODL = NIF model*/

  /*long int SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  //skip NAME
  in_File.seekg(SubLength, std::ios::cur);

  //read MODL
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cMODL)
  {
    UnexpectedRecord(cMODL, SubRecName);
    return false;
  }
  //MODL's length
  in_File.read((char*) &SubLength, 4);
  //skip MODL
  in_File.seekg(SubLength, std::ios::cur);

  return in_File.good();
}//ReadSTAT
*/
/*
bool ReadWEAP(std::ifstream& in_File, const long int FileSize)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);*/

  /*Weapons:
    NAME = item ID, required
    MODL = model filename, required
    FNAM = item name
    WPDT = Weapon Data, 0x20 bytes binary, required
        float Weight
        long  Value
        short Type? (0=ShortBladeOneHand,1=LongBladeOneHand,2=LongBladeTwoClose,
                     3=BluntOneHand,4=BluntTwoClose,5=BluntTwoWide,6=SpearTwoWide
                     7=AxeOneHand,8=AxeTwoHand,9=MarksmanBow,10=MarksmanCrossbow
                     11=MarksmanThrown,12= Arrow,13=Bolt)
        short Health
        float Speed
        float Reach
        short EnchantPts
        byte  ChopMin
        byte  ChopMax
        byte  SlashMin
        byte  SlashMax
        byte  ThrustMin
        byte  ThrustMax
        long  Flags (0 to 1) (0 = ?;1 = Ignore Normal Weapon Resistance?)
    ITEX = Iventory icon filename
    ENAM = Enchantment ID string (optional)
    SCRI = script ID string (optional) */

  /*long int SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  in_File.read((char*) &SubLength, 4);//NAME's length
  in_File.seekg(SubLength, std::ios::cur);//skip NAME

  //read MODL
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cMODL)
  {
    UnexpectedRecord(cMODL, SubRecName);
    return false;
  }
  //MODL's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur);//skip MODL

  //read FNAM
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return false;
  }
  //FNAM's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur);//skip weapon name

  //read WPDT
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cWPDT)
  {
    UnexpectedRecord(cWPDT, SubRecName);
    return false;
  }
  in_File.read((char*) &SubLength, 4);//WPDT's length
  if (SubLength!=32)
  {
    std::cout << "Error: sub record WPDT of WEAP has invalid length ("
              <<SubLength<< " bytes). Should be 32 bytes.\n";
    return false;
  }
  in_File.seekg(32, std::ios::cur);//skip WPDT

  //read ITEX
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cITEX)
  {
    UnexpectedRecord(cITEX, SubRecName);
    return false;
  }
  //ITEX's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur);//skip weapon icon
  if (in_File.tellg()>=FileSize)
  {
    return in_File.good();
  }

  //read optional ENAM
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName==cENAM)
  {
    //ENAM's length
    in_File.read((char*) &SubLength, 4);
    in_File.seekg(SubLength, std::ios::cur);//skip enchantment name
    if (in_File.tellg()>=FileSize)
    {
      return in_File.good();
    }
    //read next (optional) record name
    in_File.read((char*) &SubRecName, 4);
  }//if ENAM

  //read optional SCRI
  if (SubRecName==cSCRI)
  {
    //SCRI's length
    in_File.read((char*) &SubLength, 4);
    in_File.seekg(SubLength, std::ios::cur);//skip script name
  }//if SCRI
  else
  {
    //seek four bytes towards beginning to land before the name of the next record
    in_File.seekg(-4, std::ios::cur);
  }
  return in_File.good();
}//ReadWEAP
*/

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
