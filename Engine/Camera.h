#ifndef CAMERA_H
#define CAMERA_H

#include <Ogre.h>

namespace Dusk
{
    class Camera
    {
        public:
            Camera(Ogre::SceneManager* scn);
            virtual ~Camera();

            /* retrieves the pointer to the Ogre::Camera object which is
               internally used*/
            Ogre::Camera* getOgreCamera();

            /* sets the position of the camera

               remarks:
                   The vector does not neccessarily represent the actual
                   position of the camera object but only the position of the
                   camera before zoom is applied.
            */
            void setPosition(const Ogre::Vector3& position);

            /* Tells the camera where to look at. */
            void lookAt(const Ogre::Vector3& direction);

            /* Performs movement of the camera, i.e. translation and rotation,
               based on the time given in evt.timeSinceLastFrame. */
            void move(const Ogre::FrameEvent& evt);

            /* Adds(!) to the internally used translation vector. */
            void translate(const Ogre::Vector3& translationVector);

            /* Adds(!) to the internally used rotation value.

               remarks:
                   The value is interpreted as degrees, not as radians.
            */
            void rotate(const float rotation);

            /* Sets the zoom distance.

               remarks:
                   The distance is interpreted in world units behind the camera
                   position specified with setPosition(), i.e. greater values
                   will get the viewpoint further away from the camera position,
                   while a value of zero means you are exactly at the camera
                   position.
                   Note that the distance will be capped if outside of the range
                   [cMinimumZoom,cMaximumZoom].
            */
            void setZoom(const float distance);

            /* Returns the current zoom distance. */
            float getZoom() const;

            /* constants to limit zoom range in setZoom() */
            static const float cMinimumZoom;
            static const float cMaximumZoom;

            /* constant to indicate change in zoom for one keystroke/ one turn
               of mouse wheel (recommended)*/
            static const float cRecommendedZoomStep;
        protected:
        private:
            Ogre::Camera* m_Camera;
            Ogre::SceneNode* m_Primary;
            Ogre::SceneNode* m_Secondary;
            Ogre::Vector3 m_translationVector;
            float m_RotationPerSecond;
            static const float cAboveGroundLevel;
    };
}

#endif // CAMERA_H
