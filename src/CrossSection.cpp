#include "CrossSection.h"
#include "Assert.h"
#include "TFile.h"
#include "TH1.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "fmt/core.h"
#include "fmt/format.h"
#include "fmt/std.h"

namespace AnasenCross {

    CrossSection::CrossSection(const Target& target) :
        m_targetMaterial(target)
    {
    }

    CrossSection::~CrossSection() {}

    void CrossSection::AssertBins(const std::vector<TH1*>& histos)
    {
        for (std::size_t i=0; i<histos.size(); i++)
        {
            for(std::size_t j=0; j<histos.size(); j++)
            {
                ACROSS_ASSERT("Mismatched histogram bins!", histos[i]->GetNbinsX() == histos[j]->GetNbinsX());
            }
        }
    }

    void CrossSection::Run(const CSParams& params)
    {
        TFile* expFile = TFile::Open(params.expFilePath.c_str(), "READ");
        if (!expFile || !expFile->IsOpen())
        {
            fmt::print("Could not open experimental file {}\n", params.expFilePath);
            delete expFile;
            return;
        }

        TFile* simFile = TFile::Open(params.simFilePath.c_str(), "READ");
        if (!simFile || !simFile->IsOpen())
        {
            fmt::print("Could not open simulation file {}\n", params.simFilePath);
            delete expFile;
            delete simFile;
            return;
        }

        TH1* expHisto = expFile->Get<TH1>(params.expHistoName.c_str());
        TH1* simSamplesHisto = simFile->Get<TH1>(params.simSamplesHistoName.c_str());
        TH1* simCountsHisto = simFile->Get<TH1>(params.simCountsHistoName.c_str());
        TH1* simEffHisto = simFile->Get<TH1>(params.simEffHistoName.c_str());

        if (!expHisto || !simCountsHisto || !simEffHisto || !simSamplesHisto)
        {
            fmt::print("One of the requested histograms {}, {}, {}, {} was not found\n", params.expHistoName, params.simSamplesHistoName,
                        params.simCountsHistoName, params.simEffHistoName);
            delete expFile;
            delete simFile;
            return;
        }

        AssertBins({expHisto, simSamplesHisto, simCountsHisto, simEffHisto});

        //Conversion from Ecm to projectile lab KE
        double ecm2bke = (params.projectile.mass + params.target.mass) / params.target.mass;
        
        std::vector<CrossData> data;
        CrossData datum;
        double ecmMin, ecmMax;
        double bkeMin, bkeMax;
        double pathMin, pathMax;
        double meanPath, meanPathErr;
        double meanPathSigma;

        double simSamples, simCounts;
        for(int bin=1; bin <= expHisto->GetNbinsX(); bin++)
        {
            datum.energyCM = expHisto->GetBinCenter(bin);
            ecmMin = expHisto->GetBinLowEdge(bin);
            ecmMax = expHisto->GetBinWidth(bin) + ecmMin;
            datum.beamEnergyLab = datum.energyCM * ecm2bke;
            bkeMin = ecmMin * ecm2bke;
            bkeMax = ecmMax * ecm2bke;
            pathMin = m_targetMaterial.GetPathLength(params.projectile.Z, params.projectile.A, params.initialBeamEnergy, bkeMin);
            pathMax = m_targetMaterial.GetPathLength(params.projectile.Z, params.projectile.A, params.initialBeamEnergy, bkeMax);
            meanPath = m_targetMaterial.GetPathLength(params.projectile.Z, params.projectile.A, params.initialBeamEnergy, datum.beamEnergyLab);
            meanPathErr = m_targetMaterial.GetPathLength(params.projectile.Z, params.projectile.A, params.initialBeamEnergy,
                                                         datum.beamEnergyLab + s_beamKELabSigma);
            datum.path = pathMax - pathMin;
            datum.sigma_path = std::abs(meanPath - meanPathErr);
            datum.counts = expHisto->GetBinContent(bin);
            datum.effciency = simEffHisto->GetBinContent(bin);
            simSamples = simSamplesHisto->GetBinContent(bin);
            simCounts = simCountsHisto->GetBinContent(bin);

            datum.sigma_counts = std::sqrt(datum.counts);
            datum.sigma_eff = datum.effciency * std::sqrt(1.0/simSamples + 1.0/simCounts);

            datum.crossSection = datum.counts / 
                                (params.targetParticlesPerMolecule * m_targetMaterial.GetNumberDensity(datum.path) 
                                 * params.nIncidentParticles * datum.effciency);
            datum.crossSection *= s_cmsq2mb / s_avogadroMag; //avoid using huge numbers

            datum.sigma_crossSection = datum.crossSection * std::sqrt(std::pow(datum.sigma_counts/datum.counts, 2.0) +
                                                                      std::pow(datum.sigma_eff/datum.effciency, 2.0) +
                                                                      std::pow(datum.sigma_path/datum.path, 2.0) +
                                                                      s_beamCountPercentSigma * s_beamCountPercentSigma);

            data.push_back(datum);
        }

        delete expFile;
        delete simFile;

        std::ofstream output(params.outputFilePath);
        if(!output.is_open())
        {
            fmt::print("Could not write to output file {}", params.outputFilePath);
            return;
        }

        output << fmt::format("Ecm(MeV), BeamKE(MeV), Path(cm), SigmaPath(cm), Counts, SigmaCounts, Eff, SigmaEff, Cross(mb), SigmaCross(mb)\n");
        for (auto& item : data)
        {
            output << fmt::format("{}, {}, {}, {}, {}, {}, {}, {}, {}, {}\n",
                     item.energyCM, item.beamEnergyLab, item.path, item.sigma_path, item.counts, item.sigma_counts,
                     item.effciency, item.sigma_eff, item.crossSection, item.sigma_crossSection);
        }

        output.close();
    }
}