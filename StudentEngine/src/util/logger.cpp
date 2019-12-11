#include "stdafx.h"

#ifdef DEBUG
struct ImGuiLog {
private:
	ImGuiTextBuffer m_buffer;
	ImGuiTextFilter m_filter;
	vector<int> m_lineOffsets;
	bool m_autoScroll = true;
	bool m_scrollToBottom = false;

	ImVec4 CharToColor(char col) {
		switch (col) {
			case 0: return ImVec4(1.0, 0.0f, 0.0f, 1.0f);
		}
		return ImVec4(1.0, 1.0f, 1.0f, 1.0f);
	}

public:

	ImGuiLog() {
		Clear();
	}

	void Clear() {
		m_buffer.clear();
		m_lineOffsets.clear();
		m_lineOffsets.push_back(0);
	}

	void AddLog(const char* str, int color) {
		int old_size = m_buffer.size();
		m_buffer.append(color == 0 ? "0" : "1");
		m_buffer.append(str);
		for (int new_size = m_buffer.size(); old_size < new_size; old_size++)
			if (m_buffer[old_size] == '\n')
				m_lineOffsets.push_back(old_size + 1);
		if (m_autoScroll)
			m_scrollToBottom = true;
	}


	void Draw() {
		if (!ImGui::Begin("Log", nullptr)) {
			ImGui::End();
			return;
		}

		// Options menu
		if (ImGui::BeginPopup("Options")) {
			if (ImGui::Checkbox("Auto-scroll", &m_autoScroll))
				if (m_autoScroll)
					m_scrollToBottom = true;
			ImGui::EndPopup();
		}

		// Main window
		if (ImGui::Button("Options"))
			ImGui::OpenPopup("Options");
		ImGui::SameLine();
		bool clear = ImGui::Button("Clear");
		ImGui::SameLine();
		bool copy = ImGui::Button("Copy");
		ImGui::SameLine();
		m_filter.Draw("Filter", -100.0f);

		ImGui::Separator();
		ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

		if (clear)
			Clear();
		if (copy)
			ImGui::LogToClipboard();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		const char* buf = m_buffer.begin();
		const char* buf_end = m_buffer.end();
		if (m_filter.IsActive()) {
			for (int line_no = 0; line_no < m_lineOffsets.size(); line_no++) {
				const char* line_start = buf + m_lineOffsets[line_no];
				const char* line_end = (line_no + 1 < m_lineOffsets.size()) ? (buf + m_lineOffsets[line_no + 1] - 1) : buf_end;
				if (m_filter.PassFilter(line_start + 1, line_end)) {
					//ImGui::PushStyleColor(ImGuiCol_Text, CharToColor(*line_start));
					ImGui::TextUnformatted(line_start + 1, line_end);
					//ImGui::PopStyleColor();
				}
			}
		} else {
			ImGuiListClipper clipper;
			clipper.Begin((int)m_lineOffsets.size());
			while (clipper.Step()) {
				for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++) {
					const char* line_start = buf + m_lineOffsets[line_no];
					const char* line_end = (line_no + 1 < m_lineOffsets.size()) ? (buf + m_lineOffsets[line_no + 1] - 1) : buf_end;
					//ImGui::PushStyleColor(ImGuiCol_Text, CharToColor(*line_start));
					ImGui::TextUnformatted(line_start + 1, line_end);
					//ImGui::PopStyleColor();
				}
			}
			clipper.End();
		}
		ImGui::TextUnformatted("");
		ImGui::PopStyleVar();

		if (m_scrollToBottom)
			ImGui::SetScrollHereY(1.0f);
		m_scrollToBottom = false;
		ImGui::EndChild();
		ImGui::End();
	}
};

bool Logger::m_allocated = false;
bool Logger::m_firstEntry = true;
bool Logger::m_stopping = false;

HANDLE Logger::m_outputHandle = INVALID_HANDLE_VALUE;
CONSOLE_SCREEN_BUFFER_INFO Logger::m_screenBuffer;

mutex Logger::m_mutex;
condition_variable Logger::m_conditionVariable;
queue<Logger::QueuedMessage> Logger::m_queue;

ImGuiLog* m_imGuiLog;

/*Initialize the non-blocking logger*/
void Logger::Initialize() {
	if (m_allocated) return;
	
	AllocConsole();
	SetConsoleTitleA("StudentEngine");
	m_outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(m_outputHandle, &m_screenBuffer);

	//FILE* m_stream;
	//freopen_s(&m_stream, "CONIN$", "w", stdin);
	//freopen_s(&m_stream, "CONOUT$", "w", stdout);
	//freopen_s(&m_stream, "CONOUT$", "w", stderr);
	
	const int width = 120;
	const int height = 30;

	m_allocated = true;
	m_firstEntry = true;

	m_imGuiLog = new ImGuiLog();

	LOG("[~2Logging~x] Console allocated");
}

