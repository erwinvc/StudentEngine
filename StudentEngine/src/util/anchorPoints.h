#pragma once

enum Anchors {
	TOPLEFT,
	TOPCENTER,
	TOPRIGHT,
	MIDDLELEFT,
	MIDDLECENTER,
	MIDDLERIGHT,
	BOTTOMLEFT,
	BOTTOMCENTER,
	BOTTOMRIGHT,
	MOUSECURSOR
};

class AnchorPoints {
public:
	//Returns a ImVec2 with one of nine anchor points, using the ImGui::GetMainViewport.
	static ImVec2 GetAnchor(Anchors anchor, ImVec2 offset = ImVec2(0,0)) {
		ImVec2 pos = ImVec2(0, 0);

		switch (anchor) {
		case TOPLEFT:
			pos = ImVec2(0, 0);
			break;
		case TOPCENTER:
			pos = ImVec2(ImGui::GetMainViewport()->Size.x*0.5f, 0);
			break;
		case TOPRIGHT:
			pos = ImVec2(ImGui::GetMainViewport()->Size.x, 0);
			break;
		case MIDDLELEFT:
			pos = ImVec2(0, ImGui::GetMainViewport()->Size.y*0.5f);
			break;
		case MIDDLECENTER:
			pos = ImVec2(ImGui::GetMainViewport()->Size.x/2, ImGui::GetMainViewport()->Size.y*0.5f);
			break;
		case MIDDLERIGHT:
			pos = ImVec2(ImGui::GetMainViewport()->Size.x, ImGui::GetMainViewport()->Size.y*0.5f);
			break;
		case BOTTOMLEFT:
			pos = ImVec2(0, ImGui::GetMainViewport()->Size.y);
			break;
		case BOTTOMCENTER:
			pos = ImVec2(ImGui::GetMainViewport()->Size.x*0.5f, ImGui::GetMainViewport()->Size.y);
			break;
		case BOTTOMRIGHT:
			pos = ImVec2(ImGui::GetMainViewport()->Size.x, ImGui::GetMainViewport()->Size.y);
			break;
		case MOUSECURSOR:
			pos = ImGui::GetMousePos();
			break;
		}

		return pos + offset;
	}
};
