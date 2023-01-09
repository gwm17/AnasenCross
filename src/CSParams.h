#ifndef CS_PARAMS_H
#define CS_PARAMS_H

#include <filesystem>
#include "MassLookup.h"

namespace AnasenCross {

    struct CSParams
    {
        std::filesystem::path expFilePath = "";
        std::filesystem::path simFilePath = "";
        double initialBeamEnergy = 0.0; //MeV
        NucData projectile;
        NucData target;
    };
}

#endif