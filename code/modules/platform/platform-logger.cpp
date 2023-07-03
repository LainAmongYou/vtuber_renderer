module; 

module platform:logger;

struct Logger
{
    LogLevel min_log_level = LogLevel::trace;
    LogMode log_mode       = LogMode::console;

    LogLevelColors foreground_colors = {
        .trace = LogColor::white,
        .debug = LogColor::white,
        .info  = LogColor::white,
        .warn  = LogColor::white,
        .error = LogColor::white,
        .fatal = LogColor::white,
    };

    LogLevelColors background_colors = {
        .trace = LogColor::black,
        .debug = LogColor::black,
        .info  = LogColor::black,
        .warn  = LogColor::black,
        .error = LogColor::black,
        .fatal = LogColor::black,
    };
};

static Logger g_logger{};

void logger_initialize(LogLevel min_log_level, LogMode log_mode, LogLevelColors foreground_colors, LogLevelColors background_colors)
{
    g_logger.min_log_level = min_log_level;
    g_logger.log_mode = log_mode;
    g_logger.foreground_colors = foreground_colors;
    g_logger.background_colors = background_colors;

    log({}, "Logger is initialized.");
}

void logger_shutdown()
{
    // Nothing to do.
}

LogLevel logger_get_min_log_level()
{
    return g_logger.min_log_level;
}

LogColor logger_get_foreground_color(LogLevel level)
{
    switch (level)
    {
        case LogLevel::trace: return g_logger.foreground_colors.trace; break;
        case LogLevel::debug: return g_logger.foreground_colors.debug; break;
        case LogLevel::info:  return g_logger.foreground_colors.info;  break;
        case LogLevel::warn:  return g_logger.foreground_colors.warn;  break;
        case LogLevel::error: return g_logger.foreground_colors.error; break;
        case LogLevel::fatal: return g_logger.foreground_colors.fatal; break;
        case LogLevel::max:
        default:              return g_logger.background_colors.info;  break;
    }
}

LogColor logger_get_background_color(LogLevel level)
{
    switch (level)
    {
        case LogLevel::trace: return g_logger.background_colors.trace; break;
        case LogLevel::debug: return g_logger.background_colors.debug; break;
        case LogLevel::warn:  return g_logger.background_colors.warn;  break;
        case LogLevel::info:  return g_logger.background_colors.info;  break;
        case LogLevel::error: return g_logger.background_colors.error; break;
        case LogLevel::fatal: return g_logger.background_colors.fatal; break;
        case LogLevel::max:
        default:              return g_logger.background_colors.info;  break;
    }
}