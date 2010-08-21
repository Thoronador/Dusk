#ifndef VERTEXDATAFUNC_H
#define VERTEXDATAFUNC_H

#include <OgreMesh.h>
#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <OgreEntity.h>

namespace Dusk
{

// Get the mesh information for the given mesh.
// Code found in Wiki: www.ogre3d.org/wiki/index.php/RetrieveVertexData
void GetMeshInformation(const Ogre::MeshPtr mesh,
                        size_t &vertex_count, Ogre::Vector3* &vertices,
                        size_t &index_count, unsigned long* &indices,
                        const Ogre::Vector3 &position,
                        const Ogre::Quaternion &orient,
                        const Ogre::Vector3 &scale);

// function version for animated entities
void GetMeshInformationAnimated(const Ogre::Entity *entity,
                                size_t &vertex_count, Ogre::Vector3* &vertices,
                                size_t &index_count,  unsigned long* &indices,
                                const Ogre::Vector3 &position,
                                const Ogre::Quaternion &orient,
                                const Ogre::Vector3 &scale);
} //namespace

#endif
