#pragma once
#include <diesel.h>
//#define DS_IMGUI_IMPLEMENTATION

struct p2i {
	
	int x;
	int y;

	p2i() : x(0), y(0) {}
	explicit p2i(int v) : x(v), y(v) {}
	p2i(int xv, int yv) : x(xv), y(yv) {}
};

namespace gui {

	struct IMGUISettings {
		ds::Color headerBoxColor;
		ds::Color buttonColor;
		ds::Color backgroundColor;
		ds::Color labelBoxColor;
		ds::Color enabledBoxColor;
		ds::Color disabledBoxColor;
		ds::Color activeInputBoxColor;
		ds::Color inputBoxColor;
		ds::Color boxBackgroundColor;
		ds::Color boxSelectionColor;
		ds::Color sliderColor;
		ds::Color scrollSliderColor;
		float lineSpacing;
	};

	void init(IMGUISettings* settings = 0);

	//void start(const p2i& startPos);

	//void start(const ds::vec2& pos);
	void start();

	//void begin(const char* header, int* state);

	void begin(const char* header, int* state, p2i* position, int width = 200);

	void beginGroup();

	void endGroup();

	bool Button(const char* text);

	void Text(const char* text);

	void Label(const char* label, const char* text);

	void Value(const char* label, int v);

	void Value(const char* label, uint32_t v);

	void Value(const char* label, float v);

	void Value(const char* label, float v, const char* format);

	void Value(const char* label, const p2i& v);

	void Value(const char* label, bool v);

	void Value(const char* label, const ds::vec2& v);

	void Value(const char* label, const ds::vec3& v);

	void Value(const char* label, const ds::vec4& v);

	void Value(const char* label, const ds::Color& v);

	void FormattedText(const char* fmt, ...);

	void Message(const char* fmt, ...);

	bool Input(const char* label, char* str, int maxLength);

	void Input(const char* label, int* v);

	void Input(const char* label, float* v);

	void Input(const char* label, ds::vec2* v);

	void Input(const char* label, ds::vec3* v);

	void Input(const char* label, ds::vec4* v);

	void Input(const char* label, ds::Color* v);

	void StepInput(const char* label, int* v, int minValue, int maxValue, int steps);

	void Checkbox(const char* label, bool* state);

	void ListBox(const char* label, const char** entries, int num, int* selected, int *offset, int max);

	void DropDownBox(const char* label, const char** entries, int num, int* state, int* selected, int *offset, int max, bool closeOnSelection = false);

	void Slider(const char* label, int* v, int minValue, int maxValue, float width = 200.0f);

	void Slider(const char* label, float* v, float minValue, float maxValue, int precision = 0, float width = 200.0f);

	void SliderAngle(const char* label, float* v, float width = 200.0f);

	void Separator();

	void Histogram(float* values, int num, float minValue, float maxValue, float step, float width = 200.0f, float height = 100.0f);

	void Diagram(float* values, int num, float minValue, float maxValue, float step, float width = 200.0f, float height = 100.0f);

	void Diagram(const char* label, float* values, int num, float minValue, float maxValue, float step, float width = 200.0f, float height = 100.0f);

	void beginMenu();

	bool MenuBar(const char* name);

	bool MenuItem(const char* label);

	void MenuBar(const char** names, int num, int* state);

	void endMenu();

	void debug();

	void end();

	void shutdown();

}

#ifdef DS_IMGUI_IMPLEMENTATION
#include <SpriteBatchBuffer.h>
#include <string.h>
#include <stdarg.h>

