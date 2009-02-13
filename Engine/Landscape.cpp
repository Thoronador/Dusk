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
  m_ObjectList = NULL;
  m_numObj = 0;
}

Landscape::~Landscape()
{
  unsigned int i;
  //empty
  if (m_numObj>0)
  {
    for (i=0; i<m_numObj; i++)
    {
      /*std::stringstream convert;
      convert << i;
      Dusk::getAPI().getOgreSceneManager()->destroyManualObject("Landscape"+convert.str());*/
      Dusk::getAPI().getOgreSceneManager()->destroyManualObject(m_ObjectList[i]);
    }
    delete[] m_ObjectList;
    m_numObj = 0;
  }//if
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
  unsigned int numRecords, i,j, record;
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
    if (temp->Stride <=0.0f)
    {
      std::cout << "Landscape::LoadFromFile: File \""<<FileName<< "\" has an "
                << "invalid stride value of "<< temp->Stride <<".\n";
      delete[] m_RecordList;
      m_RecordList= NULL;
      m_numRec = 0;
      input.close();
      return false;
    }//if


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

  //search Highest and Lowest values
  for (record=0; record<numRecords; record++)
  {
    m_RecordList[record].Highest= m_RecordList[record].Height[0][0];
    m_RecordList[record].Lowest = m_RecordList[record].Height[0][0];
    for (i=0; i<65; i++)
    {
      for (j=0; j<65; j++)
      {
        if (m_RecordList[record].Height[i][j]>m_RecordList[record].Highest)
        {
          m_RecordList[record].Highest = m_RecordList[record].Height[i][j];
        }
        else if (m_RecordList[record].Height[i][j]<m_RecordList[record].Lowest)
        {
          m_RecordList[record].Lowest = m_RecordList[record].Height[i][j];
        }
      }//for j
    }//for i
  }//for record
  return true;
}

