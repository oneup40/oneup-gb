set(LIBGB1_SRC Audio.cc CPU.cc Joypad.cc LCD.cc Loader.cc Machine.cc Mapper.cc
               Serializer.cc Timer.cc)
add_library(gb1 ${LIBGB1_SRC})

if (MSVC)
    target_link_libraries(gb1 -LTCG)
endif(MSVC)
