// SPDX-License-Identifier: LGPL-3.0-only
// Copyright 2022 Christopher Gerlach <gerlachch@gmx.com>
#include <csignal>
#include <dots/Application.h>
#include <dots/tools/logging.h>
#include <SensorService.h>

int main(int argc, char* argv[])
{
    using namespace dots::type::literals;
    const std::string AppName = "sensor-service";

    try
    {
        // create application
        dots::Application app(AppName, argc, argv);

        examples::SensorService sensorService{ std::stod(argv[1])};

        // execute event loop of application 
        return app.exec();
    }
    catch (const std::exception& e)
    {
        LOG_ERROR_S("ERROR running " << AppName << " -> " << e.what());
        return EXIT_FAILURE;
    }
    catch (...)
    {
        LOG_ERROR_S("ERROR running " << AppName << " -> <unknown exception>");
        return EXIT_FAILURE;
    }
}