const unsigned char DS_IMGUI_FONT[128][16] = {
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x60, 0x60, 0x06, 0x1B, 0x3C, 0x6C, 0xC0, 0x01, 0x06, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0xF0, 0x60, 0x06, 0x1B, 0x66, 0x6C, 0x61, 0x03, 0x06, 0x18, 0x60, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0xF0, 0x60, 0x86, 0x3F, 0x06, 0xB8, 0x61, 0x03, 0x06, 0x18, 0x60, 0xC0, 0x06, 0x06, 0x00 },
	{ 0x00, 0xF0, 0x00, 0x00, 0x1B, 0x0C, 0xC0, 0xC0, 0x01, 0x00, 0x0C, 0xC0, 0x80, 0x03, 0x06, 0x00 },
	{ 0x00, 0x60, 0x00, 0x00, 0x1B, 0x18, 0x60, 0x60, 0x00, 0x00, 0x0C, 0xC0, 0xE0, 0x8F, 0x1F, 0x00 },
	{ 0x00, 0x60, 0x00, 0x00, 0x1B, 0x30, 0x30, 0x60, 0x0F, 0x00, 0x0C, 0xC0, 0x80, 0x03, 0x06, 0x00 },
	{ 0x00, 0x00, 0x00, 0x80, 0x3F, 0x60, 0xD8, 0x61, 0x06, 0x00, 0x0C, 0xC0, 0xC0, 0x06, 0x06, 0x00 },
	{ 0x00, 0x60, 0x00, 0x00, 0x1B, 0x66, 0x68, 0x63, 0x06, 0x00, 0x0C, 0xC0, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x60, 0x00, 0x00, 0x1B, 0x3C, 0x60, 0xC3, 0x0D, 0x00, 0x18, 0x60, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0xC0, 0x01, 0x00, 0x00, 0x18, 0x60, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x18, 0x78, 0xC0, 0xC0, 0x03, 0x0F, 0x0C, 0xF8, 0x81, 0x83, 0x1F, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x18, 0xCC, 0xE0, 0x60, 0x86, 0x19, 0x0C, 0x18, 0x80, 0x01, 0x18, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x0C, 0xEC, 0xF8, 0x60, 0x86, 0x19, 0x6C, 0x18, 0xC0, 0x00, 0x0C, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x0C, 0xEC, 0xC0, 0x00, 0x06, 0x18, 0x6C, 0x18, 0xE0, 0x03, 0x0C, 0x00 },
	{ 0x00, 0xF8, 0x01, 0x00, 0x06, 0xCC, 0xC0, 0x00, 0x03, 0x0E, 0x6C, 0xF8, 0x60, 0x06, 0x06, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x06, 0xDC, 0xC0, 0x80, 0x01, 0x18, 0x66, 0x80, 0x61, 0x06, 0x06, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x03, 0xDC, 0xC0, 0xC0, 0x80, 0x19, 0xFE, 0x80, 0x61, 0x06, 0x03, 0x00 },
	{ 0x38, 0x00, 0x80, 0x03, 0x03, 0xCC, 0xC0, 0x60, 0x80, 0x19, 0x60, 0xC0, 0x60, 0x06, 0x03, 0x00 },
	{ 0x38, 0x00, 0x80, 0x83, 0x01, 0x78, 0xC0, 0xE0, 0x07, 0x0F, 0x60, 0x78, 0xC0, 0x03, 0x03, 0x00 },
	{ 0x30, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x3C, 0xF0, 0x00, 0x00, 0x00, 0x60, 0x00, 0x60, 0x00, 0x0F, 0x7E, 0x60, 0xE0, 0x03, 0x0F, 0x00 },
	{ 0x66, 0x98, 0x01, 0x00, 0x00, 0x30, 0x00, 0xC0, 0x80, 0x19, 0xC3, 0xF0, 0x60, 0x86, 0x19, 0x00 },
	{ 0x66, 0x98, 0x81, 0x03, 0x0E, 0x18, 0x00, 0x80, 0x81, 0x19, 0xC3, 0x98, 0x61, 0x86, 0x19, 0x00 },
	{ 0x6E, 0x98, 0x81, 0x03, 0x0E, 0x0C, 0xF8, 0x01, 0x03, 0x0C, 0xF3, 0x98, 0x61, 0x86, 0x01, 0x00 },
	{ 0x3C, 0x98, 0x01, 0x00, 0x00, 0x06, 0x00, 0x00, 0x06, 0x06, 0xDB, 0x98, 0xE1, 0x83, 0x01, 0x00 },
	{ 0x76, 0xF0, 0x01, 0x00, 0x00, 0x0C, 0xF8, 0x01, 0x03, 0x06, 0xDB, 0xF8, 0x61, 0x86, 0x01, 0x00 },
	{ 0x66, 0xC0, 0x00, 0x00, 0x00, 0x18, 0x00, 0x80, 0x01, 0x00, 0xF3, 0x98, 0x61, 0x86, 0x19, 0x00 },
	{ 0x66, 0x60, 0x80, 0x03, 0x0E, 0x30, 0x00, 0xC0, 0x00, 0x06, 0x03, 0x98, 0x61, 0x86, 0x19, 0x00 },
	{ 0x3C, 0x70, 0x80, 0x03, 0x0E, 0x60, 0x00, 0x60, 0x00, 0x06, 0xFE, 0x98, 0xE1, 0x03, 0x0F, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x1E, 0xF8, 0xE1, 0x07, 0x0F, 0x66, 0xF0, 0x00, 0x86, 0x19, 0x06, 0x18, 0x63, 0x0C, 0x0F, 0x00 },
	{ 0x36, 0x18, 0x60, 0x80, 0x19, 0x66, 0x60, 0x00, 0x86, 0x19, 0x06, 0x18, 0x63, 0x8C, 0x19, 0x00 },
	{ 0x66, 0x18, 0x60, 0x80, 0x19, 0x66, 0x60, 0x00, 0x86, 0x0D, 0x06, 0xB8, 0xE3, 0x8C, 0x19, 0x00 },
	{ 0x66, 0x18, 0x60, 0x80, 0x01, 0x66, 0x60, 0x00, 0x86, 0x0D, 0x06, 0x58, 0xE3, 0x8D, 0x19, 0x00 },
	{ 0x66, 0xF8, 0xE0, 0x83, 0x01, 0x7E, 0x60, 0x00, 0x86, 0x07, 0x06, 0x58, 0x63, 0x8F, 0x19, 0x00 },
	{ 0x66, 0x18, 0x60, 0x80, 0x1D, 0x66, 0x60, 0x00, 0x86, 0x0D, 0x06, 0x58, 0x63, 0x8E, 0x19, 0x00 },
	{ 0x66, 0x18, 0x60, 0x80, 0x19, 0x66, 0x60, 0x60, 0x86, 0x0D, 0x06, 0x18, 0x63, 0x8C, 0x19, 0x00 },
	{ 0x36, 0x18, 0x60, 0x80, 0x19, 0x66, 0x60, 0x60, 0x86, 0x19, 0x06, 0x18, 0x63, 0x8C, 0x19, 0x00 },
	{ 0x1E, 0xF8, 0x61, 0x00, 0x1F, 0x66, 0xF0, 0xC0, 0x83, 0x19, 0x7E, 0x18, 0x63, 0x0C, 0x0F, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x3E, 0xF0, 0xE0, 0x03, 0x0F, 0x7E, 0x98, 0x61, 0x86, 0x31, 0x66, 0x98, 0xE1, 0x07, 0x0F, 0x00 },
	{ 0x66, 0x98, 0x61, 0x86, 0x19, 0x18, 0x98, 0x61, 0x86, 0x31, 0x66, 0x98, 0x01, 0x06, 0x03, 0x00 },
	{ 0x66, 0x98, 0x61, 0x86, 0x01, 0x18, 0x98, 0x61, 0x86, 0x31, 0x2C, 0x98, 0x01, 0x06, 0x03, 0x00 },
	{ 0x66, 0x98, 0x61, 0x06, 0x03, 0x18, 0x98, 0x61, 0x86, 0x35, 0x18, 0x98, 0x01, 0x03, 0x03, 0x00 },
	{ 0x3E, 0x98, 0xE1, 0x03, 0x06, 0x18, 0x98, 0x61, 0x86, 0x35, 0x18, 0xF0, 0x80, 0x01, 0x03, 0x00 },
	{ 0x06, 0x98, 0x61, 0x03, 0x0C, 0x18, 0x98, 0x61, 0x86, 0x35, 0x34, 0x60, 0xC0, 0x00, 0x03, 0x00 },
	{ 0x06, 0x98, 0x61, 0x06, 0x18, 0x18, 0x98, 0x61, 0x06, 0x1B, 0x66, 0x60, 0x60, 0x00, 0x03, 0x00 },
	{ 0x06, 0x98, 0x61, 0x86, 0x19, 0x18, 0x98, 0xC1, 0x03, 0x1B, 0x66, 0x60, 0x60, 0x00, 0x03, 0x00 },
	{ 0x06, 0xF0, 0x60, 0x06, 0x0F, 0x18, 0xF0, 0x80, 0x01, 0x1B, 0x66, 0x60, 0xE0, 0x07, 0x03, 0x00 },
	{ 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00 },
	{ 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x80, 0x01, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0xC0, 0x03, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x06, 0xF0, 0x60, 0x06, 0x00, 0x30, 0x00, 0x60, 0x00, 0x00, 0x60, 0x00, 0x80, 0x07, 0x00, 0x00 },
	{ 0x06, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x60, 0x00, 0xC0, 0x00, 0x00, 0x00 },
	{ 0x0C, 0xC0, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xE0, 0x03, 0x0F, 0x7C, 0xF0, 0xC0, 0x00, 0x1F, 0x00 },
	{ 0x0C, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x80, 0x61, 0x86, 0x19, 0x66, 0x98, 0xC1, 0x80, 0x19, 0x00 },
	{ 0x18, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x80, 0x61, 0x86, 0x01, 0x66, 0x98, 0xE1, 0x87, 0x19, 0x00 },
	{ 0x18, 0xC0, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x61, 0x86, 0x01, 0x66, 0xF8, 0xC1, 0x80, 0x19, 0x00 },
	{ 0x30, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x98, 0x61, 0x86, 0x01, 0x66, 0x18, 0xC0, 0x80, 0x19, 0x00 },
	{ 0x30, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x98, 0x61, 0x86, 0x19, 0x66, 0x18, 0xC0, 0x80, 0x19, 0x00 },
	{ 0x60, 0xC0, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xE1, 0x03, 0x0F, 0x7C, 0xF0, 0xC0, 0x00, 0x1F, 0x00 },
	{ 0x60, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00 },
	{ 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00 },
	{ 0x00, 0xF0, 0x00, 0xC0, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x0F, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x60, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x06, 0x60, 0x00, 0x83, 0x01, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x06, 0x00, 0x00, 0x80, 0x01, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x3E, 0x78, 0xC0, 0x83, 0x19, 0x18, 0xF8, 0xE1, 0x03, 0x0F, 0x3E, 0xF0, 0x61, 0x06, 0x1F, 0x00 },
	{ 0x66, 0x60, 0x00, 0x83, 0x19, 0x18, 0x58, 0x63, 0x86, 0x19, 0x66, 0x98, 0x61, 0x87, 0x01, 0x00 },
	{ 0x66, 0x60, 0x00, 0x83, 0x0D, 0x18, 0x58, 0x63, 0x86, 0x19, 0x66, 0x98, 0xE1, 0x80, 0x01, 0x00 },
	{ 0x66, 0x60, 0x00, 0x83, 0x07, 0x18, 0x58, 0x63, 0x86, 0x19, 0x66, 0x98, 0x61, 0x00, 0x0F, 0x00 },
	{ 0x66, 0x60, 0x00, 0x83, 0x0D, 0x18, 0x58, 0x63, 0x86, 0x19, 0x66, 0x98, 0x61, 0x00, 0x18, 0x00 },
	{ 0x66, 0x60, 0x00, 0x83, 0x19, 0x18, 0x58, 0x63, 0x86, 0x19, 0x66, 0x98, 0x61, 0x00, 0x18, 0x00 },
	{ 0x66, 0xF8, 0x01, 0x83, 0x19, 0x7E, 0x18, 0x63, 0x06, 0x0F, 0x3E, 0xF0, 0x61, 0x80, 0x0F, 0x00 },
	{ 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x80, 0x01, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x80, 0x01, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0xE0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x80, 0x01, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x18, 0x30, 0xE0, 0x88, 0x1F, 0x00 },
	{ 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x18, 0x60, 0xB0, 0x8D, 0x1F, 0x00 },
	{ 0x7E, 0x98, 0x61, 0x86, 0x31, 0x66, 0x98, 0xE1, 0x07, 0x06, 0x18, 0x60, 0x10, 0x87, 0x1F, 0x00 },
	{ 0x0C, 0x98, 0x61, 0x86, 0x35, 0x66, 0x98, 0x01, 0x06, 0x06, 0x18, 0x60, 0x00, 0x80, 0x1F, 0x00 },
	{ 0x0C, 0x98, 0x61, 0x86, 0x35, 0x3C, 0x98, 0x01, 0x03, 0x03, 0x18, 0xC0, 0x00, 0x80, 0x1F, 0x00 },
	{ 0x0C, 0x98, 0x61, 0x86, 0x35, 0x18, 0x98, 0x81, 0x81, 0x01, 0x18, 0x80, 0x01, 0x80, 0x1F, 0x00 },
	{ 0x0C, 0x98, 0x61, 0x86, 0x35, 0x3C, 0x98, 0xC1, 0x00, 0x03, 0x18, 0xC0, 0x00, 0x80, 0x1F, 0x00 },
	{ 0x0C, 0x98, 0xC1, 0x03, 0x1B, 0x66, 0x98, 0x61, 0x00, 0x06, 0x18, 0x60, 0x00, 0x80, 0x1F, 0x00 },
	{ 0x78, 0xF0, 0x81, 0x01, 0x1B, 0x66, 0xF0, 0xE0, 0x07, 0x06, 0x18, 0x60, 0x00, 0x80, 0x1F, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x06, 0x18, 0x60, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x0C, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
};

namespace gui {

	typedef uint32_t HashedId;

	static HashedId NULL_HASH = 0;

	HashedId HashId(const char *id) {
		// https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
		HashedId hash = 0x84222325;
		while (*id) hash = (hash ^ static_cast<uint8_t>(*id++)) * 0x000001b3;
		return hash;
	}

	HashedId HashPointer(const void *ptr) {
		HashedId hash = static_cast<HashedId>(reinterpret_cast<size_t>(ptr)) * 2654435761;
		return hash;
	}

	const static ds::vec4 INPUT_RECT = ds::vec4(0, 128, 150, 20);
	const static ds::vec4 WHITE_RECT = ds::vec4(0, 128, 128, 128);

	namespace renderer {

		enum ResizeType {
			RT_NONE,
			RT_X,
			RT_Y,
			RT_BOTH
		};

		struct UIBuffer {
			ds::vec2* positions;
			ds::vec2* sizes;
			ds::vec4* rectangles;
			ds::vec2* scales;
			ds::Color* colors;
			ResizeType* resize;
			uint16_t num;
			uint16_t capacity;
			char* data;
		};

		struct UIContext {
			UIBuffer* buffer;
			SpriteBatchBuffer* sprites;
			p2i startPos;
			p2i size;
		};

