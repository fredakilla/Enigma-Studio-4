#--------------------------------------------------------------------
# Assembly compiling step
#--------------------------------------------------------------------

# custom build options in gui :
# argument : C:\github\enigma4\tools\JWASM.EXE
# command : -coff -win64 -DASM_WIN64 -Zi -Fo%{buildDir}/callfunc.o eshared/opstacking/callfunc.asm
# path : %{sourceDir}

ASM_SOURCES = ../../eshared/opstacking/callfunc.asm
asm_compiler.input = ASM_SOURCES
asm_compiler.output = ${QMAKE_FILE_BASE}.obj
asm_compiler.commands =  ../../../tools/JWASM.EXE
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
    DESTDIR = $$PWD/../../../binary
} else {
    DESTDIR = $$PWD/../../../binary
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


win32: DEFINES +=   _HAS_ITERATOR_DEBUGGING=1 \
                    _ITERATOR_DEBUG_LEVEL=2



#--------------------------------------------------------------------
# libraries includes
#--------------------------------------------------------------------

INCLUDEPATH += $${MYLIBS}/bgfx/include
INCLUDEPATH += $${MYLIBS}/bx/include
INCLUDEPATH += ../../eshared


#--------------------------------------------------------------------
# libraries link
#--------------------------------------------------------------------

CONFIG(debug,debug|release) {
#message( debug )
win32:LIBS += -L$${MYLIBS}/bgfx/.build/win64_vs2015/bin -lbgfxDebug
win32:LIBS += -L$${MYLIBS}/bgfx/.build/win64_vs2015/bin -lbxDebug
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
    ../../eshared/opstacking/modules/tinylsys3/attractor.hpp \
    ../../eshared/opstacking/modules/tinylsys3/kdtree.hpp \
    ../../eshared/opstacking/modules/tinylsys3/rewritesystem.hpp \
    ../../eshared/opstacking/modules/tinylsys3/tinylsys3.hpp \
    ../../eshared/opstacking/modules/tinylsys3/turtleinterpreter.hpp \
    ../../eshared/engine/camera.hpp \
    ../../eshared/engine/culler.hpp \
    ../../eshared/engine/deferredrenderer.hpp \
    ../../eshared/engine/demo.hpp \
    ../../eshared/engine/editmesh.hpp \
    ../../eshared/engine/effect.hpp \
    ../../eshared/engine/engine.hpp \
    ../../eshared/engine/font.hpp \
    ../../eshared/engine/graphics.hpp \
    ../../eshared/engine/graphicsdx11.hpp \
    ../../eshared/engine/irenderable.hpp \
    ../../eshared/engine/light.hpp \
    ../../eshared/engine/material.hpp \
    ../../eshared/engine/mesh.hpp \
    ../../eshared/engine/particlesys.hpp \
    ../../eshared/engine/path.hpp \
    ../../eshared/engine/renderjob.hpp \
    ../../eshared/engine/scene.hpp \
    ../../eshared/engine/scenedata.hpp \
    ../../eshared/engine/sequencer.hpp \
    ../../eshared/engine/transfwidgets.hpp \
    ../../eshared/math/aabb.hpp \
    ../../eshared/math/math.hpp \
    ../../eshared/math/matrix.hpp \
    ../../eshared/math/plane.hpp \
    ../../eshared/math/quat.hpp \
    ../../eshared/math/ray.hpp \
    ../../eshared/math/transform.hpp \
    ../../eshared/math/vector.hpp \
    ../../eshared/opstacking/bitmapops.hpp \
    ../../eshared/opstacking/demodata.hpp \
    ../../eshared/opstacking/demoscript.hpp \
    ../../eshared/opstacking/effectops.hpp \
    ../../eshared/opstacking/ioperator.hpp \
    ../../eshared/opstacking/meshops.hpp \
    ../../eshared/opstacking/miscops.hpp \
    ../../eshared/opstacking/modelops.hpp \
    ../../eshared/opstacking/opmacros.hpp \
    ../../eshared/opstacking/oppage.hpp \
    ../../eshared/opstacking/opstacking.hpp \
    ../../eshared/opstacking/parameter.hpp \
    ../../eshared/opstacking/script.hpp \
    ../../eshared/opstacking/sequencerops.hpp \
    ../../eshared/packing/arith.hpp \
    ../../eshared/packing/bwt.hpp \
    ../../eshared/packing/ipacker.hpp \
    ../../eshared/packing/mtf.hpp \
    ../../eshared/packing/packing.hpp \
    ../../eshared/packing/rle.hpp \
    ../../eshared/synth/synth.hpp \
    ../../eshared/synth/tf4.hpp \
    ../../eshared/synth/tf4dx.hpp \
    ../../eshared/synth/tf4fx.hpp \
    ../../eshared/system/array.hpp \
    ../../eshared/system/color.hpp \
    ../../eshared/system/datastream.hpp \
    ../../eshared/system/file.hpp \
    ../../eshared/system/point.hpp \
    ../../eshared/system/profiler.hpp \
    ../../eshared/system/rect.hpp \
    ../../eshared/system/runtime.hpp \
    ../../eshared/system/simd.hpp \
    ../../eshared/system/string.hpp \
    ../../eshared/system/system.hpp \
    ../../eshared/system/threading.hpp \
    ../../eshared/system/timer.hpp \
    ../../eshared/system/types.hpp \
    ../../eshared/eshared.hpp


SOURCES += \
    ../../eshared/engine/demo.cpp \
    ../../eshared/engine/engine.cpp \
    ../../eshared/engine/font.cpp \
    ../../eshared/engine/graphicsdx11.cpp \
    ../../eshared/engine/culler.cpp \
    ../../eshared/engine/editmesh.cpp \
    ../../eshared/engine/effect.cpp \
    ../../eshared/engine/camera.cpp \
    ../../eshared/engine/light.cpp \
    ../../eshared/engine/material.cpp \
    ../../eshared/engine/mesh.cpp \
    ../../eshared/engine/particlesys.cpp \
    ../../eshared/engine/path.cpp \
    ../../eshared/engine/renderjob.cpp \
    ../../eshared/engine/scenedata.cpp \
    ../../eshared/engine/sequencer.cpp \
    ../../eshared/engine/transfwidgets.cpp \
    ../../eshared/engine/deferredrenderer.cpp \
    ../../eshared/math/aabb.cpp \
    ../../eshared/math/matrix.cpp \
    ../../eshared/math/plane.cpp \
    ../../eshared/math/quat.cpp \
    ../../eshared/math/ray.cpp \
    ../../eshared/math/transform.cpp \
    ../../eshared/math/vector.cpp \
    ../../eshared/opstacking/demodata.cpp \
    ../../eshared/opstacking/modules/physicsops.cpp \
    ../../eshared/opstacking/bitmapops.cpp \
    ../../eshared/opstacking/demoscript.cpp \
    ../../eshared/opstacking/effectops.cpp \
    ../../eshared/opstacking/ioperator.cpp \
    ../../eshared/opstacking/meshops.cpp \
    ../../eshared/opstacking/miscops.cpp \
    ../../eshared/opstacking/modelops.cpp \
    ../../eshared/opstacking/oppage.cpp \
    ../../eshared/opstacking/parameter.cpp \
    ../../eshared/opstacking/script.cpp \
    ../../eshared/opstacking/sequencerops.cpp \
    ../../eshared/opstacking/modules/tinylsys3/attractor.cpp \
    ../../eshared/opstacking/modules/tinylsys3/kdtree.cpp \
    ../../eshared/opstacking/modules/tinylsys3/rewritesystem.cpp \
    ../../eshared/opstacking/modules/tinylsys3/tinylsys3.cpp \
    ../../eshared/opstacking/modules/tinylsys3/tinylsys3ops.cpp \
    ../../eshared/opstacking/modules/tinylsys3/turtleinterpreter.cpp \
    ../../eshared/packing/arith.cpp \
    ../../eshared/packing/bwt.cpp \
    ../../eshared/packing/mtf.cpp \
    ../../eshared/packing/rle.cpp \
    ../../eshared/synth/synth.cpp \
    ../../eshared/synth/tf4.cpp \
    ../../eshared/synth/tf4dx.cpp \
    ../../eshared/synth/tf4fx.cpp \
    ../../eshared/system/array.cpp \
    ../../eshared/system/color.cpp \
    ../../eshared/system/datastream.cpp \
    ../../eshared/system/file.cpp \
    ../../eshared/system/point.cpp \
    ../../eshared/system/profiler.cpp \
    ../../eshared/system/rect.cpp \
    ../../eshared/system/runtime.cpp \
    ../../eshared/system/simd.cpp \
    ../../eshared/system/string.cpp \
    ../../eshared/system/threading.cpp \
    ../../eshared/system/timer.cpp \
    main.cpp










