#--------------------------------------------------------------------
# Global Var
#--------------------------------------------------------------------

ESHARED = ../../../eshared

#--------------------------------------------------------------------
# Assembly compiling step
#--------------------------------------------------------------------

# custom build options in gui :
# argument : C:\github\enigma4\tools\JWASM.EXE
# command : -coff -win64 -DASM_WIN64 -Zi -Fo%{buildDir}/callfunc.o eshared/opstacking/callfunc.asm
# path : %{sourceDir}

ASM_SOURCES = ../../../eshared/opstacking/callfunc.asm
asm_compiler.input = ASM_SOURCES
asm_compiler.output = ${QMAKE_FILE_BASE}.obj
asm_compiler.commands =  ../../../../tools/JWASM.EXE
asm_compiler.commands +=  -coff -win64 -DASM_WIN64 -Fo ${QMAKE_FILE_OUT} ${QMAKE_FILE_IN}
asm_compiler.variable_out = OBJECTS
asm_compiler.name = Assembling ${QMAKE_FILE_IN}
asm_compiler.CONFIG += target_predeps
QMAKE_EXTRA_COMPILERS += asm_compiler
LIBS += callfunc.obj

#--------------------------------------------------------------------
# user paths
#--------------------------------------------------------------------

win32: MYLIBS = C:/dev
unix:!macx: MYLIBS = /home/fred/libraries

#--------------------------------------------------------------------
# target
#--------------------------------------------------------------------

TEMPLATE = app
CONFIG -= console
CONFIG += windows
QT += widgets xml
CONFIG += c++11
CONFIG -= debug_and_release
CONFIG -= debug_and_release_target

#--------------------------------------------------------------------
# output directory
# copy output target to Genod/build/debug(release)
#--------------------------------------------------------------------

CONFIG(debug,debug|release){
    DESTDIR = $$PWD/../../../../binary
} else {
    DESTDIR = $$PWD/../../../../binary
}

QMAKE_CLEAN += $$DESTDIR/$$TARGET

#--------------------------------------------------------------------
# compilation flags
#--------------------------------------------------------------------

unix:!macx: QMAKE_CXXFLAGS_WARN_ON -= -Wall
unix:!macx: QMAKE_CFLAGS_WARN_ON -= -Wall
unix:!macx: QMAKE_CXXFLAGS += -Wall
unix:!macx: QMAKE_CXXFLAGS += -Wno-comment
unix:!macx: QMAKE_CXXFLAGS += -Wno-ignored-qualifiers
unix:!macx: QMAKE_CXXFLAGS += -Wno-unused-parameter
unix:!macx: QMAKE_CXXFLAGS += -std=c++11 #(or QMAKE_CXXFLAGS += -std=c++0x)


CONFIG(debug,debug|release) {
#message( debug )
} else {
#message( release )
unix:!macx: QMAKE_CXXFLAGS += -Wno-strict-aliasing

win32:QMAKE_CXXFLAGS_RELEASE -= -Zc:strictStrings

# remove possible other optimization flags
QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2

# add the desired -O3 if not present
QMAKE_CXXFLAGS_RELEASE *= -O3


}

#--------------------------------------------------------------------
# pre-processor definitions
#--------------------------------------------------------------------

CONFIG(debug,debug|release) {
#debug
DEFINES +=  \
        WIN32 \
        _DEBUG \
        _WINDOWS \
        eDEBUG \
        eEDITOR \
        eUSE_PROFILER \
        eUSE_MMX \
        eUSE_SSE \
        eAUTO_SHADERS \
        QT_NO_KEYWORDS \
} else {
# release
DEFINES +=  \
        WIN32 \
        NDEBUG \
        _WINDOWS \
        eRELEASE \
        eUSE_PROFILER \
        eEDITOR \
        eUSE_MMX \
        eUSE_SSE \
        QT_NO_KEYWORDS \
}



#win32: DEFINES += _CRT_SECURE_NO_WARNINGS
DEFINES -= UNICODE
DEFINES += _MBCS

CONFIG(debug,debug|release) {
#debug

win32: DEFINES +=   _HAS_ITERATOR_DEBUGGING=1 \
                    _ITERATOR_DEBUG_LEVEL=2
}



#--------------------------------------------------------------------
# libraries includes
#--------------------------------------------------------------------

INCLUDEPATH += $${MYLIBS}/bgfx/include
INCLUDEPATH += $${MYLIBS}/bx/include
INCLUDEPATH += $${MYLIBS}/bx/include/compat/msvc    # fix <alloca.h> include error
INCLUDEPATH += $${MYLIBS}/bgfx/tools/shaderc        # include shaderc as lib for runtime compile


INCLUDEPATH += $${ESHARED}



#--------------------------------------------------------------------
# libraries link
#--------------------------------------------------------------------

