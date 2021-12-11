#pragma once

#include "APBFramelessWindowTitleBar.h"
#include "APBPushButton.h"
#include <QtWinExtras/QtWin>
#include <windows.h>
#include <windowsx.h>
#include <dwmapi.h>

// Based on https://github.com/wangyangyangisme/Custom-Frameless-Blur-Behind-Windows-For-Qt
// On Windows platforms, allows the application to draw a stylized window, replacing the
// titlebar and handling window management events itself.
class NativeTranslucentFramelessWindow: public QWidget {
	Q_OBJECT

	public:
	explicit NativeTranslucentFramelessWindow(QWidget *parent = nullptr);

	protected:
  APBFramelessWindowTitleBar *titleBar;

	bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;

	bool isLeftBorderHit(const QRect &iRect, long iBorderWidth, long iX, long iY);
	bool isRightBorderHit(const QRect &iRect, long iBorderWidth, long iX, long iY);

	bool isTopBorderHit(const QRect &iRect, long iBorderWidth, long iX, long iY);
	bool isBottomBorderHit(const QRect &iRect, long iBorderWidth, long iX, long iY);

	bool isTopLeftCornerHit(const QRect &iRect, long iBorderWidth, long iX, long iY);
	bool isTopRightCornerHit(const QRect &iRect, long iBorderWidth, long iX, long iY);

	bool isBottomRightCornerHit(const QRect &iRect, long iBorderWidth, long iX, long iY);
	bool isBottomLeftCornerHit(const QRect &iRect, long iBorderWidth, long iX, long iY);

	bool isTitleBarHit(const QRect &iRect, long iBorderWidth, long iX, long iY);

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

