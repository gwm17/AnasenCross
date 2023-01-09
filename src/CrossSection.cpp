#include "CrossSection.h"
#include "TFile.h"
#include "TH1.h"
#include <iostream>
#include <vector>

namespace AnasenCross {

    CrossSection::CrossSection(const CSParams& params) :
        m_params(params)
    {
    }

    CrossSection::~CrossSection() {}

    void CrossSection::Run(const std::string& expHistoName, const std::string& simHistoName, const std::filesystem::path& outputPath)
    {
        TFile* expFile = TFile::Open(m_params.expFilePath.c_str(), "READ");
        if (!expFile || !expFile->IsOpen())
        {
            std::cerr << "Could not open experimental file " << m_params.expFilePath << std::endl;
            delete expFile;
            return;
        }

        TFile* simFile = TFile::Open(m_params.simFilePath.c_str(), "READ");
        if (!simFile || !simFile->IsOpen())
        {
            std::cerr << "Could not open simulation file " << m_params.simFilePath << std::endl;
            delete expFile;
            delete simFile;
            return;
        }

        TH1* expHisto = expFile->Get<TH1>(expHistoName.c_str());
        TH1* simHisto = simFile->Get<TH1>(simHistoName.c_str());

        if (!expHisto || !simHisto)
        {
            std::cerr << "One of the requested histograms " << expHistoName << ", " << simHistoName << " was not found" << std::endl;
            delete expFile;
            delete simFile;
            return;
        }

        if (expHisto->GetNbinsX() != simHisto->GetNbinsX())
        {
            std::cerr << "Mismatched number of histogram bins, exp has " << expHisto->GetNbinsX() << " bins, sim has " 
                      << simHisto->GetNbinsX() << " bins" << std::endl;
            delete expFile;
            delete simFile;
            return;
        }

        //Conversion from Ecm to projectile lab KE
        double ecm2bke = (m_params.projectile.mass + m_params.target.mass) / m_params.target.mass;
        
        std::vector<double> ecmVec, bkeVec, countsVec, effVec;
        double ecm, bke, counts, eff;
        for(int bin=1; bin <= expHisto->GetNbinsX(); bin++)
        {
            ecm = expHisto->GetBinCenter(bin);
            bke = ecm * ecm2bke;
        }
    }
}