//
// Created by Dreamtowards on 2022/12/17.
//

#ifndef ETHERTIA_SETTINGS_H
#define ETHERTIA_SETTINGS_H

#include <ethertia/util/Loader.h>
#include <ethertia/world/gen/NoiseGen.h>

#include <nlohmann/json.hpp>

class Settings
{
    using json = nlohmann::json;
public:
    inline static const char* SETTINGS_FILE = "./settings.json";

    // init properties.

    inline static int displayWidth  = 1280,
                      displayHeight = 720;

    inline static std::vector<std::string> MODS;

    template<typename T>
    static void set_ifexists(json& j, const std::string& name, T* p) {
        if (j.contains(name)) {
            *p = j[name];
        }
    }

    static void loadSettings() {
        BENCHMARK_TIMER;
        Log::info("Load Settings.\1");
        if (!Loader::fileExists(SETTINGS_FILE))
            return;

        json settings = json::parse(Loader::loadFile(SETTINGS_FILE).new_string());

        set_ifexists(settings, "view_distance",  &RenderEngine::viewDistance);
        set_ifexists(settings, "fov",            &RenderEngine::fov);

        set_ifexists(settings, "assets",         &Loader::ASSETS);

        set_ifexists(settings, "display_width",  &displayWidth);
        set_ifexists(settings, "display_height", &displayHeight);

        set_ifexists(settings, "mtl_resolution", &MaterialTextures::TEX_RESOLUTION);

        std::string simdLevel;
        set_ifexists(settings, "simd_level", &simdLevel);
        NoiseGen::g_SIMDLevel = NoiseGen::FastSIMD_ofLevelName(simdLevel);

        set_ifexists(settings, "mods", &MODS);

    }

    static void loadProgramArguments(int argc, const char* argv[]) {

    }

    static void saveSettings() {
        BENCHMARK_TIMER;
        Log::info("Save Settings.\1");

        json settings = json::object({
             {"view_distance",  RenderEngine::viewDistance},
             {"fov",            RenderEngine::fov},
             {"assets",         Loader::ASSETS},
             {"display_width",  Ethertia::getWindow()->getWidth()},
             {"display_height", Ethertia::getWindow()->getHeight()},
             {"vsync",          false},
             {"fullscreen",     false},
             {"mtl_resolution", MaterialTextures::TEX_RESOLUTION},
             {"simd_level",     NoiseGen::FastSIMD_LevelName(NoiseGen::g_SIMDLevel)},
             {"mods",           MODS}
        });

        std::ofstream f(SETTINGS_FILE);
        f << settings.dump(2);
    }


};

#endif //ETHERTIA_SETTINGS_H
