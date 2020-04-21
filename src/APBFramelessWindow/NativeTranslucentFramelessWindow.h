#pragma once

#include <QtWidgets>
#include <QtWinExtras/QtWin>
#include <windows.h>
#include <windowsx.h>
#include <dwmapi.h>

// Based on https://github.com/wangyangyangisme/Custom-Frameless-Blur-Behind-Windows-For-Qt

// Create a frameless window with support for aero glass, aero snap, resizing etc.
// using everyone's favourite Windows API.

// This class is only to encapsulate all the Windows code to its own special place so I don't
// have to see it outside of NativeTranslucentFramelessWindow.
class NativeTranslucentFramelessWindow: public QWidget {
	Q_OBJECT

	public:
	explicit NativeTranslucentFramelessWindow(QWidget *parent = nullptr);

	protected:
	bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;

	bool isLeftBorderHit(const QRect &iRect, long iBorderWidth, long iX, long iY);
	bool isRightBorderHit(const QRect &iRect, long iBorderWidth, long iX, long iY);

	bool isTopBorderHit(const QRect &iRect, long iBorderWidth, long iX, long iY);
	bool isBottomBorderHit(const QRect &iRect, long iBorderWidth, long iX, long iY);

	bool isTopLeftCornerHit(const QRect &iRect, long iBorderWidth, long iX, long iY);
	bool isTopRightCornerHit(const QRect &iRect, long iBorderWidth, long iX, long iY);

	bool isBottomRightCornerHit(const QRect &iRect, long iBorderWidth, long iX, long iY);
	bool isBottomLeftCornerHit(const QRect &iRect, long iBorderWidth, long iX, long iY);

	virtual bool isTitleBarHit(const QRect &iRect, long iBorderWidth, long iX, long iY);

	private:
	LONG borderWidth;

	typedef enum {
		ACCENT_DISABLED = 0,
		ACCENT_ENABLE_GRADIENT = 1,
		ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,
		ACCENT_ENABLE_BLURBEHIND = 3,
		ACCENT_INVALID_STATE = 4,
		ACCENT_STATE_SIZE = 0xFFFFFFFF
	} ACCENT_STATE;

	typedef struct {
		ACCENT_STATE accentState;
		int accentFlags;
		int gradientColor;
		int invalidState;
	} DWMACCENTPOLICY;

	typedef struct WINCOMPATTR_DATA {
		DWMACCENTPOLICY AccentPolicy;
	} WINCOMPATTR_DATA;

	typedef struct tagWINCOMPATTR {
		DWORD attribute; // the attribute to query
		WINCOMPATTR_DATA *pData; // buffer to store the result
		ULONG dataSize; // size of the pData buffer
	} WINCOMPATTR;
};
