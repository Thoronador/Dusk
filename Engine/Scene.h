#ifndef SCENE_H
#define SCENE_H

namespace Dusk
{
    class Scene
    {
    public:

        virtual ~Scene();
        static Scene& get();
    protected:
    private:
    Scene();
    Scene(const Scene& op){}
    };
}


#endif // SCENE_H
