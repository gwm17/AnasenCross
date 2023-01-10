#ifndef CROSS_SECTION_H
#define CROSS_SECTION_H

#include <filesystem>
#include <string>
#include "CSParams.h"
#include "Target.h"
#include <TH1.h>

namespace AnasenCross {

    struct CrossData
    {
        double energyCM = -1.0;
        double beamEnergyLab = -1.0;
        double path = -1.0;
        double sigma_path = -1.0;
        double counts = -1.0;
        double sigma_counts = -1.0;
        double effciency = -1.0;
        double sigma_eff = -1.0;
        double crossSection = -1.0;
        double sigma_crossSection = -1.0;
    };

    class CrossSection
    {
    public:
        CrossSection(const Target& target);
        ~CrossSection();

        void Run(const CSParams& params);

    private:
        //AssertBins NEVER takes ownership of the TH1's
        void AssertBins(const std::vector<TH1*>& histos);
        Target m_targetMaterial;

        static constexpr double s_cmsq2mb = 1.0e27;
        static constexpr double s_avogadroMag = 1.0e23;
        static constexpr double s_beamKELabSigma = 1.0;
        static constexpr double s_beamCountPercentSigma = 0.3;
    };

}

#endif