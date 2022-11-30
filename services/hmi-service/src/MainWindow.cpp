#include <MainWindow.h>
#include <imgui.h>
#include <fmt/format.h>
#include <dots/dots.h>
#include <Sensor.dots.h>
#include <Alarm.dots.h>

namespace examples
{
    void MainWindow::render()
    {
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::SetNextWindowPos(ImVec2{ 0.0f, 0.0f });

        if (ImGui::Begin("MainWindow", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
        {
            // title
            {
                constexpr char header[] = "--GSP HMI--";
                ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(header).x) * 0.5f);
                ImGui::Text(header);
                ImGui::PopFont();
                ImGui::Separator();
            }

            ImGui::NewLine();

            // sensors
            {
                // header
                {
                    ImGui::Text("--Sensors");
                    ImGui::Separator();
                }

                // entries
                if (ImGui::BeginTable("Sensors", 3, ImGuiTableFlags_Borders))
                {
                    ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
                    ImGui::TableSetupColumn("Sensitivity", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableHeadersRow();

                    {
                        ImGui::TableNextColumn();
                        ImGui::TextUnformatted("Lower Hallway");

                        ImGui::TableNextColumn();
                        ImGui::SetNextItemWidth(170.0f);
                        ImGui::TextUnformatted("25");

                        ImGui::TableNextColumn();
                        ImGui::SetNextItemWidth(170.0f);
                        if (auto* sensor = dots::container<Sensor>().find())
                        {
                            ImVec4 color = *sensor->activated ? ImVec4{ 1.0f, 0.0f, 0.0f, 1.0f } : ImVec4{ 0.0f, 1.0f, 0.0f, 1.0f };
                            ImGui::TextColored(color, "%d", *sensor->value);
                        }
                        else
                            ImGui::Text("N/A");
                    }

                    ImGui::EndTable();
                }
            }

            ImGui::NewLine();

            // alarms
            {
                // header
                {
                    ImGui::Text("--Alarms");
                    ImGui::Separator();
                }

                // entries
                if (dots::container<Alarm>().empty())
                    ImGui::TextColored(ImVec4{ 0.0f, 1.0f, 0.0f, 1.0f }, "<No Alarms>");
                else if (ImGui::BeginTable("Alarms", 2, ImGuiTableFlags_Borders))
                {
                    ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn("Message", ImGuiTableColumnFlags_WidthStretch);
                    ImGui::TableHeadersRow();

                    dots::container<Alarm>().forEach([](const Alarm& alarm)
                    {
                        float timeIntegral;
                        float timeFraction = std::modf(static_cast<float>(ImGui::GetTime()), &timeIntegral);
                        ImVec4 color{ 1.0f, 0.0f, 0.0f, timeFraction };

                        ImGui::TableNextColumn();
                        ImGui::TextColored(color, "%s", alarm.time->toString("%F %T").data());

                        ImGui::TableNextColumn();
                        ImGui::TextColored(color, "%s", alarm.message->data());
                    });

                    ImGui::EndTable();
                }
            }
        }

        ImGui::End();
    }
}
