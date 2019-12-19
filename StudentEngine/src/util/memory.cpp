#include "stdafx.h"

#ifdef DEBUG_0
int64 Memory::g_allocated = 0;
int64 Memory::g_totalAllocations = 0;
Memory::MemoryAllocation Memory::g_base = { &Memory::g_base, &Memory::g_base, 0, false, NULL, 0 };

void* Memory::Allocate(size_t size, bool isArray, String_t file, uint line) {
	size_t newSize = sizeof(MemoryAllocation) + size;
	MemoryAllocation* newAllocation = (MemoryAllocation*)malloc(newSize);

	newAllocation->m_next = g_base.m_next;
	newAllocation->m_prev = &g_base;
	newAllocation->m_size = size;
	newAllocation->m_array = isArray;
	newAllocation->m_file = NULL;
	newAllocation->m_line = line;

	if (file) {
		newAllocation->m_file = (char*)malloc(strlen(file) + 1);
		strcpy(newAllocation->m_file, file);
	}

	g_base.m_next->m_prev = newAllocation;
	g_base.m_next = newAllocation;

	g_allocated += size;
	g_totalAllocations++;

	return (char*)newAllocation + sizeof(MemoryAllocation);
}

void Memory::Free(void* block, bool isArray) {
	MemoryAllocation* currentAllocation = (MemoryAllocation*)((char*)block - sizeof(MemoryAllocation));

	if (currentAllocation->m_array != isArray) LOG_ERROR("Memory freed incorrectly!");

	// update the memory list
	g_allocated -= currentAllocation->m_size;

	if (currentAllocation->m_file) free(currentAllocation->m_file);
	free(currentAllocation);
}

uint Memory::CheckAllocations() {
	unsigned int count = 0;

	MemoryAllocation* ptr = g_base.m_next;
	while (ptr && *(uint64*)ptr != 0xdddddddddddddddd && ptr != &g_base) {
		//if (ptr->m_file != NULL) {
		LOG("%s 0x%llX | size: %8d | File: %s:%d", ptr->m_array ? "leak[]" : "leak  ", ptr, ptr->m_size, ptr->m_file == NULL ? "None" : ptr->m_file, ptr->m_line);
		count++;
		//}
		ptr = ptr->m_next;
	}

	if (count)
		LOG("Total %d leaks, size: %lld bytes", count, g_allocated);
	return count;
}
#endif