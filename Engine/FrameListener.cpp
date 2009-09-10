#include "FrameListener.h"
#include<sstream>
#include "Console.h"
#include "InputSystem.h"
#include "InputSystemEditor.h"
#include "InputSystemBinding.h"
#include "InputSystem.h"
#include "API.h"
#include "Camera.h"
#include "AnimationData.h"

namespace Dusk
{
//-----------------------------------------------------------------------------------
FrameListener::FrameListener()
{
    m_Continue = true;
    getAPI().setFrameListener(this);
}
//-----------------------------------------------------------------------------------
FrameListener::~FrameListener()
{
    //dtor
}
//-----------------------------------------------------------------------------------
bool FrameListener::frameStarted(const Ogre::FrameEvent& evt)
{
    InputSystem::captureInput();
    Console::getInstance()->processScripts();
    getAPI().getDuskCamera()->move(evt);
    AnimationData::GetSingleton().InjectAnimationTime(evt.timeSinceLastFrame);
    return m_Continue;
}
//-----------------------------------------------------------------------------------
bool FrameListener::frameEnded(const Ogre::FrameEvent& evt)
{
    return m_Continue;
}
//-----------------------------------------------------------------------------------
void FrameListener::exit()
{
    m_Continue = false;
}

//-----------------------------------------------------------------------------------
//Keyboard events
/*
bool FrameListener::keyPressed( const OIS::KeyEvent &arg )
{
    switch(arg.key)
        {
            case OIS::KC_ESCAPE :
            {
                m_Continue = false; break;
            }
            case OIS::KC_GRAVE :
            {
                m_EditorIsVisible = m_InputSystemEditor->toggle();

                if(m_InputHandler.getInputSystem() == m_InputSystemBinding)
                    m_InputHandler.setInputSystem(m_InputSystemEditor);
                else
                    m_InputHandler.setInputSystem(m_InputSystemBinding);

                break;
            }
            case OIS::KC_0 :
            {

            }
            case OIS::KC_1 :
            {

            }
            case OIS::KC_2 :
            {

            }
            case OIS::KC_3 :
            {

            }
            case OIS::KC_4 :
            {

            }
            case OIS::KC_5 :
            {

            }
            case OIS::KC_6 :
            {

            }
            case OIS::KC_7 :
            {

            }
            case OIS::KC_8 :
            {

            }
            case OIS::KC_9 :
            {

            }
            case OIS::KC_A :
            {

            }
            case OIS::KC_B :
            {

            }
            case OIS::KC_C :
            {

            }
            case OIS::KC_D :
            {

            }
            case OIS::KC_E :
            {

            }
            case OIS::KC_F :
            {

            }
            case OIS::KC_G :
            {

            }
            case OIS::KC_H :
            {

            }
            case OIS::KC_I :
            {

            }
            case OIS::KC_J :
            {

            }
            case OIS::KC_K :
            {

            }
            case OIS::KC_L :
            {

            }
            case OIS::KC_M :
            {

            }
            case OIS::KC_N :
            {

            }
            case OIS::KC_O :
            {

            }
            case OIS::KC_P :
            {

            }
            case OIS::KC_Q :
            {

            }
            case OIS::KC_R :
            {

            }
            case OIS::KC_S :
            {

            }
            case OIS::KC_T :
            {

            }
            case OIS::KC_U :
            {

            }
            case OIS::KC_V :
            {

            }
            case OIS::KC_W :
            {

            }
            case OIS::KC_X :
            {

            }
            case OIS::KC_Y :
            {

            }
            case OIS::KC_Z :
            {

            }
            case OIS::KC_F1 :
            {

            }
            case OIS::KC_F2 :
            {

            }
            case OIS::KC_F3 :
            {

            }
            case OIS::KC_F4 :
            {

            }
            case OIS::KC_F5 :
            {

            }
            case OIS::KC_F6 :
            {

            }
            case OIS::KC_F7 :
            {

            }
            case OIS::KC_F8 :
            {

            }
            case OIS::KC_F9 :
            {

            }
            case OIS::KC_F10 :
            {

            }
            case OIS::KC_F11 :
            {

            }
            case OIS::KC_F12 :
            {

            }
            case OIS::KC_ABNT_C1 :
            {

            }
            case OIS::KC_ABNT_C2 :
            {

            }
            case OIS::KC_ADD :
            {

            }
            case OIS::KC_APOSTROPHE :
            {

            }
            case OIS::KC_APPS :
            {

            }
            case OIS::KC_AT :
            {

            }
            case OIS::KC_AX :
            {

            }
            case OIS::KC_BACK :
            {

            }
            case OIS::KC_BACKSLASH :
            {

            }
            case OIS::KC_CALCULATOR :
            {

            }
            case OIS::KC_CAPITAL :
            {

            }
            case OIS::KC_COLON :
            {

            }
            case OIS::KC_COMMA :
            {

            }
            case OIS::KC_CONVERT :
            {

            }
            case OIS::KC_DECIMAL :
            {

            }
            case OIS::KC_DELETE :
            {

            }
            case OIS::KC_DIVIDE :
            {

            }
            case OIS::KC_DOWN :
            {

            }
            case OIS::KC_END :
            {

            }
            case OIS::KC_EQUALS :
            {

            }
            case OIS::KC_HOME :
            {

            }
            case OIS::KC_INSERT :
            {

            }
            case OIS::KC_KANA :
            {

            }
            case OIS::KC_KANJI :
            {

            }
            case OIS::KC_LBRACKET :
            {

            }
            case OIS::KC_LCONTROL :
            {

            }
            case OIS::KC_LEFT :
            {

            }
            case OIS::KC_LMENU :
            {

            }
            case OIS::KC_LSHIFT :
            {

            }
            case OIS::KC_LWIN :
            {

            }
            case OIS::KC_MAIL :
            {

            }
            case OIS::KC_MEDIASELECT :
            {

            }
            case OIS::KC_MEDIASTOP :
            {

            }
            case OIS::KC_MINUS :
            {

            }
            case OIS::KC_MULTIPLY :
            {

            }
            case OIS::KC_MUTE :
            {

            }
            case OIS::KC_MYCOMPUTER :
            {

            }
            case OIS::KC_NEXTTRACK :
            {

            }
        }*/

}

