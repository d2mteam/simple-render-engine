//
// Created by brosobad on 15/07/2025.
//

#ifndef INPUT_H
#define INPUT_H

#include "KeyCode.h"
#include <cstdint>

class GLFWwindow;

class KeyboardInput {
public:
	virtual ~KeyboardInput() = default;

	int8_t GetValue(EKeyCode keyCode) const { return (IsPressed(keyCode) || IsHeld(keyCode)) ? 1 : 0; }
	EKeyState GetState(EKeyCode keyCode) { return GetState((int32_t)keyCode); }
	bool IsPressed(EKeyCode keyCode) const { return IsPressed((int32_t)keyCode); }
	bool IsHeld(EKeyCode keyCode) const { return IsHeld((int32_t)keyCode); }
	bool IsReleased(EKeyCode keyCode) const { return IsReleased((int32_t)keyCode); }

	virtual EKeyState GetState(int32_t keyCode) const = 0;
	virtual bool IsPressed(int32_t keyCode) const = 0;
	virtual bool IsHeld(int32_t keyCode) const = 0;
	virtual bool IsReleased(int32_t keyCode) const = 0;
protected:
	KeyboardInput() = default;
};

class GLFWKeyboardInput : public KeyboardInput {
public:
	GLFWKeyboardInput(void* window);
	virtual EKeyState GetState(int32_t keyCode) const override;
	virtual bool IsPressed(int32_t keyCode) const override;
	virtual bool IsHeld(int32_t keyCode) const override;
	virtual bool IsReleased(int32_t keyCode) const override;
private:
	GLFWwindow* mWindow;
};

class MouseInput {
public:
	virtual ~MouseInput() = default;

	int8_t GetValue(EMouseButton button) const { return (IsPressed(button) || IsHeld(button)) ? 1 : 0; }
	EKeyState GetState(EMouseButton button) { return GetState((int32_t)button); }
	bool IsPressed(EMouseButton button) const { return IsPressed((int32_t)button); }
	bool IsHeld(EMouseButton button) const { return IsHeld((int32_t)button); }
	bool IsReleased(EMouseButton button) const { return IsReleased((int32_t)button); }

	virtual EKeyState GetState(int32_t button) const = 0;
	virtual bool IsPressed(int32_t button) const = 0;
	virtual bool IsHeld(int32_t button) const = 0;
	virtual bool IsReleased(int32_t button) const = 0;

	float GetPositionX() const { return mX; }
	float GetPositionY() const { return mY; }
	float GetOffsetX() const { return mOffsetX; }
	float GetOffsetY() const { return mOffsetY; }
	float GetScrollX() const { return mScrollX; }
	float GetScrollY() const { return mScrollY; }

	void SetPosition(float x, float y) { mX = x; mY = y; }
	void SetPositionX(float x) { mX = x; }
	void SetPositionY(float y) { mY = y; }

	void SetOffset(float offsetX, float offsetY) { mOffsetX = offsetX; mOffsetY = offsetY; }
	void SetOffsetX(float offsetX) { mOffsetX = offsetX; }
	void SetOffsetY(float offsetY) { mOffsetY = offsetY; }

	void SetScroll(float scrollX, float scrollY) { mScrollX = scrollX; mScrollY = scrollY; }
	void SetScrollX(float scrollX) { mScrollX = scrollX; }
	void SetScrollY(float scrollY) { mScrollY = scrollY; }

protected:
	MouseInput() = default;
protected:
	float mX, mY;
	float mOffsetX, mOffsetY;
	float mScrollX, mScrollY;
};

class GLFWMouseInput : public MouseInput {
public:
	GLFWMouseInput(void* window);
	virtual EKeyState GetState(int32_t button) const override;
	virtual bool IsPressed(int32_t button) const override;
	virtual bool IsHeld(int32_t button) const override;
	virtual bool IsReleased(int32_t button) const override;
private:
	GLFWwindow* mWindow;
};

struct InputState {
	KeyboardInput* Keyboard;
	MouseInput* Mouse;
};


#endif //INPUT_H
