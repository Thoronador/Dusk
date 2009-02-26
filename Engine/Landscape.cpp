#include "Landscape.h"
#ifndef NO_OGRE_IN_LANDSCAPE
  #include "API.h"
#endif
#include <iostream>
#include <sstream>

namespace Dusk
{

  const std::string cLandNodeName = "LandscapeNode";

//the Landscape record class
// ++++
// ++ Contains the (height and colour) data of the landscape chunks.
// ++ However, it should be created and managed by the Landscape class and not
// ++ created or manipulated directly by the application.
// ++++
LandscapeRecord::LandscapeRecord()
{
  m_Loaded = false;
  #ifndef NO_OGRE_IN_LANDSCAPE
  m_OgreObject = NULL;
  m_ObjectIndex = 0;
  #endif
  OffsetX = 0.0;
  OffsetY = 0.0;
  Stride = cDefaultStride;
  Highest = 0.0;
  Lowest = 0.0;
}

LandscapeRecord::~LandscapeRecord()
{
  //empty destructor
  #ifndef NO_OGRE_IN_LANDSCAPE
  if (m_OgreObject != NULL)
  {
    Dusk::getAPI().getOgreSceneManager()->destroyManualObject(m_OgreObject);
    m_OgreObject = NULL;
  }
  #endif
}

bool LandscapeRecord::LoadFromStream(std::ifstream *AStream)
{
  if (m_Loaded || AStream==NULL)
  {
    return false;
  }
  if (!AStream->good())
  {
    std::cout << "LandscapeRecord::LoadFromStream: ERROR: passed stream "
              << "argument contains error(s).\n";
    return false;
  }

  char Land[4];
  Land[0] = Land[1] = Land[2] = Land[3] = '\0';
  //read header "Land"
  AStream->read(Land, 4);
  if ((Land[0]!='L') || (Land[1]!='a') || (Land[2]!='n') || (Land[3]!='d'))
  {
    std::cout << "LandscapeRecord::LoadFromStream: Stream contains invalid "
              << "Landscape record header.\n";
    return false;
  }
  //read offsets
  AStream->read((char*) &OffsetX, sizeof(float));
  AStream->read((char*) &OffsetY, sizeof(float));
  //stride
  AStream->read((char*) &Stride, sizeof(float));
  if (!AStream->good())
  {
    std::cout << "LandscapeRecord::LoadFromStream: ERROR: Stream seems to "
              << "have invalid Land record data.\n";
    return false;
  }
  if (Stride <=0.0f)
  {
    std::cout << "LandscapeRecord::LoadFromStream: Stream contains an invalid "
              << "stride value of "<< Stride <<".\n";
    Stride = 0.0f;
    return false;
  }//if

  //read the height data
  AStream->read((char*) &Height[0][0], 65*65*sizeof(float));
  if (!AStream->good())
  {
    std::cout << "LandscapeRecord::LoadFromStream: ERROR: Stream seems to have"
              << " invalid Land record height data.\n";
    return false;
  }

  //colour data
  AStream->read((char*) &Colour[0][0][0], 65*65*3);
  if (!AStream->good())
  {
    std::cout << "LandscapeRecord::LoadFromStream: ERROR: Stream seems to "
              << "have invalid Land record colour data.\n";
    return false;
  }

  //search Highest and Lowest values
  unsigned int i, j;

  Highest= Height[0][0];
  Lowest = Height[0][0];
  for (i=0; i<65; i++)
  {
    for (j=0; j<65; j++)
    {
      if (Height[i][j]>Highest)
      {
        Highest = Height[i][j];
      }
      else if (Height[i][j]<Lowest)
      {
        Lowest = Height[i][j];
      }
    }//for j
  }//for i
  m_Loaded = true;
  return true;
}//LoadFromStream

bool LandscapeRecord::SaveToStream(std::ofstream *AStream)
{
  if (!m_Loaded || AStream==NULL)
  {
    return false;
  }
  if (!AStream->good())
  {
    std::cout << "LandscapeRecord::SaveToStream: ERROR: passed stream argument"
              << " contains error(s).\n";
    return false;
  }
  //write header "Land"
  AStream->write("Land", 4);
  //write offsets
  AStream->write((char*) &OffsetX, sizeof(float));
  AStream->write((char*) &OffsetY, sizeof(float));
  //stride
  AStream->write((char*) &Stride, sizeof(float));
  //height data
  AStream->write((char*) &Height[0][0], 65*65*sizeof(float));
  //colour data
  AStream->write((char*) &Colour[0][0][0], 65*65*3);
  if (!AStream->good())
  {
    std::cout << "LandscapeRecord::SaveToStream: Error while writing record to"
              << " stream.\n";
    return false;
  }//if
  return true;
}//SaveToStream

bool LandscapeRecord::IsLoaded()
{
  return m_Loaded;
}

void LandscapeRecord::SetLoadedState(const bool value)
{
  m_Loaded = value;
}

bool LandscapeRecord::Shift(const float delta)
{
  unsigned int i,j;

  for (i=0; i<65; i++)
  {
    for (j=0; j<65; j++)
    {
      Height[i][j] = Height[i][j]+delta;
    }
  }
  Highest = Highest+delta;
  Lowest = Lowest+delta;
  return true;
}

bool LandscapeRecord::Scale(const float factor)
{
  if (!m_Loaded || factor<cMinScale)
  {
    std::cout << "LandscapeRecord::Scale: ERROR: Record is not loaded yet or "
              << "scaling factor ("<<factor<<") is invalid.\n";
    return false;
  }
  unsigned int i,j;

  for (i=0; i<65; i++)
  {
    for (j=0; j<65; j++)
    {
      Height[i][j] = Height[i][j]*factor;
    }
  }
  Highest = Highest*factor;
  Lowest = Lowest*factor;
  return true;
}

bool LandscapeRecord::MakePlain(const float value)
{
  unsigned int i,j;

  for (i=0; i<65; i++)
  {
    for (j=0; j<65; j++)
    {
      Height[i][j] = value;
    }
  }
  Highest = value;
  Lowest = value;
  return true;
}

bool LandscapeRecord::IsPlain()
{
  return Highest==Lowest;
}

#ifndef NO_OGRE_IN_LANDSCAPE
bool LandscapeRecord::SendDataToEngine()
{
  if (!m_Loaded)
  {
    std::cout << "LandscapeRecord::SendDataToEngine: ERROR: REcord has no valid data (yet).\n";
    return false;
  }

  Ogre::SceneManager * scm;
  unsigned int j, k;

  scm = Dusk::getAPI().getOgreSceneManager();
  if (scm==NULL)
  {
    std::cout << "LandscapeRecord::SendDataToEngine: ERROR: Got NULL for scene manager.\n";
    return false;
  }

  //get own scene node for landscape
  if (!scm->hasSceneNode(cLandNodeName))
  {
    std::cout << "LandscapeRecord::SendDataToEngine: ERROR: LandscapeNode does not exist.\n";
    return false;
  }
  Ogre::SceneNode * landnode;
  landnode = scm->getSceneNode(cLandNodeName);



  std::stringstream convert;
  m_ObjectIndex = m_ObjectCount;
  m_ObjectCount++;
  convert << m_ObjectIndex;

  m_OgreObject = scm->createManualObject("Landscape"+convert.str());
  m_OgreObject->estimateVertexCount(65*65);
  m_OgreObject->estimateIndexCount(64*64*6);
  m_OgreObject->setDynamic(false);
  m_OgreObject->begin("Landscape/Green", Ogre::RenderOperation::OT_TRIANGLE_LIST);
  //vectors
  for (j=0; j<65; j++)
  {
    for (k=0; k<65; k++)
    {
      m_OgreObject->position(OffsetX+cDefaultStride*j,
                             Height[j][k],
                             OffsetY+cDefaultStride*k);
      m_OgreObject->colour(Colour[j][k][0]/255.0f,
                           Colour[j][k][1]/255.0f,
                           Colour[j][k][2]/255.0f);
    }//for k
  }//for j

  //triangles
  for (j=0; j<64; j++)
  {
    for (k=0; k<64; k++)
    {
      //first triangle: [j][k], [j][k+1], [j+1][k]
      m_OgreObject->index(j*65+k);
      m_OgreObject->index(j*65+k+1);
      m_OgreObject->index((j+1)*65+k);
      //second triangle: [j+1][k], [j][k+1], [j+1][k+1]
      m_OgreObject->index((j+1)*65+k);
      m_OgreObject->index(j*65+k+1);
      m_OgreObject->index((j+1)*65+k+1);
    }//for k
  }//for j
  m_OgreObject->end();
  landnode->attachObject(m_OgreObject);
  return true;
}

bool LandscapeRecord::RemoveDataFromEngine()
{
  if (m_OgreObject == NULL)
  {
    return true;
  }

  Ogre::SceneManager * scm;
  scm = Dusk::getAPI().getOgreSceneManager();
  if (scm==NULL)
  {
    std::cout << "LandscapeRecord::RemoveDataFromEngine: ERROR: Got NULL for "
              << "scene manager.\n";
    return false;
  }

  //get scene node for landscape
  if (!scm->hasSceneNode(cLandNodeName))
  {
    std::cout << "LandscapeRecord::RemoveDataFromEngine: ERROR: LandscapeNode does not exist.\n";
    return false;
  }
  Ogre::SceneNode * landnode;
  landnode = scm->getSceneNode(cLandNodeName);
  landnode->detachObject(m_OgreObject);
  scm->destroyManualObject(m_OgreObject);
  m_OgreObject = NULL;
  return true;
}
#endif //ifndef NO_OGRE_IN_LANDSCAPE

//the main Landscape class
// ++++
// ++ Basically it will be a sort of manager for all the individual landscape
// ++ records and the point through which other classes access the landscape
// ++ data.
// ++++
Landscape::Landscape()
{
  //empty
  m_RecordList = NULL;
  m_numRec = 0;
  //m_ObjectList = NULL;
  //m_numObj = 0;
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

  m_RecordList = new LandscapeRecord[numRecords];
  m_numRec = numRecords;

  //read loop
  for (i=0; i<numRecords; i=i+1)
  {
    if (!m_RecordList[i].LoadFromStream(&input))
    {
      std::cout << "Landscape::LoadFromFile: File \""<<FileName<< "\" has "
                << "invalid record data.\n";
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
    if (!m_RecordList[i].SaveToStream(&output))
    {
      std::cout << "Landscape::SaveToFile: Error while writing record "<<i+1
                << " to file \"" <<FileName<<"\".\n";
      output.close();
      return false;
    }
  }//for
  output.close();
  return true;
}//SaveToFile

unsigned int Landscape::RecordsAvailable()
{
  return m_numRec;
}

LandscapeRecord* Landscape::GetRecord(const unsigned int record)
{
  if (record>=m_numRec)
  {
    return NULL;
  }
  return &m_RecordList[record];
}

#ifndef NO_OGRE_IN_LANDSCAPE
bool Landscape::SendToEngine()
{
  if (m_numRec==0)
  {
    return false;
  }

  Ogre::SceneManager * scm;
  unsigned int i;

  scm = Dusk::getAPI().getOgreSceneManager();
  if (scm==NULL)
  {
    std::cout << "Landscape::SendToEngine: ERROR: Got NULL for scene manager.\n";
    return false;
  }

  //create own scene node for landscape
  Ogre::SceneNode * landnode;
  landnode = scm->getRootSceneNode()->createChildSceneNode(cLandNodeName, Ogre::Vector3(0.0, 0.0, 0.0));

  for (i=0; i<m_numRec; i++)
  {
    m_RecordList[i].SendDataToEngine();
  }//for
  return true;
}
#endif //ifndef NO_OGRE_IN_LANDSCAPE

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
