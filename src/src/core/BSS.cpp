
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "AppX.h"


Style_BSS::Style_BSS()
{
    this->name = "default";

    this->bss.font_ttf = "sans-bold";
    this->bss.font_size = 17.0f;
    this->bss.font_color = 0xFFE0E0E0;
    this->bss.font_shadow.x = -1;
    this->bss.font_shadow.y = -1;

    this->bss.background_color = 0xFF404040;
    this->bss.background_color_linear = 0xFF808080;

    this->bss.border_radius = 4.0f;

    this->bss.padding_type = false;

    this->bss.padding.x  = 0;
    this->bss.padding.y  = 0;
    this->bss.fpadding.x = 0.0;
    this->bss.fpadding.y = 0.0;

    this->bss.brightness = 1.0f;
    this->bss.grayscale = 0.0f;
}



Widget_BSS::Widget_BSS()
{
    this->name = "default";

    this->bss.rect_type = false;

    this->bss.rect.x = 0;
    this->bss.rect.y = 0;
    this->bss.rect.w = 10;
    this->bss.rect.h = 10;

    this->bss.frect.x = 0.0f;
    this->bss.frect.y = 0.0f;
    this->bss.frect.w = 0.125f;
    this->bss.frect.h = 0.125f;

    this->z_order = 0;
    this->type = 0;

    this->visible = true;

}

/*
void Style_BSS::button()
{

}
*/
