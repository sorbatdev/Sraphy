#pragma once

#include <fmt/format.h>
#include <fmt/color.h>

#define LOG_INFO(FormatText, ...)       fmt::print(FormatText, __VA_ARGS__);
#define LOG_WARNING(FormatText, ...)    fmt::print(fmt::fg(fmt::color::yellow), FormatText, __VA_ARGS__)
#define LOG_ERROR(FormatText, ...)      fmt::print(fmt::fg(fmt::color::red), FormatText, __VA_ARGS__)
#if defined(SR_DEBUG)
#   define LOG_DEBUG(FormatText, ...)  fmt::print(fmt::fg(fmt::color::green), FormatText, __VA_ARGS__)
#endif