		UIContext* init() {
			UIContext* ctx = new UIContext;
			ctx->buffer = new UIBuffer;
			int sz = 4096 * (sizeof(ds::vec2) + sizeof(ds::vec2) + sizeof(ds::vec4) + sizeof(ds::vec2) + sizeof(ds::Color) + sizeof(ResizeType));
			ctx->buffer->num = 0;
			ctx->buffer->capacity = 4096;
			ctx->buffer->data = new char[sz];
			ctx->buffer->positions = (ds::vec2*)(ctx->buffer->data);
			ctx->buffer->sizes = (ds::vec2*)(ctx->buffer->positions + ctx->buffer->capacity);
			ctx->buffer->rectangles = (ds::vec4*)(ctx->buffer->sizes + ctx->buffer->capacity);
			ctx->buffer->scales = (ds::vec2*)(ctx->buffer->rectangles + ctx->buffer->capacity);
			ctx->buffer->colors = (ds::Color*)(ctx->buffer->scales + ctx->buffer->capacity);
			ctx->buffer->resize = (ResizeType*)(ctx->buffer->colors + ctx->buffer->capacity);

			uint8_t* data = new uint8_t[256 * 256 * 4];
			for (int i = 0; i < 256 * 256 * 4; ++i) {
				data[i] = 255;
			}
			int index = 0;
			for (int y = 0; y < 128; ++y) {
				for (int x = 0; x < 16; ++x) {
					uint8_t t = DS_IMGUI_FONT[y][x];
					index = x * 32 + y * 256 * 4;
					for (int z = 0; z < 8; ++z) {
						if ((t >> z) & 1) {
							data[index++] = 255;
							data[index++] = 255;
							data[index++] = 255;
							data[index++] = 255;
						}
						else {
							data[index++] = 0;
							data[index++] = 0;
							data[index++] = 0;
							data[index++] = 0;
						}
					}
				}
			}

			ds::TextureInfo texInfo = { 256, 256, 4, data, ds::TextureFormat::R8G8B8A8_UNORM, ds::BindFlag::BF_SHADER_RESOURCE };
			RID textureID = ds::createTexture(texInfo);
			SpriteBatchBufferInfo sbbInfo = { 4096,textureID, ds::TextureFilters::POINT };
			ctx->sprites = new SpriteBatchBuffer(sbbInfo);
			delete[] data;

			return ctx;
		}

		void reset(UIContext* ctx) {
			ctx->buffer->num = 0;
			ctx->size = p2i(0);
		}

		void draw_buffer(UIContext* ctx) {
			ctx->sprites->begin();
			float h = ctx->size.y - 10.0f;
			float sy = h / 128.0f;
			float w = ctx->size.x + 20.0f;
			float sx = w / 128.0f;

			float bpx = ctx->startPos.x + w * 0.5f;
			float bpy = ctx->startPos.y - h * 0.5f;

			int num = ctx->buffer->num;
			for (uint16_t i = 0; i < num; ++i) {
				ds::vec2 p = ctx->buffer->positions[i];
				if (ctx->buffer->resize[i] == ResizeType::RT_X) {
					ds::vec4 rect = ctx->buffer->rectangles[i];
					rect.z = 128.0f;
					p.x = bpx;
					ctx->sprites->add(p, rect, ds::vec2(sx, 1.0f), 0.0f, ctx->buffer->colors[i]);
				}
				else if (ctx->buffer->resize[i] == ResizeType::RT_BOTH) {
					ds::vec4 rect = ctx->buffer->rectangles[i];
					rect.z = 128.0f;
					rect.w = 128.0f;
					p.x = bpx;
					p.y = bpy;
					ctx->sprites->add(p, rect, ds::vec2(sx, sy), 0.0f, ctx->buffer->colors[i]);
				}
				else {
					// FIXME: take from settings
					p.x += 5.0f;// _guiCtx->itemOffset;
					ctx->sprites->add(p, ctx->buffer->rectangles[i], ctx->buffer->scales[i], 0.0f, ctx->buffer->colors[i]);
				}
			}
			ctx->buffer->num = 0;
			ctx->sprites->flush();
		}

		void add_to_buffer(UIContext* ctx, const p2i& p, const ds::vec4& rect, const ds::vec2& scale, const ds::Color& color, ResizeType resize = ResizeType::RT_NONE) {
			if ((ctx->buffer->num + 1) >= ctx->buffer->capacity) {
				draw_buffer(ctx);
			}
			ctx->buffer->positions[ctx->buffer->num] = ds::vec2(p.x,p.y);
			ctx->buffer->scales[ctx->buffer->num] = scale;
			ctx->buffer->rectangles[ctx->buffer->num] = rect;
			ctx->buffer->colors[ctx->buffer->num] = color;
			ctx->buffer->resize[ctx->buffer->num] = resize;
			++ctx->buffer->num;
		}

		// --------------------------------------------------------
		// internal add text method
		// --------------------------------------------------------
		void add_text(UIContext* ctx, const p2i& pos, const char* text, float xoffset = 10, const ds::Color& color = ds::Color(1.0f, 1.0f, 1.0f, 1.0f)) {
			int l = strlen(text);
			p2i p = pos;
			p.x += xoffset;
			float w = 0.0f;
			for (int i = 0; i < l; ++i) {
				int idx = (int)text[i] - 32;
				if (idx >= 0 && idx < 127) {
					int c = text[i] - 32;
					int t = c / 12;
					int y = t * 16;
					int x = (c - t * 12) * 10;
					ds::vec4 rect = ds::vec4(x, y, 8, 14);
					add_to_buffer(ctx, p, rect, ds::vec2(1, 1), color, ResizeType::RT_NONE);
					p.x += rect.z;
					w += rect.z;
				}
			}
			if (w > ctx->size.x) {
				ctx->size.x = w;
			}
		}

		// --------------------------------------------------------
		// internal add box
		// --------------------------------------------------------
		void add_box(UIContext* ctx, const p2i& p, const p2i& size, const ds::Color& color, ResizeType resize = ResizeType::RT_NONE) {
			p2i pos = p;
			pos.x += size.x * 0.5f;
			ds::vec2 scale = ds::vec2(1.0f, 1.0f);
			p2i sz = size;
			if (size.x > WHITE_RECT.z) {
				sz.x = WHITE_RECT.z;
				scale.x = size.x / WHITE_RECT.z;
			}
			if (size.y > WHITE_RECT.w) {
				sz.y = WHITE_RECT.w;
				scale.y = size.y / WHITE_RECT.w;
			}
			add_to_buffer(ctx, pos, ds::vec4(256.0f, 0.0f, sz.x, sz.y), scale, color, resize);
			/*
			if ((p.x + size.x) > ctx->size.x) {
				ctx->size.x = (p.x + size.x);
			}
			*/
			if (size.x > ctx->size.x) {
				ctx->size.x = size.x;
			}
		}

		void add_box(UIContext* ctx, const p2i& p, int width, int height, const ds::Color& color, ResizeType resize = ResizeType::RT_NONE) {
			add_box(ctx, p, p2i(width, height), color, resize);
		}

		void shutdown(UIContext* ctx) {
			delete ctx->sprites;
			delete[] ctx->buffer->data;
			delete ctx->buffer;
		}

	}
	static const IMGUISettings DEFAULT_SETTINGS = {
		ds::Color(40,117,114, 255), // header
		ds::Color(68, 68, 68, 255), // button
		ds::Color(16, 16, 16, 255), // background
		ds::Color(32, 32, 32, 255), // label 
		ds::Color(0, 192, 0, 255), // enabled
		ds::Color(192, 0, 0, 255), // disabled
		ds::Color(64,64,64,255), // active input
		ds::Color(32,32,32,255), // input
		ds::Color(38,38,38,255), // box background color
		ds::Color(167,77,75,255), // box selection color
		ds::Color(96,96,96,255), // slider color
		ds::Color(255,0,255,255), // scroll slider color
		5.0f // line spacing
	};

	struct GUIContext {
		p2i mousePosition;
		bool mouseDown;
		HashedId activeItem;
		HashedId hotItem;
		int keyInput[256];
		int keys[32];
		int numKeys;
		p2i startPos;
		p2i currentPos;
		bool grouping;
		char inputText[32];
		char tmpBuffer[256];
		int caretPos;
		p2i size;
		float itemOffset;
		IMGUISettings settings;
		renderer::UIContext* uiContext;
	};

	static GUIContext* _guiCtx = 0;

	// -------------------------------------------------------
	// check if mouse cursor is inside box
	// -------------------------------------------------------
	/*
	static bool isCursorInside(const ds::vec2& p, const ds::vec2& dim) {
		ds::vec2 mp = _guiCtx->mousePos;
		if (mp.x < (p.x - dim.x * 0.5f)) {
			return false;
		}
		if (mp.x >(p.x + dim.x * 0.5f)) {
			return false;
		}
		if (mp.y < (p.y - dim.y * 0.5f)) {
			return false;
		}
		if (mp.y >(p.y + dim.y * 0.5f)) {
			return false;
		}
		return true;
	}
	*/
	// -------------------------------------------------------
	// check if mouse cursor is inside box
	// -------------------------------------------------------
	static bool isHot(const p2i& p, const p2i& dim) {
		p2i mp = _guiCtx->mousePosition;
		int half_y = dim.y / 2;
		if (mp.x < p.x) {
			return false;
		}
		if (mp.x >(p.x + dim.x)) {
			return false;
		}
		if (mp.y < (p.y - half_y)) {
			return false;
		}
		if (mp.y >(p.y + half_y)) {
			return false;
		}
		return true;
	}

