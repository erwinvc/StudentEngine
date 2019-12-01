#include "stdafx.h"

int Undo::MAXLISTSIZE = 200;
int Undo::g_newListIndex = 0;
int Undo::g_currentListIndex = 0;

GameObject Undo::g_baseObject;
Undo::UndoEntryCollection* Undo::m_currentCollection;
bool Undo::m_recording;
DoublyLinkedList<Undo::UndoEntryCollection*> Undo::m_list;
vector<UndoType*> Undo::m_types;