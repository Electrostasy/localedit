#include "StagesEditorWidget.h"

StagesEditorWidget::StagesEditorWidget(QWidget *parent): QWidget(parent) {
	auto *layout = new QVBoxLayout(this);

	tabWidget = new ExpandingTabWidget();

	pageOwner = new FramedPage();
	pageDispatch = new FramedPage();
	pageOwner->setLayout(new QVBoxLayout());
	pageDispatch->setLayout(new QVBoxLayout());

	pageOwner->setAutoFillBackground(true);
	pageDispatch->setAutoFillBackground(true);

	QPalette p = pageOwner->palette();
	p.setColor(QPalette::ColorRole::Window, QColor(255, 255, 255));
	pageOwner->setPalette(p);
	pageDispatch->setPalette(p);

	tabWidget->addTab(pageOwner, "Owner");
	tabWidget->addTab(pageDispatch, "Dispatch");

	layout->addWidget(tabWidget);
	layout->setMargin(0);
}

void StagesEditorWidget::showStages(MissionListItem *currentItem) {
	initSidedStages(pageOwner, currentItem->ownerStages());
	initSidedStages(pageDispatch, currentItem->dispatchStages());
}

void StagesEditorWidget::initSidedStages(QWidget *page, const QVector<MissionListItem::Stage> &stages) {
	if(page->layout()->count() > 0) {
		LayoutUtils::clearLayout(page->layout());
	}

	// If there are more stages than the main window can show, we insure ourselves with a scroll area
	auto *scrollableContent = new QWidget();
	auto scrollableLayout = new QVBoxLayout();
	scrollableContent->setLayout(scrollableLayout);

	auto *scrollArea = new QScrollArea();
	scrollArea->setWidgetResizable(true);
	scrollArea->setWidget(scrollableContent);

	// Back background of scroll area transparent
	QPalette p = scrollArea->palette();
	p.setColor(QPalette::ColorRole::Window, Qt::GlobalColor::transparent);
	scrollArea->setPalette(p);
	scrollArea->setContentsMargins(0, 0, 0, 0);
	scrollArea->setFrameShape(QFrame::NoFrame);

	// Build stages UI
	int stageIndex = 1;
	foreach(const MissionListItem::Stage &stage, stages) {
		if(stage.hasOpp) {
			continue;
		}
		auto *layout = new QVBoxLayout();

		auto *box = new QGroupBox();
		box->setTitle("Stage " + QString::number(stageIndex));
		box->setFlat(true);
		scrollableLayout->addWidget(box);

		box->setLayout(layout);

		auto *textField = new ExpandedTextEdit(page);
		new SyntaxHighlighter(textField->document());
		textField->setPlainText(stage.objectives);

		QFontMetrics fm = QFontMetrics(textField->font());
		int fieldHeight = fm.lineSpacing() * 2 + fm.height() * 3;
		textField->setFixedHeight(fieldHeight);

		layout->addWidget(textField, 0);
		stageIndex++;
	}

	// scrollableLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
	scrollableLayout->addStretch(9);
	page->layout()->addWidget(scrollArea);
}
