//
// Created by Dreamtowards on 2022/4/22.
//

#ifndef ETHERTIA_ETHERTIA_H
#define ETHERTIA_ETHERTIA_H

#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <stdexcept>

#include <ethertia/render/Camera.h>
#include <ethertia/init/HitCursor.h>
#include <ethertia/util/Timer.h>
#include <ethertia/util/Profiler.h>
#include <ethertia/util/Scheduler.h>
#include <ethertia/util/Registry.h>

// scope profiling
#define PROFILE_VN_CONCAT_INNR(a, b) a ## b
#define PROFILE_VN_CONCAT(a, b) PROFILE_VN_CONCAT_INNR(a, b)
#define PROFILE(x) auto PROFILE_VN_CONCAT(_profiler, __COUNTER__) = Ethertia::getProfiler().push_ap(x)
#define PROFILE_X(p, x) auto PROFILE_VN_CONCAT(_profiler, __COUNTER__) = p.push_ap(x)

// __forward_declarations

class RenderEngine;      // #include <ethertia/render/RenderEngine.h>
class World;             // #include <ethertia/world/World.h>
class EntityPlayer;      // #include <ethertia/entity/player/EntityPlayer.h>
class GuiRoot;           // #include <ethertia/gui/GuiRoot.h>
class Window;            // #include <ethertia/render/Window.h>
class AudioEngine;       // #include <ethertia/audio/AudioEngine.h>

class WorldInfo;  // ethertia/world/World.h

class Ethertia
{
    inline static bool          m_Running      = false;
    inline static RenderEngine* m_RenderEngine = nullptr;
    inline static World*        m_World        = nullptr;
    inline static EntityPlayer* m_Player       = nullptr;
    inline static GuiRoot*      m_RootGUI      = nullptr;
    inline static Window*       m_Window       = nullptr;
    inline static AudioEngine*  m_AudioEngine  = nullptr;
    inline static Timer         m_Timer{};
    inline static Scheduler     m_Scheduler{};
    inline static Scheduler     m_AsyncScheduler{};
    inline static HitCursor     m_HitCursor{};
    inline static Profiler      m_Profiler{};


public:
    Ethertia() = delete;


    static void run()
    {
        start();

        while (m_Running)
        {
            runMainLoop();
        }

        destroy();
    }

    static void start();

    static void runMainLoop();
    static void runTick();
    static void renderGUI();

    static void destroy();

    static void loadWorld(const std::string& savedir, const WorldInfo* worldinfo = nullptr);
    static void unloadWorld();

    static void dispatchCommand(const std::string& cmd);
    static void notifyMessage(const std::string& msg);

    static void shutdown() { m_Running = false; }
    static const bool& isRunning() { return m_Running; }
    static bool inMainThread() { return m_Scheduler.inThread(); }

    static RenderEngine* getRenderEngine() { return m_RenderEngine; }
    static Window* getWindow() { return m_Window; }
    static World* getWorld() { return m_World; }
    static GuiRoot* getRootGUI() { return m_RootGUI; }
    static EntityPlayer* getPlayer() { return m_Player; }
    static Timer* getTimer() { return &m_Timer; }
    static Scheduler* getScheduler() { return &m_Scheduler; }
    static Scheduler* getAsyncScheduler() { return &m_AsyncScheduler; }
    static HitCursor& getHitCursor() { return m_HitCursor; }
    static Profiler& getProfiler() { return m_Profiler; }
    static AudioEngine* getAudioEngine() { return m_AudioEngine; }

    static bool isIngame();  // controlling game.
    static float getPreciseTime();
    static float getAspectRatio();
    static float getDelta();
    static Camera* getCamera();

    template<typename T>
    static Registry<T>* getRegistry() {
        // static_assert(std::is_same<T, Item>());
        return &T::REGISTRY;
    }


    struct Version
    {
        inline static const int major = 0;
        inline static const int minor = 0;
        inline static const int revision = 2;
        inline static const char* snapshot = "23u08";  // release: nullptr.

        static const std::string& name() {
            static std::string _CACHE = snapshot ? Strings::fmt("Ethertia Alpha {} *{}.{}.{}", snapshot, major, minor, revision)
                                                 : Strings::fmt("Ethertia Alpha {}.{}.{}", major, minor, revision);
            return _CACHE;
        }
    };

};

#endif //ETHERTIA_ETHERTIA_H
