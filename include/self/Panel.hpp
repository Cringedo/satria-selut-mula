#pragma once
#include <raylib.h>

class Panel
{
public:
    Panel(Rectangle rect);
    Panel(float x, float y);

    void SetHeight(float height);    
    void SetWidth(float width);    
    
    Rectangle GetRectangle();
    void Draw();    
private:
    Rectangle rect; 
    float height; 
    float width; 

};