	// -------------------------------------------------------
	// handle mouse interaction
	// -------------------------------------------------------
	/*
	static bool isClicked(const ds::vec2& pos, const ds::vec2& size) {
		if (_guiCtx->clicked) {
			ds::vec2 p = pos;
			p.x += size.x * 0.5f + _guiCtx->itemOffset;
			if (_guiCtx->clicked && isCursorInside(p, size)) {
				return true;
			}
		}
		return false;
	}
	*/
	// -------------------------------------------------------
	// handle mouse interaction
	// -------------------------------------------------------
	/*
	static bool isHovered(const ds::vec2& pos, const ds::vec2& size) {
		ds::vec2 p = pos;
		p.x += size.x * 0.5f + _guiCtx->itemOffset;
		return isCursorInside(p, size);
	}
	*/
	// -------------------------------------------------------
	// is box selected
	// -------------------------------------------------------
	/*
	static bool isBoxSelected(HashedId id, const ds::vec2& pos, const ds::vec2& size, bool setActive = true) {
		if (_guiCtx->clicked) {
			ds::vec2 p = pos;
			p.x += size.x * 0.5f + _guiCtx->itemOffset;
			if (_guiCtx->clicked && isCursorInside(p, size)) {
				if (setActive) {
					_guiCtx->active = id;
					_guiCtx->clicked = false;
					_guiCtx->activeGroup = _guiCtx->currentGoupIndex;
				}
				return true;
			}
		}
		return false;
	}
	*/
	// -------------------------------------------------------
	// determine text size
	// -------------------------------------------------------
	p2i textSize(const char* txt) {
		int l = strlen(txt);
		p2i p(0);
		for (int i = 0; i < l; ++i) {
			int idx = (int)txt[i] - 32;
			if (idx >= 0 && idx < 127) {
				int c = idx / 16;
				int r = c - idx * 16;
				ds::vec4 rect = ds::vec4(r, c, 7, 10);
				p.x += rect.z;
				if (rect.w > p.y) {
					p.y = rect.w;
				}
			}
		}
		return p;
	}

	// -------------------------------------------------------
	// determine text size
	// -------------------------------------------------------
	p2i limitedTextSize(const char* txt, int maxLength) {
		int l = strlen(txt);
		if (l > maxLength) {
			l = maxLength;
		}
		p2i p(0);
		for (int i = 0; i < l; ++i) {
			int idx = (int)txt[i] - 32;
			if (idx >= 0 && idx < 127) {
				int c = idx / 16;
				int r = c - idx * 16;
				ds::vec4 rect = ds::vec4(r, c, 7, 10);
				p.x += rect.z;
				if (rect.w > p.y) {
					p.y = rect.w;
				}
			}
		}
		return p;
	}

	// -------------------------------------------------------
	// initialize GUI
	// -------------------------------------------------------
	void init(IMGUISettings* settings) {
		_guiCtx = new GUIContext;	
		_guiCtx->uiContext = renderer::init();
		//_guiCtx->clicked = false;
		//_guiCtx->buttonPressed = false;
		for (int i = 0; i < 255; ++i) {
			_guiCtx->keyInput[i] = 0;
		}		
		_guiCtx->grouping = false;
		_guiCtx->numKeys = 0;
		//_guiCtx->currentGoupIndex = 0;
		//_guiCtx->activeGroup = -1;
		_guiCtx->itemOffset = 10.0f;
		if (settings != 0) {
			_guiCtx->settings = *settings;
		}
		else {
			_guiCtx->settings = DEFAULT_SETTINGS;
		}
		_guiCtx->hotItem = 0;
		_guiCtx->activeItem = 0;
	}

	// --------------------------------------------------------
	// move current position to next position
	// --------------------------------------------------------
	static void moveForward(const p2i& dim) {
		if (_guiCtx->grouping) {
			_guiCtx->currentPos.x += dim.x + 10;
		}
		else {
			_guiCtx->currentPos.y -= dim.y;
			_guiCtx->size.y += dim.y;
			_guiCtx->currentPos.x = _guiCtx->startPos.x;
		}
		//++_guiCtx->currentGoupIndex;
	}

	// -------------------------------------------------------
	// handle text input
	// -------------------------------------------------------
	// FIXME: provide numeric input only
	static bool handleTextInput(bool numeric = false) {
		bool ret = false;
		int len = strlen(_guiCtx->inputText);
		for (int i = 0; i < ds::getNumInputKeys(); ++i) {
			const ds::InputKey& key = ds::getInputKey(i);
			if (key.type == ds::IKT_SYSTEM) {
				if (key.value == ds::SpecialKeys::DSKEY_Backspace) {
					if (_guiCtx->caretPos > 0) {
						if (_guiCtx->caretPos < len) {
							memmove(_guiCtx->inputText + _guiCtx->caretPos - 1, _guiCtx->inputText + _guiCtx->caretPos, len - _guiCtx->caretPos);
						}
						--_guiCtx->caretPos;
						--len;
						_guiCtx->inputText[len] = '\0';

					}
				}
				else if (key.value == ds::SpecialKeys::DSKEY_LeftArrow) {
					if (_guiCtx->caretPos > 0) {
						--_guiCtx->caretPos;
					}
				}
				else if (key.value == ds::SpecialKeys::DSKEY_RightArrow) {
					if (_guiCtx->caretPos < strlen(_guiCtx->inputText)) {
						++_guiCtx->caretPos;
					}
				}
				else if (key.value == ds::SpecialKeys::DSKEY_Home) {
					_guiCtx->caretPos = 0;
				}
				else if (key.value == ds::SpecialKeys::DSKEY_End) {
					_guiCtx->caretPos = strlen(_guiCtx->inputText);
				}
				else if (key.value == ds::SpecialKeys::DSKEY_Enter) {
					ret = true;
				}
				else if (key.value == ds::SpecialKeys::DSKEY_Delete) {
					if (len > 0) {
						if (_guiCtx->caretPos < len) {
							memmove(_guiCtx->inputText + _guiCtx->caretPos, _guiCtx->inputText + _guiCtx->caretPos + 1, len - _guiCtx->caretPos);
							--len;
							_guiCtx->inputText[len] = '\0';
						}
					}
				}
			}
			else {
				if ((key.value > 47 && key.value < 128) || key.value == '.' || key.value == '-') {
					if (len < 32) {
						if (_guiCtx->caretPos < len) {
							memmove(_guiCtx->inputText + _guiCtx->caretPos + 1, _guiCtx->inputText + _guiCtx->caretPos, len - _guiCtx->caretPos);
						}
						if (numeric) {
							if ((key.value > 47 && key.value < 58) || key.value == '.' || key.value == '-') {
								_guiCtx->inputText[_guiCtx->caretPos] = key.value;
								++len;
								++_guiCtx->caretPos;
							}
						}
						else {
							_guiCtx->inputText[_guiCtx->caretPos] = key.value;
							++len;
							++_guiCtx->caretPos;
						}
					}
				}
			}
			_guiCtx->inputText[len] = '\0';
		}
		//++len;		
		return ret;
	}
	// -------------------------------------------------------
	// input scalar
	// -------------------------------------------------------
	static bool InputScalar(int id, int index, char* v, int maxLength, float width = 150.0f) {
		HashedId new_id = id + 1024 * index;
		bool ret = false;
		p2i p = _guiCtx->currentPos;
		bool selected = isHot(p, p2i(width, 20));
		if (selected) {
			sprintf_s(_guiCtx->inputText, 32, "%s", v);
			_guiCtx->caretPos = strlen(_guiCtx->inputText);
			_guiCtx->activeItem = new_id;
		}
		if (_guiCtx->activeItem == new_id) {
			renderer::add_box(_guiCtx->uiContext, p, p2i(width, 16), _guiCtx->settings.activeInputBoxColor);
			ret = handleTextInput(false);
			strncpy(v, _guiCtx->inputText, maxLength);
			p2i textDim = textSize(_guiCtx->inputText);
			p2i cp = p;
			p2i cursorPos = limitedTextSize(_guiCtx->inputText, _guiCtx->caretPos);
			cp.x += (width - textDim.x) * 0.5f + cursorPos.x - 2.0f;
			cp.y -= 6.0f;
			renderer::add_box(_guiCtx->uiContext, cp, p2i(10, 3), ds::Color(192, 0, 192, 255));
			p.x += (width - textDim.x) * 0.5f;
			p.y -= 1.0f;
			renderer::add_text(_guiCtx->uiContext, p, _guiCtx->inputText, 0.0f);
			sprintf_s(v, maxLength, "%s", _guiCtx->inputText);
		}
		else {
			sprintf_s(_guiCtx->tmpBuffer, 64, "%s", v);
			renderer::add_box(_guiCtx->uiContext, p, p2i(width, 16), _guiCtx->settings.inputBoxColor);
			p2i textDim = textSize(_guiCtx->tmpBuffer);
			p.y -= 1.0f;
			p.x += (width - textDim.x) * 0.5f;
			renderer::add_text(_guiCtx->uiContext, p, _guiCtx->tmpBuffer, 0.0f);
		}
		return ret;
	}

	// -------------------------------------------------------
	// internal input float
	// -------------------------------------------------------
	static bool InputScalar(int id, int index, float* v, const char* format = "%g", float width = 100.0f) {
		HashedId new_id = id + 1024 * index;
		bool ret = false;
		p2i p = _guiCtx->currentPos;
		if (index > 0) {
			p.x += index * width + index * 10;
		}
		bool selected = isHot(p, p2i(width, 20));
		if (selected) {
			sprintf_s(_guiCtx->inputText, 32, format, *v);
			_guiCtx->caretPos = strlen(_guiCtx->inputText);
			_guiCtx->activeItem = new_id;
		}
		if (_guiCtx->activeItem == new_id) {
			renderer::add_box(_guiCtx->uiContext, p, width, 16,_guiCtx->settings.activeInputBoxColor);
			ret = handleTextInput(true);
			*v = static_cast<float>(atof(_guiCtx->inputText));
			p2i textDim = textSize(_guiCtx->inputText);
			p2i cp = p;
			p2i cursorPos = limitedTextSize(_guiCtx->inputText, _guiCtx->caretPos);
			cp.x += (width - textDim.x) / 2 + cursorPos.x - 2;
			cp.y -= 6.0f;
			renderer::add_box(_guiCtx->uiContext, cp, p2i(10, 3), ds::Color(192, 0, 192, 255));
			p.x += (width - textDim.x) * 0.5f;
			p.y -= 1.0f;
			renderer::add_text(_guiCtx->uiContext, p, _guiCtx->inputText, 0.0f);
		}
		else {
			sprintf_s(_guiCtx->tmpBuffer, 64, format, *v);
			p2i textDim = textSize(_guiCtx->tmpBuffer);
			renderer::add_box(_guiCtx->uiContext, p, width, 16, _guiCtx->settings.inputBoxColor);
			p.y -= 1.0f;
			p.x += (width - textDim.x) * 0.5f;
			renderer::add_text(_guiCtx->uiContext, p, _guiCtx->tmpBuffer, 0.0f);
		}
		return ret;
	}

