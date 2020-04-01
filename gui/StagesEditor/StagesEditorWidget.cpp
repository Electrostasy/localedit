#include "StagesEditorWidget.h"

StagesEditorWidget::StagesEditorWidget(QWidget *parent): QWidget(parent) {
	auto *layout = new QVBoxLayout(this);

	tabWidget = new ExpandingTabWidget();
	tabWidget->setDocumentMode(true);

	pageOwner = new QWidget();
	pageDispatch = new QWidget();
	pageOwner->setLayout(new QVBoxLayout());
	pageDispatch->setLayout(new QVBoxLayout());

	tabWidget->addTab(pageOwner, "Owner");
	tabWidget->addTab(pageDispatch, "Dispatch");

	layout->addWidget(tabWidget);
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
	scrollableContent->setLayout(new QVBoxLayout());

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
		auto *layout = new QVBoxLayout();

		auto *box = new QGroupBox();
		if(stage.hasOpp) {
			box->setTitle("Stage " + QString::number(stageIndex - 1) + " (Opp)");
		} else {
			box->setTitle("Stage " + QString::number(stageIndex));
		}
		box->setFlat(true);
		scrollableContent->layout()->addWidget(box);

		box->setLayout(layout);

		auto *textField = new QTextEdit(page);
		new SyntaxHighlighter(textField->document());
		textField->setPlainText(stage.objectives);

		QFontMetrics fm = QFontMetrics(textField->font());
		int fieldHeight = fm.lineSpacing() * 2 + fm.height() * 3;
		textField->setFixedHeight(fieldHeight);

		layout->addWidget(textField);
		stageIndex++;
	}

	page->layout()->addWidget(scrollArea);
}
