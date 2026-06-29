//	easing.cpp
//  イージング処理
//
//	2024.07.01		大山凌太朗

#include <cmath>


#include "easing.h"

#ifndef PI
#define PI 3.1415926545f
#endif

float easeInSine(float t) {
    return sinf(1.5707963f * t);
}

float easeOutSine(float t) {
    return 1 + sinf(1.5707963f * (--t));
}

float easeInOutSine(float t) {
    return 0.5f * (1.0f + sinf(3.1415926f * (t - 0.5f)));
}

float easeInQuad(float t) {
    return t * t;
}

float easeOutQuad(float t) {
    return t * (2.0f - t);
}

float easeInOutQuad(float t) {
    return t < 0.5f ? 2.0f * t * t : t * (4.0f - 2.0f * t) - 1;
}

float easeInCubic(float t) {
    return t * t * t;
}

float easeOutCubic(float t) {
    return 1 + (--t) * t * t;
}

float easeInOutCubic(float t) {
    return t < 0.5f ? 4.0f * t * t * t : 1.0f + (--t) * (2.0f * (--t)) * (2.0f * t);
}

float easeInQuart(float t) {
    t *= t;
    return t * t;
}

float easeOutQuart(float t) {
    t = (--t) * t;
    return 1 - t * t;
}

float easeInOutQuart(float t) {
    if (t < 0.5f) {
        t *= t;
        return 8.0f * t * t;
    }
    else {
        t = (--t) * t;
        return 1.0f - 8.0f * t * t;
    }
}

float easeInQuint(float t) {
    float t2 = t * t;
    return t * t2 * t2;
}

float easeOutQuint(float t) {
    float t2 = (--t) * t;
    return 1.0f + t * t2 * t2;
}

float easeInOutQuint(float t) {
    float t2;
    if (t < 0.5f) {
        t2 = t * t;
        return 16.0f * t * t2 * t2;
    }
    else {
        t2 = (--t) * t;
        return 1.0f + 16.0f * t * t2 * t2;
    }
}

float easeInExpo(float t) {
    return (powf(2.0f, 8.0f * t) - 1.0f) / 255.0f;
}

float easeOutExpo(float t) {
    return 1 - powf(2.0f, -8.0f * t);
}

float easeInOutExpo(float t) {
    if (t < 0.5f) {
        return (powf(2.0f, 16.0f * t) - 1.0f) / 510.0f;
    }
    else {
        return 1.0f - 0.5f * powf(2.0f, -16.0f * (t - 0.5f));
    }
}

float easeInCirc(float t) {
    return 1 - sqrtf(1.0f - t);
}

float easeOutCirc(float t) {
    return sqrtf(t);
}

float easeInOutCirc(float t) {
    if (t < 0.5f) {
        return (1.0f - sqrtf(1.0f - 2.0f * t)) * 0.5f;
    }
    else {
        return (1.0f + sqrtf(2.0f * t - 1.0f)) * 0.5f;
    }
}

float easeInBack(float t) {
    return t * t * (2.70158f * t - 1.70158f);
}

float easeOutBack(float t) {
    return 1 + (--t) * t * (2.70158f * t + 1.70158f);
}

float easeInOutBack(float t) {
    if (t < 0.5f) {
        return t * t * (7.0f * t - 2.5f) * 2.0f;
    }
    else {
        return 1 + (--t) * t * 2.0f * (7.0f * t + 2.5f);
    }
}

float easeInElastic(float t) {
    float t2 = t * t;
    return t2 * t2 * sinf(t * PI * 4.5f);
}

float easeOutElastic(float t) {
    float t2 = (t - 1.0f) * (t - 1.0f);
    return 1 - t2 * t2 * cos(t * PI * 4.5f);
}

float easeInOutElastic(float t) {
    float t2;
    if (t < 0.45f) {
        t2 = t * t;
        return 8.0f * t2 * t2 * sinf(t * PI * 9.0f);
    }
    else if (t < 0.55f) {
        return 0.5f + 0.75f * sinf(t * PI * 4.0f);
    }
    else {
        t2 = (t - 1.0f) * (t - 1.0f);
        return 1.0f - 8.0f * t2 * t2 * sinf(t * PI * 9.0f);
    }
}

float easeInBounce(float t) {
    return powf(2, 6 * (t - 1)) * abs(sinf(t * PI * 3.5f));
}

float easeOutBounce(float t) {
    return 1.0f - powf(2.0f, -6.0f * t) * abs(cos(t * PI * 3.5f));
}

float easeInOutBounce(float t) {
    if (t < 0.5f) {
        return 8.0f * powf(2.0f, 8.0f * (t - 1.0f)) * abs(sinf(t * PI * 7.0f));
    }
    else {
        return 1.0f - 8.0f * powf(2.0f, -8.0f * t) * abs(sinf(t * PI * 7.0f));
    }
}