	// --------------------------------------------------------
	// start
	// --------------------------------------------------------
	void start() {
		renderer::reset(_guiCtx->uiContext);
		ds::vec2 mp = ds::getMousePosition();
		_guiCtx->mousePosition = p2i(mp.x, mp.y);
		_guiCtx->hotItem = 0;
		if (ds::isMouseButtonPressed(0) && !_guiCtx->mouseDown) {
			_guiCtx->mouseDown = true;			
		}
		if (!ds::isMouseButtonPressed(0) && _guiCtx->mouseDown) {
			_guiCtx->mouseDown = false;
		}
		//_guiCtx->startPos = startPos;
		//_guiCtx->uiContext->startPos = startPos;
		//_guiCtx->currentPos = startPos;
		_guiCtx->size = p2i(0);
		_guiCtx->grouping = false;
	}

	// --------------------------------------------------------
	// begin
	// --------------------------------------------------------
	/*
	void start(const ds::vec2& pos) {
		renderer::reset(_guiCtx->uiContext);
		_guiCtx->startPos = pos;
		_guiCtx->uiContext->startPos = pos;
		_guiCtx->currentPos = pos;
		_guiCtx->size = ds::vec2(0.0f, 0.0f);
		//_guiCtx->uiContext->size = ds::vec2(0.0f, 0.0f);
		_guiCtx->grouping = false;
		_guiCtx->mousePos = ds::getMousePosition();
		if (_guiCtx->clicked) {
			_guiCtx->clicked = false;
		}
		if (ds::isMouseButtonPressed(0)) {
			_guiCtx->buttonPressed = true;
		}
		else {
			if (_guiCtx->buttonPressed) {
				_guiCtx->clicked = true;				
			}
			_guiCtx->buttonPressed = false;
		}
	}
	*/

	// --------------------------------------------------------
	// begin with header
	// --------------------------------------------------------
	/*
	void begin(const char* header, int* state) {
		_guiCtx->currentGoupIndex = 0;
		ds::vec2 pos = _guiCtx->currentPos;
		renderer::add_box(_guiCtx->uiContext, pos, 256, 256, _guiCtx->settings.backgroundColor, renderer::ResizeType::RT_BOTH);
		renderer::add_box(_guiCtx->uiContext, pos, 10, 20, _guiCtx->settings.headerBoxColor, renderer::ResizeType::RT_X);
		pos.x += 30.0f;
		renderer::add_text(_guiCtx->uiContext, pos, header);

		pos = _guiCtx->currentPos;
		pos.x -= 10.0f;
		renderer::add_box(_guiCtx->uiContext, pos, 20, 20, _guiCtx->settings.buttonColor);
		if (*state == 0) {
			renderer::add_text(_guiCtx->uiContext, pos, "+");
		}
		else {
			renderer::add_text(_guiCtx->uiContext, pos, "-");
		}
		if (isClicked(pos, ds::vec2(20, 20))) {
			if (*state == 0) {
				*state = 1;
			}
			else {
				*state = 0;
			}
		}
		float advance = 20.0f + _guiCtx->settings.lineSpacing;
		moveForward(ds::vec2(10.0f, advance));
	}
	*/
	// --------------------------------------------------------
	// begin with header
	// --------------------------------------------------------
	void begin(const char* header, int* state, p2i* position, int width) {
		//_guiCtx->currentGoupIndex = 0;
		
		_guiCtx->currentPos = *position;
		_guiCtx->startPos = _guiCtx->currentPos;
		_guiCtx->uiContext->startPos = _guiCtx->currentPos;
		_guiCtx->currentPos = _guiCtx->currentPos;
		p2i pos = _guiCtx->currentPos;
		
		renderer::add_box(_guiCtx->uiContext, pos, width, 256, _guiCtx->settings.backgroundColor, renderer::ResizeType::RT_Y);
		renderer::add_box(_guiCtx->uiContext, pos, width, 20, _guiCtx->settings.headerBoxColor, renderer::ResizeType::RT_X);
		pos.x += 30.0f;
		renderer::add_text(_guiCtx->uiContext, pos, header);

		pos = _guiCtx->currentPos;
		pos.x -= 10.0f;
		renderer::add_box(_guiCtx->uiContext, pos, 20, 20, _guiCtx->settings.buttonColor);
		if (*state == 0) {
			renderer::add_text(_guiCtx->uiContext, pos, "+");
		}
		else {
			renderer::add_text(_guiCtx->uiContext, pos, "-");
		}
		/*
		if (isClicked(pos, ds::vec2(20, 20))) {
			if (*state == 0) {
				*state = 1;
			}
			else {
				*state = 0;
			}
		}
		*/
		//else 
		if (isHot(*position, p2i(width, 20))) {
			if (_guiCtx->mouseDown) {
				position->x = _guiCtx->mousePosition.x - width / 2;
				position->y = _guiCtx->mousePosition.y;
			}
		}
		float advance = 20.0f + _guiCtx->settings.lineSpacing;
		moveForward(p2i(10, advance));
	}

	// --------------------------------------------------------
	// begin grouping
	// --------------------------------------------------------
	void beginGroup() {
		_guiCtx->grouping = true;
	}

	// --------------------------------------------------------
	// end grouping
	// --------------------------------------------------------
	void endGroup() {
		_guiCtx->grouping = false;
		moveForward(p2i(10, 30));
	}

	// --------------------------------------------------------
	// Button
	// --------------------------------------------------------
	bool Button(const char* text) {
		HashedId id = HashPointer(text);
		p2i p = _guiCtx->currentPos;
		if (isHot(p2i(p.x, p.y), p2i(120, 20))) {
			_guiCtx->hotItem = id;
			if (_guiCtx->activeItem == 0 && _guiCtx->mouseDown) {
				_guiCtx->activeItem = id;
			}
		}
		renderer::add_box(_guiCtx->uiContext, p, 120, 20, _guiCtx->settings.buttonColor);
		p2i dim = p2i(120, 20);
		p2i textDim = textSize(text);
		p.x += (120 - textDim.x) / 2;
		renderer::add_text(_guiCtx->uiContext, p, text, 0.0f);
		dim.y = 20;
		p2i buttonPos = _guiCtx->currentPos;
		p2i m = dim;
		m.y += _guiCtx->settings.lineSpacing;
		moveForward(m);
		
		//return isClicked(buttonPos, dim);
		if (!_guiCtx->mouseDown && _guiCtx->hotItem == id && _guiCtx->activeItem == id) {
			return true;
		}
		return false;
	}

	// --------------------------------------------------------
	// Button
	// --------------------------------------------------------
	void Checkbox(const char* label, bool* state) {
		p2i p = _guiCtx->currentPos;
		renderer::add_box(_guiCtx->uiContext, p, p2i(16, 16),_guiCtx->settings.buttonColor);
		p.x += 4;
		if (*state) {			
			renderer::add_box(_guiCtx->uiContext, p, 8, 8, _guiCtx->settings.enabledBoxColor);
		}
		else {
			renderer::add_box(_guiCtx->uiContext, p, 8, 8, _guiCtx->settings.disabledBoxColor);
		}
		/*
		if (isClicked(_guiCtx->currentPos, ds::vec2(16, 16))) {
			*state = !*state;
		}
		*/
		p = _guiCtx->currentPos;
		p.x += 20;
		renderer::add_text(_guiCtx->uiContext, p, label);
		int advance = 16 + _guiCtx->settings.lineSpacing;
		moveForward(p2i(40, advance));
	}

	// --------------------------------------------------------
	// Label
	// --------------------------------------------------------
	void Label(const char* label, const char* text) {
		HashedId id = HashPointer(label);

		renderer::add_box(_guiCtx->uiContext, _guiCtx->currentPos, p2i(150, 20), _guiCtx->settings.labelBoxColor);
		p2i cp = p2i(_guiCtx->currentPos.x, _guiCtx->currentPos.y);
		if (isHot(cp, p2i(150, 20))) {
			_guiCtx->hotItem = id;
		}
		p2i textDim = textSize(text);

		p2i textPos = _guiCtx->currentPos;
		textPos.x += (160 - textDim.x) / 2;
		renderer::add_text(_guiCtx->uiContext, textPos, text, 0.0f);

		p2i labelPos = _guiCtx->currentPos;
		labelPos.x += 160;
		renderer::add_text(_guiCtx->uiContext, labelPos, label);
		p2i size = textSize(label);
		moveForward(p2i(size.x, 30));
	}

	// --------------------------------------------------------
	// Value - vec2
	// --------------------------------------------------------
	void Value(const char* label, const ds::vec2& v) {
		sprintf_s(_guiCtx->tmpBuffer, 256, "%g %g", v.x, v.y);
		Label(label, _guiCtx->tmpBuffer);
	}

	// --------------------------------------------------------
	// Value - int
	// --------------------------------------------------------
	void Value(const char* label, uint32_t v) {
		sprintf_s(_guiCtx->tmpBuffer, 256, "%d", v);
		Label(label, _guiCtx->tmpBuffer);
	}

