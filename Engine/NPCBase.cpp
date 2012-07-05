/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2009, 2010, 2011, 2012 thoronador

    The Dusk Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Dusk Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Dusk Engine.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------------
*/

#include "NPCBase.h"
#include "Messages.h"
#include "DuskConstants.h"
#include "DuskExceptions.h"
#include "DuskFunctions.h"

namespace Dusk
{

NPCAttributes NPCAttributes::getNullAttributes()
{
  NPCAttributes temp;
  temp.Str=temp.Agi=temp.Vit=temp.Int=temp.Will=temp.Cha=temp.Luck=0;
  return temp;
}

const NPCAnimations& NPCAnimations::getNullAnimations()
{
  static NPCAnimations anims;
  anims.Idle = anims.Walk = anims.MeleeAttack = anims.ProjectileAttack
             = anims.Jump = anims.Death = "";
  return anims;
}

const NPCTagPoints& NPCTagPoints::getNullTagPoints()
{
  static NPCTagPoints tags;
  tags.HandLeft = tags.HandRight = tags.SheathLeft = tags.SheathRight = "";
  return tags;
}

uint32_t NPCRecord::getRecordType() const
{
  return cHeaderNPC_;
}

bool NPCRecord::saveToStream(std::ofstream& output) const
{
  if (!(output.good()))
  {
    DuskLog() << "NPCRecord::saveToStream: ERROR: Bad stream.\n";
    return false;
  }
  uint32_t len;

  //header
  output.write((const char*) &cHeaderNPC_, sizeof(uint32_t));
  //ID
  len = ID.length();
  output.write((const char*) &len, sizeof(uint32_t));
  output.write(ID.c_str(), len);
  //NPC data
  // -- name
  len = Name.length();
  output.write((const char*) &len, sizeof(uint32_t));
  output.write(Name.c_str(), len);
  // -- mesh
  len = Mesh.length();
  output.write((const char*) &len, sizeof(uint32_t));
  output.write(Mesh.c_str(), len);
  // -- level
  output.write((const char*) &Level, 1);
  // -- attributes
  output.write((const char*) &(Attributes.Str), 1);
  output.write((const char*) &(Attributes.Agi), 1);
  output.write((const char*) &(Attributes.Vit), 1);
  output.write((const char*) &(Attributes.Int), 1);
  output.write((const char*) &(Attributes.Will), 1);
  output.write((const char*) &(Attributes.Cha), 1);
  output.write((const char*) &(Attributes.Luck), 1);
  // -- female flag
  output.write((const char*) &Female, sizeof(bool));
  //inventory
  if (!InventoryAtStart.saveToStream(output))
  {
    DuskLog() << "NPCRecord::saveToStream: ERROR while writing inventory data "
              << "of NPC \""<<ID<<"\" to stream.\n";
    return false;
  }
  // -- animations
  // ---- idle
  len = Animations.Idle.length();
  output.write((const char*) &len, sizeof(uint32_t));
  output.write(Animations.Idle.c_str(), len);
  // ---- walk
  len = Animations.Walk.length();
  output.write((const char*) &len, sizeof(uint32_t));
  output.write(Animations.Walk.c_str(), len);
  // ---- melee
  len = Animations.MeleeAttack.length();
  output.write((const char*) &len, sizeof(uint32_t));
  output.write(Animations.MeleeAttack.c_str(), len);
  // ---- projectile
  len = Animations.ProjectileAttack.length();
  output.write((const char*) &len, sizeof(uint32_t));
  output.write(Animations.ProjectileAttack.c_str(), len);
  // ---- jump
  len = Animations.Jump.length();
  output.write((const char*) &len, sizeof(uint32_t));
  output.write(Animations.Jump.c_str(), len);
  // ---- death
  len = Animations.Death.length();
  output.write((const char*) &len, sizeof(uint32_t));
  output.write(Animations.Death.c_str(), len);
  // -- tag points
  // ---- left hand
  len = TagPoints.HandLeft.length();
  output.write((const char*) &len, sizeof(uint32_t));
  output.write(TagPoints.HandLeft.c_str(), len);
  // ---- right hand
  len = TagPoints.HandRight.length();
  output.write((const char*) &len, sizeof(uint32_t));
  output.write(TagPoints.HandRight.c_str(), len);
  // ---- left sheath
  len = TagPoints.SheathLeft.length();
  output.write((const char*) &len, sizeof(uint32_t));
  output.write(TagPoints.SheathLeft.c_str(), len);
  // ---- right sheath
  len = TagPoints.SheathRight.length();
  output.write((const char*) &len, sizeof(uint32_t));
  output.write(TagPoints.SheathRight.c_str(), len);
  if (!output.good())
  {
    DuskLog() << "NPCRecord::saveToStream: ERROR while writing animations and "
              << "tagpoints of NPC \""<<ID<<"\" to stream.\n";
    return false;
  }
  return output.good();
}

bool NPCRecord::loadFromStream(std::ifstream& input)
{
  if (!(input.good()))
  {
    DuskLog() << "NPCRecord::loadFromStream: ERROR: Bad stream.\n";
    return false;
  }
  uint32_t len = 0;
  //header
  input.read((char*) &len, sizeof(uint32_t));
  if (len != cHeaderNPC_)
  {
    DuskLog() << "NPCRecord::loadFromStream: ERROR: Invalid header.\n";
    return false;
  }
  //ID
  len = 0;
  input.read((char*) &len, sizeof(uint32_t));
  if (len > 255)
  {
    DuskLog() << "NPCRecord::loadFromStream: ERROR: NPC ID seems to be"
              << " longer than 255 characters.\n";
    return false;
  }
  char buffer[256];
  buffer[0] = '\0';
  input.read(buffer, len);
  buffer[len] = '\0';
  if (!(input.good()))
  {
    DuskLog() << "NPCRecord::loadFromStream: ERROR while reading NPC ID.\n";
    return false;
  }
  ID = std::string(buffer);
  //name
  len = 0;
  input.read((char*) &len, sizeof(uint32_t));
  if (len > 255)
  {
    DuskLog() << "NPCRecord::loadFromStream: ERROR: NPC name seems to "
              << "be longer than 255 characters.\n";
    return false;
  }
  buffer[0] = '\0';
  input.read(buffer, len);
  buffer[len] = '\0';
  if (!(input.good()))
  {
    DuskLog() << "NPCRecord::loadFromStream: ERROR while reading NPC "
              << "name from stream.\n";
    return false;
  }
  Name = std::string(buffer);
  //mesh
  len = 0;
  input.read((char*) &len, sizeof(uint32_t));
  if (len > 255)
  {
    DuskLog() << "NPCRecord::loadFromStream: ERROR: NPC mesh seems to "
              << "be longer than 255 characters.\n";
    return false;
  }
  buffer[0] = '\0';
  input.read(buffer, len);
  buffer[len] = '\0';
  if (!(input.good()))
  {
    DuskLog() << "NPCRecord::loadFromStream: ERROR while reading NPC "
              << "mesh location from stream.\n";
    return false;
  }
  Mesh = std::string(buffer);
  //level
  input.read((char*) &Level, 1);
  //attributes
  input.read((char*) &(Attributes.Str), 1);
  input.read((char*) &(Attributes.Agi), 1);
  input.read((char*) &(Attributes.Vit), 1);
  input.read((char*) &(Attributes.Int), 1);
  input.read((char*) &(Attributes.Will), 1);
  input.read((char*) &(Attributes.Cha), 1);
  input.read((char*) &(Attributes.Luck), 1);
  //female?
  input.read((char*) &Female, sizeof(bool));
  if (!input.good())
  {
    DuskLog() << "NPCRecord::loadFromStream: ERROR while reading NPC "
              << "level or attributes from stream.\n";
    return false;
  }
  //inventory
  if (!InventoryAtStart.loadFromStream(input))
  {
    DuskLog() << "NPCRecord::loadFromStream: ERROR while reading NPC "
              << "inventory from stream.\n";
    return false;
  }
  //animations
  // -- idle
  len = 0;
  input.read((char*) &len, sizeof(uint32_t));
  if (len > 255)
  {
    DuskLog() << "NPCRecord::loadFromStream: ERROR: NPC idle animation"
              << " name of \""<<ID<<"\" seems to be longer than 255 "
              << "characters.\n";
    return false;
  }
  buffer[0] = '\0';
  input.read(buffer, len);
  buffer[len] = '\0';
  if (!input.good())
  {
    DuskLog() << "NPCRecord::loadFromStream: ERROR while reading NPC "
              << "idle animation name from stream.\n";
    return false;
  }
  Animations.Idle = std::string(buffer);
  // -- walk
  len = 0;
  input.read((char*) &len, sizeof(uint32_t));
  if (len > 255)
  {
    DuskLog() << "NPCRecord::loadFromStream: ERROR: NPC walk animation "
              << " name of \""<<ID<<"\" seems to be longer than 255 "
              << "characters.\n";
    return false;
  }
  buffer[0] = '\0';
  input.read(buffer, len);
  buffer[len] = '\0';
  if (!input.good())
  {
    DuskLog() << "NPCRecord::loadFromStream: ERROR while reading NPC "
              << "walk animation name from stream.\n";
    return false;
  }
  Animations.Walk = std::string(buffer);
  // -- melee
  len = 0;
  input.read((char*) &len, sizeof(uint32_t));
  if (len > 255)
  {
    DuskLog() << "NPCRecord::loadFromStream: ERROR: NPC melee animation"
              << " name of \""<<ID<<"\" seems to be longer than 255 "
              << "characters.\n";
    return false;
  }
  buffer[0] = '\0';
  input.read(buffer, len);
  buffer[len] = '\0';
  if (!input.good())
  {
    DuskLog() << "NPCRecord::loadFromStream: ERROR while reading NPC "
              << "melee animation name from stream.\n";
    return false;
  }
  Animations.MeleeAttack = std::string(buffer);
  // -- projectile
  len = 0;
  input.read((char*) &len, sizeof(uint32_t));
  if (len > 255)
  {
    DuskLog() << "NPCRecord::loadFromStream: ERROR: NPC projectile "
              << "animation name of\""<<ID<< " seems to be longer than 255 "
              << "characters.\n";
    return false;
  }
  buffer[0] = '\0';
  input.read(buffer, len);
  buffer[len] = '\0';
  if (!input.good())
  {
    DuskLog() << "NPCRecord::loadFromStream: ERROR while reading NPC "
              << "projectile attack animation name from stream.\n";
    return false;
  }
  Animations.ProjectileAttack = std::string(buffer);
  // -- jump
  len = 0;
  input.read((char*) &len, sizeof(uint32_t));
  if (len > 255)
  {
    DuskLog() << "NPCRecord::loadFromStream: ERROR: NPC jum animation "
              << "name of \""<<ID<<"\" seems to be longer than 255 "
              << "characters.\n";
    return false;
  }
  buffer[0] = '\0';
  input.read(buffer, len);
  buffer[len] = '\0';
  if (!input.good())
  {
    DuskLog() << "NPCRecord::loadFromStream: ERROR while reading NPC "
              << "jump animation name from stream.\n";
    return false;
  }
  Animations.Jump = std::string(buffer);
  // -- death
  len = 0;
  input.read((char*) &len, sizeof(uint32_t));
  if (len > 255)
  {
    DuskLog() << "NPCRecord::loadFromStream: ERROR: NPC death animation"
              << " name of \""<<ID<<"\" seems to be longer than 255 "
              << "characters.\n";
    return false;
  }
  buffer[0] = '\0';
  input.read(buffer, len);
  buffer[len] = '\0';
  if (!input.good())
  {
    DuskLog() << "NPCRecord::loadFromStream: ERROR while reading NPC "
              << "death animation name from stream.\n";
    return false;
  }
  Animations.Death = std::string(buffer);

  //tag points
  // -- left hand
  len = 0;
  input.read((char*) &len, sizeof(uint32_t));
  if (len > 255)
  {
    DuskLog() << "NPCRecord::loadFromStream: ERROR: NPC TagPoint name"
              << " (left hand) of \""<<ID<<"\" seems to be longer than 255"
              << " characters.\n";
    return false;
  }
  buffer[0] = '\0';
  input.read(buffer, len);
  buffer[len] = '\0';
  if (!input.good())
  {
    DuskLog() << "NPCRecord::loadFromStream: ERROR while reading NPC "
              << "TagPoint name (left hand) of \""<<ID<<"\" from stream.\n";
    return false;
  }
  TagPoints.HandLeft = std::string(buffer);
  // -- right hand
  len = 0;
  input.read((char*) &len, sizeof(uint32_t));
  if (len > 255)
  {
    DuskLog() << "NPCRecord::loadFromStream: ERROR: NPC TagPoint name"
              << " seems to be longer than 255 characters.\n";
    return false;
  }
  buffer[0] = '\0';
  input.read(buffer, len);
  buffer[len] = '\0';
  if (!input.good())
  {
    DuskLog() << "NPCRecord::loadFromStream: ERROR while reading NPC "
              << "TagPoint name (right hand) of \""<<ID<<"\" from stream.\n";
    return false;
  }
  TagPoints.HandRight = std::string(buffer);
  // -- left sheath
  len = 0;
  input.read((char*) &len, sizeof(uint32_t));
  if (len > 255)
  {
    DuskLog() << "NPCRecord::loadFromStream: ERROR: NPC TagPoint name"
              << " (left sheath) of \""<<ID<<"\" seems to be longer than "
              << "255 characters.\n";
    return false;
  }
  buffer[0] = '\0';
  input.read(buffer, len);
  buffer[len] = '\0';
  if (!input.good())
  {
    DuskLog() << "NPCRecord::loadFromStream: ERROR while reading NPC "
              << "TagPoint name (left sheath) of \""<<ID<<"\" from stream.\n";
    return false;
  }
  TagPoints.SheathLeft = std::string(buffer);
  // -- righ sheath
  len = 0;
  input.read((char*) &len, sizeof(uint32_t));
  if (len > 255)
  {
    DuskLog() << "NPCRecord::loadFromStream: ERROR: NPC TagPoint name"
              << " (left sheath) of \""<<ID<<"\" seems to be longer than "
              << "255 characters.\n";
    return false;
  }
  buffer[0] = '\0';
  input.read(buffer, len);
  buffer[len] = '\0';
  if (!input.good())
  {
    DuskLog() << "NPCRecord::loadFromStream: ERROR while reading NPC "
              << "TagPoint name (right sheath) of \""<<ID<<"\" from stream.\n";
    return false;
  }
  TagPoints.SheathRight = std::string(buffer);

  return input.good();
}

}//namespace
