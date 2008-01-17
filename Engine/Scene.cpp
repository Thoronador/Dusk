#include "Scene.h"
#include "API.h"
namespace Dusk
{

Scene::Scene()
{

    //ctor
    getAPI().setScene(this);
}

Scene::~Scene()
{
    //dtor
}

}
