#include <iostream>
#include <raylib.h>
#include <imgui.h>
#include <rlImGui.h>
#include <../ImGui_Theme/BlackDevil.h>
#include <gameMain.h>


int main()
{

#if PRODUCTION_BUILD == 1
	SetTraceLogLevel(LOG_NONE);
#endif

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 450, "window name");
	SetExitKey(KEY_NULL); // Disable Esc from closing window
	SetTargetFPS(240);

#pragma region imgui
	rlImGuiSetup(true);

	ImGuiIO& io = ImGui::GetIO();
	// Enabling windows docking into one
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	// Scale the output of ImGui
	io.FontGlobalScale = 2;
#pragma endregion

	if (!initGame()) {
		return 0;
	}

	while (!WindowShouldClose()) { // If I don't press the x button on the window nor the escape key (because WindowShouldClose will be true if one of these happens)
		BeginDrawing();
		ClearBackground(RAYWHITE);

	#pragma region imgui
		rlImGuiBegin();

		// Apply Black Devil style
		SetupImGuiStyle();

		// Dock to main game screen / viewport, but before that make it transparent
		// so that it doesn't cover with gray background the whole screen
		ImGui::PushStyleColor(ImGuiCol_WindowBg, {});
		ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, {});
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		ImGui::PopStyleColor(2);
	#pragma endregion

	#pragma region imgui windows

	#pragma region imgui MainMenuBar
		ImGui::BeginMainMenuBar();
		if (ImGui::BeginMenu("File")) {
			ImGui::MenuItem("(demo menu)", NULL, false, false);
			if (ImGui::MenuItem("New")) {}
			if (ImGui::MenuItem("Open", "Ctrl+O")) {}
			if (ImGui::BeginMenu("Open Recent")) {
				ImGui::MenuItem("fish_hat.c");
				ImGui::MenuItem("fish_hat.inl");
				ImGui::MenuItem("fish_hat.h");
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	#pragma endregion

	#pragma region imgui list of checkboxes
		ImGui::Begin("CheckBoxes!");
		ImGui::BeginListBox("MyCheckBoxList");

		static bool check1 = true;
		static bool check2 = false;
		ImGui::Checkbox("First Check", &check1);
		ImGui::Checkbox("Second Check", &check2);

		ImGui::EndListBox();

		ImGui::End();
	#pragma endregion

	#pragma region imgui Tooltip on hover
		ImGui::Begin("Tooltip");
		ImGui::Text("Speed");
		ImGui::SameLine();
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::Text("Controls how fast the player moves.");
			ImGui::EndTooltip();
		}
		ImGui::End();
	#pragma endregion

		ImGui::Begin("test");

		ImGui::Text("hello");
		//ImGui::Button("button");

		if (ImGui::Button("button")) {
			std::cout << "Text\n";
		}
		ImGui::SameLine();
		if (ImGui::Button("Button2")) {
			std::cout << "Second button \n";
		}

		ImGui::End();

		ImGui::Begin("second window");

		ImGui::Text("hello");
		ImGui::Separator();
		ImGui::NewLine();
		static float a = 0;
		ImGui::SliderFloat("slider", &a, 0, 1);

		ImGui::End();
		//ImGui::ShowDemoWindow();
	#pragma endregion

		if (!updateGame()) {
			CloseWindow();
		}

	#pragma region imgui
		rlImGuiEnd();
	#pragma endregion

		EndDrawing();
	}

	CloseWindow(); // this will force the operating system to clear any stuff from memory related (like the openGL context)

	closeGame();

#pragma region imgui
	rlImGuiShutdown();
#pragma endregion

	return 0;
}