/*Set the text color of the next print to console*/
void Logger::SetTextColor(const int color) {
	WORD attributes = m_screenBuffer.wAttributes & ~FOREGROUND_RED & ~FOREGROUND_GREEN & ~FOREGROUND_BLUE & ~FOREGROUND_INTENSITY;
	attributes |= color;
	SetConsoleTextAttribute(m_outputHandle, attributes);
}

/*Get the time as a printable string*/
const char* Logger::GetTimeAsString() {
	struct tm timeStruct;
	time_t t = time(nullptr);
	localtime_s(&timeStruct, &t);

	static char buffer[48];
	sprintf_s(buffer, "[%02d:%02d:%02d]", timeStruct.tm_hour, timeStruct.tm_min, timeStruct.tm_sec);
	return buffer;
}

/*Process a QueuedMessage (async)*/
void Logger::Message(int color, const char* type, const char* fmt, ...) {
	if (m_stopping) return;
	
	char buffer[0xffff] = { 0 };
	va_list va_alist;

	va_start(va_alist, fmt);
	vsprintf_s(buffer, fmt, va_alist);
	va_end(va_alist);

	String message = buffer;
	String_t formattedTime = GetTimeAsString();
	String outputString;
	String formattedMessage = Utils::ReplaceString(message, "~", "`~");
	vector<String> splitMessage = Utils::Split(formattedMessage, "`");

	unique_lock <mutex> lock(m_mutex);
	SetTextColor(color);
	printf("%s", formattedTime);

	for (auto& splStr : splitMessage) {
		if (splStr[0] == '~') {
			if (splStr.length() < 2) continue;
			switch (splStr[1]) {
				case '0': SetTextColor((int)ConsoleColor::WHITE); break;
				case '1': SetTextColor((int)ConsoleColor::GRAY); break;
				case '2': SetTextColor((int)ConsoleColor::DARKGRAY); break;
				case '3': SetTextColor((int)ConsoleColor::BLACK); break;
				case 'R': SetTextColor((int)ConsoleColor::DARKRED); break;
				case 'r': SetTextColor((int)ConsoleColor::RED); break;
				case 'Y': SetTextColor((int)ConsoleColor::DARKYELLOW); break;
				case 'y': SetTextColor((int)ConsoleColor::YELLOW); break;
				case 'G': SetTextColor((int)ConsoleColor::DARKGREEN); break;
				case 'g': SetTextColor((int)ConsoleColor::GREEN); break;
				case 'M': SetTextColor((int)ConsoleColor::DARKMAGENTA); break;
				case 'm': SetTextColor((int)ConsoleColor::MAGENTA); break;
				case 'C': SetTextColor((int)ConsoleColor::DARKCYAN); break;
				case 'c': SetTextColor((int)ConsoleColor::CYAN); break;
				case 'B': SetTextColor((int)ConsoleColor::DARKBLUE); break;
				case 'b': SetTextColor((int)ConsoleColor::BLUE); break;
				case 'X':
				case 'x':SetTextColor(color); break;
				default:SetTextColor(color); break;
			}
			splStr.erase(0, 2);
		}
		printf("%s", splStr.c_str());
		outputString += splStr;
	}
	printf("\n");

	char buffer2[0xffff] = { 0 };
	sprintf_s(buffer2, "%s%s %s\n", formattedTime, type, outputString.c_str());
	LogToFile(buffer2);
	m_imGuiLog->AddLog(buffer2, (color == (int)ConsoleColor::WHITE ? 0 : 1));

	SetTextColor((int)ConsoleColor::WHITE);
}

/*Print the message to a logging file*/
void Logger::LogToFile(const char * buff) {
	// #Dirty add proper path
	const auto fileName = "log.log";
	ofstream logFile;
	logFile.open(fileName, ios_base::app);
	if (m_firstEntry) {
		ofstream ofs;
		ofs.open(fileName, ofstream::out | ofstream::trunc);
		ofs.close();
		logFile << endl;
		m_firstEntry = false;
	}
	logFile << buff;
}

void Logger::Cleanup() {
	if (!m_allocated) return;
	LOG("[~gLogging~x] Deallocating console");
	delete m_imGuiLog;
	//auto window = GetConsoleWindow();
	//FreeConsole();
	//PostMessage(window, WM_CLOSE, 0, 0);
}

void Logger::OnImGui() {
	m_imGuiLog->Draw();
}
#endif