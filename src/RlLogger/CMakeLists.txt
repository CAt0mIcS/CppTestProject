set(SRC
    "include/RlLogger/Formatters/BracketFormatter.h"
    "include/RlLogger/Formatters/DateTimeFormatter.h"
    "include/RlLogger/Formatters/Formatter.h"
    "include/RlLogger/Formatters/LogLevelFormatter.h"
    "include/RlLogger/Formatters/NullTerminatorFormatter.h"
    "include/RlLogger/Loggers/BaseLogger.h"
    "include/RlLogger/Loggers/FileLogger.h"
    "include/RlLogger/LogConfigure.h"
    "include/RlLogger/LogLevel.h"

    "include/RlLogger/Log.h"
)

set(INC
    "include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../RlUtilities/include"
)


add_library("RlLogger" INTERFACE)

target_include_directories("RlLogger" INTERFACE "${INC}")
target_link_libraries("RlLogger" INTERFACE "RlUtilities")

# set_target_properties("RlLogger" PROPERTIES FOLDER "RlUtilities")
add_dependencies("RlLogger" "RlUtilities")
AddRecursiveFilters("${SRC}")
