#include "Animation.h"

// Updated constructor with the new parameters
Animation::Animation(unsigned x, unsigned y, unsigned frameWidth, unsigned frameHeight, unsigned numFrames, float holdTime)
    : x(x), y(y), frameWidth(frameWidth), frameHeight(frameHeight), numFrames(numFrames), holdTime(holdTime), time(0.0f), iFrame(0) {}

// Apply the current frame of animation to a sprite, considering the x and y offsets
void Animation::ApplyToSprite(sf::Sprite& s) const {
    s.setTextureRect(sf::IntRect(x + frameWidth * iFrame, y, frameWidth, frameHeight));
}

// Update the animation based on the time passed
void Animation::Update(float dt) {
    time += dt;
    while (time >= holdTime) {
        time -= holdTime;
        Advance();
    }
}

// Advance to the next frame in the animation
void Animation::Advance() {
    if (++iFrame >= numFrames) {
        iFrame = 0; // Loop back to the first frame
    }
}
