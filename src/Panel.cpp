#include "self/Panel.hpp"
#include <raylib.h>


Panel::Panel(Rectangle rect) : rect(rect)
{
}

Panel::Panel(float x, float y): Panel({x, y, 100, 100})
{
}
void Panel::SetHeight(float h)
{
    height = h;
}

void Panel::SetWidth(float w)
{
    width = w;
}

Rectangle Panel::GetRectangle()
{
    return rect;
}

void Panel::Draw()
{
    DrawRectangleRec(rect, GRAY);
}
