#ifndef DUSKOBJECT_H
#define DUSKOBJECT_H

#include <string>
#include <OgreEntity.h>
#include <OgreVector3.h>

namespace Dusk{

class DuskObject
{
    public:
        DuskObject();
        virtual ~DuskObject();
    protected:
        std::string ID;
        std::string objectType;
        Ogre::Entity *entity;
        Ogre::Vector3 position, rotation;
};

}

#endif // DUSKOBJECT_H
