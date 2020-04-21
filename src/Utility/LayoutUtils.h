#pragma once

#include <QLayout>
#include <QWidget>

class LayoutUtils {
	public:
	static void clearLayout(QLayout *layout);
	static QLayout *findParentLayout(QWidget *w, QLayout *topLevelLayout);
	static QLayout *findParentLayout(QWidget *w);
};
