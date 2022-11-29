// SPDX-License-Identifier: LGPL-3.0-only
// Copyright 2022 Christopher Gerlach <gerlachch@gmx.com>
#include <imgui.h>
#include <fmt/format.h>
#include <dots/Application.h>
#include <dots/asio.h>
#include <dots/tools/logging.h>
#include <MainWindow.h>
#include <Backend.h>
#include <CousineRegular.h>

int main(int argc, char* argv[])
{
    using namespace dots::type::literals;
    const std::string AppName = "hmi-service";

    try
    {
        // create application
        dots::Application app(AppName, argc, argv);

        std::optional<Backend> backend{ std::in_place, 960, 1080, AppName };

        // load font
        {
            ImGuiIO& io = ImGui::GetIO();
            io.Fonts->AddFontFromMemoryCompressedTTF(CousineRegular_compressed_data, CousineRegular_compressed_size, 28);
            io.Fonts->AddFontFromMemoryCompressedTTF(CousineRegular_compressed_data, CousineRegular_compressed_size, 52);
        }

        examples::MainWindow mainWindow;

        backend->run([&]
        {
            app.transceiver().ioContext().poll();
            mainWindow.render();
        });
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
