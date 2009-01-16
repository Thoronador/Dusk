#include "Landscape.h"
#include "API.h"
#include <fstream>
#include <iostream>
#include <sstream>
// #include <OgreSceneManager.h>

namespace Dusk
{

Landscape::Landscape()
{
  //empty
  m_RecordList = NULL;
  m_numRec = 0;
}

Landscape::~Landscape()
{
  //empty
}

Landscape& Landscape::GetSingleton()
{
    static Landscape Instance;
    return Instance;
}

bool Landscape::LoadFromFile(const std::string FileName)
{
  if (m_RecordList!=NULL)
  {
    std::cout << "Landscape::LoadFromFile: Landscape data is already present.\n";
    return false;
  }

  TLandscapeRecord * temp;
  unsigned int numRecords, i;
  char Header[4];
  std::ifstream input;

  input.open(FileName.c_str(), std::ios::in | std::ios::binary);
  if(!input)
  {
    std::cout << "Landscape::LoadFromFile: Could not open file \""<<FileName
              << "\" for reading in binary mode.\n";
    return false;
  }//if

  //read header "Dusk"
  input.read(Header, 4);
  if ((Header[0]!='D') || (Header[1]!='u') || (Header[2]!='s') || (Header[3]!='k'))
  {
    std::cout << "Landscape::LoadFromFile: File \""<<FileName<< "\" has "
              << "invalid header.\n";
    delete temp;
    input.close();
    return false;
  }
  //read total number of records in file
  input.read((char*) &numRecords, sizeof(unsigned int));
  if (!input.good())
  {
    std::cout << "Landscape::LoadFromFile: File \""<<FileName<< "\" has "
              << "invalid, (short?) header.\n";
    input.close();
    return false;
  }

  if (numRecords>cMaxLandRecords)
  {
    std::cout << "Landscape::LoadFromFile: File \""<<FileName<< "\" has "
              << "more than "<<cMaxLandRecords<<" records.\n";
    input.close();
    return false;
  }

  m_RecordList = new TLandscapeRecord[numRecords];
  m_numRec = numRecords;

  //read loop
  for (i=0; i<numRecords; i=i+1)
  {
    temp = &m_RecordList[i];
    //read header "Land"
    input.read(temp->Land, 4);
    if ((temp->Land[0]!='L') || (temp->Land[1]!='a') || (temp->Land[2]!='n') || (temp->Land[3]!='d'))
    {
      std::cout << "Landscape::LoadFromFile: File \""<<FileName<< "\" has "
                << "invalid record header.\n";
      delete[] m_RecordList;
      m_RecordList= NULL;
      m_numRec = 0;
      input.close();
      return false;
    }
    //read offsets
    input.read((char*) &(temp->OffsetX), sizeof(float));
    input.read((char*) &(temp->OffsetY), sizeof(float));
    //stride
    input.read((char*) &(temp->Stride), sizeof(float));
    if (!input.good())
    {
      std::cout << "Landscape::LoadFromFile: File \""<<FileName<< "\" seems to "
                << "have invalid Land record data.\n";
      delete[] m_RecordList;
      m_RecordList= NULL;
      m_numRec = 0;
      input.close();
      return false;
    }
    //read the height data
    input.read((char*) &(temp->Height[0][0]), 65*65*sizeof(float));
    if (!input.good())
    {
      std::cout << "Landscape::LoadFromFile: File \""<<FileName<< "\" seems to "
                << "have invalid Land record height data.\n";
      delete[] m_RecordList;
      m_RecordList= NULL;
      m_numRec = 0;
      input.close();
      return false;
    }
    //colour data
    input.read((char*) &(temp->Colour[0][0][0]), 65*65*3);
    if (!input.good())
    {
      std::cout << "Landscape::LoadFromFile: File \""<<FileName<< "\" seems to "
                << "have invalid Land record colour data.\n";
      delete[] m_RecordList;
      m_RecordList= NULL;
      m_numRec = 0;
      input.close();
      return false;
    }
  }//for
  input.close();
  return true;
}

void Landscape::InitObjects(const unsigned int num)
{
  /*
  unsigned int i;
  
  m_ObjectList = new (Ogre::ManualObject *)[num];
  m_numObj = num;
  
  for (i=0; i<num; i++)
  {
    m_ObjectList[i] = NULL;
  }
  */
}

bool Landscape::SendToEngine()
{
  if (m_numRec==0)
  {
    return false;
  }
  /*
  Ogre::SceneManager * scm;
  unsigned int i, j, k;
  
  scm = Dusk::getAPI().getOgreSceneManager();
  if (scm==NULL)
  {
    std::cout << "Landscape::SendToEngine: ERROR: Got NULL for scene manager.\n";
    return false;
  }
  
  InitObjects(m_numRec);
  for (i=0; i<m_numRec; i++)
  {
    std::stringstream convert;
    convert << i;
      
    m_ObjectList[i] = scm->createManualObject("Landscape"+convert.str());
    m_ObjectList[i]->estimateVertexCount(65*65);
    m_ObjectList[i]->setDynamic(false);
    m_ObjectList[i]->begin(Ogre::OT_TRIANGLE_LIST);
    for (j=0; j<64; j++)
    {
      for (k=0; k<64; k++)
      {
        //first triangle
        m_ObjectList[i]->position(
                    m_RecordList[i].OffsetX+cDefaultStride*j,
                    m_RecordList[i].height[j][k],
                    m_RecordList[i].OffsetY+cDefaultStride*k);
        m_ObjectList[i]->position(
                    m_RecordList[i].OffsetX+cDefaultStride*(j+1),
                    m_RecordList[i].height[j+1][k],
                    m_RecordList[i].OffsetY+cDefaultStride*k);
        m_ObjectList[i]->position(
                    m_RecordList[i].OffsetX+cDefaultStride*j,
                    m_RecordList[i].height[j][k+1],
                    m_RecordList[i].OffsetY+cDefaultStride*(k+1));
        //second triangle
        m_ObjectList[i]->position(
                    m_RecordList[i].OffsetX+cDefaultStride*(j+1),
                    m_RecordList[i].height[j+1][k],
                    m_RecordList[i].OffsetY+cDefaultStride*k);
        m_ObjectList[i]->position(
                    m_RecordList[i].OffsetX+cDefaultStride*(j+1),
                    m_RecordList[i].height[j+1][k+1],
                    m_RecordList[i].OffsetY+cDefaultStride*(k+1));
        m_ObjectList[i]->position(
                    m_RecordList[i].OffsetX+cDefaultStride*j),
                    m_RecordList[i].height[j][k+1],
                    m_RecordList[i].OffsetY+cDefaultStride*(k+1));
      }//for k
    }//for j
    m_ObjectList[i]->end();
  }//for
  return true;
  */
  return false;
}


float Landscape::GetHeigtAtPosition(const float x, const float y) const
{
  if (m_RecordList == NULL)
  {
    return 0.0;
  }
  unsigned int i, x_idx, y_idx;
  for(i=0; i<m_numRec; i++)
  {
    if ((x>=m_RecordList[i].OffsetX) || (x<=m_RecordList[i].OffsetX+64*cDefaultStride)
       ||(y>=m_RecordList[i].OffsetY) || (y<=m_RecordList[i].OffsetY+64*cDefaultStride))
    {
      //got it
      //not implemented exactly yet, but at least we have a return value which
      // is somewhat near the real value
      x_idx = (x-m_RecordList[i].OffsetX)/cDefaultStride;
      y_idx = (y-m_RecordList[i].OffsetY)/cDefaultStride;
      return m_RecordList[i].Height[x_idx][y_idx];
    }//if
  }//for
  return 0.0;
}

/*to do:
-load and save routines
-sending data to Ogre via Ogre::ManualObject
 (Maybe NURBS would be better, but what's Ogre's class/function for that?)

*/
} //namespace
