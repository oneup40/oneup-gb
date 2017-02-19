set(LIBGB1_SRC audio/Audio.cc audio/Channel1.cc audio/Channel2.cc 
               audio/Channel3.cc audio/Channel4.cc cpu/CPU.cc io/IO.cc 
               joypad/Joypad.cc lcd/LCD.cc Loader.cc Machine.cc Mapper.cc 
               serial/Serial.cc Serializer.cc timer/Timer.cc)

if (MSVC)
    add_library(gb1 ${LIBGB1_SRC})
    target_link_libraries(gb1 -LTCG)
else (MSVC)
    add_library(gb1 SHARED ${LIBGB1_SRC})
endif(MSVC)
