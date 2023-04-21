//Copyright (c) 2011-2020 <>< Charles Lohr - Under the MIT/x11 or NewBSD License you choose.
// NO WARRANTY! NO GUARANTEE OF SUPPORT! USE AT YOUR OWN RISK

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "os_generic.h"
#include <GLES3/gl3.h>
#include <asset_manager.h>
#include <asset_manager_jni.h>
#include <android_native_app_glue.h>
#include <android/sensor.h>
#include "CNFGAndroid.h"

#define CNFG_IMPLEMENTATION
#define CNFG3D
#include "CNFG.h"
#include "grid.h" // grid coordinate for 
#include "image_data.h" // file with image in array image_data[100*100] bh

ASensorManager * sm;
const ASensor * as;
ASensorEventQueue* aeq;
ALooper * l;

int level = 1;
int prev_level = 0;
int level_change = 0; 
int levelMsgCounter = 0; 
int img_w=100; 
int img_h=100;
int targetFound = 0; 
char dsp_msg[25];

//image coordinate variables bh
int imgLocation[2]={0,0};
int imgDelta[2]={0,0};


struct obj{
	// curr grid location of bitmap
	int grid_x;
	int grid_y;
	// false boolean to determin if bitmap can move in 4-directions
	int moveLt;
	int moveRt;
	int moveUp; 
	int moveDwn;
} img;



void moveBitMap(struct obj* img, int* delta ){
	// get current loca
	int gx = img->grid_x;
	int gy = img->grid_y;
	// get direction.
	static int frame = 0; 
	static int index = 0; 
	const int indexMax = 10;
	static int dx[indexMax] = {0};
	static int dy[indexMax] = {0};
	int mod=10; 

	frame++; 
	dx[index]=delta[0]; 
	dy[index++]=delta[1];
	if(index >4){index = 0;}
	int avg_dx = 0;
	int avg_dy = 0; 
	if( (frame % mod) == 0){
		for( int i = 0; i< indexMax; i++){
			avg_dx += dx[i]; 
			avg_dy += dy[i];
		}
		avg_dx = avg_dx/indexMax;
		avg_dy = avg_dy/indexMax;
	}
	// check if can move in x- and y- direction
	if (gx == 0){		//bitmap at left edge chech right side
		img->moveLt = 0;
		if(bk_grid[gx+1][gy] == 0){
			img->moveRt = 0;
		}else{
			img->moveRt = 1;
		}
	}else if(gx == 18){	// bitmap at right edge, check left side 
		img->moveRt = 0;
		if(bk_grid[gx-1][gy] == 0){
			img->moveLt = 0;
		}else{
			img->moveLt = 1;
		}
	}else{
		// check grid left and right for blocks in the way
		if(bk_grid[gx-1][gy] == 0){img->moveLt = 0;}
		else{img->moveLt = 1;}
		if(bk_grid[gx+1][gy] == 0){img->moveRt = 0;}
		else{img->moveRt = 1;}
	}

	if(gy == 0){	// bitmap at top of screen, check below bitmap
		img->moveUp = 0; 
		if(bk_grid[gx][gy+1] == 0){
			img->moveDwn = 0;
		}else{
			img->moveDwn = 1;
		}
	}
	else if (gy == 9){	// bitmap at bottom, check above
		img->moveDwn = 0; 
		if(bk_grid[gx][gy-1] == 0){
			img->moveUp = 0;
		}else{img->moveUp =1;
		}
	}
	else{	// check above and below
		if(bk_grid[gx][gy-1] == 0){img->moveUp = 0;}
		else{img->moveUp =1;}
		if(bk_grid[gx][gy+1] == 0){img->moveDwn = 0;}
		else{img->moveDwn = 1;}
	}	
	
	// move over 1 grid block
	if( (avg_dx < 0) && (img->moveLt == 1) ){gx--;}
	else if ( (avg_dx > 1) && (img->moveRt == 1) ){gx++; }

	if( (avg_dy < 0) &&  (img->moveUp ==1) ) {gy--;}
	else if( (avg_dy > 0) && (img->moveDwn == 1) ){gy++;}
	
	img->grid_x = gx; 
	img->grid_y = gy; 
}

void display_level_Complete(){
			int x0 = 100; 
			int y0 = 395;
			int msgSize = 30;
			int w = 0; 			
			int h =0; 
			sprintf(dsp_msg, "Level %d Completed! ", level-1);
			CNFGDialogColor = 0x000000ff;			
			CNFGGetTextExtents( dsp_msg, &w, &h, msgSize  );
			CNFGDrawBox(x0, y0, x0 + w, y0 + h); //1250, 910);
			CNFGPenX = x0 + msgSize;
			CNFGPenY = y0 + msgSize;
			CNFGColor( 0xffffffff );
			CNFGDrawText(dsp_msg, msgSize);
}


