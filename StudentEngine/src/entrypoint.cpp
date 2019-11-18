#include "stdafx.h"

int main() {
	srand((uint)time(NULL));
	Logger::Initialize();

	LOG("test");
	Logger::Cleanup();
	std::cin.get();
}