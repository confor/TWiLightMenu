/******************************************************************************
 *******************************************************************************
	A simple font class for Easy GL2D DS created by:

	Relminator (Richard Eric M. Lope BSN RN)
	Http://Rel.Phatcode.Net

 *******************************************************************************
 ******************************************************************************/

#include <nds.h>
#include <stdio.h>
#include "common/gl2d.h"
#include "FontGraphic.h"

int fontTextureID[2];


int FontGraphic::load(int textureID, glImage *_font_sprite,
				  const unsigned int numframes,
				  const unsigned int *texcoords,
				  GL_TEXTURE_TYPE_ENUM type,
				  int sizeX,
				  int sizeY,
				  int param,
				  int pallette_width,
				  const u16 *palette,
				  const uint8 *texture,
				  const unsigned short int *_mapping
				  )

{
	fontSprite = _font_sprite;
	imageCount = numframes;
	mapping = _mapping;
	if (fontTextureID[textureID]) glDeleteTextures(1, &fontTextureID[textureID]);
	fontTextureID[textureID] =
			glLoadSpriteSet(fontSprite,
							numframes,
							texcoords,
							type,
							sizeX,
							sizeY,
							param,
							pallette_width,
							palette,
							texture
							);

	return fontTextureID[textureID];

}

/**
 * Get the index in the UV coordinate array where the letter appears
 */
unsigned int FontGraphic::getSpriteIndex(const u16 letter) {
	unsigned int spriteIndex = 0;
	long int left = 0;
	long int right = imageCount;
	long int mid = 0;

	while (left <= right)
	{
		mid = left + ((right - left) / 2);
		if (mapping[mid] == letter) {
			spriteIndex = mid;
			break;
		}

		if (mapping[mid] < letter) {
			left = mid + 1;
		} else {
			right = mid - 1;
		}
	}
	return spriteIndex;
}

void FontGraphic::print(int x, int y, const char *text)
{
	unsigned short int fontChar = 0;
	while (*text)
	{
		// UTF-8 handling
		if((*text & 0x80) == 0) {
			fontChar = getSpriteIndex(*text++);
		} else if((*text & 0xE0) == 0xC0) {
			char16_t c = ((*text++ & 0x1F) << 6);
			if((*text & 0xC0) == 0x80) c |= *text++ & 0x3F;

			fontChar = getSpriteIndex(c);
		} else if((*text & 0xF0) == 0xE0) {
			char16_t c = (*text++ & 0xF) << 12;
			if((*text & 0xC0) == 0x80) c |= (*text++ & 0x3F) << 6;
			if((*text & 0xC0) == 0x80) c |=  *text++ & 0x3F;

			fontChar = getSpriteIndex(c);
		} else if((*text & 0xF8) == 0xF0) {
			char16_t c = (*text++ & 0x7) << 18;
			if((*text & 0xC0) == 0x80) c |= (*text++ & 0x3F) << 12;
			if((*text & 0xC0) == 0x80) c |= (*text++ & 0x3F) << 6;
			if((*text & 0xC0) == 0x80) c |=  *text++ & 0x3F;

			fontChar = getSpriteIndex(c);
		}

		glSprite(x, y, GL_FLIP_NONE, &fontSprite[fontChar]);
		x += fontSprite[fontChar].width;
	}
}