	// --------------------------------------------------------
	// Value - uint32_t
	// --------------------------------------------------------
	void Value(const char* label, int v) {
		sprintf_s(_guiCtx->tmpBuffer, 256, "%d", v);
		Label(label, _guiCtx->tmpBuffer);
	}

	// --------------------------------------------------------
	// Value - float
	// --------------------------------------------------------
	void Value(const char* label, float v) {
		sprintf_s(_guiCtx->tmpBuffer, 256, "%g", v);
		Label(label, _guiCtx->tmpBuffer);
	}

	// --------------------------------------------------------
	// Value - float
	// --------------------------------------------------------
	void Value(const char* label, float v, const char* format) {
		sprintf_s(_guiCtx->tmpBuffer, 256, format, v);
		Label(label, _guiCtx->tmpBuffer);
	}

	// --------------------------------------------------------
	// Value - vec3
	// --------------------------------------------------------
	void Value(const char* label, const ds::vec3& v) {
		sprintf_s(_guiCtx->tmpBuffer, 256, "%g %g %g", v.x, v.y, v.z);
		Label(label, _guiCtx->tmpBuffer);
	}

	// --------------------------------------------------------
	// Value - p2i
	// --------------------------------------------------------
	void Value(const char* label, const p2i& v) {
		sprintf_s(_guiCtx->tmpBuffer, 256, "%d %d", v.x, v.y);
		Label(label, _guiCtx->tmpBuffer);
	}

	// --------------------------------------------------------
	// Value - bool
	// --------------------------------------------------------
	void Value(const char* label, bool v) {
		if (v) {
			sprintf_s(_guiCtx->tmpBuffer, 256, "true");
		}
		else {
			sprintf_s(_guiCtx->tmpBuffer, 256, "false");
		}
		Label(label, _guiCtx->tmpBuffer);
	}

	// --------------------------------------------------------
	// Value - vec4
	// --------------------------------------------------------
	void Value(const char* label, const ds::vec4& v) {
		sprintf_s(_guiCtx->tmpBuffer, 256, "%g %g %g %g", v.x, v.y, v.z, v.w);
		Label(label, _guiCtx->tmpBuffer);
	}

	// --------------------------------------------------------
	// Value - color
	// --------------------------------------------------------
	void Value(const char* label, const ds::Color& v) {
		int r = static_cast<int>(v.r * 255.0f);
		int g = static_cast<int>(v.g * 255.0f);
		int b = static_cast<int>(v.b * 255.0f);
		int a = static_cast<int>(v.a * 255.0f);
		sprintf_s(_guiCtx->tmpBuffer, 256, "%d %d %d %d", r, g, b, a);
		p2i p = _guiCtx->currentPos;
		p2i textDim = textSize(label);
		p.x = _guiCtx->currentPos.x + textDim.x + 200;
		renderer::add_box(_guiCtx->uiContext, p, p2i(20, 20), v);
		Label(label, _guiCtx->tmpBuffer);

	}

	// --------------------------------------------------------
	// Text
	// --------------------------------------------------------
	void Text(const char* text) {
		size_t l = strlen(text);
		p2i p = _guiCtx->currentPos;
		renderer::add_text(_guiCtx->uiContext, p, text);
		p2i size = textSize(text);
		moveForward(p2i(size.x, 30));
	}

	// --------------------------------------------------------
	// Formatted text
	// --------------------------------------------------------
	void FormattedText(const char* fmt, ...) {
		char buffer[1024];
		va_list args;
		va_start(args, fmt);
		vsprintf(buffer, fmt, args);
		Text(buffer);
		va_end(args);

	}

	// --------------------------------------------------------
	// Message
	// --------------------------------------------------------
	void Message(const char* fmt, ...) {
		p2i pos = _guiCtx->currentPos;
		renderer::add_box(_guiCtx->uiContext, pos, 10, 20, _guiCtx->settings.labelBoxColor, renderer::ResizeType::RT_X);
		char buffer[1024];
		va_list args;
		va_start(args, fmt);
		vsprintf(buffer, fmt, args);
		Text(buffer);
		va_end(args);

	}

	// -------------------------------------------------------
	// input string
	// -------------------------------------------------------
	bool Input(const char* label, char* str, int maxLength) {
		HashedId id = HashPointer(str);
		p2i p = _guiCtx->currentPos;
		p.x += 160.0f;
		renderer::add_text(_guiCtx->uiContext, p, label);
		bool ret = InputScalar(id, 0, str, maxLength, 150.0f);
		moveForward(p2i(150, 22));
		return ret;
	}

	// -------------------------------------------------------
	// input int
	// -------------------------------------------------------
	void Input(const char* label, int* v) {
		HashedId id = HashPointer(v);
		p2i p = _guiCtx->currentPos;
		p.x += 160;
		renderer::add_text(_guiCtx->uiContext, p, label);
		float f = static_cast<float>(*v);
		bool ret = InputScalar(id, 0, &f, "%.0f", 150.0f);
		*v = static_cast<int>(f);
		p2i ts = textSize(label);
		moveForward(p2i(150 + ts.x + 10, 22));
	}

	// -------------------------------------------------------
	// input float
	// -------------------------------------------------------
	void Input(const char* label, float* v) {
		HashedId id = HashPointer(v);
		p2i p = _guiCtx->currentPos;
		p.x += 160.0f;
		renderer::add_text(_guiCtx->uiContext, p, label);
		bool ret = InputScalar(id, 0, v, "%g", 150.0f);
		p2i ts = textSize(label);
		moveForward(p2i(150 + ts.x + 10, 22));
	}

	// -------------------------------------------------------
	// input vec2
	// -------------------------------------------------------
	void Input(const char* label, ds::vec2* v) {
		HashedId id = HashPointer(v);
		p2i p = _guiCtx->currentPos;
		p.x += 160;
		renderer::add_text(_guiCtx->uiContext, p, label);
		InputScalar(id, 0, &v->x, "%g", 70.0f);
		bool ret = InputScalar(id, 1, &v->y, "%g", 70.0f);
		p2i ts = textSize(label);
		moveForward(p2i(150 + ts.x + 10, 22));
	}

	// -------------------------------------------------------
	// input vec3
	// -------------------------------------------------------
	void Input(const char* label, ds::vec3* v) {
		HashedId id = HashPointer(v);
		p2i p = _guiCtx->currentPos;
		p.x += 240;
		renderer::add_text(_guiCtx->uiContext, p, label);
		InputScalar(id, 0, &v->x, "%g", 70.0f);
		InputScalar(id, 1, &v->y, "%g", 70.0f);
		InputScalar(id, 2, &v->z, "%g", 70.0f);
		p2i ts = textSize(label);
		moveForward(p2i(150 + ts.x + 10, 22));
	}

	// -------------------------------------------------------
	// input vec4
	// -------------------------------------------------------
	void Input(const char* label, ds::vec4* v) {
		HashedId id = HashPointer(v);
		p2i p = _guiCtx->currentPos;
		p.x += 320;
		renderer::add_text(_guiCtx->uiContext, p, label);
		InputScalar(id, 0, &v->x, "%g", 70.0f);
		InputScalar(id, 1, &v->y, "%g", 70.0f);
		InputScalar(id, 2, &v->z, "%g", 70.0f);
		InputScalar(id, 3, &v->w, "%g", 70.0f);
		p2i ts = textSize(label);
		moveForward(p2i(150 + ts.x + 10, 22));
	}

	// -------------------------------------------------------
	// Slider
	// -------------------------------------------------------	
	void ColorSlider(float* v, char prefix, int index, float width) {
		HashedId id = HashPointer(v);
		p2i p = _guiCtx->currentPos;
		p.x += index * (width + 10);
		renderer::add_box(_guiCtx->uiContext, p, p2i(width, 20), _guiCtx->settings.labelBoxColor);
		// calculate offset
		int val = *v * 255.0f;
		int d = 255;
		/*
		if (isClicked(p, ds::vec2(width, 20.0f))) {
			ds::vec2 mp = ds::getMousePosition();
			float dx = mp.x - p.x - _guiCtx->itemOffset;
			val = static_cast<int>(dx * d / width);
		}
		if (_guiCtx->buttonPressed && isHovered(p, ds::vec2(width, 20.0f))) {
			ds::vec2 mp = ds::getMousePosition();
			float dx = mp.x - p.x - _guiCtx->itemOffset;
			val = static_cast<int>(dx * d / width);
		}
		if (val < 0) {
			val = 0;
		}
		if (val > 255) {
			val = 255;
		}
		*/
		p.x += static_cast<float>(val) / static_cast<float>(d) * width;
		p.x -= 4.0f;
		renderer::add_box(_guiCtx->uiContext, p, p2i(8, 24), _guiCtx->settings.sliderColor);
		p = _guiCtx->currentPos;
		p.x += index * (width + 10.0f);
		sprintf_s(_guiCtx->tmpBuffer, 256, "%c: %d", prefix, val);
		p2i textDim = textSize(_guiCtx->tmpBuffer);
		p.x += (width - textDim.x) * 0.5f;
		renderer::add_text(_guiCtx->uiContext, p, _guiCtx->tmpBuffer,0.0f);
		*v = static_cast<float>(val) / 255.0f;
	}

	// -------------------------------------------------------
	// input color
	// -------------------------------------------------------
	void Input(const char* label, ds::Color* v) {
		HashedId id = HashPointer(v);
		ColorSlider(&v->r, 'R', 0, 70.0f);
		ColorSlider(&v->g, 'G', 1, 70.0f);
		ColorSlider(&v->b, 'B', 2, 70.0f);
		ColorSlider(&v->a, 'A', 3, 70.0f);
		p2i p = _guiCtx->currentPos;
		p.x = _guiCtx->currentPos.x + 320;
		renderer::add_box(_guiCtx->uiContext, p, p2i(20, 20), *v);
		p.x = _guiCtx->currentPos.x + 350;
		renderer::add_text(_guiCtx->uiContext, p, label);
		p2i ts = textSize(label);
		moveForward(p2i(150 + ts.x + 10, 22));
	}

