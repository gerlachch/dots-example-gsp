#include <MainWindow.h>
#include <imgui.h>
#include <fmt/format.h>
#include <dots/dots.h>
#include <Sensor.dots.h>
#include <SensorConfig.dots.h>
#include <Alarm.dots.h>
#include <AlarmConfig.dots.h>
#include <AlarmReset.dots.h>

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
                if (dots::container<SensorConfig>().empty())
                {
                    ImGui::Text("<No Sensors>");
                }
                else if (ImGui::BeginTable("Sensors", 4, ImGuiTableFlags_Borders))
                {
                    ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
                    ImGui::TableSetupColumn("Enabled", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn("Sensitivity", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableHeadersRow();

                    dots::container<SensorConfig>().forEach([](const SensorConfig& sensorConfig)
                    {
                        ImGui::TableNextColumn();
                        ImGui::Text("%s", sensorConfig.id->data());

                        ImGui::TableNextColumn();
                        bool sensorEnabled = *sensorConfig.enabled;
                        if (ImGui::Checkbox(fmt::format("##{}Enabled", *sensorConfig.id).data(), &sensorEnabled))
                        {
                            dots::publish(SensorConfig{
                                .id = *sensorConfig.id,
                                .enabled = sensorEnabled
                            });
                        }

                        ImGui::BeginDisabled(!sensorEnabled);

                        ImGui::TableNextColumn();
                        ImGui::SetNextItemWidth(170.0f);
                        if (int sensitivity = *sensorConfig.sensitivity; ImGui::SliderInt(fmt::format("##{}Sensitivity", *sensorConfig.id).data(), &sensitivity, 0, 100))
                        {
                            dots::publish(SensorConfig{
                                .id = *sensorConfig.id,
                                .sensitivity = static_cast<dots::uint8_t>(sensitivity)
                            });
                        }

                        ImGui::TableNextColumn();
                        ImGui::SetNextItemWidth(170.0f);
                        if (auto* sensor = dots::container<Sensor>().find({ .id = sensorConfig.id }))
                        {
                            ImVec4 color = *sensor->activated ? ImVec4{ 1.0f, 0.0f, 0.0f, 1.0f } : ImVec4{ 0.0f, 1.0f, 0.0f, 1.0f };
                            ImGui::TextColored(color, "%d", *sensor->value);
                        }
                        else
                        {
                            ImGui::Text("N/A");
                        }

                        ImGui::EndDisabled();
                    });

                    ImGui::EndTable();
                }
            }

            ImGui::NewLine();

            // alarms
            {
                auto* alarmConfig = dots::container<AlarmConfig>().find();
                bool alarmsEnabled = alarmConfig ? *alarmConfig->enabled : false;

                // header
                {
                    ImGui::Text("--Alarms");

                    ImGui::SameLine();
                    if (ImGui::Checkbox("##AlarmsEnabled", &alarmsEnabled))
                    {
                        dots::publish(AlarmConfig{
                            .enabled = alarmsEnabled
                        });
                    }

                    ImGui::SameLine();
                    if (ImGui::Button("Reset All"))
                    {
                        dots::publish(AlarmReset{});
                    }

                    ImGui::Separator();
                }

                ImGui::BeginDisabled(!alarmsEnabled);

                // entries
                if (dots::container<Alarm>().empty())
                {
                    ImGui::TextColored(ImVec4{ 0.0f, 1.0f, 0.0f, 1.0f }, "<No Alarms>");
                }
                else if (ImGui::BeginTable("Alarms", 3, ImGuiTableFlags_Borders))
                {
                    ImGui::TableSetupColumn("Reset", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoHeaderLabel);
                    ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn("Message", ImGuiTableColumnFlags_WidthStretch);
                    ImGui::TableHeadersRow();

                    dots::container<Alarm>().forEach([](const Alarm& alarm)
                    {
                        ImGui::TableNextColumn();
                        if (ImGui::Button(fmt::format("Reset##{}", alarm.id->toString()).data()))
                        {
                            dots::publish(AlarmReset{
                                .alarms = { *alarm.id }
                            });
                        }

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

                ImGui::EndDisabled();
            }
        }

        ImGui::End();
    }
}
