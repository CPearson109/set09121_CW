#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Animation {
public:


    // Updated constructor with new parameters
    Animation(unsigned x, unsigned y, unsigned frameWidth, unsigned frameHeight, unsigned numFrames, float holdTime);

    // Reset the animation to its initial frame
    void Reset() {
        iFrame = 0;
        time = 0.0f;
    }

    void ApplyToSprite(sf::Sprite& s) const;
    void Update(float dt);

private:
    void Advance();

private:
    // New member variables for the position and size of the frames in the sprite sheet
    const unsigned x;          // X position in the sprite sheet
    const unsigned y;          // Y position in the sprite sheet
    const unsigned frameWidth; // Width of each frame
    const unsigned frameHeight;// Height of each frame

    const unsigned numFrames;  // Number of frames in the animation
    const float holdTime;      // Time to hold each frame
    float time = 0.0f;         // Time passed since the last frame change
    unsigned iFrame = 0;       // Current frame index
};
