#include "PreviewBoxWidget.h"

PreviewBoxWidget::PreviewBoxWidget(QWidget *parent): QWidget(parent) {
	auto layout = new QVBoxLayout(this);
	layout->setSpacing(0);

	// Declare items
	auto header = new QWidget();
	auto headerLayout = new QHBoxLayout(header);

	auto icon = new QLabel();
	QPixmap pixmap(":/resources/mission_exclamation_mark");
	icon->setPixmap(pixmap);
	icon->setMask(pixmap.mask());
	icon->setMargin(0);
	missionName = new QLabel("MISSION NAME HERE");
	auto timer = new QLabel("04:00");
	missionName->setMargin(0);
	timer->setMargin(0);

	headerLayout->addWidget(icon);
	headerLayout->addWidget(missionName);
	headerLayout->addWidget(timer);

	missionText = new QPlainTextEdit("0/0 Now the Enforcers know we're importing raw Glow, we're moving the refining lab before the Enforcers can find it. <Col: StageText>Keep </Col>the <Col: StageText>lab equipment </Col>safe from Enforcers <Col: StageText>or take out </Col>the <Col: StageText>investigators</Col>.");
	missionText->setReadOnly(true);
	missionText->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
	missionText->setWordWrapMode(QTextOption::WrapMode::WordWrap);
	// missionText->setLineWrapMode(QPlainTextEdit::LineWrapMode::WidgetWidth);

	auto footer = new QWidget();
	auto footerLabel = new QLabel();
	auto footerLayout = new QHBoxLayout(footer);
	footerLayout->addWidget(footerLabel);

	// Set sizing properties
	missionName->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));
	missionText->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
	footer->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));

	missionName->setAlignment(Qt::AlignTop | Qt::AlignLeft);
	timer->setAlignment(Qt::AlignTop | Qt::AlignRight);
	footerLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

	icon->setMargin(0);
	missionName->setMargin(0);
	timer->setMargin(0);
	footerLabel->setMargin(0);

	// Calculate optimal sizes for screen
	auto font = missionName->font();
	// font.setPointSizeF(font.pointSize() * 0.845);
	// missionName->setFont(font);
	// timer->setFont(font);

	// 0.845 is the "ratio" that everything scales with
	auto fontSize = font.pointSize() * 0.845;
	double margin = 4 / 0.845;
	double ratio = (6 * margin + 5 * fontSize) / 103;

	// Limit line count in missionText (albeit crudely)
	int lineLength = QFontMetrics(missionText->font()).horizontalAdvance(missionText->document()->toPlainText());
	int lineCount = qCeil(lineLength / static_cast<double>(missionText->width()));

	this->setFixedHeight(qFloor(103 / ratio));
	header->setFixedHeight(qFloor(18 / ratio));
	// missionText->setFixedHeight(qFloor(61 / ratio));
	footer->setFixedHeight(qFloor(24 / ratio));

	headerLayout->setContentsMargins(5 / 0.845, 3 / 0.845, 5 / 0.845, 4 / 0.845);
	// missionTextContainer->setContentsMargins(4 / 0.845, 3 / 0.845, 4 / 0.845, 6 / 0.845);

	layout->addWidget(header);
	layout->addWidget(missionText);
	layout->addWidget(footer);

	auto palette = missionText->palette();
	palette.setColor(QPalette::Base, QColor(0, 0, 0, 210));
	palette.setColor(QPalette::Text, QColor(255, 255, 255));
	missionText->setPalette(palette);

	setPaletteBG(header, QColor(0, 0, 0, 230));
	setPaletteBG(footer, QColor(0, 0, 0, 230));
	setPaletteFG(this, QColor(255, 255, 255));
	setPaletteFG(timer, QColor(255, 240, 0));
}

void PreviewBoxWidget::setPaletteBG(QWidget *widget, const QColor &colour) {
	QPalette palette = widget->palette();
	palette.setColor(QPalette::Window, colour);
	widget->setAutoFillBackground(true);
	widget->setPalette(palette);
}

void PreviewBoxWidget::setPaletteFG(QWidget *widget, const QColor &colour) {
	QPalette palette = widget->palette();
	palette.setColor(QPalette::WindowText, colour);
	widget->setPalette(palette);
}

void PreviewBoxWidget::setMissionName(QListWidgetItem *selectedItem) {
	this->missionName->setText(selectedItem->text());
}

void PreviewBoxWidget::setMissionStageText(QTextDocument *document) {
	this->missionText->setDocument(document);
}

