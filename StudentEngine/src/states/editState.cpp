#include "stdafx.h"

void EditState::EnterState() {
	LOG("[~GStates~x] Entered ~1%s~x state", typeid(*this).name());
}

void EditState::ExitState() {
	LOG("[~GStates~x] Exited ~1%s~x state", typeid(*this).name());
}