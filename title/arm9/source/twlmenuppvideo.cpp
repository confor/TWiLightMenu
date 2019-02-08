#include <nds.h>
#include <fat.h>
#include <stdio.h>

#include "common/gl2d.h"

#include "icon_nds.h"
#include "icon_gba.h"
#include "icon_gb.h"
#include "icon_nes.h"
#include "icon_sms.h"
#include "icon_gg.h"
#include "icon_md.h"
#include "icon_snes.h"

static int ndsTexID;
static int gbaTexID;
static int gbTexID;
static int nesTexID;
static int smsTexID;
static int ggTexID;
static int mdTexID;
static int snesTexID;

static glImage ndsIcon[1];
static glImage gbaIcon[1];
static glImage gbIcon[(32 / 32) * (64 / 32)];
static glImage nesIcon[1];
static glImage smsIcon[1];
static glImage ggIcon[1];
static glImage mdIcon[1];
static glImage snesIcon[1];

extern u16 bmpImageBuffer[256*192];
extern u16 videoImageBuffer[40][256*144];

static char videoFrameFilename[256];

static FILE* videoFrameFile;

//static int currentFrame = 0;
static int frameDelay = 0;
static bool frameDelayEven = true;	// For 24FPS
static int frameDelaySprite = 0;
static bool frameDelaySpriteEven = true;	// For 24FPS
static bool loadFrame = true;
static bool loadFrameSprite = true;

static int zoomingIconXpos[9] = {-32, 128, 256, 256, 128, -32, -128, 128, 256+96};
static int zoomingIconYpos[9] = {0, -32, -64, 192+64, 192+128, 192, -96, -168, -240};

