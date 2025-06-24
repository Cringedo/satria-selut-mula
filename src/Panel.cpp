#include "self/Panel.hpp"
#include <raylib.h>


Panel::Panel(Rectangle rect) : rect(rect)
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
