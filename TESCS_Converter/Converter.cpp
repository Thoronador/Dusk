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
const long int cBSND = 0x444E5342; //"BSND" = 42 53 4E 44
const long int cBYDT = 0x54445942; //"BYDT" = 42 59 44 54
const long int cBVFX = 0x58465642; //"BVFX" = 42 56 46 58
const long int cCLAS = 0x53414C43; //"CLAS" = 43 4C 41 53
const long int cCLDT = 0x54444C43; //"CLDT" = 43 4C 44 54
const long int cCLOT = 0x544F4C43; //"CLOT" = 43 4C 4F 54
const long int cCNAM = 0x4D414E43; //"CNAM" = 43 4E 41 4D
const long int cCNDT = 0x54444E43; //"CNDT" = 43 4E 44 54
const long int cCONT = 0x544E4F43; //"CONT" = 43 4F 4E 54
const long int cCSND = 0x444E5343; //"CSND" = 43 53 4E 44
const long int cCTDT = 0x54445443; //"CTDT" = 43 54 44 54
const long int cCVFX = 0x58465643; //"CVFX" = 43 56 46 58
const long int cDATA = 0x41544144; //"DATA" = 44 41 54 41
const long int cDESC = 0x43534544; //"DESC" = 44 45 53 43
const long int cDOOR = 0x524F4F44; //"DOOR" = 44 4F 4F 52
const long int cENAM = 0x4D414E45; //"ENAM" = 45 4E 41 4D
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
const long int cINTV = 0x56544E49; //"INTV" = 49 4E 54 56
const long int cITEX = 0x58455449; //"ITEX" = 49 54 45 58
const long int cLAND = 0x444E414C; //"LAND" = 4C 41 4E 44
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
const long int cPTEX = 0x58455450; //"PTEX" = 50 54 45 58
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
const long int cSNAM = 0x4D414E53; //"SNAM" = 53 4E 41 4D
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
bool ProcessNextRecord(std::ifstream& in_File, const std::string& DuskFileName, const int FileSize);

//routines for reading/ skipping different records
bool ReadACTI(std::ifstream& in_File, const long int FileSize);
bool ReadALCH(std::ifstream& in_File, const long int FileSize);
bool ReadAPPA(std::ifstream& in_File, const long int FileSize);
bool ReadARMO(std::ifstream& in_File, const long int FileSize);
bool ReadBODY(std::ifstream& in_File);
bool ReadBOOK(std::ifstream& in_File);
bool ReadCLAS(std::ifstream& in_File);
bool ReadCLOT(std::ifstream& in_File, const long int FileSize);
bool ReadCONT(std::ifstream& in_File, const long int FileSize);
bool ReadDOOR(std::ifstream& in_File);
bool ReadFACT(std::ifstream& in_File);
bool ReadGLOB(std::ifstream& in_File);
bool ReadGMST(std::ifstream& in_File);
bool ReadLAND(std::ifstream& in_File, const std::string& DuskFile);
bool ReadLIGH(std::ifstream& in_File, const long int FileSize);
bool ReadLOCK(std::ifstream& in_File, const long int FileSize);
bool ReadLTEX(std::ifstream& in_File);
bool ReadMGEF(std::ifstream& in_File);
bool ReadMISC(std::ifstream& in_File);
bool ReadREGN(std::ifstream& in_File);
bool ReadREPA(std::ifstream& in_File, const long int FileSize);
bool ReadSCPT(std::ifstream& in_File);
bool ReadSOUN(std::ifstream& in_File);
bool ReadSPEL(std::ifstream& in_File);
bool ReadSSCR(std::ifstream& in_File);
bool ReadSTAT(std::ifstream& in_File);
bool ReadWEAP(std::ifstream& in_File, const long int FileSize);


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
    Go_on_processing = ProcessNextRecord(input, DuskFileName, FileSize);
    Processed_Records++;
    if (input.tellg()>=FileSize)
    {
      Go_on_processing = false;
    }
  }
  std::cout << "Number of processed records (including possible failures): "
            <<Processed_Records<<"\n"
            << "Current file position: "<<input.tellg()<< " bytes.\n";
  //end
  input.close();
  return false;
}