void twlMenuVideo_loadTopGraphics(void) {
	// NDS
	glDeleteTextures(1, &ndsTexID);
	
	ndsTexID =
	glLoadTileSet(ndsIcon, // pointer to glImage array
				32, // sprite width
				32, // sprite height
				32, // bitmap width
				32, // bitmap height
				GL_RGB16, // texture type for glTexImage2D() in videoGL.h
				TEXTURE_SIZE_32, // sizeX for glTexImage2D() in videoGL.h
				TEXTURE_SIZE_32, // sizeY for glTexImage2D() in videoGL.h
				TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
				16, // Length of the palette to use (16 colors)
				(u16*) icon_ndsPal, // Load our 16 color tiles palette
				(u8*) icon_ndsBitmap // image data generated by GRIT
				);

	// GBA
	glDeleteTextures(1, &gbaTexID);
	
	gbaTexID =
	glLoadTileSet(gbaIcon, // pointer to glImage array
				32, // sprite width
				32, // sprite height
				32, // bitmap width
				32, // bitmap height
				GL_RGB16, // texture type for glTexImage2D() in videoGL.h
				TEXTURE_SIZE_32, // sizeX for glTexImage2D() in videoGL.h
				TEXTURE_SIZE_32, // sizeY for glTexImage2D() in videoGL.h
				TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
				16, // Length of the palette to use (16 colors)
				(u16*) icon_gbaPal, // Load our 16 color tiles palette
				(u8*) icon_gbaBitmap // image data generated by GRIT
				);

	// GB/GBC
	glDeleteTextures(1, &gbTexID);
	
	gbTexID =
	glLoadTileSet(gbIcon, // pointer to glImage array
				32, // sprite width
				32, // sprite height
				32, // bitmap image width
				64, // bitmap image height
				GL_RGB16, // texture type for glTexImage2D() in videoGL.h
				TEXTURE_SIZE_32, // sizeX for glTexImage2D() in videoGL.h
				TEXTURE_SIZE_64, // sizeY for glTexImage2D() in videoGL.h
				TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT,
				16, // Length of the palette to use (16 colors)
				(u16*) icon_gbPal, // Image palette
				(u8*) icon_gbBitmap // Raw image data
				);

	// NES
	glDeleteTextures(1, &nesTexID);
	
	nesTexID =
	glLoadTileSet(nesIcon, // pointer to glImage array
				32, // sprite width
				32, // sprite height
				32, // bitmap image width
				32, // bitmap image height
				GL_RGB16, // texture type for glTexImage2D() in videoGL.h
				TEXTURE_SIZE_32, // sizeX for glTexImage2D() in videoGL.h
				TEXTURE_SIZE_32, // sizeY for glTexImage2D() in videoGL.h
				TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT,
				16, // Length of the palette to use (16 colors)
				(u16*) icon_nesPal, // Image palette
				(u8*) icon_nesBitmap // Raw image data
				);

	// SMS
	glDeleteTextures(1, &smsTexID);
	
	smsTexID =
	glLoadTileSet(smsIcon, // pointer to glImage array
				32, // sprite width
				32, // sprite height
				32, // bitmap image width
				32, // bitmap image height
				GL_RGB16, // texture type for glTexImage2D() in videoGL.h
				TEXTURE_SIZE_32, // sizeX for glTexImage2D() in videoGL.h
				TEXTURE_SIZE_32, // sizeY for glTexImage2D() in videoGL.h
				TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT,
				16, // Length of the palette to use (16 colors)
				(u16*) icon_smsPal, // Image palette
				(u8*) icon_smsBitmap // Raw image data
				);

	// GG
	glDeleteTextures(1, &ggTexID);
	
	ggTexID =
	glLoadTileSet(ggIcon, // pointer to glImage array
				32, // sprite width
				32, // sprite height
				32, // bitmap image width
				32, // bitmap image height
				GL_RGB16, // texture type for glTexImage2D() in videoGL.h
				TEXTURE_SIZE_32, // sizeX for glTexImage2D() in videoGL.h
				TEXTURE_SIZE_32, // sizeY for glTexImage2D() in videoGL.h
				TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT,
				16, // Length of the palette to use (16 colors)
				(u16*) icon_ggPal, // Image palette
				(u8*) icon_ggBitmap // Raw image data
				);

	// MD
	glDeleteTextures(1, &mdTexID);
	
	mdTexID =
	glLoadTileSet(mdIcon, // pointer to glImage array
				32, // sprite width
				32, // sprite height
				32, // bitmap image width
				32, // bitmap image height
				GL_RGB16, // texture type for glTexImage2D() in videoGL.h
				TEXTURE_SIZE_32, // sizeX for glTexImage2D() in videoGL.h
				TEXTURE_SIZE_32, // sizeY for glTexImage2D() in videoGL.h
				TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT,
				16, // Length of the palette to use (16 colors)
				(u16*) icon_mdPal, // Image palette
				(u8*) icon_mdBitmap // Raw image data
				);

	// SNES
	glDeleteTextures(1, &snesTexID);
	
	snesTexID =
	glLoadTileSet(snesIcon, // pointer to glImage array
				32, // sprite width
				32, // sprite height
				32, // bitmap image width
				32, // bitmap image height
				GL_RGB16, // texture type for glTexImage2D() in videoGL.h
				TEXTURE_SIZE_32, // sizeX for glTexImage2D() in videoGL.h
				TEXTURE_SIZE_32, // sizeY for glTexImage2D() in videoGL.h
				TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT,
				16, // Length of the palette to use (16 colors)
				(u16*) icon_snesPal, // Image palette
				(u8*) icon_snesBitmap // Raw image data
				);
}

