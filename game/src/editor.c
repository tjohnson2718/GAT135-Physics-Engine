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

#define EDITOR_DATA(data) TextFormat("%0.2f", data), &data

void InitEditor()
{
    GuiLoadStyle("raygui/styles/cyber/style_cyber.rgs");

    Image image = LoadImage("resources/reticle.png");
    cursorTexture = LoadTextureFromImage(image);
    UnloadImage(image);
    HideCursor();
 
    ncEditorData.anchor01 = (Vector2){ 920, 48 };
    ncEditorData.anchor02 = (Vector2){ 936, 96 };
    ncEditorData.anchor03 = (Vector2){ 944, 312 };

    //body
    ncEditorData.EditorWindowActive = true;
    ncEditorData.BodyTypeDropDownEditMode = false;
    ncEditorData.BodyTypeDropDownActive = 0;
    ncEditorData.MassSliderValue = 0.5f;
    ncEditorData.DampingSliderValue = 0.0f;
    ncEditorData.GS_SliderValue = 15.0f;
    ncEditorData.StiffnessSliderValue = 0.0f;
    ncEditorData.GravitationSliderValue = 0.0f;
    ncEditorData.RestitutionSliderValue = 0.0f;

    //world
    ncEditorData.TimestepSliderValue = 60.0f;
    ncEditorData.GravitySliderValue = 0.0f;
    ncEditorData.Reset_BTPressed = false;
    ncEditorData.Simulate_BTActive = true;

    editorRect = (Rectangle){ ncEditorData.anchor01.x + 0, ncEditorData.anchor01.y + 0, 304, 616 };
}

void UpdateEditor(Vector2 position)
{
    // toggle show / hide editor box with key press 

    if (IsKeyPressed(KEY_TAB))
    {
        ncEditorData.EditorWindowActive = !ncEditorData.EditorWindowActive;
    }

    // check if cursor position is intersecting the editor box  
    ncEditorIntersect = ncEditorData.EditorWindowActive && CheckCollisionPointRec(position, editorRect);
}

void DrawEditor(Vector2 position)
{
    if (ncEditorData.BodyTypeDropDownEditMode) GuiLock();

    if (ncEditorData.EditorWindowActive)
    {
        ncEditorData.EditorWindowActive = !GuiWindowBox((Rectangle) { ncEditorData.anchor01.x + 0, ncEditorData.anchor01.y + 0, 288, 672 }, "Editor");
        GuiToggle((Rectangle) { ncEditorData.anchor01.x + 192, ncEditorData.anchor01.y + 624, 88, 24 }, "Simulate", & ncEditorData.Simulate_BTActive);
    }
    GuiGroupBox((Rectangle) { ncEditorData.anchor02.x + 8, ncEditorData.anchor02.y + 0, 240, 192 }, "Body");
    GuiSliderBar((Rectangle) { ncEditorData.anchor02.x + 95, ncEditorData.anchor02.y + 72, 120, 16 }, "Mass", EDITOR_DATA(ncEditorData.MassSliderValue), 0, 5);
    GuiSliderBar((Rectangle) { ncEditorData.anchor02.x + 95, ncEditorData.anchor02.y + 96, 120, 16 }, "Damping", EDITOR_DATA(ncEditorData.DampingSliderValue), 0, 100);
    GuiSliderBar((Rectangle) { ncEditorData.anchor02.x + 95, ncEditorData.anchor02.y + 120, 120, 16 }, "Gravity Scale", EDITOR_DATA(ncEditorData.GS_SliderValue), 0, 100);
    GuiSliderBar((Rectangle) { ncEditorData.anchor02.x + 95, ncEditorData.anchor02.y + 144, 120, 16 }, "Stiffness (k)", EDITOR_DATA(ncEditorData.StiffnessSliderValue), 0, 100);
    GuiGroupBox((Rectangle) { ncEditorData.anchor03.x + 0, ncEditorData.anchor03.y + 0, 240, 144 }, "World");
    GuiSliderBar((Rectangle) { ncEditorData.anchor03.x + 75, ncEditorData.anchor03.y + 48, 120, 16 }, "Gravitation", EDITOR_DATA(ncEditorData.GravitationSliderValue), 0, 100);
    GuiSliderBar((Rectangle) { ncEditorData.anchor02.x + 95, ncEditorData.anchor02.y + 168, 120, 16 }, "Restitution", EDITOR_DATA(ncEditorData.RestitutionSliderValue), 0, 1);
    GuiSliderBar((Rectangle) { ncEditorData.anchor03.x + 75, ncEditorData.anchor03.y + 72, 120, 16 }, "Timestep", EDITOR_DATA(ncEditorData.TimestepSliderValue), 30, 60);
    GuiSlider((Rectangle) { ncEditorData.anchor03.x + 75, ncEditorData.anchor03.y + 24, 120, 16 }, "Gravity", EDITOR_DATA(ncEditorData.GravitySliderValue), -100, 100);
    ncEditorData.Reset_BTPressed = GuiButton((Rectangle) { ncEditorData.anchor01.x + 100, ncEditorData.anchor01.y + 624, 88, 24 }, "Reset");
    if (GuiDropdownBox((Rectangle) { ncEditorData.anchor02.x + 32, ncEditorData.anchor02.y + 24, 192, 24 }, "DYNAMIC; KINEMATIC; STATIC", & ncEditorData.BodyTypeDropDownActive, ncEditorData.BodyTypeDropDownEditMode)) ncEditorData.BodyTypeDropDownEditMode = !ncEditorData.BodyTypeDropDownEditMode;


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