CONFIG(debug,debug|release) {
#message( debug )
win32:LIBS += -L$${MYLIBS}/bgfx/.build/win64_vs2015/bin -lbgfxDebug
win32:LIBS += -L$${MYLIBS}/bgfx/.build/win64_vs2015/bin -lbxDebug
win32:LIBS += -L$${MYLIBS}/bgfx/.build/win64_vs2015/bin -lshadercDebug
} else {
#message( release )
win32:LIBS += -L$${MYLIBS}/bgfx/.build/win64_vs2015/bin -lbgfxRelease
win32:LIBS += -L$${MYLIBS}/bgfx/.build/win64_vs2015/bin -lbxRelease
}


win32:LIBS +=   kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib \
                ole32.lib oleaut32.lib uuid.lib imm32.lib winmm.lib wsock32.lib opengl32.lib glu32.lib version.lib \
                #C:\github\enigma4\code\estudio4\debug\callfunc.obj \



win32:LIBS += opengl32.lib glu32.lib winmm.lib dsound.lib dxgi.lib d3d11.lib d3dcompiler.lib imm32.lib msimg32.lib version.lib ws2_32.lib \
#qwindowsd.lib qt5platformsupportd.lib qt5guid.lib qt5cored.lib qt5xmld.lib qt5widgetsd.lib qtmaind.lib

#--------------------------------------------------------------------
# project files
#--------------------------------------------------------------------





HEADERS += \
    $${ESHARED}/opstacking/modules/tinylsys3/attractor.hpp \
    $${ESHARED}/opstacking/modules/tinylsys3/kdtree.hpp \
    $${ESHARED}/opstacking/modules/tinylsys3/rewritesystem.hpp \
    $${ESHARED}/opstacking/modules/tinylsys3/tinylsys3.hpp \
    $${ESHARED}/opstacking/modules/tinylsys3/turtleinterpreter.hpp \
    $${ESHARED}/engine/camera.hpp \
    $${ESHARED}/engine/culler.hpp \
    $${ESHARED}/engine/deferredrenderer.hpp \
    $${ESHARED}/engine/demo.hpp \
    $${ESHARED}/engine/editmesh.hpp \
    $${ESHARED}/engine/effect.hpp \
    $${ESHARED}/engine/engine.hpp \
    $${ESHARED}/engine/font.hpp \
    $${ESHARED}/engine/graphics.hpp \
    #$${ESHARED}/engine/graphicsdx11.hpp \
    $${ESHARED}/engine/irenderable.hpp \
    $${ESHARED}/engine/light.hpp \
    $${ESHARED}/engine/material.hpp \
    $${ESHARED}/engine/mesh.hpp \
    $${ESHARED}/engine/particlesys.hpp \
    $${ESHARED}/engine/path.hpp \
    $${ESHARED}/engine/renderjob.hpp \
    $${ESHARED}/engine/scene.hpp \
    $${ESHARED}/engine/scenedata.hpp \
    $${ESHARED}/engine/sequencer.hpp \
    $${ESHARED}/engine/transfwidgets.hpp \
    $${ESHARED}/math/aabb.hpp \
    $${ESHARED}/math/math.hpp \
    $${ESHARED}/math/matrix.hpp \
    $${ESHARED}/math/plane.hpp \
    $${ESHARED}/math/quat.hpp \
    $${ESHARED}/math/ray.hpp \
    $${ESHARED}/math/transform.hpp \
    $${ESHARED}/math/vector.hpp \
    $${ESHARED}/opstacking/bitmapops.hpp \
    $${ESHARED}/opstacking/demodata.hpp \
    $${ESHARED}/opstacking/demoscript.hpp \
    $${ESHARED}/opstacking/effectops.hpp \
    $${ESHARED}/opstacking/ioperator.hpp \
    $${ESHARED}/opstacking/meshops.hpp \
    $${ESHARED}/opstacking/miscops.hpp \
    $${ESHARED}/opstacking/modelops.hpp \
    $${ESHARED}/opstacking/opmacros.hpp \
    $${ESHARED}/opstacking/oppage.hpp \
    $${ESHARED}/opstacking/opstacking.hpp \
    $${ESHARED}/opstacking/parameter.hpp \
    $${ESHARED}/opstacking/script.hpp \
    $${ESHARED}/opstacking/sequencerops.hpp \
    $${ESHARED}/packing/arith.hpp \
    $${ESHARED}/packing/bwt.hpp \
    $${ESHARED}/packing/ipacker.hpp \
    $${ESHARED}/packing/mtf.hpp \
    $${ESHARED}/packing/packing.hpp \
    $${ESHARED}/packing/rle.hpp \
    $${ESHARED}/synth/synth.hpp \
    $${ESHARED}/synth/tf4.hpp \
    $${ESHARED}/synth/tf4dx.hpp \
    $${ESHARED}/synth/tf4fx.hpp \
    $${ESHARED}/system/array.hpp \
    $${ESHARED}/system/color.hpp \
    $${ESHARED}/system/datastream.hpp \
    $${ESHARED}/system/file.hpp \
    $${ESHARED}/system/point.hpp \
    $${ESHARED}/system/profiler.hpp \
    $${ESHARED}/system/rect.hpp \
    $${ESHARED}/system/runtime.hpp \
    $${ESHARED}/system/simd.hpp \
    $${ESHARED}/system/string.hpp \
    $${ESHARED}/system/system.hpp \
    $${ESHARED}/system/threading.hpp \
    $${ESHARED}/system/timer.hpp \
    $${ESHARED}/system/types.hpp \
    $${ESHARED}/eshared.hpp


