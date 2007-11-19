#ifndef DUSKOBJECT_H
#define DUSKOBJECT_H


class DuskObject
{
    public:
        DuskObject();
        virtual ~DuskObject();
    protected:
    private:
        std::string ID;
        std::string objectType;
        Ogre::Entity *entity;
};



#endif // DUSKOBJECT_H
