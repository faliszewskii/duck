#pragma once
// https://github.com/diharaw/sky-models/blob/master/src/preetham_sky_model.h

#include "SkyModel.h"

// A Practical Analytic Model for Daylight (A. J. Preetham, Peter Shirley, Brian Smits)
class PreethamSkyModel : public SkyModel
{
public:
    PreethamSkyModel();
    ~PreethamSkyModel();

    bool initialize() override;
    void update() override;
    void set_render_uniforms(Shader &shader) override;

private:
    glm::vec3 A, B, C, D, E;
    glm::vec3 Z;
};