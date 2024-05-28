#pragma once
// https://github.com/diharaw/sky-models/blob/master/src/sky_model.h
#include "glm/vec3.hpp"
#include "../Shader.h"

class SkyModel
{
public:
    virtual bool initialize() = 0;
    virtual void update() = 0;
    virtual void set_render_uniforms(Shader &shader) = 0;

    inline glm::vec3 direction() { return m_direction; }
    inline void set_direction(glm::vec3 dir) { m_direction = -dir; }

    inline float turbidity() { return m_turbidity; }
    inline void set_turbidity(float t) { m_turbidity = t; }

protected:
    glm::vec3 m_direction;
    float m_normalized_sun_y = 1.15f;
    float m_albedo = 0.1f;
    float m_turbidity = 4.0f;
};