#include "stdafx.h"

int Undo::MAXSTACKSIZE = 200;
GameObject Undo::g_baseObject;
Undo::UndoEntryCollection* Undo::m_currentCollection;
bool Undo::m_recording;
DoublyLinkedList<Undo::UndoEntryCollection*> Undo::m_list;
vector<UndoType*> Undo::m_types;