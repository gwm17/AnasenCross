#ifndef APPLICATION_H
#define APPLICATION_H

#include "CSParams.h"
#include "Target.h"
#include <vector>
#include <filesystem>

namespace AnasenCross {

    class Application
    {
    public:
        Application(const std::filesystem::path& config);
        ~Application();

        void Run();
        const bool IsInit() const { return m_isInit; }

    private:
        void Init(const std::filesystem::path& config);

        bool m_isInit;
        Target m_target;
        std::vector<CSParams> m_paramList;
    };
}

#endif