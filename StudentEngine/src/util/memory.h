#pragma once

#ifdef DEBUG
#pragma warning (disable: 4291) /*no matching operator delete found*/

class Memory {
	struct MemoryAllocation {
		MemoryAllocation* m_next;
		MemoryAllocation* m_prev;
		size_t m_size;
		bool m_array;
		char* m_file;
		uint m_line;
	};
private:
	static int64 g_allocated;
	static int64 g_totalAllocations;
	static MemoryAllocation g_base;
public:

	static void* Allocate(size_t size, bool isArray, String_t file, uint line);
	static void Free(void* block, bool isArray);

	static uint CheckAllocations();
};

inline void* operator new (size_t _size) {
	return Memory::Allocate(_size, false, NULL, 0);
}

inline void* operator new[](size_t _size) {
	return Memory::Allocate(_size, true, NULL, 0);
}

inline void* operator new (size_t _size, char* _file, unsigned int _line) {
	return Memory::Allocate(_size, false, _file, _line);
}

inline void* operator new[](size_t _size, char* _file, unsigned int _line) {
	return Memory::Allocate(_size, true, _file, _line);
}

inline void operator delete (void* _ptr) noexcept {
	if (_ptr)
		Memory::Free(_ptr, false);
}

inline void operator delete[](void* _ptr) noexcept {
	if (_ptr)
		Memory::Free(_ptr, true);
}

#define new new(__FILE__, __LINE__)
#else
class Memory {
public:
	static uint CheckAllocations() { return 0; }
};
#endif