void SetupIMU()
{
	sm = ASensorManager_getInstance();
	as = ASensorManager_getDefaultSensor( sm, ASENSOR_TYPE_ACCELEROMETER); //ASENSOR_TYPE_GYROSCOPE );
	l = ALooper_prepare( ALOOPER_PREPARE_ALLOW_NON_CALLBACKS );
	aeq = ASensorManager_createEventQueue( sm, (ALooper*)&l, 0, 0, 0 ); //XXX??!?! This looks wrong.
	ASensorEventQueue_enableSensor( aeq, as);
	printf( "setEvent Rate: %d\n", ASensorEventQueue_setEventRate( aeq, as, 10000 ) );
}

float accx, accy, accz;
int accs;

void AccCheck()
{
	ASensorEvent evt;
	do
	{
		ssize_t s = ASensorEventQueue_getEvents( aeq, &evt, 1 );
		if( s <= 0 ) break;
		accx = evt.vector.v[0];
		accy = evt.vector.v[1];
		accz = evt.vector.v[2];
		accs++;
	} while( 1 );
}

void AndroidDisplayKeyboard(int pShow);

int lastbuttonx = 0;
int lastbuttony = 0;
int lastmotionx = 0;
int lastmotiony = 0;
int lastbid = 0;
int lastmask = 0;
int lastkey, lastkeydown;

static int keyboard_up;

void HandleKey( int keycode, int bDown )
{
	lastkey = keycode;
	lastkeydown = bDown;
	if( keycode == 10 && !bDown ) { keyboard_up = 0; AndroidDisplayKeyboard( keyboard_up );  }

	if( keycode == 4 ) { AndroidSendToBack( 1 ); } //Handle Physical Back Button.
}

void HandleButton( int x, int y, int button, int bDown )
{
	lastbid = button;
	lastbuttonx = x;
	lastbuttony = y;

	if( bDown ) { keyboard_up = !keyboard_up; AndroidDisplayKeyboard( keyboard_up ); }
}

void HandleMotion( int x, int y, int mask )
{
	lastmask = mask;
	lastmotionx = x;
	lastmotiony = y;
}

#define HMX 162
#define HMY 162
short screenx, screeny;


extern struct android_app * gapp;


void HandleDestroy()
{
	printf( "Destroying\n" );
	exit(10);
}

volatile int suspended;

void HandleSuspend()
{
	suspended = 1;
}

void HandleResume()
{
	suspended = 0;
}

