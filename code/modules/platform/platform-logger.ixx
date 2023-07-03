module;

export module platform:logger;

import cstd;
import core;

export enum class LogColor : u8 
{
    black,
    dark_blue,
    dark_green,
    dark_cyan,
    dark_red,
    dark_magenta,
    dark_yellow,
    grey,
    dark_grey,
    blue,
    green,
    cyan,
    red,
    magenta,
    yellow,
    white,
    max,
};

export enum class LogLevel : u8
{
    trace,
    debug,
    info,
    warn,
    error,
    fatal,
    max,
};

export enum class LogMode : u8
{
    console = as_bit<u8>(0),
    file    = as_bit<u8>(1), // TODO(enlynn): currently unsupported
};

export struct LogLevelColors
{
    LogColor trace = LogColor::white;
    LogColor debug = LogColor::white;
    LogColor info  = LogColor::white;
    LogColor warn  = LogColor::white;
    LogColor error = LogColor::white;
    LogColor fatal = LogColor::white;
};

//
// Platform specific logging.
//

void log_internal(const char* message, s64 message_length, LogLevel level, LogColor foreground, LogColor background);

//
// Logger API
//

//
// The Platform Logger is a global structure that can be accessed anywhere.
//
export void logger_initialize(
    LogLevel       min_log_level     = LogLevel::trace,
    LogMode        log_mode          = LogMode::console,
    LogLevelColors foreground_colors = {
        .trace = LogColor::white,
        .debug = LogColor::white,
        .info  = LogColor::white,
        .warn  = LogColor::white,
        .error = LogColor::white,
        .fatal = LogColor::white,
    },
    LogLevelColors background_colors = {
        .trace = LogColor::black,
        .debug = LogColor::black,
        .info  = LogColor::black,
        .warn  = LogColor::black,
        .error = LogColor::black,
        .fatal = LogColor::black,
});
export void logger_shutdown();

export LogLevel logger_get_min_log_level();
export LogColor logger_get_foreground_color(LogLevel level);
export LogColor logger_get_background_color(LogLevel level);

export struct LogPacket 
{
    LogLevel             level    = LogLevel::info;
    std::source_location location = std::source_location::current(); // gets the file information of the caller
};

//
// Default log level is ::info
//
// Usage:
//     log({}, "Hello world")
//     log({.level = LogLevel::warn}, "Hello world")
//     log({}, "Hello world {} {}", some_variable, some_other_variable)
//
// Log Format:
//     [level] [filename:line] : Message
//
export template <typename... Args> void log(
    LogPacket                    packet,
    std::format_string<Args...>  fmt,
    Args&&...                    args) 
{
    LogLevel min_log_level = logger_get_min_log_level();
    if (packet.level < min_log_level)
        return;
    
    const char *log_level_strings[] = {
        "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
    };

    // TODO(enlynn): It would be ideal to avoid the allocations here, but using std::string for simplicity
    std::string log_header = std::format(
        "[{}] [{}:{}] : ", log_level_strings[int(packet.level)], 
        packet.location.file_name(), packet.location.line()
    );

    std::string log_message = log_header + std::format(fmt, std::forward<Args>(args)...) + "\n";
    log_internal(
        log_message.c_str(), log_message.size(), 
        packet.level, 
        logger_get_foreground_color(packet.level), 
        logger_get_background_color(packet.level)
    );
}