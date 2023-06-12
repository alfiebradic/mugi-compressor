/*
  ==============================================================================

    filter.h
    Created: 3 May 2023 2:31:56pm
    Author:  Alfie Bradic

  ==============================================================================
*/

#pragma once

class Biquad{
public:
    Biquad(){
        
    }
    float process(float input){return 0;}
    void updateCoefs(float cutoff, int sample_rate){}
protected:
    float mCutoff;
    float mNormCutoff;
    float mB0;
    float mB1;
    float mB2;
    float mA0;
    float mA1;
    float mA2;
    // processing members
    float mXnz2;
    float mXnz1;
    float mYnz2;
    float mYnz1;
};

class ButterworthHPF: public Biquad{
public:
    void updateCoefs(float cutoff, int sample_rate){
        mNormCutoff = cutoff / sample_rate;
        float c = tan(M_PI * mNormCutoff);
        mB0 = 1.0 / (1.0 + M_SQRT2 * c + pow(c, 2.0));
        mB1 = -2.0 * mB0;
        mB2 = mB0;
        mA1 = 2.0 * mB0 * (pow(c, 2.0) - 1.0);
        mA2 = mB0 * (1.0 - M_SQRT2 * c + pow(c, 2.0));
        mCutoff = cutoff;
    }
    float process(double x){
        float y = mB0 * x + mB1 * mXnz1 + mB2 * mXnz2 - mA1 * mYnz1 - mA2 * mYnz2;
        mXnz2 = mXnz1;
        mXnz1 = x;
        mYnz2 = mYnz1;
        mYnz1 = y;
        return y;
    }
};


class ResonantLPF: public Biquad{
public:
    void updateCoefs(float cutoff, int sample_rate, float resonance){
        mNormCutoff = cutoff / sample_rate;

        
        float g = pow(10.0, -0.05 * resonance);
        float w0 = M_PI * mNormCutoff;
        float cos_w0 = cos(w0);
        float alpha = 0.5 * sin(w0) * g;
        
        mA0 = 1.0 + alpha;
        
        mB1 = (1.0 - cos_w0) / mA0;
        mB0 = (0.5 * mB1) / mA0;
        mB2 = mB0;
        
        mA1 = (-2.0 * cos_w0) / mA0;
        mA2 = (1.0 - alpha) / mA0;
        mCutoff = cutoff;
    }
    float process (float x){
        float y = mB0 * x + mB1 * mXnz1 + mB2 * mXnz2 - mA1 * mYnz1 - mA2 * mYnz2;
        mXnz2 = mXnz1;
        mXnz1 = x;
        mYnz2 = mYnz1;
        mYnz1 = y;
        return y;
    }
};

class LinkwitzRileyLPF: public Biquad{
public:
    void updateCoefs(float cutoff, int sample_rate){
        
        mNormCutoff = cutoff / sample_rate;
        mCutoff = cutoff;
        
        float theta = M_PI * mNormCutoff;
        float omega = M_PI * mCutoff;
        float omegaSquared = pow(omega, 2.0);
        float kappa = omega / tan(theta);
        float kappaSquared = pow(kappa, 2.0);
        float delta = kappaSquared + omegaSquared + (2 * kappa * omega);
        
        mB0 = omegaSquared / delta;
        mB1 = 2.0 * (omegaSquared / delta);
        mB2 = mB0;
        mA1 = (-2.0 * kappaSquared + 2.0 * omegaSquared) / delta;
        mA2 = (-2.0 * kappa * omega + kappaSquared + omegaSquared) / delta;

    }
    float process(double x){
        float y = mB0 * x + mB1 * mXnz1 + mB2 * mXnz2 - mA1 * mYnz1 - mA2 * mYnz2;
        mXnz2 = mXnz1;
        mXnz1 = x;
        mYnz2 = mYnz1;
        mYnz1 = y;
        return y;
    }
};

class LinkwitzRileyHPF: public Biquad{
public:
    void updateCoefs(float cutoff, int sample_rate){
        
        mNormCutoff = cutoff / sample_rate;
        mCutoff = cutoff;
        
        float theta = M_PI * mNormCutoff;
        float omega = M_PI * mCutoff;
        float omegaSquared = pow(omega, 2.0);
        float kappa = omega / tan(theta);
        float kappaSquared = pow(kappa, 2.0);
        float delta = kappaSquared + omegaSquared + (2 * kappa * omega);
        
        mB0 = kappaSquared / delta;
        mB1 = (-2.0 * kappaSquared) / delta;
        mB2 = mB0;
        mA1 = (-2.0 * kappaSquared + 2.0 * omegaSquared) / delta;
        mA2 = (-2.0 * kappa * omega + kappaSquared + omegaSquared) / delta;

    }
    float process(double x){
        float y = mB0 * x + mB1 * mXnz1 + mB2 * mXnz2 - mA1 * mYnz1 - mA2 * mYnz2;
        mXnz2 = mXnz1;
        mXnz1 = x;
        mYnz2 = mYnz1;
        mYnz1 = y;
        return y;
    }
};

class APF: public Biquad{
public:
    void updateCoefs(float cutoff, int sample_rate){
        
        mNormCutoff = cutoff / sample_rate;
        mCutoff = cutoff;
        
        float tanCutoff = tan(M_PI * mNormCutoff);
        float alpha = (tanCutoff - 1) / (tanCutoff + 1);
        
        mB0 = alpha;
        mB1 = 1.0;
        mB2 = 0.0;
        mA1 = alpha;
        mA2 = 0.0;

    }
    float process(double x){
        float y = mB0 * x + mB1 * mXnz1 + mB2 * mXnz2 - mA1 * mYnz1 - mA2 * mYnz2;
        mXnz2 = mXnz1;
        mXnz1 = x;
        mYnz2 = mYnz1;
        mYnz1 = y;
        return y;
    }
};
