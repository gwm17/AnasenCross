#ifndef CS_PARAMS_H
#define CS_PARAMS_H

#include <filesystem>
#include "MassLookup.h"

namespace AnasenCross {

    struct CSParams
    {
        std::filesystem::path expFilePath = "";
        std::filesystem::path simFilePath = "";
        std::filesystem::path outputFilePath = "";
        std::string expHistoName = "";
        std::string simEffHistoName = "";
        std::string simSamplesHistoName = "";
        std::string simCountsHistoName = "";
        double initialBeamEnergy = 0.0; //MeV
        NucData projectile;
        NucData target;
        double nIncidentParticles = 0.0;
        double targetParticlesPerMolecule = 0.0;
    };
}

#endif