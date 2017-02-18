set(LIBGB1_SRC audio/Audio.cc cpu/CPU.cc Joypad.cc LCD.cc Loader.cc Machine.cc
               Mapper.cc Serializer.cc Timer.cc)

if (MSVC)
    add_library(gb1 ${LIBGB1_SRC})
    target_link_libraries(gb1 -LTCG)
else (MSVC)
    add_library(gb1 SHARED ${LIBGB1_SRC})
endif(MSVC)