void twlMenuVideo_topGraphicRender(void) {
	glBegin2D();
	{
		if (zoomingIconYpos[0] < 64) {
			glSprite(zoomingIconXpos[0], zoomingIconYpos[0], GL_FLIP_NONE, nesIcon);
		}
		if (zoomingIconYpos[1] < 64) {
			glSprite(zoomingIconXpos[1], zoomingIconYpos[1], GL_FLIP_NONE, smsIcon);
		}
		if (zoomingIconYpos[2] < 64) {
			glSprite(zoomingIconXpos[2], zoomingIconYpos[2], GL_FLIP_NONE, mdIcon);
		}
		if (zoomingIconYpos[3] > 64+32) {
			glSprite(zoomingIconXpos[3], zoomingIconYpos[3], GL_FLIP_NONE, gbIcon);
		}
		if (zoomingIconYpos[4] > 64+32) {
			glSprite(zoomingIconXpos[4], zoomingIconYpos[4], GL_FLIP_NONE, ggIcon);
		}
		if (zoomingIconYpos[5] > 64+32) {
			glSprite(zoomingIconXpos[5], zoomingIconYpos[5], GL_FLIP_NONE, &gbIcon[1]);
		}
		if (zoomingIconYpos[6] < 64) {
			glSprite(zoomingIconXpos[6], zoomingIconYpos[6], GL_FLIP_NONE, snesIcon);
		}
		if (zoomingIconYpos[7] < 64) {
			glSprite(zoomingIconXpos[7], zoomingIconYpos[7], GL_FLIP_NONE, gbaIcon);
		}
		if (zoomingIconYpos[8] < 64) {
			glSprite(zoomingIconXpos[8], zoomingIconYpos[8], GL_FLIP_NONE, ndsIcon);
		}

		glBoxFilled(0, 0, 256, 23, RGB15(0, 0, 0));
		glBoxFilled(0, 168, 256, 192, RGB15(0, 0, 0));
		glColor(RGB15(31, 31, 31));
	}
	glEnd2D();
	GFX_FLUSH = 0;

	if (!loadFrameSprite) {
		frameDelaySprite++;
		loadFrameSprite = (frameDelaySprite == 2+frameDelaySpriteEven);
	}

	if (loadFrameSprite) {
		zoomingIconXpos[0] += 4;
		zoomingIconYpos[0] += 4;

		zoomingIconYpos[1] += 4;

		zoomingIconXpos[2] -= 4;
		zoomingIconYpos[2] += 4;

		zoomingIconXpos[3] -= 4;
		zoomingIconYpos[3] -= 4;

		zoomingIconYpos[4] -= 5;

		zoomingIconXpos[5] += 4;
		zoomingIconYpos[5] -= 4;

		zoomingIconXpos[6] += 5;
		zoomingIconYpos[6] += 4;

		zoomingIconYpos[7] += 5;

		zoomingIconXpos[8] -= 5;
		zoomingIconYpos[8] += 6;

		frameDelaySprite = 0;
		frameDelaySpriteEven = !frameDelaySpriteEven;
		loadFrameSprite = false;
	}
}

