#ifndef CROSS_SECTION_H
#define CROSS_SECTION_H

#include <filesystem>
#include <string>
#include "CSParams.h"

namespace AnasenCross {

    class CrossSection
    {
    public:
        CrossSection(const CSParams& params);
        ~CrossSection();

        void Run(const std::string& expHistoName, const std::string& simHistoName, const std::filesystem::path& outputPath);

    private:
        CSParams m_params;
        
    };

}

#endif