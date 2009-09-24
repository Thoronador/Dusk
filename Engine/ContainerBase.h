#ifndef CONTAINERBASE_H
#define CONTAINERBASE_H

#include <string>
#include <map>
#include "Inventory.h"

namespace Dusk
{
  struct ContainerRecord
  {
    std::string Mesh;
    Inventory ContainerInventory;
  };

  class ContainerBase
  {
    public:
      virtual ~ContainerBase();
      ///singleton access method
      static ContainerBase& GetSingleton();

      /* Adds Container prototype with given ID, the mesh _mesh and the contents of ''contents''. */
      void AddContainer(const std::string& ID, const std::string& _mesh, const Inventory& contents);

      /* Deletes Container with given ID. Returns true, if such a Container
         was present, false otherwise. */
      bool DeleteContainer(const std::string& ID);

      /* Returns true, if a Container with ID ID is present, false otherwise.*/
      bool HasContainer(const std::string& ID) const;

      /* Returns the mesh of container with given ID, if present. */
      std::string GetContainerMesh(const std::string& ID, const bool UseMarkerOnError=true) const;

      /* Returns the inventory of the container, if present. */
      const Inventory& GetContainerInventory(const std::string& ID) const;

      /* Removes all containers from list. */
      void DeleteAllContainers();
    private:
      ContainerBase();
      // copy constructor
      ContainerBase(const ContainerBase& op) {}
      std::map<std::string, ContainerRecord> m_ContainerList;
  };//class

}//namespace

#endif // CONTAINERBASE_H
