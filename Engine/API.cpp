/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2007, 2008, 2010 walljumper, thoronador

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

#include "API.h"
#ifndef DUSK_EDITOR
  #include "Application.h"
#else
  #include "../Editor/EditorApplication.h"
#endif

namespace Dusk
{
    API& API::getSingelton()
    {
        static API Instance;
        return Instance;
    }

    API::API()
    {
        //ctor
        m_Root = NULL;
        m_Window = NULL;
        m_SceneManager = NULL;
        m_Application = NULL;
        m_FrameListener = NULL;
    }

    void API::setOgreObjects(Ogre::Root* root,Ogre::RenderWindow* window,Ogre::SceneManager* mgr)
    {
        m_Root = root;
        m_Window = window;
        m_SceneManager = mgr;
    }

    #ifndef DUSK_EDITOR
    void API::setApplication(Dusk::Application* app)
    {
        m_Application = app;
    }

    void API::setFrameListener(Dusk::FrameListener* op)
    {
        m_FrameListener = op;
    }
    #else
    void API::setApplication(EditorApplication* app)
    {
        m_Application = app;
    }

    void API::setFrameListener(EditorFrameListener* op)
    {
        m_FrameListener = op;
    }
    #endif

    //basic get functions
    Ogre::Root* API::getOgreRoot()
    {
        return m_Root;
    }

    Ogre::RenderWindow* API::getOgreRenderWindow()
    {
        return m_Window;
    }

    Ogre::SceneManager* API::getOgreSceneManager()
    {
        return m_SceneManager;
    }

    #ifndef DUSK_EDITOR
    Dusk::Application* API::getApplication()
    {
        return m_Application;
    }

    Dusk::FrameListener* API::getFrameListener()
    {
        return m_FrameListener;
    }
    #else
    EditorApplication* API::getApplication()
    {
        return m_Application;
    }

    EditorFrameListener* API::getFrameListener()
    {
        return m_FrameListener;
    }
    #endif

} //namespace
