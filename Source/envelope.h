/*
  ==============================================================================

    envFolower.h
    Created: 9 Jun 2023 11:39:50pm
    Author:  Alfie Bradic

  ==============================================================================
*/

#pragma once

class envelope {
    public:
    float processPeak (float x, float ta, float tr, float ts) {
        if(ts != mTs){
            mTs = ts;
        }
        if(ta != mTa){
            mTa = ta;
            mAlpha_a = exp(-mTs/mTa);
        }
        if(tr != mTr){
            mTr = tr;
            mAlpha_r = exp(-mTs/mTr);
        }
        if(m_y < x){
            m_y = mAlpha_a * m_y + (1 - mAlpha_a) * abs(x);
        }
        else{
            m_y = mAlpha_r * m_y + (1 - mAlpha_r) * abs(x);
        }
        return m_y;
    }

    private:

    float mTa;
    float mTr;
    float mTs;
    float mAlpha_a;
    float mAlpha_r;
    float m_y;
};
