//
// Created by Bartek Jadczak on 27/04/2024.
//

#ifndef PUMAROBOT_ROOM_H
#define PUMAROBOT_ROOM_H

#include "../Shader.h"
#include "../mesh/Mesh.h"
#include "../mesh/PositionNormalVertex.h"
#include "../importer/Model.h"
#include "../material/Material.h"
#include "../mesh/PosNorTexVertex.h"
#include "../texture/Cubemap.h"

class Room
{
    std::unique_ptr<Mesh<PosNorTexVertex>> wallsMesh;

    Material material{{1,1,1,1},0.0,0};

    void generateRoom(float width, float height, float depth);

public:
    std::unique_ptr<Cubemap> cubemap;

    Room();

    void render(Shader &shader);

};


#endif //PUMAROBOT_ROOM_H
