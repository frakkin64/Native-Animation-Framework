-- include subprojects
includes("lib/commonlibf4")

-- set project constants
set_project("NAF")
set_version("1.0.15")
set_license("GPL-3.0")
set_languages("c++23")
set_warnings("allextra")

-- add common rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- config options
set_config("commonlib_toml", true)

-- add packages
package("fp16")
    set_kind("library", { headeronly = true })
    set_description("Header-only library for conversion to/from half-precision floating point formats")
    add_urls("https://github.com/Maratyszcza/FP16.git")
    add_versions("2024.1.1", "782eea126dc5c755827be751a099eb01826175cf")
    on_install(function(package)
        os.cp("include/fp16.h", package:installdir("include"))
        os.cp("include/fp16", package:installdir("include"))
    end)
package_end()

add_requires("cereal", "nlohmann_json", "zstr", "libzippp", "fp16 2024.1.1", "pugixml")

-- inverse kinematics library (C, generated headers; mirrors extern/ik/ik/CMakeLists.txt)
target("ik", function()
    set_kind("static")
    set_languages("c11")
    set_warnings("none")
    set_default(false)

    local root = path.join(os.scriptdir(), "extern", "ik", "ik")
    local gen = path.join(os.scriptdir(), "build", "ik-gen")

    add_defines("IK_BUILDING", "_CRT_SECURE_NO_DEPRECATE")
    add_cflags("/EHa", "/wd4305", "/wd4201", "/wd4706", "/wd4100", "/wd4244", "/wd4477", "/wd4003", { force = true })
    add_includedirs(path.join(root, "include", "public"), path.join(root, "include", "private"), { public = false })
    add_includedirs(path.join(gen, "include", "public"), path.join(gen, "include", "private"))
    add_includedirs(path.join(root, "include", "public"), { public = true })
    add_includedirs(path.join(gen, "include", "public"), { public = true })

    for _, f in ipairs({
        "bstv", "chain", "ik", "log_static", "memory", "quat_static", "retcodes", "solver_static",
        "transform_chains", "transform_tree", "util", "vec3_static", "vector", "tests_static_stub",
        "solver/base/constraint_base", "solver/base/effector_base", "solver/base/node_base", "solver/base/solver_base",
        "solver/FABRIK/node_FABRIK", "solver/FABRIK/solver_FABRIK", "solver/MSS/solver_MSS",
        "solver/ONE_BONE/solver_ONE_BONE", "solver/TWO_BONE/solver_TWO_BONE"
    }) do
        add_files(path.join(root, "src", f .. ".c"))
    end
    add_files(path.join(gen, "src", "build_info.c"))

    on_load(function(target)
        local root = path.join(os.scriptdir(), "extern", "ik", "ik")
        local gen = path.join(os.scriptdir(), "build", "ik-gen")
        local py = assert(os.iorunv("python", {"-c", "import sys;print(sys.executable)"}):trim(), "python required for ik vtable generation")

        local config = [[
#ifndef IK_CONFIG_H
#define IK_CONFIG_H
#define IKAPI ik
#define IK_HAVE_STDINT_H
#define IK_PRECISION_FLOAT
#define IK_PIC
#define IK_WARN_UNUSED _Check_return_
#define IK_HELPER_API_EXPORT
#define IK_HELPER_API_IMPORT
#define IK_HELPER_API_LOCAL
#if defined (IK_BUILDING)
#   define IK_PUBLIC_API IK_HELPER_API_EXPORT
#else
#   define IK_PUBLIC_API IK_HELPER_API_IMPORT
#endif
#define IK_PRIVATE_API IK_HELPER_API_LOCAL
#ifdef __cplusplus
#   define C_BEGIN extern "C" {
#   define C_END }
#else
#   define C_BEGIN
#   define C_END
#endif
typedef float ikreal_t;
#include <float.h>
#define IK_EPSILON FLT_EPSILON
#define IK_INTERFACE(i) struct ik_##i##_t
#include "ik/retcodes.h"
#include <stdint.h>
#endif
]]
        local cfgfile = path.join(gen, "include", "public", "ik", "config.h")
        if not os.isfile(cfgfile) then io.writefile(cfgfile, config) end

        local bi = path.join(gen, "src", "build_info.c")
        if not os.isfile(bi) then
            io.writefile(bi, [[
#include "ik/build_info_static.h"
#define IK_VERSION "0.2.0"
#define IK_BUILD_NUMBER 0
const char* ik_build_info_static_version(void) { return IK_VERSION; }
int ik_build_info_static_build_number(void) { return IK_BUILD_NUMBER; }
const char* ik_build_info_static_host(void) { return "Windows"; }
const char* ik_build_info_static_date(void) { return ""; }
const char* ik_build_info_static_commit(void) { return ""; }
const char* ik_build_info_static_compiler(void) { return "MSVC"; }
const char* ik_build_info_static_cmake(void) { return ""; }
const char* ik_build_info_static_all(void) { return "IK " IK_VERSION; }
]])
        end

        for _, v in ipairs(os.files(path.join(root, "include", "vtables", "*.v"))) do
            local out = path.join(gen, "include", "private", "ik", path.basename(v) .. ".h")
            if not os.isfile(out) then
                os.vrunv(py, {path.join(root, "ik_gen_vtable.py"), v, out}, {curdir = root})
            end
        end
    end)
end)

-- define targets
target("NAF", function()
    add_rules("commonlibf4.plugin", {
        name = "NAF",
        author = "Deweh",
        description = "Native Animation Framework"
    })

    -- add dependencies
    add_deps("ik")
    add_packages("cereal", "nlohmann_json", "zstr", "libzippp", "fp16", "pugixml")

    -- add src files
    add_files("src/**.cpp")
    add_headerfiles("src/**.h", "include/**.h")
    add_includedirs("src", "include")
    add_linkdirs("src/detourxs")
    set_pcxxheader("src/PCH.h")
end)
