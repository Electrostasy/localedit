#include "StagesEditorWidget.h"

// TODO: StagesEditorWidget should have an option for parent widget, issues with the QWidget c-tor
StagesEditorWidget::StagesEditorWidget(MissionListWidget *missionListWidget) {
	ownerPage = new QWidget();
	dispatchPage = new QWidget();

	ownerPage->setLayout(new QVBoxLayout());
	dispatchPage->setLayout(new QVBoxLayout());

	auto *ownerScrollable = new QScrollArea();
	auto *dispatchScrollable = new QScrollArea();

	ownerScrollable->setWidget(ownerPage);
	dispatchScrollable->setWidget(dispatchPage);

	// Widget needs a layout, QTabWidget can't just use `this` as parent widget
	auto *layout = new QVBoxLayout(this);

	tabWidget = new QTabWidget();
	tabWidget->addTab(ownerPage, "Owner");
	tabWidget->addTab(dispatchPage, "Dispatch");

	layout->addWidget(tabWidget);

	// Items selected in the mission list will update the stages
	connect(missionListWidget->missionList, &QListWidget::currentItemChanged, this, [=](const QListWidgetItem *currentItem) {
		clearLayout(ownerPage->layout());
		clearLayout(dispatchPage->layout());

		auto missionCode = currentItem->data(Qt::UserRole).toString();

			int i = 1;
			foreach(const QString &stage, missionListWidget->missionStagesMap[missionCode].first) {
				auto *textField = new QTextEdit(ownerPage);
				textField->setPlainText(stage);

				new SyntaxHighlighter(textField->document());

				auto *layout = new QVBoxLayout();
				layout->addWidget(textField);

				auto *box = new QGroupBox("Stage " + QString::number(i));
				box->setFlat(true);
				box->setLayout(layout);
				ownerPage->layout()->addWidget(box);
				i++;
			}

			int j = 1;
			foreach(const QString &stage, missionListWidget->missionStagesMap[missionCode].second) {
				auto *textField = new QTextEdit(dispatchPage);
				textField->setPlainText(stage);

				new SyntaxHighlighter(textField->document());

				auto *layout = new QVBoxLayout();
				layout->addWidget(textField);

				auto *box = new QGroupBox("Stage " + QString::number(j));
				box->setLayout(layout);
				box->setFlat(true);

				dispatchPage->layout()->addWidget(box);
				j++;
			}
		});
}

void StagesEditorWidget::clearLayout(QLayout *layout) {
	if(layout) {
		while(layout->count() > 0) {
			QLayoutItem *item = layout->takeAt(0);
			QWidget *widget = item->widget();
			if(widget) {
				delete widget;
			}
			delete item;
		}
	}
}