bool Landscape::SaveToFile(const std::string FileName)
{
  if (m_RecordList==NULL)
  {
    std::cout << "Landscape::SaveToFile: No Landscape data is present.\n";
    return false;
  }

  std::ofstream output;

  output.open(FileName.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
  if(!output)
  {
    std::cout << "Landscape::SaveToFile: Could not open file \""<<FileName
              << "\" for writing in binary mode.\n";
    return false;
  }//if

  //write header "Dusk"
  output.write("Dusk", 4);
  output.write((char*) &m_numRec, sizeof(unsigned int));

  unsigned int i;

  for (i=0; i<m_numRec; i=i+1)
  {
    //write header "Land"
    output.write("Land", 4);
    //write offsets
    output.write((char*) &(m_RecordList[i].OffsetX), sizeof(float));
    output.write((char*) &(m_RecordList[i].OffsetY), sizeof(float));
    //stride
    output.write((char*) &(m_RecordList[i].Stride), sizeof(float));
    //height data
    output.write((char*) &(m_RecordList[i].Height[0][0]), 65*65*sizeof(float));
    //colour data
    output.write((char*) &(m_RecordList[i].Colour[0][0][0]), 65*65*3);
    if (!output.good())
    {
      std::cout << "Landscape::SaveToFile: Error while writing record "<<i+1
                << " to file \"" <<FileName<<"\".\n";
      output.close();
      return false;
    }//if
  } //for
  output.close();
  return true;
}//SaveToFile

unsigned int Landscape::RecordsAvailable()
{
  return m_numRec;
}

void Landscape::InitObjects(const unsigned int num)
{
  unsigned int i;

  m_ObjectList = new Ogre::ManualObject* [num];
  m_numObj = num;

  for (i=0; i<num; i++)
  {
    m_ObjectList[i] = NULL;
  }
}

bool Landscape::SendToEngine()
{
  if (m_numRec==0)
  {
    return false;
  }

  Ogre::SceneManager * scm;
  unsigned int i, j, k;

  scm = Dusk::getAPI().getOgreSceneManager();
  if (scm==NULL)
  {
    std::cout << "Landscape::SendToEngine: ERROR: Got NULL for scene manager.\n";
    return false;
  }

  //create own scene node for landscape
  Ogre::SceneNode * landnode;
  landnode = scm->getRootSceneNode()->createChildSceneNode("LandscapeNode", Ogre::Vector3(0.0, 0.0, 0.0));


  InitObjects(m_numRec);
  for (i=0; i<m_numRec; i++)
  {
    std::stringstream convert;
    convert << i;

    m_ObjectList[i] = scm->createManualObject("Landscape"+convert.str());
    m_ObjectList[i]->estimateVertexCount(65*65);
    m_ObjectList[i]->estimateIndexCount(64*64*6);
    m_ObjectList[i]->setDynamic(false);
    m_ObjectList[i]->begin("Landscape/Green", Ogre::RenderOperation::OT_TRIANGLE_LIST);
    for (j=0; j<65; j++)
    {
      for (k=0; k<65; k++)
      {
        m_ObjectList[i]->position(m_RecordList[i].OffsetX+cDefaultStride*j,
                                  m_RecordList[i].Height[j][k],
                                  m_RecordList[i].OffsetY+cDefaultStride*k);
        m_ObjectList[i]->colour(m_RecordList[i].Colour[j][k][0]/255.0f,
                                m_RecordList[i].Colour[j][k][1]/255.0f,
                                m_RecordList[i].Colour[j][k][2]/255.0f);
      }//for k
    }//for j

    //triangles
    for (j=0; j<64; j++)
    {
      for (k=0; k<64; k++)
      {
        //first triangle: [j][k], [j][k+1], [j+1][k]
        m_ObjectList[i]->index(j*65+k);
        m_ObjectList[i]->index(j*65+k+1);
        m_ObjectList[i]->index((j+1)*65+k);
        //second triangle: [j+1][k], [j][k+1], [j+1][k+1]
        m_ObjectList[i]->index((j+1)*65+k);
        m_ObjectList[i]->index(j*65+k+1);
        m_ObjectList[i]->index((j+1)*65+k+1);
      }//for k
    }//for j
    m_ObjectList[i]->end();
    landnode->attachObject(m_ObjectList[i]);
  }//for
  return true;
}

bool Landscape::Shift(const unsigned int record, const float delta)
{
  if (record>=m_numRec)
  {
    return false;
  }
  unsigned int i,j;

  for (i=0; i<65; i++)
  {
    for (j=0; j<65; j++)
    {
      m_RecordList[record].Height[i][j] = m_RecordList[record].Height[i][j]+delta;
    }
  }
  m_RecordList[record].Highest = m_RecordList[record].Highest+delta;
  m_RecordList[record].Lowest = m_RecordList[record].Lowest+delta;
  return true;
}//Shift

bool Landscape::Scale(const unsigned int record, const float factor)
{
  if (record>=m_numRec || factor<cMinScale)
  {
    return false;
  }
  unsigned int i,j;

  for (i=0; i<65; i++)
  {
    for (j=0; j<65; j++)
    {
      m_RecordList[record].Height[i][j] = m_RecordList[record].Height[i][j]*factor;
    }
  }
  m_RecordList[record].Highest = m_RecordList[record].Highest*factor;
  m_RecordList[record].Lowest = m_RecordList[record].Lowest*factor;
  return true;
}//Scale

bool Landscape::MakePlain(const unsigned int record, const float value)
{
  if (record>=m_numRec)
  {
    return false;
  }
  unsigned int i,j;

  for (i=0; i<65; i++)
  {
    for (j=0; j<65; j++)
    {
      m_RecordList[record].Height[i][j] = value;
    }
  }
  m_RecordList[record].Highest = value;
  m_RecordList[record].Lowest = value;
  return true;
}//MakePlain

bool Landscape::IsPlain(const unsigned int record)
{
  if (record>=m_numRec)
  {
    return true;
  }
  return (m_RecordList[record].Highest==m_RecordList[record].Lowest);
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
    if ((x>=m_RecordList[i].OffsetX) && (x<=m_RecordList[i].OffsetX+64*cDefaultStride)
       &&(y>=m_RecordList[i].OffsetY) && (y<=m_RecordList[i].OffsetY+64*cDefaultStride))
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
-sending data to Ogre via Ogre::ManualObject
 (Maybe NURBS would be better, but what's Ogre's class/function for that?)
*/
} //namespace
