#pragma once

#include <JuceHeader.h>
#include "envelope.h"

using namespace juce;

class FilmStrip : public Slider
{
public:
    FilmStrip(Image image, const int numFrames)
        :
        mFilmstrip(image),
        mNumFrames(numFrames)
    {
        mFrameHeight = mFilmstrip.getHeight();
        mFrameWidth = mFilmstrip.getWidth() / mNumFrames;
        
    }
    void paint(Graphics& g) override
    {
            auto sliderPos = valueToProportionOfLength(getValue());

                int value = round(sliderPos * (mNumFrames - 1));
                g.drawImage(mFilmstrip, 0, 0, getWidth(), getHeight(),
                value * mFrameWidth, 0, mFrameWidth, mFrameHeight);
    }
private:
    Image mFilmstrip;
    const int mNumFrames;
    int mFrameWidth, mFrameHeight;
};


class VUComponent : public Component{
    
public:
    
    VUComponent(Image background)
    :
    mBackground(background){}
    
    void paint(Graphics& g) override
    {
        g.drawImageAt(mBackground, 0, 0, false);
    }
    
private:
    Image mBackground;
};

class NeedleComponent : public Component
{
public:
    NeedleComponent(Image image, bool clockwise, int anchorX, int anchorY)
    :
    mNeedle(image),
    mClockwise(clockwise),
    mAnchorX(anchorX),
    mAnchorY(anchorY){}
    void paint(Graphics& g) override
    {
        const float min = -12.0f;
        const float max = +12.f;
        float levelClamped = jlimit(min, max, level);

        if (mClockwise == true){
            auto angleDegrees = jmap(levelClamped, min, max, 47.0f, -47.0f);
            mAngleRadians = angleDegrees * (M_PI / 180);
        }

        else{
            auto angleDegrees = jmap(levelClamped, min, max, -47.0f, 47.0f);
            mAngleRadians = angleDegrees * (M_PI / 180);
        }
        
        
        setTransform(AffineTransform::rotation(mAngleRadians, mAnchorX, mAnchorY));
        g.drawImageAt(mNeedle, 0, 0, false);

        
    }
    void setLevel(const float value) { level = value; }

private:
    Image mNeedle;
    bool mClockwise;
    int mAnchorX;
    int mAnchorY;
    float level = 0.f;
    float mAngleRadians = 0.f;
    envelope mGRsmooth;
};


// NOT IN USE : ABANDONED FILMSTRIP VU
//class MeterStrip : public Component
//{
//public:
//    MeterStrip(Image image, const int numFrames)
//    :
//    mFilmstrip(image),
//    mNumFrames(numFrames)
//    {
//        mFrameHeight = mFilmstrip.getHeight();
//        mFrameWidth = mFilmstrip.getWidth() / mNumFrames;
//    }
//    void paint(Graphics& g) override
//    {
//
//        //====FOR UPWARDS COMPRESSION===================
//        if (level >0.0001){
//            const float min = 0.0f;
//            const float max = +12.f;
//            float levelClamped = jlimit(min, max, level);
//
//            int value = round(((max - levelClamped) / (max - min) * ((mNumFrames / 2) - 1)));
//            g.drawImage(mFilmstrip, 0, 0, getWidth(), getHeight(), value * mFrameWidth + 1, 0, mFrameWidth, mFrameHeight);
//        }
//
//        //====FOR DOWNARDS COMPRESSION==================
//        else{
//                const float min = -13.f; // meter calibrated (default 12.0f)
//                const float max = 0.f;
//                float levelClamped = jlimit(min, max, level);
//
//                int value = round(((levelClamped - min) / (max - min) * (mNumFrames / 2)));
//                int reversedValue = mNumFrames / 2 - value;
//
//                g.drawImage(mFilmstrip, 0, 0, getWidth(), getHeight(), (reversedValue + mNumFrames / 2) * mFrameWidth, 0, mFrameWidth, mFrameHeight);
//        }
//
//    }
//    void setLevel(const float value) { level = value; }
//
//private:
//    float level = 0.f;
//    Image mFilmstrip;
//    const int mNumFrames;
//    int mFrameWidth, mFrameHeight;
//    envelope mGRsmooth;
//};
