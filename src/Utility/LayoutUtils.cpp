#include "LayoutUtils.h"

// https://stackoverflow.com/a/56966160
void LayoutUtils::clearLayout(QLayout *layout) {
	QLayoutItem *layoutItem;
	while((layoutItem = layout->takeAt(0)) != nullptr) {
		if(layoutItem->layout()) {
			clearLayout(layoutItem->layout());
			layoutItem->layout()->deleteLater();
		}
		if(layoutItem->widget()) {
			layoutItem->widget()->deleteLater();
		}
		delete layoutItem;
	}
}

// https://stackoverflow.com/a/37186369
QLayout *LayoutUtils::findParentLayout(QWidget *widget, QLayout *topLevelLayout) {
	for(QObject *qo: topLevelLayout->children()) {
		auto *layout = qobject_cast<QLayout *>(qo);
		if(layout != nullptr) {
			if(layout->indexOf(widget) > -1)
				return layout;
			else if(!layout->children().isEmpty()) {
				layout = findParentLayout(widget, layout);
				if(layout != nullptr)
					return layout;
			}
		}
	}
	return nullptr;
}

QLayout *LayoutUtils::findParentLayout(QWidget *widget) {
	if(widget->parentWidget() != nullptr) {
		if(widget->parentWidget()->layout() != nullptr) {
			return findParentLayout(widget, widget->parentWidget()->layout());
		}
	}
	return nullptr;
}
