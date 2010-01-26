#include "NPC.h"
#include "NPCBase.h"
#include "Settings.h"
#include "DuskConstants.h"
#include <sstream>

namespace Dusk
{

NPC::NPC()
  : AnimatedObject("", Ogre::Vector3::ZERO, Ogre::Vector3::ZERO, 1.0f)
{
  //assume some random data
  //all attributes to one
  m_Strength=m_Agility=m_Vitality=m_Intelligence=m_Willpower=m_Charisma=m_Luck=1;
  m_Level = 1; //assume level 1
  m_Inventory.MakeEmpty(); //empty inventory
  m_Health = getMaxHealth();
}

NPC::NPC(const std::string _ID, const Ogre::Vector3 pos, const Ogre::Vector3 rot, const float Scale)
 : AnimatedObject(_ID, pos, rot, Scale)
{
  if (NPCBase::GetSingleton().hasNPC(ID))
  {
    //feed the data from NPCBase
    //-- attributes
    const NPCAttributes temp_attr = NPCBase::GetSingleton().getAttributes(ID);
    m_Strength = temp_attr.Str;
    m_Agility = temp_attr.Agi;
    m_Vitality = temp_attr.Vit;
    m_Intelligence = temp_attr.Int;
    m_Willpower = temp_attr.Will;
    m_Charisma = temp_attr.Cha;
    m_Luck = temp_attr.Luck;
    //-- level
    m_Level = NPCBase::GetSingleton().getLevel(ID);
    m_Inventory = NPCBase::GetSingleton().getNPCInventory(ID);
  }
  else
  { //assume some random data
    //all attributes to one
    m_Strength=m_Agility=m_Vitality=m_Intelligence=m_Willpower=m_Charisma=m_Luck=1;
    m_Level = 1; //assume level 1
    m_Inventory.MakeEmpty(); //empty inventory
  }
  m_Health = getMaxHealth();
}

NPC::~NPC()
{
  //empty
}

ObjectTypes NPC::GetType() const
{
  return otNPC;
}

float NPC::getHealth() const
{
  return m_Health;
}

void NPC::setHealth(const float new_health)
{
  m_Health = new_health;
  if (m_Health<0.0f) m_Health = 0.0f;
}
bool NPC::isAlive() const
{
  return (m_Health>0.0f);
}

uint8 NPC::getLevel() const
{
  return m_Level;
}

uint8 NPC::getStrength() const
{
  return m_Strength;
}

uint8 NPC::getAgility() const
{
  return m_Strength;
}

uint8 NPC::getVitality() const
{
  return m_Vitality;
}

uint8 NPC::getIntelligence() const
{
  return m_Intelligence;
}

uint8 NPC::getCharisma() const
{
  return m_Charisma;
}

uint8 NPC::getWillpower() const
{
  return m_Willpower;
}

uint8 NPC::getLuck() const
{
  return m_Luck;
}

void NPC::setLevel(const uint8 new_level)
{
  m_Level = new_level;
}


void NPC::setStrength(const uint8 str)
{
  m_Strength = str;
}

void NPC::setAgility(const uint8 agi)
{
  m_Agility = agi;
}

void NPC::setVitality(const uint8 vit)
{
  m_Vitality = vit;
}

void NPC::setIntelligence(const uint8 intelligence)
{
  m_Intelligence = intelligence;
}

void NPC::setWillpower(const uint8 wil)
{
  m_Willpower = wil;
}

void NPC::setCharisma(const uint8 cha)
{
  m_Charisma = cha;
}

void NPC::setLuck(const uint8 luck)
{
  m_Luck = luck;
}

Inventory& NPC::getInventory()
{
  return m_Inventory;
}

const Inventory& NPC::getConstInventory() const
{
  return m_Inventory;
}

unsigned int NPC::getMaxEncumbrance() const
{
  const Settings& set = Settings::GetSingleton();
  return set.getSetting_uint("BaseEncumbrance") +
         set.getSetting_uint("EncumbranceStrengthCoefficient")*m_Strength;
  //cBaseEncumbrance + cEncumbranceStrengthCoefficient*m_Strength
}

unsigned int NPC::getMaxHealth() const
{
  const Settings& set = Settings::GetSingleton();
  return set.getSetting_uint("HealthBase")
         + set.getSetting_uint("HealthVitalityFactor")*m_Strength
         + set.getSetting_uint("HealthLevelFactor")*m_Level;
  //return cHealthBase + cHealthVitalityFactor*m_Vitality + cHealthLevelFactor*m_Level;
}

bool NPC::Enable(Ogre::SceneManager* scm)
{
  if (entity!=NULL)
  {
    return true;
  }
  if (scm==NULL)
  {
    std::cout << "NPC::Enable: ERROR: no scene manager present.\n";
    return false;
  }
  //generate unique entity name
  std::stringstream entity_name;
  entity_name << ID << GenerateUniqueObjectID();
  //create entity + node and attach entity to node
  entity = scm->createEntity(entity_name.str(), NPCBase::GetSingleton().getNPCMesh(ID));
  Ogre::SceneNode* ent_node = scm->getRootSceneNode()->createChildSceneNode(entity_name.str(), position);
  ent_node->attachObject(entity);
  ent_node->scale(m_Scale, m_Scale, m_Scale);
  //not sure whether this is the best one for rotation
  ent_node->rotate(Ogre::Vector3::UNIT_X, Ogre::Degree(rotation.x));
  ent_node->rotate(Ogre::Vector3::UNIT_Y, Ogre::Degree(rotation.y));
  ent_node->rotate(Ogre::Vector3::UNIT_Z, Ogre::Degree(rotation.z));
  //set user defined object to this NPC as reverse link
  entity->setUserObject(this);
  if (m_Anim != "")
  {
    Ogre::AnimationStateSet* anim_set = entity->getAllAnimationStates();
    if (anim_set->hasAnimationState(m_Anim))
    {
      Ogre::AnimationState* state = anim_set->getAnimationState(m_Anim);
      state->setTimePosition(0.0f);
      state->setLoop(m_LoopAnim);
      state->setEnabled(true);
    }
  }
  return (entity!=NULL);
}

bool NPC::SaveToStream(std::ofstream& OutStream) const
{
  if (!OutStream.good())
  {
    std::cout << "NPC::SaveToStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //write header "RefN" (reference of NPC)
  OutStream.write((char*) &cHeaderRefN, sizeof(unsigned int));
  //save stuff inherited from DuskObject
  if (!SaveDuskObjectPart(OutStream))
  {
    std::cout << "NPC::SaveToStream: ERROR while saving basic data!\n";
    return false;
  }
  // go on with new data members from AnimatedObject
  if (!SaveAnimatedObjectPart(OutStream))
  {
    std::cout << "NPC::SaveToStream: ERROR while saving animation data!\n";
    return false;
  }
  //done with inherited data members; go on with NPC stuff
  //health
  OutStream.write((char*) &m_Health, sizeof(float));
  //level
  OutStream.write((char*) &m_Level, 1);
  //attributes
  OutStream.write((char*) &m_Strength, 1);
  OutStream.write((char*) &m_Agility, 1);
  OutStream.write((char*) &m_Vitality, 1);
  OutStream.write((char*) &m_Intelligence, 1);
  OutStream.write((char*) &m_Willpower, 1);
  OutStream.write((char*) &m_Charisma, 1);
  OutStream.write((char*) &m_Luck, 1);

  if (!OutStream.good())
  {
    std::cout << "NPC::SaveToStream: ERROR while writing character data.\n";
    return false;
  }
  if (!(m_Inventory.SaveToStream(OutStream)))
  {
    std::cout << "NPC::SaveToStream: ERROR while writing inventory data.\n";
    return false;
  }
  return OutStream.good();
}

bool NPC::LoadFromStream(std::ifstream& InStream)
{
  if (entity!=NULL)
  {
    std::cout << "NPC::LoadFromStream: ERROR: Cannot load from stream while "
              << "object is enabled.\n";
    return false;
  }
  if (!InStream.good())
  {
    std::cout << "NPC::LoadFromStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //read header "RefN"
  unsigned int Header = 0;
  InStream.read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderRefN)
  {
    std::cout << "NPC::LoadFromStream: ERROR: Stream contains invalid reference"
              << "header.\n";
    return false;
  }
  //read DuskObject stuff
  if (!LoadDuskObjectPart(InStream))
  {
    std::cout << "NPC::LoadFromStream: ERROR while reading basic data.\n";
    return false;
  }
  // go on with data members from AnimatedObject
  if (!LoadAnimatedObjectPart(InStream))
  {
    std::cout << "NPC::LoadFromStream: ERROR while loading animation data.\n";
    return false;
  }
  //done with inherited data, go on with NPC data

  //health
  InStream.read((char*) &m_Health, sizeof(float));
  //level
  InStream.read((char*) &m_Level, 1);
  //attributes
  InStream.read((char*) &m_Strength, 1);
  InStream.read((char*) &m_Agility, 1);
  InStream.read((char*) &m_Vitality, 1);
  InStream.read((char*) &m_Intelligence, 1);
  InStream.read((char*) &m_Willpower, 1);
  InStream.read((char*) &m_Charisma, 1);
  InStream.read((char*) &m_Luck, 1);

  if (!InStream.good())
  {
    std::cout << "NPC::LoadFromStream: ERROR while reading character data.\n";
    return false;
  }
  if (!(m_Inventory.LoadFromStream(InStream)))
  {
    std::cout << "NPC::LoadFromStream: ERROR while reading inventory data.\n";
    return false;
  }
  return InStream.good();
}

} //namespace
