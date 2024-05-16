#include "editor.h"
#include "Body.h"
#include "render.h"
#define RAYGUI_IMPLEMENTATION
#include "../../raygui/src/raygui.h"

bool ncEditorActive = true;
bool ncEditorIntersect = false;

ncEditorData_t ncEditorData;
Rectangle editorRect;

Vector2 anchor01 = { 1000, 50 };
Texture2D cursorTexture;

bool WindowBox000Active = true;
float MassMaxValue = 0.0f;
float MassMinValue = 0.0f;
float GravitationValue = 0.0f;
float GravityScaleValue = 0.0f;
float WorldGravitationValue = 0.0f;
float DampingValue = 0.0f;
bool BodyTypeEditMode = false;
int BodyTypeActive = 0;

void InitEditor()
{
    GuiLoadStyle("raygui/styles/cyber/style_cyber.rgs");

    Image image = LoadImage("resources/reticle.png");
    cursorTexture = LoadTextureFromImage(image);
    UnloadImage(image);
    HideCursor();
 
    ncEditorData.anchor01 = (Vector2){ 950, 50 };
    ncEditorData.EditorBoxActive = true;
    ncEditorData.MassMinValue = 2.0f;
    ncEditorData.MassMaxValue = 0.0f;
    ncEditorData.GravitationValue = 0.0f;
    ncEditorData.BodyTypeEditMode = false;
    ncEditorData.BodyTypeActive = 0;
    ncEditorData.DampingValue = 0.0f;
    ncEditorData.GravityScaleValue = 0.0f;

    editorRect = (Rectangle){ ncEditorData.anchor01.x + 0, ncEditorData.anchor01.y + 0, 304, 616 };
}

void UpdateEditor(Vector2 position)
{
    //
}

void DrawEditor(Vector2 position)
{
    if (BodyTypeEditMode) GuiLock();

    if (ncEditorData.EditorBoxActive)
    {
        if (WindowBox000Active)
        {
            WindowBox000Active = !GuiWindowBox((Rectangle) { 672, 48, 312, 672 }, "SAMPLE TEXT");
            GuiSliderBar((Rectangle) { 816, 240, 120, 16 }, "Gravitation", NULL, & GravitationValue, 0, 100);
            GuiSliderBar((Rectangle) { 816, 280, 120, 16 }, "Gravity Scale", NULL, & GravityScaleValue, 0, 100);
            GuiSliderBar((Rectangle) { 816, 320, 120, 16 }, "World Gravitation", NULL, & WorldGravitationValue, 0, 100);
            GuiSliderBar((Rectangle) { 816, 360, 120, 16 }, "Damping", NULL, & DampingValue, 0, 100);
        }
        GuiGroupBox((Rectangle) { 696, 96, 264, 456 }, "Body");
        GuiSliderBar((Rectangle) { 816, 168, 120, 16 }, "Mass Max", NULL, & MassMaxValue, 0, 100);
        GuiSliderBar((Rectangle) { 816, 120, 120, 16 }, "Mass Min", NULL, & MassMinValue, 0, 100);
        if (GuiDropdownBox((Rectangle) { 816, 408, 120, 24 }, "Body Type", & BodyTypeActive, BodyTypeEditMode)) BodyTypeEditMode = !BodyTypeEditMode;
    }
    
    GuiUnlock();


    DrawTexture(cursorTexture, (int)position.x - cursorTexture.width / 2, (int)position.y - cursorTexture.height / 2, WHITE);
}

ncBody* GetBodyIntersect(ncBody* bodies, Vector2 position)
{
    for (ncBody* body = bodies; body; body = body->next)
    {
        Vector2 screen = ConvertWorldToScreen(body->position);
        if (CheckCollisionPointCircle(position, screen, ConvertWorldToPixel(body->mass * 0.5f)))
        {
            return body;
        }
    }

    return NULL;
}

void DrawLineBodyToPosition(ncBody* body, Vector2 position)
{
    Vector2 screen = ConvertWorldToScreen(body->position);
    DrawLine((int)screen.x, (int)screen.y, (int)position.x - cursorTexture.width / 2, (int)position.y - cursorTexture.height / 2, YELLOW);
}
