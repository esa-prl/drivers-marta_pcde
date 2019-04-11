rock_library(pcde
    SOURCES Dummy.cpp
    HEADERS Dummy.hpp)

rock_executable(pcde_bin Main.cpp
    DEPS pcde)

