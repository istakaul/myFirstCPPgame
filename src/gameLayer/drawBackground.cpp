#include "drawBackground.h"
#include <assetManager.h>
#include <raymath.h>
#include <algorithm>

void DrawBackground::draw(float deltaTime, AssetManager &assetManager, Camera2D camera, Vector2 mapSize)
{
	auto drawOneBackground = [&](float parallax, float opacity) {
		Texture bg = assetManager.forestBG;

		int screenW = GetScreenWidth();
		int screenH = GetScreenHeight();

		float aspectRatio = (float)bg.width / (float)bg.height;
		float bgScaleScreen = 2.0f;

		float base = std::max(screenW, screenH) * bgScaleScreen;

		// scale background so that it always covers the screen while keeping aspect ratio
		float bgW = base;
		float bgH = base;

		if (aspectRatio > 1.0f) {
			// bgW is wider, scale height accordingly
			bgH = bgW / aspectRatio;
		}
		else {
			// bgH is taller, scale width accordingly
			bgW = bgH * aspectRatio;
		}

		// compute camera ranges
		const float halfViewW = (screenW * 0.5f) / camera.zoom;
		const float halfViewH = (screenH * 0.5f) / camera.zoom;

		const float camMinX = halfViewW;
		const float camMaxX = mapSize.x - halfViewW;
		const float camMinY = halfViewH;
		const float camMaxY = mapSize.y - halfViewH;

		// restrict the camera in between these limits
		Vector2 camPos = camera.target;
		camPos.x = Clamp(camPos.x, camMinX, camMaxX);
		camPos.y = Clamp(camPos.y, camMinY, camMaxY);

		// where the camera is in this range?
		const float camRangeX = std::max(0.0f, camMaxX - camMinX);
		const float camRangeY = std::max(0.0f, camMaxY - camMinY);

		const float normX = camRangeX > 0.0f ? (camPos.x - camMinX) / camRangeX : 0.0f;
		const float normY = camRangeY > 0.0f ? (camPos.y - camMinY) / camRangeY : 0.0f;

		// how far the background can move inside the screen (in screen space)
		const float maxOffX = bgW - screenW;
		const float maxOffY = bgH - screenH;

		const float offX = -maxOffX * normX * parallax;
		const float offY = -maxOffY * normY * parallax;

		Rectangle src = { 0,0,(float)bg.width, (float)bg.height };
		Rectangle dest = { offX, offY, bgW, bgH };

		DrawTexturePro(bg, src, dest, { 0,0 }, 0.0f, { 255,255,255,(unsigned char)(255 * opacity) });
	};

	drawOneBackground(0.3, 1);
}