int main()
{
	int x, y;
	double ThisTime;
	double LastFPSTime = OGGetAbsoluteTime();
	int linesegs = 0;
 

	// initialize img bitmap
	img.grid_x= 0;
	img.grid_y= 0;
	// false boolean to determin if bitmap can move in 4-directions
	img.moveLt=1;
	img.moveRt=1;
	img.moveUp=1; 
	img.moveDwn=1;

	//grid_pts = grid_pts2;
	//gp = gp2;

	//create grid objects bh
	//setUpGridBlocks(grid_pts, gp);


	CNFGBGColor = 0xEEFF00;
	CNFGDialogColor = 0xE0E0FF;
	CNFGSetupFullscreen( "Test Bench", 0 );
	//CNFGSetup( "Test Bench", 0, 0 );


	const char * assettext = "Not Found";
	AAsset * file = AAssetManager_open( gapp->activity->assetManager, "asset.txt", AASSET_MODE_BUFFER );
	if( file )
	{
		size_t fileLength = AAsset_getLength(file);
		char * temp = malloc( fileLength + 1);
		memcpy( temp, AAsset_getBuffer( file ), fileLength );
		temp[fileLength] = 0;
		assettext = temp;
	}
	SetupIMU();


	while(1)
	{

		CNFGHandleInput();
		AccCheck();

		if( suspended ) { usleep(50000); continue; }

		CNFGClearFrame();
		CNFGColor( 0xFFFFFFFF );
		CNFGGetDimensions( &screenx, &screeny );


		int accx_, accy_, accz_, shift_val;
		shift_val = 0;
		accx_ = (int)(accx) << shift_val; accy_ = (int)(accy) << shift_val; accz_ = (int)(accz) << shift_val;

		
		// Update obj location 
		// check if obj takes a secret passage
		if(secret){
			int locx = img.grid_x; 
			int locy = img.grid_y; 
			if ( bk_grid[locx][locy] == 3){
				if(secretFlag == 0){
					if( (sp[0][0] == locx) && ( sp[0][1] == locy ) ){
						locx = sp[1][0]; 
						locy = sp[1][1];
						img.grid_x = locx; 
						img.grid_y = locy; 
					}else{
						locx = sp[0][0]; 
						locy = sp[0][1];
						img.grid_x = locx; 
						img.grid_y = locy; 
					}
					secretFlag = 1; // set to true (obj moved thru passage) so obj isn't moved from loc to loc after every screen update
				}
			}else if (secretFlag == 1){
				secretFlag = 0; 
			}
		}


		//check if previous move was to target location
		if ( bk_grid[img.grid_x][img.grid_y] == 2){
			targetFound = 1; 
			level++;
			if (level > MAXLEVELS){ level= MAXLEVELS;} 
			// reset image start location
			img.grid_x = 0; 
			img.grid_y = 0; 
		}


		//assign grid pointer to respective grid based on current level
		if (prev_level != level){
			level_change = 1; 
			levelMsgCounter = 0; 
			if(level == 1){ 
				grid_pts = grid_pts1;
				gp = gp1;
				target_x = target_x1;
				target_y = target_y1; 
				secret = secret1; 
				if(secret){ sp = secretPass1;};
				CNFGBGColor =bkColor1; //0xEEFF00;
				CNFGColor( gridColor1); 	
			}else if(level == 2){ 
				grid_pts = grid_pts2;
				gp = gp2;
				target_x = target_x2;
				target_y = target_y2;
				secret = secret2; 
				if(secret){ sp = secretPass2;};
				CNFGBGColor = bkColor2;
				CNFGColor( gridColor2); 
			}else if(level == 3){
				grid_pts = grid_pts3;
				gp = gp3;
				target_x = target_x3;
				target_y = target_y3;
				secret = secret3; 
				if(secret){ sp = secretPass3;};
				CNFGBGColor = bkColor3;
				CNFGColor( gridColor3); 
			} else{ // (level == 4){
				grid_pts = grid_pts4;
				gp = gp4;
				target_x = target_x4;
				target_y = target_y4;
				secret = secret4; 
				if(secret){ sp = secretPass4;};
				CNFGBGColor = bkColor4;
				CNFGColor( gridColor4); 
			} 

			setUpGridBlocks(grid_pts, gp, target_x, target_y, secret);
			prev_level = level; 
			targetFound = 0;
			

		}

	
		// Create block
		//CNFGColor( 0xAA0000FF); 
		drawMazeGrid(grid_pts, gp);
		int tx = target_x*100; 
		int ty = target_y * 100; 
		CNFGBlitImage(image_data,tx, ty, img_w, img_h);

		//fill in bottom border andright border pixels with black
		CNFGColor(0x000000FF);
		CNFGTackRectangle(0, 1000, screenx,screeny);
		CNFGTackRectangle(1900, 0, screenx,screeny);

		// moving bitmap image around screen - bh 
		// B/c screen is in Landscape mode, flip x- and y- axes
		imgDelta[0]= accy_;
		imgDelta[1]= accx_; 
		//move_box(imgLocation, imgDelta, screenx, screeny);
		
		if( (level_change > 0) && (level !=1) ){
			levelMsgCounter++; 
			/*
			int x0 = 100; 
			int y0 = 395;
			int msgSize = 30;
			int w = 0; 			
			int h =0; 
			sprintf(dsp_msg, "Level %d Completed! ", level);
			CNFGDialogColor = 0x000000ff;			
			CNFGGetTextExtents( dsp_msg, &w, &h, msgSize  );
			CNFGDrawBox(x0, y0, x0 + w, y0 + h); //1250, 910);
			CNFGPenX = x0 + msgSize;
			CNFGPenY = y0 + msgSize;
			CNFGColor( 0xffffffff );
			CNFGDrawText(dsp_msg, msgSize);
			*/
			display_level_Complete();
			//CNFGDrawTextBox(225, 400,dsp_msg, 20); 
			if(levelMsgCounter > 150){
				level_change = 0; 
			}
		}else{
			moveBitMap(&img, imgDelta );
		}
		int imgX = img.grid_x *100;
		int imgY = img.grid_y * 100;
		//CNFGUpdateScreenWithBitmap( image_data, img_w, img_h); //bh
		CNFGBlitImage( target_img, imgX, imgY, img_w, img_h); //bh
		

		// print level number on screen
		char str[50];
		CNFGColor( 0xffffffff );
		CNFGPenX = 1950; CNFGPenY = 100;
		sprintf(str, "LEVEL: %d\n", level);
		CNFGDrawText(str, 10);   


		//On Android, CNFGSwapBuffers must be called, and CNFGUpdateScreenWithBitmap does not have an implied framebuffer swap.
		CNFGSwapBuffers();

		ThisTime = OGGetAbsoluteTime();
		if( ThisTime > LastFPSTime + 1 )
		{
			
			linesegs = 0;
			LastFPSTime+=1;

		}

	}

	return(0);
}

