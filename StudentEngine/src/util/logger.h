#pragma once

#ifdef DEBUG
#define LOG( fmt, ...)					Logger::Message((int)ConsoleColor::WHITE,				" [Info]",		fmt, ##__VA_ARGS__ )
#define LOG_WARN( fmt, ...)				Logger::Message((int)ConsoleColor::RED,					" [Warn]",		fmt, ##__VA_ARGS__ )
#define LOG_ERROR( fmt, ...)			{Logger::Message((int)ConsoleColor::RED,			" [Fail]",		fmt, ##__VA_ARGS__ ); __debugbreak();}


enum class ConsoleColor {
	BLACK = 0,
	DARKBLUE = FOREGROUND_BLUE,
	DARKGREEN = FOREGROUND_GREEN,
	DARKCYAN = FOREGROUND_GREEN | FOREGROUND_BLUE,
	DARKRED = FOREGROUND_RED,
	DARKMAGENTA = FOREGROUND_RED | FOREGROUND_BLUE,
	DARKYELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
	DARKGRAY = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	GRAY = FOREGROUND_INTENSITY,
	BLUE = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
	GREEN = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
	CYAN = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
	RED = FOREGROUND_INTENSITY | FOREGROUND_RED,
	MAGENTA = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
	YELLOW = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
	WHITE = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	RESET = 16
};


class Logger {
public:
	struct QueuedMessage {
		int m_color;
		String m_message;
		String m_type;
		time_t m_time;
		QueuedMessage(int color, String message, String type, time_t time) : m_color(color), m_message(message), m_type(type), m_time(time) {}
		QueuedMessage() : m_color(0), m_message(""), m_type(""), m_time(0) {}
	};
	static const int MAXQUEUESIZE = 1000;
	static void Initialize();
	static void Message(int color, const char* type, const char* fmt, ...);
	static void Cleanup();
	static void OnImGui();
	static bool m_allocated;

private:
	static FILE *m_stream;
	static HANDLE m_outputHandle;
	static CONSOLE_SCREEN_BUFFER_INFO m_screenBuffer;
	static bool m_firstEntry;
	static bool m_stopping;

	static void LogToFile(const char * buff);
	static void SetTextColor(const int color);
	static const char* GetTimeAsString();

	//Queue
	static mutex m_mutex;
	static condition_variable m_conditionVariable;
	static queue<QueuedMessage> m_queue;
};

#else
#define LOG( fmt, ...)					
#define LOG_WARN( fmt, ...)				
#define LOG_ERROR( fmt, ...)

class Logger {
public:
	static void Initialize() {}
	static void Cleanup() {}
};
#endif