	// -------------------------------------------------------
	// Separator
	// -------------------------------------------------------	
	void Separator() {
		p2i p = _guiCtx->currentPos;
		p.y += 8;
		renderer::add_box(_guiCtx->uiContext, p, p2i(10, 4), ds::Color(32, 32, 32, 255), renderer::ResizeType::RT_X);
		moveForward(p2i(10, 16));
	}

	// -------------------------------------------------------
	// int input using steps
	// -------------------------------------------------------
	void StepInput(const char* label, int* v, int minValue, int maxValue, int steps) {
		HashedId id = HashPointer(v);
		p2i p = _guiCtx->currentPos;
		// -
		renderer::add_box(_guiCtx->uiContext, p, 20, 20, _guiCtx->settings.buttonColor);
		/*
		if (isClicked(p, ds::vec2(20, 20))) {
			*v -= steps;
			if (*v < minValue) {
				*v = minValue;
			}
		}
		*/
		renderer::add_text(_guiCtx->uiContext, p, "-");
		// value
		sprintf_s(_guiCtx->tmpBuffer, 256, "%d", *v);
		p2i textDim = textSize(_guiCtx->tmpBuffer);
		p = _guiCtx->currentPos;
		p.x += 20;
		renderer::add_box(_guiCtx->uiContext, p, p2i(110, 20), _guiCtx->settings.labelBoxColor);
		p.x += (110 - textDim.x) / 2;
		renderer::add_text(_guiCtx->uiContext, p, _guiCtx->tmpBuffer);
		// +
		p = _guiCtx->currentPos;
		p.x += 130;
		renderer::add_box(_guiCtx->uiContext, p,20, 20, _guiCtx->settings.buttonColor);
		/*
		if (isClicked(p, ds::vec2(20, 20))) {
			*v += steps;
			if (*v > maxValue) {
				*v = maxValue;
			}
		}
		*/
		renderer::add_text(_guiCtx->uiContext, p, "+");
		// label
		p.x += 30;
		renderer::add_text(_guiCtx->uiContext, p, label);
		p2i ts = textSize(label);
		moveForward(p2i(150 + ts.x + 30, 30));
	}

	// -------------------------------------------------------
	// Slider
	// -------------------------------------------------------	
	void Slider(const char* label, int* v, int minValue, int maxValue, float width) {
		HashedId id = HashPointer(v);
		p2i p = _guiCtx->currentPos;
		renderer::add_box(_guiCtx->uiContext, _guiCtx->currentPos, p2i(width, 20), _guiCtx->settings.labelBoxColor);
		// calculate offset
		int d = maxValue - minValue;
		/*
		if (isClicked(p, ds::vec2(width, 20.0f))) {
			ds::vec2 mp = ds::getMousePosition();
			float dx = mp.x - p.x - _guiCtx->itemOffset;
			*v = static_cast<int>(dx * d / width);
		}
		if (_guiCtx->buttonPressed && isHovered(p, ds::vec2(width, 20.0f))) {
			ds::vec2 mp = ds::getMousePosition();
			float dx = mp.x - p.x - _guiCtx->itemOffset;
			*v = static_cast<int>(dx * d / width);
		}
		*/
		if (*v < minValue) {
			*v = minValue;
		}
		if (*v > maxValue) {
			*v = maxValue;
		}
		p.x += static_cast<float>(*v) / static_cast<float>(d) * width;
		renderer::add_box(_guiCtx->uiContext, p, p2i(8, 28), _guiCtx->settings.sliderColor);
		p = _guiCtx->currentPos;
		sprintf_s(_guiCtx->tmpBuffer, 256, "%d", *v);
		p2i textDim = textSize(_guiCtx->tmpBuffer);
		p.x += (width - textDim.x) / 2;
		renderer::add_text(_guiCtx->uiContext, p, _guiCtx->tmpBuffer);
		p = _guiCtx->currentPos;
		p.x += width + 10;
		renderer::add_text(_guiCtx->uiContext, p, label);
		p2i ts = textSize(label);
		moveForward(p2i(width + ts.x + 20, 30));
	}

	// -------------------------------------------------------
	// Slider
	// -------------------------------------------------------	
	void Slider(const char* label, float* v, float minValue, float maxValue, int precision, float width) {
		HashedId id = HashPointer(v);
		p2i p = _guiCtx->currentPos;
		renderer::add_box(_guiCtx->uiContext, _guiCtx->currentPos, p2i(width, 20), _guiCtx->settings.buttonColor);
		// calculate offset
		float d = maxValue - minValue;
		/*
		if (isClicked(p, ds::vec2(width, 20.0f))) {
			ds::vec2 mp = ds::getMousePosition();
			float dx = mp.x - p.x - _guiCtx->itemOffset;
			*v = dx * d / width;
		}
		if (_guiCtx->buttonPressed && isHovered(p, ds::vec2(width, 20.0f))) {
			ds::vec2 mp = ds::getMousePosition();
			float dx = mp.x - p.x - _guiCtx->itemOffset;
			*v = dx * d / width;
		}
		*/
		if (*v < minValue) {
			*v = minValue;
		}
		if (*v > maxValue) {
			*v = maxValue;
		}
		if (precision > 0) {
			float prec = 10.0f * static_cast<float>(precision);
			*v = roundf(*v * prec) / prec;
		}
		p.x += *v / d * width;
		renderer::add_box(_guiCtx->uiContext, p, p2i(8, 28), _guiCtx->settings.sliderColor);
		p = _guiCtx->currentPos;
		sprintf_s(_guiCtx->tmpBuffer, 256, "%g", *v);
		p2i textDim = textSize(_guiCtx->tmpBuffer);
		p.x += (width - textDim.x) / 2;
		renderer::add_text(_guiCtx->uiContext, p, _guiCtx->tmpBuffer);
		p = _guiCtx->currentPos;
		p.x += width + 10;
		renderer::add_text(_guiCtx->uiContext, p, label);
		p2i ts = textSize(label);
		moveForward(p2i(width + ts.x + 20, 30));
	}

	void SliderAngle(const char* label, float* v, float width) {
		int d = static_cast<int>(*v / ds::TWO_PI * 360.0f);
		Slider(label, &d, 0, 360, width);
		*v = d / 360.0f * ds::TWO_PI;
	}

	// -------------------------------------------------------
	// Histogram
	// -------------------------------------------------------	
	void Histogram(float* values, int num, float minValue, float maxValue, float step, float width, float height) {
		p2i p = _guiCtx->currentPos;
		HashedId id = HashPointer(values);
		float barWidth = 10.0f;
		p.y -= height / 2.0f;
		float delta = maxValue - minValue;
		if (delta == 0.0f) {
			delta = 1.0f;
		}
		float st = width / static_cast<float>(num - 1);
		float bw = width / static_cast<float>(num);
		renderer::add_box(_guiCtx->uiContext, p, p2i(width + barWidth, height), ds::Color(51, 51, 51, 255));
		p.x += width + 20.0f;
		p.y += height / 2.0f;
		char buffer[16];
		sprintf_s(buffer, 16, "%g", maxValue);
		renderer::add_text(_guiCtx->uiContext, p, buffer);
		p.y -= height;
		sprintf_s(buffer, 16, "%g", minValue);
		renderer::add_text(_guiCtx->uiContext, p, buffer);
		for (int i = 0; i < num; ++i) {
			float v = values[i];
			if (v > maxValue) {
				v = maxValue;
			}
			float current = (v - minValue) / delta;
			float yp = current * height;
			p = _guiCtx->currentPos;
			p.y -= (height - yp * 0.5f);
			p.x += static_cast<float>(i) * bw + 2.0f;
			renderer::add_box(_guiCtx->uiContext, p, p2i(bw - 4, yp), ds::Color(192, 0, 0, 255));
		}
		step = delta / 10.0f;
		int d = static_cast<int>(delta / step) + 1;
		for (int i = 0; i < d; ++i) {
			p = _guiCtx->currentPos;
			float current = 1.0f - (step*i) / delta;
			float yp = current * height;
			p.y -= yp;
			renderer::add_box(_guiCtx->uiContext, p, p2i(width + barWidth, 1), ds::Color(16, 16, 16, 255));
		}
		moveForward(p2i(width, height + 30));
	}

	// -------------------------------------------------------
	// Diagram
	// -------------------------------------------------------	
	void DiagramInternal(const p2i& pos, float* values, int num, float minValue, float maxValue, float step, float width, float height) {
		p2i p = pos;
		HashedId id = HashPointer(values);
		p.y -= height / 2.0f;
		float delta = (maxValue - minValue);
		if (delta == 0.0f) {
			delta = 1.0f;
		}
		float st = width / static_cast<float>(num - 1);
		renderer::add_box(_guiCtx->uiContext, p, p2i(width, height), ds::Color(51, 51, 51, 255));
		p.x += width + 20.0f;
		p.y += height / 2.0f;
		char buffer[16];
		sprintf_s(buffer, 16, "%g", maxValue);
		renderer::add_text(_guiCtx->uiContext, p, buffer);
		p.y -= height;
		sprintf_s(buffer, 16, "%g", minValue);
		renderer::add_text(_guiCtx->uiContext, p, buffer);

		for (int i = 0; i < num; ++i) {
			float v = values[i];
			if (v > maxValue) {
				v = maxValue;
			}
			if (v < minValue) {
				v = minValue;
			}
			float norm = (v - minValue) / delta;
			float yp = norm * height;
			p = pos;
			p.y = pos.y - height + yp;
			p.x += static_cast<float>(i)* st - 2.0f;
			renderer::add_box(_guiCtx->uiContext, p, p2i(4, 4), ds::Color(192, 0, 0, 255));
		}

		step = delta / 10.0f;
		int d = static_cast<int>(delta / step) + 1;
		for (int i = 0; i < d; ++i) {
			p = pos;
			float current = 1.0f - (step*i) / delta;
			float yp = current * height;
			p.y -= yp;
			renderer::add_box(_guiCtx->uiContext, p, p2i(width, 1), ds::Color(16, 16, 16, 255));
		}
		moveForward(p2i(width, height + 30));
	}

