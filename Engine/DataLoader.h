#ifndef DATALOADER_H
#define DATALOADER_H

#include <string>

namespace Dusk
{
  //Flags to indicate which portions to load/ save
  const unsigned int CONTAINER_BIT = 1;
  const unsigned int LANDSCAPE_BIT = 2;
  const unsigned int LIGHT_BIT = 4;
  const unsigned int ITEM_BIT = 8;
  const unsigned int OBJECT_BIT = 16;
  const unsigned int REFERENCE_BIT = 32;


  const unsigned int ALL_BITS = CONTAINER_BIT | LANDSCAPE_BIT | LIGHT_BIT |
                                ITEM_BIT | OBJECT_BIT | REFERENCE_BIT;


/*class DataLoader:
        This class is (or will be) the main entry point for loading game data
        from files and saving data to files. It calls the individual classes for
        the different types of data encountered in a file and dispatches reading
        to them as needed. Thus, all one will need to do later is to call the
        corresponding method (LoadFromFile/ SaveToFile?) for every data file to
        load everything one needs.
*/
class DataLoader
{
  public:
    virtual ~DataLoader();
    static DataLoader& GetSingleton();
    bool SaveToFile(const std::string& FileName, const unsigned int bits = ALL_BITS);
    bool LoadFromFile(const std::string& FileName);
    void ClearData(const unsigned int bits = ALL_BITS);
  private:
    DataLoader();
    DataLoader(const DataLoader& op){}
};//class

}//namespace

#endif //DATALOADER_H
