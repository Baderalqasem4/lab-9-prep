#ifndef CAMERA_H
#define CAMERA_H

#include "vector_util.h"
#include <SFML/System.hpp>

class Camera {
public:
    Camera(const sf::Vector2f& windowSize, const sf::Vector2f& viewCenterWorld,
           float worldUnitsPerPixel = 1.f)
        : mWindowSize{windowSize},
          mViewCenterWorld{viewCenterWorld},
          mWorldUnitsPerPixel{worldUnitsPerPixel} {}

    // convert window coordinates to world coordinates
    sf::Vector2f windowToWorld(const sf::Vector2f& pWindow) { 
        return mViewCenterWorld + (pWindow - mWindowSize / 2.f) * mWorldUnitsPerPixel;
    }

    // convert world coordinates to window coordinates
    sf::Vector2f worldToWindow(const sf::Vector2f& pWorld) { 
        return mWindowSize / 2.f + (pWorld - mViewCenterWorld) / mWorldUnitsPerPixel;
    };

    // convert window percentages to world coordinates
    sf::Vector2f windowPercentToWorld(const sf::Vector2f& pWindowPercent) { 
        // convert percentages into pixel positions
        sf::Vector2f pWindow = {
            pWindowPercent.x * mWindowSize.x,
            pWindowPercent.y * mWindowSize.y
        };

        return windowToWorld(pWindow);
    };

    // convert world coordinates to window percentages
    sf::Vector2f worldToWindowPercent(const sf::Vector2f& pWorld) {
        // first convert the world position to window pixels
        sf::Vector2f pWindow = worldToWindow(pWorld);

        // divide by window size to get percentages
        return {
            pWindow.x / mWindowSize.x,
            pWindow.y / mWindowSize.y
        };
     };

    // move camera using world units
    void translateWorld(const sf::Vector2f& deltaWorld) {
        mViewCenterWorld += deltaWorld;
    };

    // move camera using window pixels
    void translateWindow(const sf::Vector2f& deltaScreen) {
        // convert pixels to world units before moving
        translateWorld(deltaScreen * mWorldUnitsPerPixel);
    };

    // move camera using percentages of the window size
    void translateWindowPercent(const sf::Vector2f& deltaScreenPercent) {
        // convert percentages to pixels
        translateWindow({
            deltaScreenPercent.x * mWindowSize.x,
            deltaScreenPercent.y * mWindowSize.y
        });
    };

    // move camera so a world point appears at a given window position
    void setCamera(const sf::Vector2f& pWorld, const sf::Vector2f& pWindow) {
        mViewCenterWorld = pWorld - (pWindow - mWindowSize / 2.f) * mWorldUnitsPerPixel;
    };

    // set the camera using a window percentage instead of pixels
    void setCameraPercent(const sf::Vector2f& pWorld, const sf::Vector2f& pWindowPercent) {
        // convert percentages to pixels and set the camera
        setCamera(pWorld, {
            pWindowPercent.x * mWindowSize.x,
            pWindowPercent.y * mWindowSize.y
        });
    };

    // set the center of the camera in world coordinates
    void setCameraCenter(const sf::Vector2f& pWorld) {
        mViewCenterWorld = pWorld;
    };

    // return current camera center in world coordinates
    sf::Vector2f getCameraCenter() const { 
        return mViewCenterWorld ;
    };

private:
    sf::Vector2f mWindowSize;       // size of the window in screen pixels (i.e. window coords)
    sf::Vector2f mViewCenterWorld;  // world coords point corresponding to the center of our window.
    float mWorldUnitsPerPixel;      // distance in world coords covered by one pixel
};

#endif  // CAMERA_H
