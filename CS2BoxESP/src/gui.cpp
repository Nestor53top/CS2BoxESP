#include "gui.h"
#include "config.h"
#include "imgui/imgui_internal.h"
#include <vector>
#include <cmath>
#include <cstdlib>

static std::vector<MenuParticle> g_Particles;
static bool g_ParticlesInit = false;

static float randf() { return (float)rand() / RAND_MAX; }

static float easeInSine(float t) { return 1.0f - cosf(t * 3.14159265f / 2.0f); }
static float easeOutSine(float t) { return sinf(t * 3.14159265f / 2.0f); }

namespace MenuParticles {
    void Init(int count) {
        g_Particles.clear();
        for (int i = 0; i < count; i++) {
            MenuParticle p;
            p.pos = ImVec2(randf() * 1920, randf() * 1080);
            p.vel = ImVec2(randf() * 200 - 100, randf() * 200 - 100);
            g_Particles.push_back(p);
        }
        g_ParticlesInit = true;
    }

    void Update(float dt) {
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 screenSize = io.DisplaySize;

        for (auto& p : g_Particles) {
            p.pos.x += p.vel.x * dt * Config::Menu::flAnimationSpeed;
            p.pos.y += p.vel.y * dt * Config::Menu::flAnimationSpeed;

            if (p.pos.x < 0 || p.pos.x > screenSize.x) p.vel.x = -p.vel.x;
            if (p.pos.y < 0 || p.pos.y > screenSize.y) p.vel.y = -p.vel.y;
        }
    }

    void Render(ImDrawList* draw, float alpha) {
        if (!g_ParticlesInit) Init(100);

        ImVec4 col4 = ImVec4(
            Config::Menu::colAccent0[0],
            Config::Menu::colAccent0[1],
            Config::Menu::colAccent0[2],
            alpha * 0.6f
        );
        ImU32 col = ImGui::ColorConvertFloat4ToU32(col4);

        ImVec4 lineCol4 = ImVec4(
            Config::Menu::colAccent2[0],
            Config::Menu::colAccent2[1],
            Config::Menu::colAccent2[2],
            alpha * 0.3f
        );
        ImU32 lineCol = ImGui::ColorConvertFloat4ToU32(lineCol4);

        for (size_t i = 0; i < g_Particles.size(); i++) {
            draw->AddCircleFilled(g_Particles[i].pos, 2.0f, col, 6);

            for (size_t j = i + 1; j < g_Particles.size(); j++) {
                float dx = g_Particles[i].pos.x - g_Particles[j].pos.x;
                float dy = g_Particles[i].pos.y - g_Particles[j].pos.y;
                float dist = sqrtf(dx * dx + dy * dy);
                if (dist < 200.0f) {
                    float a = (200.0f - dist) / 200.0f;
                    ImU32 lc = ImGui::ColorConvertFloat4ToU32(ImVec4(lineCol4.x, lineCol4.y, lineCol4.z, alpha * 0.3f * a));
                    draw->AddLine(g_Particles[i].pos, g_Particles[j].pos, lc, 0.5f);
                }
            }
        }
    }
}

namespace MenuSystem {
    void Setup() {
        ImGuiStyle& style = ImGui::GetStyle();
        style.Alpha = 1.0f;
        style.WindowPadding = ImVec2(10, 10);
        style.WindowRounding = 6.0f;
        style.WindowBorderSize = 1.0f;
        style.ChildRounding = 4.0f;
        style.ChildBorderSize = 1.0f;
        style.PopupRounding = 4.0f;
        style.FramePadding = ImVec2(6, 3);
        style.FrameRounding = 4.0f;
        style.ItemSpacing = ImVec2(8, 4);
        style.ItemInnerSpacing = ImVec2(4, 4);
        style.ScrollbarSize = 6.0f;
        style.ScrollbarRounding = 9.0f;
        style.GrabMinSize = 0.0f;
        style.GrabRounding = 4.0f;
        style.TabRounding = 4.0f;

        UpdateColors();
    }

    void Shutdown() {}

    void OnNewFrame() {
        ImGuiIO& io = ImGui::GetIO();

        if (bAnimSwitch)
            flAnimAlpha += io.DeltaTime * 2.0f * Config::Menu::flAnimationSpeed;
        else
            flAnimAlpha -= io.DeltaTime * 2.0f * Config::Menu::flAnimationSpeed;

        if (flAnimAlpha > 1.0f) flAnimAlpha = 1.0f;
        if (flAnimAlpha < 0.0f) flAnimAlpha = 0.0f;

        if (Config::Menu::bParticleBackground)
            MenuParticles::Update(io.DeltaTime);
    }

