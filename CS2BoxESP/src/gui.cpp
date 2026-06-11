#include "gui.h"
#include "config.h"
#include "imgui/imgui_internal.h"

static void RenderWindowGlow(ImDrawList* draw, ImVec2 pos, ImVec2 size) {
    float glowSize = 12.0f;
    int layers = 6;
    for (int i = layers; i >= 1; i--) {
        float t = (float)i / layers;
        float expand = glowSize * t;
        float alpha = 0.15f * (1.0f - t);
        ImU32 col = IM_COL32(140, 60, 255, (int)(alpha * 255));
        ImVec2 p0(pos.x - expand, pos.y - expand);
        ImVec2 p1(pos.x + size.x + expand, pos.y + size.y + expand);
        draw->AddRect(p0, p1, col, 8.0f + expand * 0.3f, 0, 1.0f);
    }
}

void GUI_LoadFont() {
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontDefault();
}

void GUI_Init() {}

void GUI_Render() {
    if (!Config::bMenuOpen) return;

    ImGui::SetNextWindowSize(ImVec2(520, 420), ImGuiCond_Always);
    ImGui::Begin("BoxESP", &Config::bMenuOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    if (ImGui::BeginTabBar("##tabs")) {
        if (ImGui::BeginTabItem("Visuals")) {
            ImGui::Spacing();
            ImGui::Checkbox("BoxESP", &Config::bBoxESP);
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            if (ImGui::CollapsingHeader("Particles", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::Indent(10.0f);
                ImGui::Spacing();

                ImGui::Checkbox("Rain", &Config::bRain);
                if (Config::bRain) {
                    ImGui::SameLine(180);
                    ImGui::PushItemWidth(160);
                    ImGui::SliderInt("##rain_d", &Config::iRainDensity, 50, 500, "density %d");
                    ImGui::PopItemWidth();
                }

                ImGui::Checkbox("Snow", &Config::bSnow);
                if (Config::bSnow) {
                    ImGui::SameLine(180);
                    ImGui::PushItemWidth(160);
                    ImGui::SliderInt("##snow_d", &Config::iSnowDensity, 50, 400, "density %d");
                    ImGui::PopItemWidth();
                }

                ImGui::Checkbox("Fog", &Config::bFog);
                if (Config::bFog) {
                    ImGui::SameLine(180);
                    ImGui::PushItemWidth(160);
                    ImGui::SliderInt("##fog_a", &Config::iFogAlpha, 10, 100, "alpha %d");
                    ImGui::PopItemWidth();
                }

                ImGui::Checkbox("Sand", &Config::bSand);
                if (Config::bSand) {
                    ImGui::SameLine(180);
                    ImGui::PushItemWidth(160);
                    ImGui::SliderInt("##sand_d", &Config::iSandDensity, 50, 300, "density %d");
                    ImGui::PopItemWidth();
                }

                ImGui::Checkbox("Sparks", &Config::bSparks);
                if (Config::bSparks) {
                    ImGui::SameLine(180);
                    ImGui::PushItemWidth(160);
                    ImGui::SliderInt("##sparks_d", &Config::iSparksDensity, 20, 200, "density %d");
                    ImGui::PopItemWidth();
                }

                ImGui::Checkbox("Ash", &Config::bAsh);
                if (Config::bAsh) {
                    ImGui::SameLine(180);
                    ImGui::PushItemWidth(160);
                    ImGui::SliderInt("##ash_d", &Config::iAshDensity, 50, 350, "density %d");
                    ImGui::PopItemWidth();
                }

                ImGui::Unindent(10.0f);
            }
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Others")) {
            ImGui::Spacing();
            ImGui::Checkbox("FOV Changer", &Config::bFOVChanger);
            if (Config::bFOVChanger) {
                ImGui::SameLine();
                ImGui::PushItemWidth(120.0f);
                ImGui::SliderInt("##fov", &Config::iFOV, 60, 160, "%d");
                ImGui::PopItemWidth();
            }
            ImGui::Spacing();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("INFO")) {
            ImGui::Spacing();
            ImGui::BulletText("INSERT - toggle menu");
            ImGui::BulletText("DELETE - unload");
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            ImGui::TextWrapped("BoxESP - simple box ESP for CS2");
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::End();

    ImGuiWindow* w = ImGui::FindWindowByName("BoxESP");
    if (w) {
        ImDrawList* bg = ImGui::GetBackgroundDrawList();
        RenderWindowGlow(bg, w->Pos, w->Size);
    }
}
