#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "myLib.h"
#include "font.h"
#include "bulldog.h"
#include "gameover.h"
#include "win.h"
#include "cover.h"
#include "buzz.h"


int main() {
	REG_DISPCNT = MODE3 | BG2_ENABLE;

	ATTACKER attackers[numShip];

	for (int i = 0; i < numShip; i++) {
		attackers[i].row = 10;
		attackers[i].col = 10 + 15 * i;
		attackers[i].size = BULLDOG_HEIGHT;
		attackers[i].isAlive = true;
		attackers[i].isMove = true;
		if ((i % 2) == 0) {
			attackers[i].ATTACKTYPE = SHOOT;
		}
		else {
			attackers[i].ATTACKTYPE = RUSH;
		}
	}

	PLAYER player;

	player.row = 140;
	player.col = 120;
	player.size = BUZZ_HEIGHT;
	player.lives = 5;

	MISSILE missiles[ammo];

	int ammoSize = 2;

	for (int i=0; i < ammo; i++) {
    	missiles[i].row = player.row;
    	missiles[i].col = player.col;
    	missiles[i].isFired = false;
    	missiles[i].isAlive = false;
  	}

  	//game state machine
	int gameState = 1;
	bool isCleared = false;
	int attackersLeft = 14;
	while(1) { 
		if (gameState == 1) {
			if (isCleared == false) {
				clearScreen();
				isCleared = true;
			}
			drawImage3(0,0,240,160, cover);
			if (KEY_DOWN_NOW(BUTTON_START)) {
				clearScreen();
				gameState = 2;

				for (int i = 0; i < numShip; i++) {
					attackers[i].row = 10;
					attackers[i].col = 10 + 15 * i;
					attackers[i].size = BULLDOG_HEIGHT;
					attackers[i].isAlive = true;
					attackers[i].isMove = true;
					if ((i % 2) == 0) {
						attackers[i].ATTACKTYPE = SHOOT;
					}
					else {
						attackers[i].ATTACKTYPE = RUSH;
					}
				}
				player.lives = 5;
				player.row = 140;
				player.col = 120;
				attackersLeft = 14;

				for (int i=0; i<ammo; i++) {
    				missiles[i].row = player.row;
    				missiles[i].col = player.col;
    				missiles[i].isFired = false;
    				missiles[i].isAlive = false;
  				}
			}
		}
		if (KEY_DOWN_NOW(BUTTON_SELECT)) {
			gameState = 1;
			isCleared = false;
		}

		if (gameState == 2) {

			char playerLives[41];
			drawRectangle3(0, 110, 10, 10, BLACK);
    		sprintf(playerLives, "Lives: %d", player.lives);
    		drawString(0, 70, playerLives, WHITE);

			
    		//drawRectangle3(0, 200, 30, 10, BLACK);
    		//sprintf(buffer, "ATTACKERS: %d", attackersLeft);
    		drawString(0, 125, "Shoot the Bulldogs", GREEN);

    		drawString(0,0, "FIRST WAVE ", BLUE);

			if (player.lives == 0) {
				gameState = 3;
			}
			if (attackersLeft == 0) {
				gameState = 4;
			}
			int oldrow = player.row;
			int oldcol = player.col;


			if (KEY_DOWN_NOW(BUTTON_A)) {
				for (int i = 0; i < ammo; i ++) {
					if (missiles[i].isFired == false) {
						missiles[i].isFired = true;
						missiles[i].isAlive = true;
    					missiles[i].row = player.row;
    					missiles[i].col = player.col + player.size / 3;
    					i = ammo;
					}
				}
			}

			if(KEY_DOWN_NOW(BUTTON_LEFT)) {
				player.col -= 2;
				if(player.col < 0) {
					player.col = 0;
				}
			}
			if(KEY_DOWN_NOW(BUTTON_RIGHT)) {
				player.col += 2;
				if(player.col>220) {
					player.col = 220;
				}
			}
			if(KEY_DOWN_NOW(BUTTON_UP)) {
				player.row -= 2;
				if(player.row < 20) {
					player.row = 20;
				}
			}
			if(KEY_DOWN_NOW(BUTTON_DOWN)) {
				player.row += 2;
				if(player.row > 140) {
					player.row = 140;
				}
			}

			for (int i = 0; i < numShip; i++) {		
				if (!attackers[i].isAlive) {
					continue;
				}		
				if ((attackers[i].ATTACKTYPE == RUSH) && (rand() % 2 == 0)) {
					if (attackers[i].row >= 140 || attackers[i].row <= 8) {
						attackers[i].isMove = !(attackers[i].isMove);
					}
					int oldBULLDOGRow = attackers[i].row;
					if (attackers[i].isMove) {
						attackers[i].row++;
						drawRectangle3(oldBULLDOGRow, attackers[i].col, attackers[i].size, attackers[i].size, BLACK);
						drawImage3(attackers[i].row, attackers[i].col, BULLDOG_WIDTH, BULLDOG_HEIGHT, bulldog);
					}
					else {
						attackers[i].row--;
						drawRectangle3(oldBULLDOGRow, attackers[i].col, attackers[i].size, attackers[i].size, BLACK);
						drawImage3(attackers[i].row, attackers[i].col, BULLDOG_HEIGHT, BULLDOG_HEIGHT, bulldog);
					}
				}

				if (((attackers[i].row) <= (player.row + player.size)) && (attackers[i].row + attackers[i].size >= player.row) 
					&& (attackers[i].col <= (player.col + player.size)) && (attackers[i].col + attackers[i].size >= player.col)) {
					drawRectangle3(attackers[i].row, attackers[i].col, attackers[i].size, attackers[i].size, BLACK);
					if (attackers[i].isAlive) {
						attackers[i].isAlive = false;
						player.lives--;
						attackersLeft--;
					}
				}

				for (int j = 0; j < ammo; j++) {
					if (((attackers[i].row) <= (missiles[j].row + ammoSize)) && (attackers[i].row + attackers[i].size >= missiles[j].row) 
						&& (attackers[i].col <= (missiles[j].col + ammoSize)) && (attackers[i].col + attackers[i].size >= missiles[j].col)) {
						drawRectangle3(attackers[i].row, attackers[i].col, attackers[i].size, attackers[i].size, BLACK);
						drawRectangle3(missiles[i].row, missiles[i].col, ammoSize*2, ammoSize*2, BLACK);
						missiles[j].isAlive = false;
						if (attackers[i].isAlive && missiles[j].isFired) {
							attackersLeft -= 1;
							attackers[i].isAlive = false;
						}
					}	
				}
			}

			for (int i = 0; i < ammo; i++) {
				if (missiles[i].row < -10) {
					missiles[i].isAlive = false;
					missiles[i].isFired = false;
			}
				if (missiles[i].isFired && missiles[i].isAlive) {
					drawRectangle3(missiles[i].row, missiles[i].col, 5, 5, BLACK);
		    		missiles[i].row--;
		    		drawRectangle3(missiles[i].row, missiles[i].col, ammoSize, ammoSize, RED);
	    		}
  			}

			waitForVblank();

			for (int i = 0; i < numShip; i++) {
				if (attackers[i].isAlive) {
					drawImage3(attackers[i].row, attackers[i].col, BULLDOG_HEIGHT, BULLDOG_HEIGHT, bulldog);
				}
			}
			drawRectangle3(oldrow, oldcol, player.size, player.size, BLACK);
			drawImage3(player.row, player.col, player.size, player.size, buzz);
		}

		if (gameState == 3) {
			if (isCleared == false) {
				clearScreen();
				isCleared = true;
			}
			drawImage3(0,0,240,160, gameover);
		}

		if (gameState == 4) {
			if (isCleared == false) {
				clearScreen();
				isCleared = true;
			}
			drawImage3(0,0,240,160, win);
		}
	}
}