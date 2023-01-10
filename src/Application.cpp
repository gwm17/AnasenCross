#include "Application.h"
#include "CrossSection.h"
#include "nlohmann/json.hpp"
#include "fmt/core.h"
#include "fmt/format.h"
#include "fmt/std.h"

#include <fstream>

namespace AnasenCross {

    Application::Application(const std::filesystem::path& config) :
        m_isInit(false)
    {
        Init(config);
    }

    Application::~Application() {}

    void Application::Init(const std::filesystem::path& config)
    {
        if(!std::filesystem::exists(config))
        {
            fmt::print("Config file {} does not exist\n", config);
            m_isInit = false;
            return;
        }

        std::ifstream configFile(config);
        if(!configFile.is_open())
        {
            fmt::print("Config file {} could not be opened\n", config);
            m_isInit = false;
            return;
        }

        try
        {
            nlohmann::json json;
            configFile >> json;

            std::vector<uint32_t> tz, ta;
            std::vector<int> ts;
            for (auto& j : json["target"]["elements"])
            {
                tz.push_back(j[0].get<uint32_t>());
                ta.push_back(j[1].get<uint32_t>());
                ts.push_back(j[2].get<int>());
            }

            m_target = Target(tz, ta, ts, json["target"]["density"].get<double>());

            MassLookup& mass = MassLookup::GetInstance();
            for(auto& j : json["reactions"])
            {
                m_paramList.push_back({
                    j["experiment_file"].get<std::string>(),
                    j["simulation_file"].get<std::string>(),
                    j["output_file"].get<std::string>(),
                    j["experiment_histo_name"].get<std::string>(),
                    j["sim_eff_histo_name"].get<std::string>(),
                    j["sim_samples_histo_name"].get<std::string>(),
                    j["sim_counts_histo_name"].get<std::string>(),
                    j["beam_energy_MeV"].get<double>(),
                    mass.FindData(j["projectileZ"].get<uint32_t>(), j["projectileA"].get<uint32_t>()),
                    mass.FindData(j["targetZ"].get<uint32_t>(), j["targetA"].get<uint32_t>()),
                    j["n_incident_particles"].get<double>(),
                    j["target_particles_per_molecule"].get<double>()
                });
                auto& params = m_paramList.back();
                if(!std::filesystem::exists(params.expFilePath))
                {
                    fmt::print("Experiment file {} does not exist!\n", params.expFilePath);
                    m_isInit = false;
                    return;
                }
                if(!std::filesystem::exists(params.simFilePath))
                {
                    fmt::print("Simulation file {} does not exist!\n", params.simFilePath);
                    m_isInit = false;
                    return;
                }
                if(!std::filesystem::exists(params.outputFilePath.parent_path()))
                {
                    fmt::print("Creating output directory {}\n", params.outputFilePath.parent_path());
                    std::filesystem::create_directories(params.outputFilePath.parent_path());
                }
            }

            m_isInit = true;
        }
        catch(std::exception& e)
        {
            fmt::print("Error parsing json file: {}\n", e.what());
            m_isInit = false;
            return;
        }
    }

    void Application::Run()
    {
        if(!m_isInit)
            return;

        CrossSection cross(m_target);
        for (auto& params : m_paramList)
        {
            fmt::print("---------------------------------\n");
            fmt::print("Analyzing data...\nExp. file {}\nSim. file {}\nOut. file {}\n", 
                       params.expFilePath, params.simFilePath, params.outputFilePath);
            cross.Run(params);
            fmt::print("Done.\n");
            fmt::print("---------------------------------\n");
        }
    }
}