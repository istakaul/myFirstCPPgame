#pragma once


struct EntityAnimation {
	
	int positionX = 0; // that is the frame number. it represent the column in the spritesheet. That is the first frame
	int positionY = 0; // this will play the default animation (it the first row of the spritesheet). 1 is the second row, 2 the third etc.

	float timer = 0;

	void update(float deltaTime, float frameDuration, int framesCount) {
		
		timer -= deltaTime;

		if (timer <= 0) { // when the timer reaches 0 or less
			timer += frameDuration; // set it equal to frameDuration
			positionX++; // and move to the next frame.
		}

		positionX %= framesCount; // make sure this is the right frame. 1%4 = 1, 2%4=2, 3%4=3, 4%4=0
	}

	// If we change the animation (the positionY), start the new animation from the beginning (positionX = 0)
	void setAnimation(int animation) {
		if (positionY != animation) {
			positionX = 0;
			positionY = animation;
		}
	}
};