#ifndef _GRID_H_
#define _GRID_H_


//Create grid to label objects and maze walls
bk_grid[19][10]; 

#define MAXLEVELS 4

int gp = 0;
short (*grid_pts)[4];

int target_x = 0;
int target_y = 0;
secret = 0;  
secretFlag = 0; 
int (*sp)[2]; 


int target_x1 = 18; 
int target_y1 = 9;
int gp1 = 34;
int secret1 = 0;
int secretPass1[2][2] = {{0,0},{0,0}}; 
uint32_t bkColor1 = 0xEEFF00;
uint32_t gridColor1 =  0xAA0000FF;
short grid_pts1[34][4] = { {9, 13, 0, 0 },
											 {1, 7, 1, 1 },
											 {13, 13, 1, 1 },
											 {15, 15, 1, 1 },
											 {4, 4, 2, 2 },
											 {11, 13, 2, 2 },
											 {15, 15, 2, 2 },
											 {1, 2, 3, 3 },
											 {4, 4, 3, 3 },
											 {6, 9, 3, 3 },
											 {15, 15, 3, 3 },
											 {17, 18, 3, 3 },
											 {0, 0, 4, 4 },
											 {2, 2, 4, 4 },
											 {4, 4, 4, 4 },
											 {7, 7, 4, 4 },
											 {9, 15, 4, 4 },
											 {2, 2, 5, 5 },
											 {4, 4, 5, 5 },
											 {16, 17, 5, 5 },
											 {1, 1, 6, 6 },
											 {4, 10, 6, 6 },
											 {12, 12, 6, 6 },
											 {14, 14, 6, 6 },
											 {0, 2, 7, 7 },
											 {12, 12, 7, 7 },
											 {14, 17, 7, 7 },
											 {2, 2, 8, 8 },
											 {5, 5, 8, 8 },
											 {7, 12, 8, 8 },
											 {14, 14, 8, 8 },
											 {4, 5, 9, 9 },
											 {12, 14, 9, 9 },
											 {17, 17, 9, 9 },
											 };

// Level 2: Christopher's grid
int target_x2 = 16; 
int target_y2 = 9;
int gp2 = 33;
int secret2 = 0; 
int secretPass2[2][2] = {{0,0},{0,0}};  
uint32_t bkColor2 = 0xffe3ffff;
uint32_t gridColor2 = 0x50237eff;
short grid_pts2[33][4] = { {8,18,0,0},
													{1,6,1,1,},
													{18,18,1,1,},
													{1,1,2,2,},
													{6,12,2,2},
													{14,18,2,2},
													{1,2,3,3},
													{4,4,3,3},
													{6,6,3,3},
													{10,11,3,3},
													{2,2,4,4},
													{4,4,4,4},
													{6,6,4,4},
													{8,8,4,4},
													{10,11,4,4},
													{13,17,4,4},
													{0,0,5,5},
													{2,2,5,5},
													{4,4,5,5},
													{13,13,5,5},
													{17,17,5,5},
													{0,0,6,6},
													{2,2,6,6},
													{4,9,6,6},
													{11,13,6,6},
													{15,15,6,6},
													{17,17,6,6},
													{2,2,7,7},
													{15,15,7,7},
													{17,17,7,7},
													{1,13,8,8},
													{15,15,9,9},
													{17,18,9,9},
													};

// Level 3: Emilie's Grid
int target_x3 = 15; 
int target_y3 = 9;
int gp3 = 35;
int secret3 = 1; //(4,2), (17,2)
int secretPass3[2][2] = {{4,2},{17,2}};
uint32_t bkColor3 = 0x8eb3ffff;
uint32_t gridColor3 = 0x0001ccff; 
short grid_pts3[35][4] = { {1,1,0,0},
													{3,3,0,0},
													{1,1,1,1},
													{3,3,1,1},
													{6,6,1,1},
													{8,10,1,1},
													{12,12,1,1},
													{15,15,1,1},
													{1,1,2,2},
													{3,3,2,2},
													{6,6,2,2},
													{10,10,2,2},
													{12,12,2,2},
													{15,16,2,2},
													{1,1,3,3},
													{3,6,3,3},
													{10,12,3,3},
													{16,18,3,3},
													{1,1,4,4},
													{10,10,4,4},
													{15,16,4,4},
													{1,1,5,5},
													{8,8,5,5},
													{10,10,5,5},
													{15,16,5,5},
													{3,4,6,6},
													{6,8,6,6},
													{10,13,6,6},
													{15,16,6,6},
													{4,4,7,7},
													{6,6,7,7},
													{12,12,7,7},
													{16,17,7,7},
													{1,14,8,8},
													{14,14,9,9},
													};

