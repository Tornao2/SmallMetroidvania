#include "objects.h"

Items::Items(int lx, int ly, int lwidth, int lheight, int lrotation, ItemLabels llabel) {
	x = lx;
	y = ly;
	width = lwidth;
	height = lheight;
	rotation = lrotation;
	itemLabel = llabel;
	label = items;
}

Items::Items() {
	label = items;
	itemLabel = none;
}

BackgroundWall::BackgroundWall(int lx, int ly, int lwidth, int lheight, int lrotation) {
	x = lx;
	y = ly;
	width = lwidth;
	height = lheight;
	rotation = lrotation;
	label = wall;
}

DamageZone::DamageZone(int lx, int ly, int lwidth, int lheight, int lrotation) {
	x = lx;
	y = ly;
	width = lwidth;
	height = lheight;
	rotation = lrotation;
	label = damageZone;
}

LaunchPad::LaunchPad(int lx, int ly, int lwidth, int lheight, int lrotation) {
	x = lx;
	y = ly;
	width = lwidth;
	height = lheight;
	rotation = lrotation;
	label = launch;
}

Player::Player() {
	spawnPoint = { -50, -50 };
	x = -50;
	y = -50;
	moveX = 0;
	moveY = 0;
	width = 32;
	height = 32;
	rotation = 0;
	jumped = 0;
	allowedJumps = 1;
	label = player;
	isInAir = false;
	respawning = false;
	respawnTime = 0;
}

void Player::setSpawn(int lx, int ly) {
	spawnPoint.x = (float) lx;
	spawnPoint.y = (float) ly;
}

void Player::move(std::vector <BackgroundWall> WallArr, std::vector <DamageZone> DamageArr, std::vector <LaunchPad> LaunchArr, Items& currentItem, SoundLibrary SL) {
	if (respawning){
		if (respawnTime + 3 < GetTime()) {
			respawning = false;
			respawn();
		}
	}
	else {
		collisionX(WallArr, DamageArr, LaunchArr, currentItem, SL);
		x += moveX;
		collisionY(WallArr, DamageArr, LaunchArr, currentItem, SL);
		y += moveY;
		moveY += 1;
		if (IsKeyDown(KEY_RIGHT) && moveX < 6)
			moveX += 3;
		else if (IsKeyDown(KEY_LEFT) && moveX > -6)
			moveX -= 3;
		else if (moveX > 0)
			moveX -= 1;
		else if (moveX < 0)
			moveX += 1;
		if (IsKeyDown(KEY_R)) {			
			respawning = true;
			respawnTime = GetTime();
			PlaySound(SL.DeathSound);
		}
		if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_SPACE)) && jumped < allowedJumps) {
			if (isInAir && allowedJumps != 1) {
				jumped = 2;
				moveY = -15;
				PlaySound(SL.JumpSound);
			}
			else if (!isInAir) {
				jumped++;
				moveY = -15;
				PlaySound(SL.JumpSound);
			}
		}
	}

}

void Player::respawn() {
	x = spawnPoint.x;
	y = spawnPoint.y;
	moveX = 0;
	moveY = 0;
	jumped = allowedJumps;
	rotation = 0;
}

void Player::collisionX(std::vector <BackgroundWall> WallArr, std::vector <DamageZone> DamageArr, std::vector <LaunchPad> LaunchArr, Items& currentItem, SoundLibrary SL) {
	if (moveX != 0) {
		if (CheckCollisionPointRec({ (float)x, (float)y }, { (float)currentItem.x, (float)currentItem.y, 32, 32 })) {
			currentItem.itemLabel = none;
			allowedJumps = 2;
		}
		for (std::vector <LaunchPad>::iterator it = LaunchArr.begin(); it != LaunchArr.end(); it++) {
			if (CheckCollisionRecs({ (float)it->x, (float)it->y, (float)it->width, (float)it->height }, { (float)x, (float)y + moveX, (float)width, (float)height })) {
				if (it->rotation == 90)
					moveX = -30;
				else if (it->rotation == 270)
					moveX = 30;
				PlaySound(SL.LaunchSound);
			}
		}
		for (std::vector <DamageZone>::iterator it = DamageArr.begin(); it != DamageArr.end(); it++) {
			if (CheckCollisionRecs({ (float)it->x, (float)it->y, (float)it->width, (float)it->height }, { (float)x + moveX, (float)y, (float)width, (float)height })) {
				respawning = true;
				respawnTime = GetTime();
				PlaySound(SL.DeathSound);
			}
		}
		for (std::vector <BackgroundWall>::iterator it = WallArr.begin(); it != WallArr.end(); it++) {
			while (moveX) {
				if (CheckCollisionRecs({ (float)it->x, (float)it->y, (float)it->width, (float)it->height }, { (float)x + moveX, (float)y, (float)width, (float)height })) {
					if (moveX > 0)
						moveX--;
					else if (moveX < 0)
						moveX++;
					else {
						return;
					}
				}
				else
					break;
			}
		}
	}
}

void Player::collisionY(std::vector <BackgroundWall> WallArr, std::vector <DamageZone> DamageArr, std::vector <LaunchPad> LaunchArr, Items& currentItem, SoundLibrary SL) {
	isInAir = true;;
	if (moveY != 0) {
		if (CheckCollisionRecs({ (float)x, (float)y, 32, 32 }, { (float)currentItem.x, (float)currentItem.y, 32, 32 })) {
			currentItem.itemLabel = none;
			allowedJumps = 2;
		}
		for (std::vector <LaunchPad>::iterator it = LaunchArr.begin(); it != LaunchArr.end(); it++) {
			if (CheckCollisionRecs({ (float)it->x, (float)it->y, (float)it->width, (float)it->height }, { (float)x, (float)y + moveY, (float)width, (float)height })) {
				if (it->rotation == 180)
					moveY = -25;
				else if (it->rotation == 90)
					moveX = -30;
				else if (it->rotation == 270)
					moveX = 30;
				PlaySound(SL.LaunchSound);
			}
		}
		for (std::vector <DamageZone>::iterator it = DamageArr.begin(); it != DamageArr.end(); it++) {
			if (CheckCollisionRecs({ (float)it->x, (float)it->y, (float)it->width, (float)it->height }, { (float)x, (float)y + moveY, (float)width, (float)height })) {
				respawning = true;
				respawnTime = GetTime();
				PlaySound(SL.DeathSound);
			}
		}
		for (std::vector <BackgroundWall>::iterator it = WallArr.begin(); it != WallArr.end(); it++) {
			while (moveY) {
				if (CheckCollisionRecs({ (float)it->x, (float)it->y, (float)it->width, (float)it->height }, { (float)x, (float)y + moveY, (float)width, (float)height })) {
					jumped = 0;
					isInAir = false;
					if (moveY > 0)
						moveY--;
					else if (moveY < 0)
						moveY++;
					else {
						return;
					}
				}
				else
					break;
			}
		}
	}
}

GameManager::GameManager() {
	originalH = 720;
	originalW = 1280;
	framerate = 60;
	sceneLabel = title;
	currentItem = Items();
}

void GameManager::InitSounds() {
	SL.JumpSound = LoadSound("Sounds/jump.wav");
	SL.DeathSound = LoadSound("Sounds/death.wav");
	SL.LaunchSound = LoadSound("Sounds/launch.wav");
	SL.SelectSound = LoadSound("Sounds/select.wav");
}

void GameManager::changeScene(EnScene temp) {
	sceneLabel = temp;
}