bool ProcessNextRecord(std::ifstream& in_File, const std::string& DuskFileName, const int FileSize)
{
  long int RecordName; //normally should be 4 char, but char is not eligible for switch
  RecordName = 0;

  bool Success = false;

  //read record name
  in_File.read((char*) &RecordName, 4);
  switch(RecordName)
  {
    case cACTI:
         Success = ReadACTI(in_File, FileSize); break;
    case cALCH:
         Success = ReadALCH(in_File, FileSize); break;
    case cAPPA:
         Success = ReadAPPA(in_File, FileSize); break;
    case cARMO:
         Success = ReadARMO(in_File, FileSize); break;
    case cBODY:
         Success = ReadBODY(in_File); break;
    case cBOOK:
         Success = ReadBOOK(in_File); break;
    case cCLAS:
         Success = ReadCLAS(in_File); break;
    case cCLOT:
         Success = ReadCLOT(in_File, FileSize); break;
    case cCONT:
         Success = ReadCONT(in_File, FileSize); break;
    case cDOOR:
         Success = ReadDOOR(in_File); break;
    case cFACT:
         Success = ReadFACT(in_File); break;
    case cGLOB:
         Success = ReadGLOB(in_File); break;
    case cGMST:
         Success = ReadGMST(in_File); break;
    case cLAND:
         Success = ReadLAND(in_File, DuskFileName); break;
    case cLIGH:
         Success = ReadLIGH(in_File, FileSize); break;
    case cLOCK:
         Success = ReadLOCK(in_File, FileSize); break;
    case cLTEX:
         Success = ReadLTEX(in_File); break;
    case cMGEF:
         Success = ReadMGEF(in_File); break;
    case cMISC:
         Success = ReadMISC(in_File); break;
    case cREGN:
         Success = ReadREGN(in_File); break;
    case cREPA:
         Success = ReadREPA(in_File, FileSize); break;
    case cSCPT:
         Success = ReadSCPT(in_File); break;
    case cSOUN:
         Success = ReadSOUN(in_File); break;
    case cSPEL:
         Success = ReadSPEL(in_File); break;
    case cSSCR:
         Success = ReadSSCR(in_File); break;
    case cSTAT:
         Success = ReadSTAT(in_File); break;
    case cWEAP:
         Success = ReadWEAP(in_File, FileSize); break;
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

bool ReadACTI(std::ifstream& in_File, const long int FileSize)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

  /*Activators:
    NAME = Item ID, required
    MODL = Model Name, required
    FNAM = Item Name, required
    SCRI = Script Name (optional) */

  long int SubRecName, SubLength;
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


bool ReadALCH(std::ifstream& in_File, const long int FileSize)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

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

  long int SubRecName, SubLength;
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


bool ReadAPPA(std::ifstream& in_File, const long int FileSize)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

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

  long int SubRecName, SubLength;
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


bool ReadARMO(std::ifstream& in_File, const long int FileSize)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

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

  long int SubRecName, SubLength;
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


bool ReadBODY(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

  /*
  NAME = Body Part ID
  MODL = Path to Mesh
  FNAM = Race ID
  BYDT = Body part data (4 bytes)
  */
  long int SubRecName, SubLength;
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


bool ReadBOOK(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

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

  long int SubRecName, SubLength;
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


bool ReadCLAS(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

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
    DESC = Description string */
  long int SubRecName, SubLength;
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

  //read DESC
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cDESC)
  {
    UnexpectedRecord(cDESC, SubRecName);
    return false;
  }
  //DESC's length
  in_File.read((char*) &SubLength, 4);
  in_File.seekg(SubLength, std::ios::cur); //skip class description string

  return in_File.good();
}//ReadCLAS


bool ReadCLOT(std::ifstream& in_File, const long int FileSize)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

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

  long int SubRecName, SubLength;
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


bool ReadCONT(std::ifstream& in_File, const long int FileSize)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

  /*Container:
    NAME = ID
    MODL = NIF Model
    FNAM = Container name
    CNDT = Container data (4 bytes)
        float Weight
    FLAG = Container flags (4 bytes, bit-field)
        (0x0001=Organic;0x0002=Respawns, organic only;0x0008=Default, unknown)
    NPCO = An item record (36 bytes, 0+ times)
        long	Count	  Number of the item
        char	Name[32]  The ID of the item */

  long int SubRecName, SubLength;
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


bool ReadDOOR(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

  /*Door:
    NAME = door ID
    FNAM = door name
    MODL = NIF model filename
    SCRI = Script (optional)
    SNAM = Sound name open (optional)
    ANAM = Sound name close (optional)
  */

  long int SubRecName, SubLength;
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


bool ReadFACT(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

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

  long int SubRecName, SubLength;
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
  } while (SubRecName == cRNAM);

  /*//seek 4 bytes towards beginning to land before next record name
  in_File.seekg(-4, std::ios::cur);

  //read FADT
  in_File.read((char*) &SubRecName, 4);*/

  //last SubRecord read in do-while-loop is already the next (FADT)

  if (SubRecName!=cFADT)
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

bool ReadGLOB(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

  /*Global variable:
    NAME = Global ID
    FNAM = Type of global (1 byte)
        's' = short
        'l' = long
        'f' = float
    FLTV = Float data (4 bytes) */
  long int SubRecName, SubLength;
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


bool ReadGMST(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

  /*GMST: Game Setting
    NAME = Setting ID string
    STRV = String value/ INTV = Integer value (4 btes)/FLTV = Float value (4 bytes)
  */
  long int SubRecName, SubLength;
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
         //read STRV
         in_File.read((char*) &SubRecName, 4);
         if (SubRecName!=cSTRV)
         {
           UnexpectedRecord(cSTRV, SubRecName);
           return false;
         }
         //STRV's length
         in_File.read((char*) &SubLength, 4);
         //skip STRV
         in_File.seekg(SubLength, std::ios::cur);
         break;
  }//switch
  return in_File.good();
}//ReadGMST


bool ReadLAND(std::ifstream& in_File, const std::string& DuskFile)
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
  SubRecName = SubLength = 0;

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
  in_File.seekg(8, std::ios::cur); //skip cell offsets

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
  memset(MW_Height, 255, 65*65*3);

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
      std::cout <<"Error: sub record VCLR of LAND has invalid length ("<<SubLength
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
  Dusk::LandscapeRecord DuskLand;
  unsigned int i,j;

  for (i=0; i<65; i++)
  {
    for (j=0; j<65; j++)
    {
      HeightOffset = HeightOffset + MW_Height[i][j];
      DuskLand.Height[i][j] = HeightOffset;

      DuskLand.Colour[i][j][0] = MW_Colour[i][j][0];
      DuskLand.Colour[i][j][1] = MW_Colour[i][j][1];
      DuskLand.Colour[i][j][2] = MW_Colour[i][j][2];
    }//for j
  }//for i

  std::ofstream DuskOutput;
  DuskOutput.open(DuskFile.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
  if(!DuskOutput)
  {
    std::cout << "ReadLAND: Could not create file \""<<DuskFile
              << "\" for writing in binary mode.\n";
    return false;
  }//if

  //write header "Dusk"
  DuskOutput.write("Dusk", 4);
  i = 1;
  DuskOutput.write((char*) &i, sizeof(unsigned int));
  DuskLand.SaveToStream(&DuskOutput);
  DuskOutput.close();

  return in_File.good();
}//ReadLAND


bool ReadLIGH(std::ifstream& in_File, const long int FileSize)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

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

  long int SubRecName, SubLength;
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


bool ReadLOCK(std::ifstream& in_File, const long int FileSize)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

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

  long int SubRecName, SubLength;
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


bool ReadLTEX(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

  /*Land texture(?): (no documentation known)
    NAME = texture name/ID (?)
    INTV = texture index (?)
    DATA = path to texture */

  long int SubRecName, SubLength;
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


bool ReadMGEF(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

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

  long int SubRecName, SubLength;
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


bool ReadMISC(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

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

  long int SubRecName, SubLength;
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


bool ReadREGN(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);
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

  long int SubRecName, SubLength;
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


bool ReadREPA(std::ifstream& in_File, const long int FileSize)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

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

  long int SubRecName, SubLength;
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


bool ReadSCPT(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

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

  long int SubRecName, SubLength;
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

bool ReadSOUN(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

  /*Sound:
    NAME = Sound ID
    FNAM = Sound Filename (relative to Sounds\)
    DATA = Sound Data (3 bytes)
        byte Volume (0=0.00, 255=1.00)
        byte MinRange
        byte MaxRange*/

  long int SubRecName, SubLength;
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


bool ReadSPEL(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

  /*Spell:
    NAME = Spell ID
    FNAM = Spell Name
    SPDT = Spell Data (12 bytes)
        long Type (0 = Spell,1 = Ability,2 = Blight,3 = Disease,4 = Curse,5 = Power)
        long SpellCost
        long Flags (0x0001 = AutoCalc,0x0002 = PC Start,0x0004 = Always Succeeds)
    ENAM = Enchantment data (24 bytes, 0 to 8) */

  long int SubRecName, SubLength;
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


bool ReadSSCR(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

  /*Start Script(?): (no documentation known)
    DATA = ? (a sequence of digits)
    NAME = ID string*/

  long int SubRecName, SubLength;
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

bool ReadSTAT(std::ifstream& in_File)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

  /*Static:
    NAME = ID string
    MODL = NIF model*/

  long int SubRecName, SubLength;
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


bool ReadWEAP(std::ifstream& in_File, const long int FileSize)
{
  long int Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

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

  long int SubRecName, SubLength;
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