void twlMenuVideo(void) {
	extern bool twlMenuSplash;
	twlMenuSplash = true;
	//dmaFillHalfWords(0, BG_GFX, 0x18000);

	for (int selectedFrame = 0; selectedFrame < 40; selectedFrame++) {
		if (selectedFrame < 10) {
			snprintf(videoFrameFilename, sizeof(videoFrameFilename), "nitro:/video/twlmenupp/frame0%i.bmp", selectedFrame);
		} else {
			snprintf(videoFrameFilename, sizeof(videoFrameFilename), "nitro:/video/twlmenupp/frame%i.bmp", selectedFrame);
		}
		videoFrameFile = fopen(videoFrameFilename, "rb");

		if (videoFrameFile) {
			// Start loading
			fseek(videoFrameFile, 0xe, SEEK_SET);
			u8 pixelStart = (u8)fgetc(videoFrameFile) + 0xe;
			fseek(videoFrameFile, pixelStart, SEEK_SET);
			fread(bmpImageBuffer, 2, 0x14000, videoFrameFile);
			u16* src = bmpImageBuffer;
			int x = 0;
			int y = 143;
			for (int i=0; i<256*144; i++) {
				if (x >= 256) {
					x = 0;
					y--;
				}
				u16 val = *(src++);
				videoImageBuffer[selectedFrame][y*256+x] = ((val>>10)&0x1f) | ((val)&(0x1f<<5)) | (val&0x1f)<<10 | BIT(15);
				x++;
			}
		}
		fclose(videoFrameFile);

		scanKeys();
		if ((keysHeld() & KEY_START) || (keysHeld() & KEY_SELECT)) return;
	}

	while (zoomingIconYpos[8] < 64) {
		scanKeys();
		if ((keysHeld() & KEY_START) || (keysHeld() & KEY_SELECT)) return;
	}

	for (int i = 0; i < 40; i++) {
		while (1) {
			if (!loadFrame) {
				frameDelay++;
				loadFrame = (frameDelay == 2+frameDelayEven);
			}

			if (loadFrame) {
				dmaCopy((void*)videoImageBuffer[i], (u16*)BG_GFX+(256*24), 0x12000);

				frameDelay = 0;
				frameDelayEven = !frameDelayEven;
				loadFrame = false;
				break;
			}
			swiWaitForVBlank();
		}
		scanKeys();
		if ((keysHeld() & KEY_START) || (keysHeld() & KEY_SELECT)) return;
		swiWaitForVBlank();
	}

	for (int selectedFrame = 40; selectedFrame <= 43; selectedFrame++) {
		snprintf(videoFrameFilename, sizeof(videoFrameFilename), "nitro:/video/twlmenupp/frame%i.bmp", selectedFrame);
		videoFrameFile = fopen(videoFrameFilename, "rb");

		if (videoFrameFile) {
			// Start loading
			fseek(videoFrameFile, 0xe, SEEK_SET);
			u8 pixelStart = (u8)fgetc(videoFrameFile) + 0xe;
			fseek(videoFrameFile, pixelStart, SEEK_SET);
			fread(bmpImageBuffer, 2, 0x14000, videoFrameFile);
			u16* src = bmpImageBuffer;
			int x = 0;
			int y = 143;
			for (int i=0; i<256*144; i++) {
				if (x >= 256) {
					x = 0;
					y--;
				}
				u16 val = *(src++);
				videoImageBuffer[0][y*256+x] = ((val>>10)&0x1f) | ((val)&(0x1f<<5)) | (val&0x1f)<<10 | BIT(15);
				x++;
			}
			dmaCopy((void*)videoImageBuffer[0], (u16*)BG_GFX+(256*24), 0x12000);
		}
		fclose(videoFrameFile);

		scanKeys();
		if ((keysHeld() & KEY_START) || (keysHeld() & KEY_SELECT)) return;
	}

	// Change TWL letters to user color
	snprintf(videoFrameFilename, sizeof(videoFrameFilename), "nitro:/graphics/TWL_%i.bmp", (int)PersonalData->theme);
	videoFrameFile = fopen(videoFrameFilename, "rb");

	if (videoFrameFile) {
		// Start loading
		fseek(videoFrameFile, 0xe, SEEK_SET);
		u8 pixelStart = (u8)fgetc(videoFrameFile) + 0xe;
		fseek(videoFrameFile, pixelStart, SEEK_SET);
		fread(bmpImageBuffer, 2, 0x800, videoFrameFile);
		u16* src = bmpImageBuffer;
		int x = 68;
		int y = 69;
		for (int i=0; i<62*14; i++) {
			if (x >= 130) {
				x = 68;
				y--;
			}
			u16 val = *(src++);
			if (val != 0x7C1F) {
				BG_GFX[(y+24)*256+x] = ((val>>10)&0x1f) | ((val)&(0x1f<<5)) | (val&0x1f)<<10 | BIT(15);
			}
			x++;
		}
	}
	fclose(videoFrameFile);

	for (int i = 0; i < 60 * 2; i++)
	{
		scanKeys();
		if ((keysHeld() & KEY_START) || (keysHeld() & KEY_SELECT)) return;
		swiWaitForVBlank();
	}
}