// Level 4: Sydney's grid
int target_x4 = 18; 
int target_y4 = 9;
int gp4 = 32;
int secret4 = 1;  
int secretPass4[2][2] = {{2,4},{8,2}};
uint32_t bkColor4 = 0xfefb84ff;
uint32_t gridColor4 = 0x115823ff; 
short grid_pts4[32][4] = { {1,9,0,0},
													 {14,15,0,0},
													 {7,7,1,1},
													 {9,9,1,1},
													 {11,12,1,1},
													 {14,15,1,1},
													 {17,17,1,1},
													 {1,5,2,2},
													 {7,7,2,2},
													 {9,9,2,2},
													 {11,12,2,2},
													 {14,15,2,2},
													 {17,17,2,2},
													 {1,1,3,3},
													 {5,5,3,3},
													 {7,9,3,3},
													 {11,12,3,3},
													 {14,15,3,3},
													 {17,17,3,3},
													 {1,1,4,4},
													 {3,3,4,4},
													 {11,12,4,4},
													 {17,17,4,4},
													 {0,17,5,5},
													 {1,3,7,7},
													 {5,10,7,7},
													 {12,13,7,7},
													 {1,1,8,8},
													 {3,7,8,8},
													 {9,13,8,8},
													 {15,17,8,8},
													 {15,17,9,9},
													};


// resets bk_grid to 1 before grid maze is added
void resetBkGrid(){
	for( int i = 0; i < 19; i++){
		for( int j = 0; j < 10; j++){
			bk_grid[i][j] = 1; 
		}
	}
}


// creates maze block coordinates according to grid_pts[][]
// sets maze blocks = 0, open areas = 1, target = 2, secret passage = 3
void setUpGridBlocks(short (*pts)[4], int gp_, int tx, int ty, secret_){
	int x0, x1, y0, y1;
	//reinit bk_grid t0 1's
	resetBkGrid();

	for (int i = 0; i < gp_; i++){
		x0 = pts[i][0]; x1 = pts[i][1]; y0 = pts[i][2]; y1 = pts[i][3];
		// fill grid
		for(int y = y0; y<= y1; y++){
			for(int x = x0; x<=x1; x++){
				bk_grid[x][y] = 0;
			}
		}
	bk_grid[tx][ty] = 2;

	} 
	
	//check for secret passage and add if necessary
	if(secret_){
		int sx0 = sp[0][0];
		int sy0 = sp[0][1]; 
		int sx1 = sp[1][0];
		int sy1 = sp[1][1]; 

		bk_grid[sx0][sy0] = 3; 
		bk_grid[sx1][sy1] = 3; 

	}

}


// draws the maze of grids according to grid_pts[][]
int drawMazeGrid(short (*pts)[4], int gp_){
	int x0, y0, x1, y1;
	int y = 0; 
	//set color to white
	CNFGColor(0x505050);
	int w = 100;
	for(int x = 0; x < gp_; x++){
		int y = 0; 
		x0 = pts[x][y++];
		x0 = x0*w;
		x1 = pts[x][y++];
		x1 = (x1*w)+w;
		y0 = pts[x][y++];
		y0 = y0*w; 
		y1 = pts[x][y++];
		y1 = (y1*w) + w;
		CNFGTackRectangle(x0, y0, x1, y1);
		//printf("%i %i %i %i \n", x0, x1, y0, y1);
	}
}
	

#endif