int FontGraphic::calcWidth(const char *text)
{
	unsigned short int fontChar = 0;
	int x = 0;

	while (*text)
	{
		// UTF-8 handling
		if((*text & 0x80) == 0) {
			fontChar = getSpriteIndex(*text++);
		} else if((*text & 0xE0) == 0xC0) {
			char16_t c = ((*text++ & 0x1F) << 6);
			if((*text & 0xC0) == 0x80) c |= *text++ & 0x3F;

			fontChar = getSpriteIndex(c);
		} else if((*text & 0xF0) == 0xE0) {
			char16_t c = (*text++ & 0xF) << 12;
			if((*text & 0xC0) == 0x80) c |= (*text++ & 0x3F) << 6;
			if((*text & 0xC0) == 0x80) c |=  *text++ & 0x3F;

			fontChar = getSpriteIndex(c);
		} else if((*text & 0xF8) == 0xF0) {
			char16_t c = (*text++ & 0x7) << 18;
			if((*text & 0xC0) == 0x80) c |= (*text++ & 0x3F) << 12;
			if((*text & 0xC0) == 0x80) c |= (*text++ & 0x3F) << 6;
			if((*text & 0xC0) == 0x80) c |=  *text++ & 0x3F;

			fontChar = getSpriteIndex(c);
		}

		x += fontSprite[fontChar].width;
	}
	return x;
}

void FontGraphic::print(int x, int y, int value)
{
	sprintf(buffer, "%i", value);
	print(x, y, buffer);
}

int FontGraphic::getCenteredX(const char *text)
{
	unsigned short int fontChar = 0;
	int total_width = 0;
	while (*text)
	{
		// UTF-8 handling
		if((*text & 0x80) == 0) {
			fontChar = getSpriteIndex(*text++);
		} else if((*text & 0xE0) == 0xC0) {
			char16_t c = ((*text++ & 0x1F) << 6);
			if((*text & 0xC0) == 0x80) c |= *text++ & 0x3F;

			fontChar = getSpriteIndex(c);
		} else if((*text & 0xF0) == 0xE0) {
			char16_t c = (*text++ & 0xF) << 12;
			if((*text & 0xC0) == 0x80) c |= (*text++ & 0x3F) << 6;
			if((*text & 0xC0) == 0x80) c |=  *text++ & 0x3F;

			fontChar = getSpriteIndex(c);
		} else if((*text & 0xF8) == 0xF0) {
			char16_t c = (*text++ & 0x7) << 18;
			if((*text & 0xC0) == 0x80) c |= (*text++ & 0x3F) << 12;
			if((*text & 0xC0) == 0x80) c |= (*text++ & 0x3F) << 6;
			if((*text & 0xC0) == 0x80) c |=  *text++ & 0x3F;

			fontChar = getSpriteIndex(c);
		}

		total_width += fontSprite[fontChar].width;
	}
	return (SCREEN_WIDTH - total_width) / 2;
}

void FontGraphic::printCentered(int y, const char *text)
{
	unsigned short int fontChar = 0;

	int x = getCenteredX(text);
	while (*text)
	{
		// UTF-8 handling
		if((*text & 0x80) == 0) {
			fontChar = getSpriteIndex(*text++);
		} else if((*text & 0xE0) == 0xC0) {
			char16_t c = ((*text++ & 0x1F) << 6);
			if((*text & 0xC0) == 0x80) c |= *text++ & 0x3F;

			fontChar = getSpriteIndex(c);
		} else if((*text & 0xF0) == 0xE0) {
			char16_t c = (*text++ & 0xF) << 12;
			if((*text & 0xC0) == 0x80) c |= (*text++ & 0x3F) << 6;
			if((*text & 0xC0) == 0x80) c |=  *text++ & 0x3F;

			fontChar = getSpriteIndex(c);
		} else if((*text & 0xF8) == 0xF0) {
			char16_t c = (*text++ & 0x7) << 18;
			if((*text & 0xC0) == 0x80) c |= (*text++ & 0x3F) << 12;
			if((*text & 0xC0) == 0x80) c |= (*text++ & 0x3F) << 6;
			if((*text & 0xC0) == 0x80) c |=  *text++ & 0x3F;

			fontChar = getSpriteIndex(c);
		}

		glSprite(x, y, GL_FLIP_NONE, &fontSprite[fontChar]);
		x += fontSprite[fontChar].width;
	}
}

void FontGraphic::printCentered(int y, int value)
{
	sprintf(buffer, "%i", value);
	printCentered(y, buffer);
}