    void UpdateColors() {
        ImGuiStyle& style = ImGui::GetStyle();

        ImVec4 accent0 = ImVec4(Config::Menu::colAccent0[0], Config::Menu::colAccent0[1], Config::Menu::colAccent0[2], 1.0f);
        ImVec4 accent1 = ImVec4(Config::Menu::colAccent1[0], Config::Menu::colAccent1[1], Config::Menu::colAccent1[2], 1.0f);
        ImVec4 accent2 = ImVec4(Config::Menu::colAccent2[0], Config::Menu::colAccent2[1], Config::Menu::colAccent2[2], 1.0f);

        ImVec4 bg = ImVec4(Config::Menu::colBackground[0], Config::Menu::colBackground[1], Config::Menu::colBackground[2], 1.0f);
        ImVec4 text = ImVec4(Config::Menu::colText[0], Config::Menu::colText[1], Config::Menu::colText[2], 1.0f);
        ImVec4 disabled = ImVec4(Config::Menu::colDisabled[0], Config::Menu::colDisabled[1], Config::Menu::colDisabled[2], 1.0f);

        style.Colors[ImGuiCol_Text] = text;
        style.Colors[ImGuiCol_TextDisabled] = disabled;
        style.Colors[ImGuiCol_WindowBg] = bg;
        style.Colors[ImGuiCol_ChildBg] = ImVec4(bg.x * 0.8f, bg.y * 0.8f, bg.z * 0.8f, 1.0f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(bg.x * 0.9f, bg.y * 0.9f, bg.z * 0.9f, 1.0f);
        style.Colors[ImGuiCol_Border] = ImVec4(0.15f, 0.15f, 0.2f, 0.5f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(bg.x * 1.2f, bg.y * 1.2f, bg.z * 1.2f, 1.0f);
        style.Colors[ImGuiCol_FrameBgHovered] = accent1;
        style.Colors[ImGuiCol_FrameBgActive] = accent2;
        style.Colors[ImGuiCol_TitleBg] = accent0;
        style.Colors[ImGuiCol_TitleBgActive] = accent2;
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(bg.x * 0.5f, bg.y * 0.5f, bg.z * 0.5f, 1.0f);
        style.Colors[ImGuiCol_ScrollbarGrab] = accent2;
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = accent1;
        style.Colors[ImGuiCol_ScrollbarGrabActive] = accent2;
        style.Colors[ImGuiCol_CheckMark] = accent0;
        style.Colors[ImGuiCol_SliderGrab] = accent0;
        style.Colors[ImGuiCol_SliderGrabActive] = accent1;
        style.Colors[ImGuiCol_Button] = ImVec4(bg.x * 1.1f, bg.y * 1.1f, bg.z * 1.1f, 1.0f);
        style.Colors[ImGuiCol_ButtonHovered] = accent1;
        style.Colors[ImGuiCol_ButtonActive] = accent2;
        style.Colors[ImGuiCol_Header] = accent0;
        style.Colors[ImGuiCol_HeaderHovered] = accent1;
        style.Colors[ImGuiCol_HeaderActive] = accent2;
        style.Colors[ImGuiCol_Separator] = ImVec4(0.2f, 0.2f, 0.25f, 1.0f);
        style.Colors[ImGuiCol_Tab] = ImVec4(bg.x * 0.9f, bg.y * 0.9f, bg.z * 0.9f, 1.0f);
        style.Colors[ImGuiCol_TabHovered] = accent1;
        style.Colors[ImGuiCol_TabActive] = accent2;
        style.Colors[ImGuiCol_ResizeGrip] = accent0;
        style.Colors[ImGuiCol_ResizeGripHovered] = accent1;
        style.Colors[ImGuiCol_ResizeGripActive] = accent2;
    }

    void Render() {
        if (!bMenuOpened && flAnimAlpha <= 0.0f) return;

        UpdateColors();

        ImGuiIO& io = ImGui::GetIO();
        ImVec2 screenSize = io.DisplaySize;

        ImDrawList* bgDraw = ImGui::GetBackgroundDrawList();

        if (flAnimAlpha > 0.0f) {
            if (Config::Menu::bDimBackground)
                bgDraw->AddRectFilled(ImVec2(0, 0), screenSize, IM_COL32(0, 0, 0, (int)(125 * flAnimAlpha)));

            if (Config::Menu::bParticleBackground)
                MenuParticles::Render(bgDraw, flAnimAlpha);
        }

        ImGuiStyle& style = ImGui::GetStyle();
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, flAnimAlpha);

        ImGui::SetNextWindowPos(ImVec2(screenSize.x * 0.5f, screenSize.y * 0.5f), ImGuiCond_Once, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(580, 420), ImGuiCond_Always);

        if (Config::Menu::bGlowEffect) {
            ImVec4 glowCol = ImVec4(
                Config::Menu::colAccent2[0],
                Config::Menu::colAccent2[1],
                Config::Menu::colAccent2[2],
                0.8f
            );
            ImU32 glowU32 = ImGui::ColorConvertFloat4ToU32(glowCol);
            ImVec2 wPos = ImGui::GetCursorScreenPos();
            bgDraw->AddShadowRect(wPos, ImVec2(wPos.x + 580, wPos.y + 420), glowU32, 64.0f, style.WindowRounding, ImDrawFlags_ShadowCutOutShapeBackground);
        }

        ImGui::Begin("##NanoTron", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);
        {
            ImVec2 menuPos = ImGui::GetWindowPos();
            ImVec2 menuSize = ImGui::GetWindowSize();
            ImDrawList* draw = ImGui::GetWindowDrawList();

            ImVec4 accent0 = ImVec4(Config::Menu::colAccent0[0], Config::Menu::colAccent0[1], Config::Menu::colAccent0[2], 1.0f);
            ImU32 titleCol = ImGui::ColorConvertFloat4ToU32(accent0);
            draw->AddText(ImVec2(menuPos.x + menuSize.x - 80, menuPos.y + 10), titleCol, "NanoTron");

            static int currentTab = 0;
            if (ImGui::BeginTabBar("##main_tabs")) {
                if (ImGui::BeginTabItem("Visuals")) {
                    currentTab = 0;
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Aimbot")) {
                    currentTab = 1;
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Misc")) {
                    currentTab = 2;
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Settings")) {
                    currentTab = 3;
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Info")) {
                    currentTab = 4;
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }

            switch (currentTab) {
            case 0: {
                ImGui::Checkbox("BoxESP", &Config::bBoxESP);
                if (Config::bBoxESP) {
                    ImGui::SameLine(200);
                    ImGui::PushItemWidth(140);
                    const char* styles[] = { "Rectangle", "Corner", "Filled", "Rounded", "Crosshair", "Gradient", "Bold Corner", "Double", "Dots", "Arrows" };
                    ImGui::Combo("##boxstyle", &Config::boxType, styles, IM_ARRAYSIZE(styles));
                    ImGui::PopItemWidth();
                }

                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                if (ImGui::CollapsingHeader("Particles", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::Indent(10);
                    ImGui::Checkbox("Rain", &Config::bRain);
                    if (Config::bRain) { ImGui::SameLine(200); ImGui::PushItemWidth(140); ImGui::SliderInt("##rain", &Config::iRainDensity, 50, 500, "%d"); ImGui::PopItemWidth(); }

                    ImGui::Checkbox("Snow", &Config::bSnow);
                    if (Config::bSnow) { ImGui::SameLine(200); ImGui::PushItemWidth(140); ImGui::SliderInt("##snow", &Config::iSnowDensity, 50, 400, "%d"); ImGui::PopItemWidth(); }

                    ImGui::Checkbox("Fog", &Config::bFog);
                    if (Config::bFog) { ImGui::SameLine(200); ImGui::PushItemWidth(140); ImGui::SliderInt("##fog", &Config::iFogAlpha, 10, 100, "%d"); ImGui::PopItemWidth(); }

                    ImGui::Checkbox("Sand", &Config::bSand);
                    if (Config::bSand) { ImGui::SameLine(200); ImGui::PushItemWidth(140); ImGui::SliderInt("##sand", &Config::iSandDensity, 50, 300, "%d"); ImGui::PopItemWidth(); }

                    ImGui::Checkbox("Sparks", &Config::bSparks);
                    if (Config::bSparks) { ImGui::SameLine(200); ImGui::PushItemWidth(140); ImGui::SliderInt("##sparks", &Config::iSparksDensity, 20, 200, "%d"); ImGui::PopItemWidth(); }

                    ImGui::Checkbox("Ash", &Config::bAsh);
                    if (Config::bAsh) { ImGui::SameLine(200); ImGui::PushItemWidth(140); ImGui::SliderInt("##ash", &Config::iAshDensity, 50, 350, "%d"); ImGui::PopItemWidth(); }

                    ImGui::Unindent(10);
                }
                break;
            }
            case 1: {
                ImGui::Checkbox("Enable Aimbot", &Config::bAimbot);
                if (Config::bAimbot) {
                    ImGui::Spacing();
                    ImGui::Indent(10);
                    ImGui::PushItemWidth(250);
                    ImGui::SliderFloat("FOV", &Config::flAimbotFOV, 1.0f, 180.0f, "%.1f");
                    ImGui::SliderFloat("Smoothing", &Config::flAimbotSmoothing, 1.0f, 20.0f, "%.1f");
                    ImGui::PopItemWidth();
                    ImGui::Unindent(10);
                }
                break;
            }
            case 2: {
                if (ImGui::CollapsingHeader("Movement", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::Indent(10);
                    ImGui::Checkbox("Auto Bhop", &Config::bAutoBhop);
                    ImGui::Checkbox("Auto Strafe", &Config::bAutoStrafe);
                    ImGui::Unindent(10);
                }

                ImGui::Spacing();
                if (ImGui::CollapsingHeader("FOV Changer", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::Indent(10);
                    ImGui::Checkbox("Enable FOV Changer", &Config::bFOVChanger);
                    if (Config::bFOVChanger) {
                        ImGui::PushItemWidth(250);
                        ImGui::SliderInt("FOV Value", &Config::iFOV, 60, 160, "%d");
                        ImGui::PopItemWidth();
                    }
                    ImGui::Unindent(10);
                }
                break;
            }
            case 3: {
                if (ImGui::CollapsingHeader("Menu", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::Indent(10);
                    ImGui::Checkbox("Dim Background", &Config::Menu::bDimBackground);
                    ImGui::Checkbox("Particle Background", &Config::Menu::bParticleBackground);
                    ImGui::Checkbox("Glow Effect", &Config::Menu::bGlowEffect);
                    ImGui::SliderFloat("Animation Speed", &Config::Menu::flAnimationSpeed, 0.1f, 3.0f, "%.1f");
                    ImGui::Unindent(10);
                }

                ImGui::Spacing();
                if (ImGui::CollapsingHeader("Colors", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::Indent(10);
                    ImGui::ColorEdit3("Accent 0", Config::Menu::colAccent0);
                    ImGui::ColorEdit3("Accent 1", Config::Menu::colAccent1);
                    ImGui::ColorEdit3("Accent 2", Config::Menu::colAccent2);
                    ImGui::ColorEdit3("Background", Config::Menu::colBackground);
                    ImGui::Unindent(10);
                }
                break;
            }
            case 4: {
                ImGui::Spacing();
                ImGui::BulletText("INSERT - toggle menu");
                ImGui::BulletText("DELETE - unload");
                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();
                ImGui::TextWrapped("NanoTron CS2 Cheat");
                ImGui::TextWrapped("BoxESP + Aimbot + Bhop + FOV Changer");
                ImGui::TextWrapped("Build: " __DATE__ " " __TIME__);
                break;
            }
            }
        }
        ImGui::End();

        ImGui::PopStyleVar();
    }
}

static void RenderWindowGlow(ImDrawList* draw, ImVec2 pos, ImVec2 size) {
    float glowSize = 12.0f;
    int layers = 6;
    for (int i = layers; i >= 1; i--) {
        float t = (float)i / layers;
        float expand = glowSize * t;
        float alpha = 0.15f * (1.0f - t);
        ImU32 col = IM_COL32(
            (int)(Config::Menu::colAccent2[0] * 255),
            (int)(Config::Menu::colAccent2[1] * 255),
            (int)(Config::Menu::colAccent2[2] * 255),
            (int)(alpha * 255)
        );
        ImVec2 p0(pos.x - expand, pos.y - expand);
        ImVec2 p1(pos.x + size.x + expand, pos.y + size.y + expand);
        draw->AddRect(p0, p1, col, 8.0f + expand * 0.3f, 0, 1.0f);
    }
}

void GUI_LoadFont() {
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontDefault();
}

void GUI_Init() {
    MenuSystem::Setup();
}

void GUI_Render() {
    MenuSystem::OnNewFrame();
    MenuSystem::Render();
}