SOURCES += \
    $${ESHARED}/engine/demo.cpp \
    $${ESHARED}/engine/engine.cpp \
    $${ESHARED}/engine/font.cpp \
    #$${ESHARED}/engine/graphicsdx11.cpp \
    $${ESHARED}/engine/culler.cpp \
    $${ESHARED}/engine/editmesh.cpp \
    $${ESHARED}/engine/effect.cpp \
    $${ESHARED}/engine/camera.cpp \
    $${ESHARED}/engine/light.cpp \
    $${ESHARED}/engine/material.cpp \
    $${ESHARED}/engine/mesh.cpp \
    $${ESHARED}/engine/particlesys.cpp \
    $${ESHARED}/engine/path.cpp \
    $${ESHARED}/engine/renderjob.cpp \
    $${ESHARED}/engine/scenedata.cpp \
    $${ESHARED}/engine/sequencer.cpp \
    $${ESHARED}/engine/transfwidgets.cpp \
    $${ESHARED}/engine/deferredrenderer.cpp \
    $${ESHARED}/math/aabb.cpp \
    $${ESHARED}/math/matrix.cpp \
    $${ESHARED}/math/plane.cpp \
    $${ESHARED}/math/quat.cpp \
    $${ESHARED}/math/ray.cpp \
    $${ESHARED}/math/transform.cpp \
    $${ESHARED}/math/vector.cpp \
    $${ESHARED}/opstacking/demodata.cpp \
    $${ESHARED}/opstacking/modules/physicsops.cpp \
    $${ESHARED}/opstacking/bitmapops.cpp \
    $${ESHARED}/opstacking/demoscript.cpp \
    $${ESHARED}/opstacking/effectops.cpp \
    $${ESHARED}/opstacking/ioperator.cpp \
    $${ESHARED}/opstacking/meshops.cpp \
    $${ESHARED}/opstacking/miscops.cpp \
    $${ESHARED}/opstacking/modelops.cpp \
    $${ESHARED}/opstacking/oppage.cpp \
    $${ESHARED}/opstacking/parameter.cpp \
    $${ESHARED}/opstacking/script.cpp \
    $${ESHARED}/opstacking/sequencerops.cpp \
    $${ESHARED}/opstacking/modules/tinylsys3/attractor.cpp \
    $${ESHARED}/opstacking/modules/tinylsys3/kdtree.cpp \
    $${ESHARED}/opstacking/modules/tinylsys3/rewritesystem.cpp \
    $${ESHARED}/opstacking/modules/tinylsys3/tinylsys3.cpp \
    $${ESHARED}/opstacking/modules/tinylsys3/tinylsys3ops.cpp \
    $${ESHARED}/opstacking/modules/tinylsys3/turtleinterpreter.cpp \
    $${ESHARED}/packing/arith.cpp \
    $${ESHARED}/packing/bwt.cpp \
    $${ESHARED}/packing/mtf.cpp \
    $${ESHARED}/packing/rle.cpp \
    $${ESHARED}/synth/synth.cpp \
    $${ESHARED}/synth/tf4.cpp \
    $${ESHARED}/synth/tf4dx.cpp \
    $${ESHARED}/synth/tf4fx.cpp \
    $${ESHARED}/system/array.cpp \
    $${ESHARED}/system/color.cpp \
    $${ESHARED}/system/datastream.cpp \
    $${ESHARED}/system/file.cpp \
    $${ESHARED}/system/point.cpp \
    $${ESHARED}/system/profiler.cpp \
    $${ESHARED}/system/rect.cpp \
    $${ESHARED}/system/runtime.cpp \
    $${ESHARED}/system/simd.cpp \
    $${ESHARED}/system/string.cpp \
    $${ESHARED}/system/threading.cpp \
    $${ESHARED}/system/timer.cpp \
    fullvertex.cpp



#CONFIG += RENDERER_D3D11
#RENDERER_D3D11 {
#    message(d3d11)
    #HEADERS += $${ESHARED}/engine/graphicsdx11.hpp
    #SOURCES += $${ESHARED}/engine/graphicsdx11.cpp
#} else {
    message(bgfx)
    HEADERS += $${ESHARED}/engine/graphicsbgfx.hpp
    SOURCES += $${ESHARED}/engine/graphicsbgfx.cpp
#}




