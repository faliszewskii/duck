//
// Created by Bartek Jadczak on 29/05/2024.
//

#ifndef DUCKONWATER_CUBEMAP_H
#define DUCKONWATER_CUBEMAP_H

#include <string>
#include <vector>

class Cubemap
{
    unsigned int id{};

public:
    explicit Cubemap(
            std::vector<std::string> faces
    );

    void bind(int slot) const;
};


#endif //DUCKONWATER_CUBEMAP_H
