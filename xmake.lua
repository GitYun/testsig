add_rules("mode.debug", "mode.release")

target("./testsig")
    set_kind("binary")
    add_files("src/*.cpp")

    add_files("3rdparty/cJSON/cJSON.c")
    add_files("3rdparty/CSerialPort/src/SerialPort.cpp", "3rdparty/CSerialPort/src/SerialPortInfo.cpp",
              "3rdparty/CSerialPort/src/SerialPortBase.cpp", "3rdparty/CSerialPort/src/SerialPortInfoBase.cpp",
              "3rdparty/CSerialPort/src/SerialPortWinBase.cpp", "3rdparty/CSerialPort/src/SerialPortInfoWinBase.cpp")
    
    add_files("3rdparty/liblightmodbus/src/lightmodbus.c",
              "3rdparty/liblightmodbus/src/master.c",
              "3rdparty/liblightmodbus/src/master/*.c")

    add_includedirs("3rdparty")
    add_includedirs("3rdparty/sigslot/include")
    add_includedirs("3rdparty/CSerialPort/include")
    add_includedirs("3rdparty/liblightmodbus/include")
    add_includedirs("3rdparty/xlnt/include")

    add_includedirs("D:/OpenFree/FLTK-1.3.6/include")

    add_defines("WIN32", "USE_OPENGL32", "_LARGEFILE_SOURCE", "_LARGEFILE64_SOURCE", "_FILE_OFFSET_BITS=64")
    add_ldflags("-mwindows", "-lole32", "-luuid", "-lcomctl32", "-lsetupapi")

    if is_arch("x86_64") then
        add_linkdirs("D:/OpenFree/FLTK-1.3.6/lib")
        add_linkdirs("3rdparty/xlnt/lib")
    else
        add_linkdirs("D:/OpenFree/FLTK-1.3.6/lib/lib32")
        add_linkdirs("3rdparty/xlnt/lib/lib32")
    end
    add_links("fltk")
    -- add_ldflags("fltk_gl", "fltk_images", "fltk_png", "fltk_jpeg")
    add_links("xlnt")
    add_links("wsock32")

    add_ldflags("-static-libgcc -static-libstdc++ -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")


--
-- If you want to known more usage about xmake, please see https://xmake.io
--
-- ## FAQ
--
-- You can enter the project directory firstly before building project.
--
--   $ cd projectdir
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. Where is the build output directory?
--
--   The default output directory is `./build` and you can configure the output directory.
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run [targetname]
--   $ xmake run -d [targetname]
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
-- @code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- add macro defination
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--