	void Diagram(float* values, int num, float minValue, float maxValue, float step, float width, float height) {
		DiagramInternal(_guiCtx->currentPos, values, num, minValue, maxValue, step, width, height);
	}

	void Diagram(const char* label, float* values, int num, float minValue, float maxValue, float step, float width, float height) {
		p2i p = _guiCtx->currentPos;
		renderer::add_text(_guiCtx->uiContext, p, label);
		moveForward(p2i(10, 20));
		DiagramInternal(p, values, num, minValue, maxValue, step, width, height);
	}
	/*
	void beginMenu() {
		// FIXME: get screen dimension!
		_guiCtx->menuPosition = ds::vec2(0, 755);
		renderer::add_box(_guiCtx->uiContext, _guiCtx->menuPosition, ds::vec2(10, 30), ds::Color(0, 0, 192, 255), renderer::ResizeType::RT_X);
	}

	void MenuBar(const char** names, int num, int* state) {
		ds::vec2 pos = _guiCtx->menuPosition;
		for (int i = 0; i < num; ++i) {
			ds::vec2 dim = textSize(names[i]);
			dim.x += 20.0f;
			dim.y = 30.0f;
			renderer::add_box(_guiCtx->uiContext, pos, dim, ds::Color(192, 0, 192, 255));
			renderer::add_text(_guiCtx->uiContext, pos, names[i]);

			if (isClicked(pos, dim)) {
				int idx = i + 1;
				if (*state == idx) {
					*state = 0;
				}
				else {
					*state = idx;
				}
			}
			pos.x += dim.x + 2.0f;
		}
	}

	bool MenuBar(const char* name) {
		HashedId id = HashPointer(name);
		ds::vec2 pos = _guiCtx->menuPosition;
		ds::vec2 dim = textSize(name);
		dim.x += 20.0f;
		dim.y = 30.0f;
		if (isHovered(pos, dim)) {
			renderer::add_box(_guiCtx->uiContext, pos, dim, ds::Color(192, 0, 192, 255));
		}
		else {
			renderer::add_box(_guiCtx->uiContext, pos, dim, ds::Color(192, 192, 0, 255));
		}
		renderer::add_text(_guiCtx->uiContext, pos, name);
		bool clicked = isClicked(pos, dim);
		if (clicked) {
			_guiCtx->active = id;
		}
		_guiCtx->menuPosition.x += dim.x + 2.0f;
		return clicked;
	}

	bool MenuItem(const char* label) {
		return false;
	}

	void endMenu() {

	}
	*/
	// -------------------------------------------------------
	// prepare combo box
	// -------------------------------------------------------
	void prepareComboBox(int id, int* offset, int size, int max) {
		float width = 200.0f;
		p2i p = _guiCtx->currentPos;
		float height = max * 20.0f;
		p.y -= height * 0.5f - 10.0f;
		// background
		renderer::add_box(_guiCtx->uiContext, p, p2i(width + 20, height), _guiCtx->settings.boxBackgroundColor);
		if (size > max) {
			// up
			p = _guiCtx->currentPos;
			p.x += width;
			renderer::add_box(_guiCtx->uiContext, p, 20, 20, _guiCtx->settings.buttonColor);
			renderer::add_text(_guiCtx->uiContext, p, "-");
			/*
			if (isBoxSelected(id, p, ds::vec2(20.0f, 20.0f))) {
				*offset -= 1;
				if (*offset < 0) {
					*offset = 0;
				}
			}
			*/
			float sideHeight = height - 2.0f * 20.0f;
			p.y -= sideHeight * 0.5f;
			p.y -= 20.0f * 0.5f;
			float cy = p.y;
			renderer::add_box(_guiCtx->uiContext, p, p2i(20, sideHeight), ds::Color(20, 20, 20, 255));

			// down
			p.y = _guiCtx->currentPos.y - (max - 1) * 20.0f;
			renderer::add_box(_guiCtx->uiContext, p, 20, 20, _guiCtx->settings.buttonColor);
			renderer::add_text(_guiCtx->uiContext, p, "+");
			/*
			if (isBoxSelected(id, p, ds::vec2(20.0f, 20.0f))) {
				if ((*offset + max) < size) {
					*offset += 1;
				}
			}
			*/
			float d = 1.0f - static_cast<float>(*offset) / static_cast<float>(max);
			float dy = d * (sideHeight) - 20.0f;
			p.y = cy + dy;
			renderer::add_box(_guiCtx->uiContext, p, p2i(20, 6), _guiCtx->settings.scrollSliderColor);
			
		}
	}
	// -------------------------------------------------------
	// ListBox
	// -------------------------------------------------------	
	void ListBox(const char* label, const char** entries, int num, int* selected, int *offset, int max) {
		HashedId id = HashPointer(&entries);
		prepareComboBox(id, offset, num, max);
		float width = 200.0f;
		p2i p = _guiCtx->currentPos;
		float height = max * 20.0f;
		//bool hot = isHot(id, p, v2(width, height));
		int start = *offset;
		int end = *offset + max;
		if (end >= num) {
			end = num;
		}
		for (int i = start; i < end; ++i) {
			//if (isBoxSelected(id, p, ds::vec2(width, 20.0f))) {
				//*selected = i;
			//}
			if (*selected == i) {
				renderer::add_box(_guiCtx->uiContext, p, p2i(width, 20), _guiCtx->settings.boxSelectionColor);
			}
			renderer::add_text(_guiCtx->uiContext, p, entries[i]);
			p.y -= 20.0f;
		}
		moveForward(p2i(width, height + 4));
	}

	// -------------------------------------------------------
	// DropDownBox
	// -------------------------------------------------------	
	void DropDownBox(const char* label, const char** entries, int num, int* state, int* selected, int *offset, int max, bool closeOnSelection) {
		HashedId id = HashPointer(&entries);
		p2i p = _guiCtx->currentPos;
		/*
		if (*state == 0) {
			renderer::add_box(_guiCtx->uiContext, p, 20, 20, _guiCtx->settings.buttonColor);
			renderer::add_text(_guiCtx->uiContext, p, "+");
			if (isBoxSelected(id, p, ds::vec2(20.0f, 20.0f))) {
				*state = 1;
			}
		}
		else {
			renderer::add_box(_guiCtx->uiContext, p, 20, 20, _guiCtx->settings.buttonColor);
			renderer::add_text(_guiCtx->uiContext, p, "-");
			if (isBoxSelected(id, p, ds::vec2(20.0f, 20.0f))) {
				*state = 0;
			}
		}
		*/
		p.x += 20.0f;
		renderer::add_box(_guiCtx->uiContext, p, p2i(200, 20), _guiCtx->settings.labelBoxColor);
		if (*selected >= 0 && *selected < num) {
			renderer::add_text(_guiCtx->uiContext, p, entries[*selected]);
		}
		else {
			renderer::add_text(_guiCtx->uiContext, p, "{Select entry}");
		}
		p.x += 180.0f;
		renderer::add_box(_guiCtx->uiContext, p, 20, 20, _guiCtx->settings.buttonColor);
		renderer::add_text(_guiCtx->uiContext, p, "x");
		//if (isBoxSelected(id, p, ds::vec2(20.0f, 20.0f))) {
			//*selected = -1;
		//}
		
		p.x += 30.0f;
		renderer::add_text(_guiCtx->uiContext, p, label);
		
		moveForward(p2i(300, 20));
		if (*state == 1) {
			prepareComboBox(id, offset, num, max);
			float width = 200.0f;
			// if there is no scroll border then expand it
			if (num <= max) {
				width += 20.0f;
			}
			p2i p = _guiCtx->currentPos;
			float height = max * 20.0f;
			int start = *offset;
			int end = *offset + max;
			if (end >= num) {
				end = num;
			}
			for (int i = start; i < end; ++i) {
				/*
				if (isBoxSelected(id, p, ds::vec2(width, 20.0f))) {
					*selected = i;
					if (closeOnSelection) {
						*state = 0;
					}
				}
				*/
				if (*selected == i) {
					renderer::add_box(_guiCtx->uiContext, p, p2i(width, 20), _guiCtx->settings.boxSelectionColor);
				}
				renderer::add_text(_guiCtx->uiContext, p, entries[i]);
				p.y -= 20.0f;
			}
			moveForward(p2i(width, height + 4));
		}
		else {
			moveForward(p2i(300, 4));
		}

	}


	void debug() {
		Value("Mouse", _guiCtx->mousePosition);
		Value("Button", _guiCtx->mouseDown);
		Value("Hot", _guiCtx->hotItem);
		Value("Active", _guiCtx->activeItem);
	}
	// --------------------------------------------------------
	// end - render all draw calls
	// --------------------------------------------------------
	void end() {
		_guiCtx->uiContext->size.y = _guiCtx->size.y;
		renderer::draw_buffer(_guiCtx->uiContext);
		_guiCtx->numKeys = 0;
		if (!_guiCtx->mouseDown) {
			_guiCtx->activeItem = 0;
		}
	}

	// --------------------------------------------------------
	// shutdown GUI
	// --------------------------------------------------------
	void shutdown() {
		if (_guiCtx != 0) {
			renderer::shutdown(_guiCtx->uiContext);
			delete _guiCtx->uiContext;
			delete _guiCtx;
		}
	}

}

#endif