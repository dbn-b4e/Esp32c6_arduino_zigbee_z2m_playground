#pragma once

#include <Arduino.h>

#include <functional>
#include <map>
#include <vector>

class CommandParser {
  public:
    enum class CaseSensivity {
        EQUALS,
        IGNORE
    };

  public:
    using CommandHandler    = std::function<void(const std::vector<String>&)>;
    using OverflowHandler   = std::function<void(void)>;
    using CommandHandlerMap = std::map<String, CommandHandler>;

  public:
    CommandParser(Stream& stream, size_t buffer_size = 64);
    ~CommandParser();

    void addChar(char c);
    void processFromStream(Stream& stream, bool echo = false);
    void processCommands(bool echo=false);

    void onCommand(const String& command, CommandHandler handler, CaseSensivity cs = CaseSensivity::EQUALS);
    void onCommand(const String& command, CommandHandler handler, String helpText, CaseSensivity cs = CaseSensivity::EQUALS);
    void onNotFound(CommandHandler handler);
    void onOverflow(OverflowHandler handler);

  protected:
    void resetIndex();
    void handleOverflow();
    void handleCommand();
    void handleHelp(const std::vector<String>& params);

  protected:
    struct CommandHandlerInfo {
        CommandHandlerInfo(const String& command, CommandHandler handler, String helpText, CaseSensivity cs);
        String         cmd;
        String         helpText;
        CommandHandler handler;
        CaseSensivity  cs;
    };

  protected:
    Stream&                         stream;
    size_t                          buffer_size;
    char*                           buffer;
    size_t                          index = 0;
    std::vector<CommandHandlerInfo> commandHandlers;
    OverflowHandler                 overflowHandler = nullptr;
    CommandHandler                  notFoundHandler = nullptr;
};
