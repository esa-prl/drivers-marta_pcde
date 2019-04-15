rock_library(pcde
    SOURCES PCDE.cpp SerialPort.cpp Command.cpp
    HEADERS PCDE.hpp SerialPort.hpp Command.hpp
    DEPS_PKGCONFIG iodrivers_base)

rock_executable(pcde_bin Main.cpp
    DEPS pcde)

